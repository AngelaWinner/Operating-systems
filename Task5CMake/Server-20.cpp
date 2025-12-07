#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <memory>
#include <set>
#include "../Headers/Employee.h"
#include "../Headers/Functions.h"

using std::string;

std::string fileName;
int numberOfEmployees;
Employee* employees;
int numberOfClients;

class ReadWriteLock {
private:
    std::mutex readMutex;
    std::mutex writeMutex;
    std::condition_variable cv;
    int readerCount;
    int writerCount;
    bool writerWaiting;

public:
    ReadWriteLock() : readerCount(0), writerCount(0), writerWaiting(false) {}

    void acquireReadLock() {
        std::unique_lock<std::mutex> lock(writeMutex);
        cv.wait(lock, [this]() { return writerCount == 0 && !writerWaiting; });
        {
            std::lock_guard<std::mutex> readLock(readMutex);
            readerCount++;
        }
    }

    void releaseReadLock() {
        std::lock_guard<std::mutex> lock(readMutex);
        readerCount--;
        if (readerCount == 0) {
            cv.notify_one();
        }
    }

    void acquireWriteLock() {
        std::unique_lock<std::mutex> lock(writeMutex);
        writerWaiting = true;
        cv.wait(lock, [this]() { return readerCount == 0 && writerCount == 0; });
        writerWaiting = false;
        writerCount++;
    }

    void releaseWriteLock() {
        {
            std::lock_guard<std::mutex> lock(writeMutex);
            writerCount--;
        }
        cv.notify_all();
    }
};

struct EmployeeSync {
    ReadWriteLock readWriteLock;
    std::mutex dataMutex;

    EmployeeSync() = default;
};

std::vector<std::unique_ptr<EmployeeSync>> employeeSync;
std::vector<std::thread> messageThreads;
std::vector<PROCESS_INFORMATION> piArray;

void messaging(HANDLE hPipe) {
    DWORD dwBytesRead;
    DWORD dwBytesWrite;

    char operation;
    int employeeId;
    Employee employeeTemp;
    bool success;
    int indexOfEmployee = -1;
    char finishSignal;

    while (true) {
        if (!ReadFile(hPipe, &operation, sizeof(operation), &dwBytesRead, NULL)) break;

        if (operation == '3') {
            std::cout << " * Client requested exit.\n";
            break;
        }

        if (!ReadFile(hPipe, &employeeId, sizeof(employeeId), &dwBytesRead, NULL)) break;

        indexOfEmployee = -1;
        for (int i = 0; i < numberOfEmployees; i++) {
            if (employees[i].num == employeeId) {
                indexOfEmployee = i;
                break;
            }
        }

        bool found = (indexOfEmployee != -1);
        WriteFile(hPipe, &found, sizeof(found), &dwBytesWrite, NULL);

        if (!found) continue;

        if (operation == '2') {
            employeeSync[indexOfEmployee]->readWriteLock.acquireReadLock();

            {
                std::lock_guard<std::mutex> lock(employeeSync[indexOfEmployee]->dataMutex);
                employeeTemp.num = employees[indexOfEmployee].num;
                employeeTemp.hours = employees[indexOfEmployee].hours;
                strcpy_s(employeeTemp.name, employees[indexOfEmployee].name);
            }

            success = WriteFile(hPipe, &employeeTemp, sizeof(Employee), &dwBytesWrite, NULL);

            if (success) std::cout << " * Reading data for employee ID " << employeeId << " was sent.\n";
            else std::cout << " * Failed to send reading data.\n";

            ReadFile(hPipe, &finishSignal, sizeof(finishSignal), &dwBytesRead, NULL);

            employeeSync[indexOfEmployee]->readWriteLock.releaseReadLock();
        }
        else if (operation == '1') {
            employeeSync[indexOfEmployee]->readWriteLock.acquireWriteLock();

            {
                std::lock_guard<std::mutex> lock(employeeSync[indexOfEmployee]->dataMutex);

                employeeTemp.num = employees[indexOfEmployee].num;
                employeeTemp.hours = employees[indexOfEmployee].hours;
                strcpy_s(employeeTemp.name, employees[indexOfEmployee].name);

                success = WriteFile(hPipe, &employeeTemp, sizeof(Employee), &dwBytesWrite, NULL);

                if (success) std::cout << " * Data for modification (employee ID " << employeeId << ") was sent.\n";
                else std::cout << " * Failed to send data for modification.\n";

                Employee modifiedEmployee;
                ReadFile(hPipe, &modifiedEmployee, sizeof(Employee), &dwBytesRead, NULL);

                employees[indexOfEmployee].hours = modifiedEmployee.hours;
                strcpy_s(employees[indexOfEmployee].name, modifiedEmployee.name);

                std::fstream file(fileName, std::ios::binary | std::ios::in | std::ios::out);
                if (file.is_open()) {
                    file.seekp(sizeof(int) + indexOfEmployee * sizeof(Employee), std::ios::beg);
                    file.write(reinterpret_cast<const char*>(&employees[indexOfEmployee]), sizeof(Employee));
                    file.close();
                    std::cout << " * Employee ID " << employeeId << " updated in file.\n";
                }
                else {
                    std::cerr << " * Error: Failed to update file for employee ID " << employeeId << "\n";
                }

                ReadFile(hPipe, &finishSignal, sizeof(finishSignal), &dwBytesRead, NULL);
            }

            employeeSync[indexOfEmployee]->readWriteLock.releaseWriteLock();
        }
    }

    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
}

void pipeListener() {
    while (true) {
        HANDLE hPipe = CreateNamedPipeW(
            L"\\\\.\\pipe\\pipe_name",
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            1024,
            1024,
            0,
            NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            std::cout << " * Failed to create named pipe. Error: " << GetLastError() << "\n";
            return;
        }

        std::cout << " * Named pipe created. Waiting for connections...\n";

        BOOL connected = ConnectNamedPipe(hPipe, NULL);
        if (!connected) {
            DWORD error = GetLastError();
            if (error == ERROR_PIPE_CONNECTED) {
                std::cout << " * Client connected.\n";
            }
            else {
                std::cout << " * Connection failed. Error: " << error << "\n";
                CloseHandle(hPipe);
                return;
            }
        }
        else {
            std::cout << " * Client connected.\n";
        }

        messageThreads.emplace_back(messaging, hPipe);
    }
}

void cleanUp() {
    for (auto& thread : messageThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    for (auto& pi : piArray) {
        if (pi.hProcess != NULL) {
            WaitForSingleObject(pi.hProcess, 1000);
            CloseHandle(pi.hProcess);
        }
        if (pi.hThread != NULL) {
            CloseHandle(pi.hThread);
        }
    }

    if (employees != nullptr) {
        delete[] employees;
        employees = nullptr;
    }
}

int main() {
    getFileName(fileName);

    string numberOfEmployeesWelcome = "Enter number of employees: \n";
    getNumber(numberOfEmployees, numberOfEmployeesWelcome);

    employees = new Employee[numberOfEmployees];

    std::set<int> usedIds;
    for (int i = 0; i < numberOfEmployees; i++) {
        std::cout << "\n=== Enter data for employee " << i + 1 << " ===\n";
        getEmployeeData(employees[i], i, usedIds);
        string hoursWelcome = "Enter employee hours:\n";
        getDouble(employees[i].hours, hoursWelcome);
    }

    std::ofstream out(fileName, std::ios::binary | std::ios::out);
    if (!out.is_open()) {
        std::cerr << "\nError: Failed to create or open binary file '" << fileName << "'\n";
        delete[] employees;
        std::cout << "Press any key to exit...\n";
        _getch();
        return 1;
    }

    out.write(reinterpret_cast<const char*>(&numberOfEmployees), sizeof(numberOfEmployees));
    for (int i = 0; i < numberOfEmployees; i++) {
        out.write(reinterpret_cast<const char*>(&employees[i]), sizeof(Employee));
    }
    out.close();

    std::cout << "\nBinary file '" << fileName << "' created successfully.\n";

    std::cout << "\n=== Contents of binary file ===" << std::endl;

    std::ifstream in(fileName, std::ios::binary | std::ios::in);
    if (!in.is_open()) {
        std::cerr << "Error: Failed to open binary file for reading\n";
        delete[] employees;
        return 1;
    }

    showEmployees(in);
    in.close();

    employeeSync.reserve(numberOfEmployees);
    for (int i = 0; i < numberOfEmployees; i++) {
        employeeSync.emplace_back(std::make_unique<EmployeeSync>());
    }

    string numberOfClientsWelcome = "\nEnter number of clients: \n";
    getNumber(numberOfClients, numberOfClientsWelcome);

    std::cout << "\nCreating named pipe before starting clients...\n";

    std::thread listenerThread(pipeListener);

    Sleep(500);

    std::cout << "Named pipe should be ready. Starting clients...\n";

    piArray.resize(numberOfClients);
    for (int i = 0; i < numberOfClients; ++i) {
        ZeroMemory(&piArray[i], sizeof(PROCESS_INFORMATION));

        std::wstring cmd = L"Client-20.exe";

        STARTUPINFOW siw;
        ZeroMemory(&siw, sizeof(STARTUPINFOW));
        siw.cb = sizeof(STARTUPINFOW);

        if (!CreateProcessW(NULL, &cmd[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &siw, &piArray[i])) {
            std::cerr << "Failed to create client process " << i + 1 << ". Error: " << GetLastError() << "\n";
        }
        else {
            std::cout << "Client process " << i + 1 << " started.\n";
        }
    }

    std::cout << "\nWaiting for all clients to finish...\n";
    for (int i = 0; i < numberOfClients; i++) {
        if (piArray[i].hProcess != NULL) {
            WaitForSingleObject(piArray[i].hProcess, INFINITE);
            std::cout << "Client " << i + 1 << " finished.\n";
        }
    }

    Sleep(500);

    std::cout << "\nAll clients have finished their work.\n";

    std::cout << "\n=== Final contents of binary file ===" << std::endl;

    std::ifstream fin(fileName, std::ios::binary | std::ios::in);
    if (fin.is_open()) {
        showEmployees(fin);
        fin.close();
    }
    else {
        std::cerr << "Error: Failed to open final file for reading\n";
    }

    if (listenerThread.joinable()) {
        listenerThread.detach();
    }

    cleanUp();

    std::cout << "\nPress any key to finish the server: ";
    _getch();

    return 0;
}