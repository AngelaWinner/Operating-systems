#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include "../Headers/Employee.h"
#include "../Headers/Functions.h"
using std::string;

std::string fileName;
int numberOfEmployees;
Employee* employees;
int numberOfClients;

struct EmployeeSync {
    HANDLE readMutex;
    HANDLE writeSemafore;
    HANDLE employeeDataMutex;
    int readerCount;

    EmployeeSync() : readMutex(NULL), writeSemafore(NULL), employeeDataMutex(NULL), readerCount(0) {}
};

EmployeeSync* employeeSync;
HANDLE* hThreads;
PROCESS_INFORMATION* piArray;

DWORD WINAPI messaging(LPVOID param)
{
    HANDLE hPipe = (HANDLE)param;
    DWORD dwBytesRead;
    DWORD dwBytesWrite;

    char operation;
    int employeeId;
    Employee employeeTemp;
    bool success;
    int indexOfEmployee = -1;
    char finishSignal;

    while (true)
    {
        if (!ReadFile(hPipe, &operation, sizeof(operation), &dwBytesRead, NULL)) break;

        if (operation == '3') {
            std::cout << "Client requested exit.\n";
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
            WaitForSingleObject(employeeSync[indexOfEmployee].readMutex, INFINITE);
            employeeSync[indexOfEmployee].readerCount++;
            if (employeeSync[indexOfEmployee].readerCount == 1) {
                WaitForSingleObject(employeeSync[indexOfEmployee].writeSemafore, INFINITE);
            }
            ReleaseMutex(employeeSync[indexOfEmployee].readMutex);

            WaitForSingleObject(employeeSync[indexOfEmployee].employeeDataMutex, INFINITE);
            employeeTemp.num = employees[indexOfEmployee].num;
            employeeTemp.hours = employees[indexOfEmployee].hours;
            strcpy_s(employeeTemp.name, employees[indexOfEmployee].name);
            ReleaseMutex(employeeSync[indexOfEmployee].employeeDataMutex);

            success = WriteFile(hPipe, &employeeTemp, sizeof(Employee), &dwBytesWrite, NULL);

            if (success) std::cout << "Reading data for employee ID " << employeeId << " was sent.\n";
            else std::cout << "Failed to send reading data.\n";

            ReadFile(hPipe, &finishSignal, sizeof(finishSignal), &dwBytesRead, NULL);

            WaitForSingleObject(employeeSync[indexOfEmployee].readMutex, INFINITE);
            employeeSync[indexOfEmployee].readerCount--;
            if (employeeSync[indexOfEmployee].readerCount == 0) {
                ReleaseSemaphore(employeeSync[indexOfEmployee].writeSemafore, 1, NULL);
            }
            ReleaseMutex(employeeSync[indexOfEmployee].readMutex);
        }
        else if (operation == '1') {
            WaitForSingleObject(employeeSync[indexOfEmployee].writeSemafore, INFINITE);
            WaitForSingleObject(employeeSync[indexOfEmployee].employeeDataMutex, INFINITE);

            employeeTemp.num = employees[indexOfEmployee].num;
            employeeTemp.hours = employees[indexOfEmployee].hours;
            strcpy_s(employeeTemp.name, employees[indexOfEmployee].name);

            success = WriteFile(hPipe, &employeeTemp, sizeof(Employee), &dwBytesWrite, NULL);

            if (success) std::cout << "Data for modification (employee ID " << employeeId << ") was sent.\n";
            else std::cout << "Failed to send data for modification.\n";

            Employee modifiedEmployee;
            ReadFile(hPipe, &modifiedEmployee, sizeof(Employee), &dwBytesRead, NULL);

            employees[indexOfEmployee].hours = modifiedEmployee.hours;
            strcpy_s(employees[indexOfEmployee].name, modifiedEmployee.name);

            std::fstream file(fileName, std::ios::binary | std::ios::in | std::ios::out);
            if (file.is_open()) {
                file.seekp(sizeof(int) + indexOfEmployee * sizeof(Employee), std::ios::beg);
                file.write(reinterpret_cast<const char*>(&employees[indexOfEmployee]), sizeof(Employee));
                file.close();
                std::cout << "Employee ID " << employeeId << " updated in file.\n";
            }
            else {
                std::cerr << "Error: Failed to update file for employee ID " << employeeId << "\n";
            }

            ReadFile(hPipe, &finishSignal, sizeof(finishSignal), &dwBytesRead, NULL);

            ReleaseMutex(employeeSync[indexOfEmployee].employeeDataMutex);
            ReleaseSemaphore(employeeSync[indexOfEmployee].writeSemafore, 1, NULL);
        }
    }

    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    return 0;
}

DWORD WINAPI pipeListener(LPVOID param)
{
    HANDLE hPipe = CreateNamedPipe(
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
        std::cout << "Failed to create named pipe. Error: " << GetLastError() << "\n";
        return 1;
    }

    std::cout << "Named pipe created. Waiting for connections...\n";

    while (true)
    {
        BOOL connected = ConnectNamedPipe(hPipe, NULL);
        if (!connected) {
            DWORD error = GetLastError();
            if (error == ERROR_PIPE_CONNECTED) {
                std::cout << "Client connected.\n";
            }
            else {
                std::cout << "Connection failed. Error: " << error << "\n";
                CloseHandle(hPipe);
                return 1;
            }
        }
        else {
            std::cout << "Client connected.\n";
        }

        HANDLE hThread = CreateThread(NULL, 0, messaging, (LPVOID)hPipe, 0, NULL);
        if (hThread == NULL) {
            std::cerr << "Failed to create thread for client. Error: " << GetLastError() << "\n";
            DisconnectNamedPipe(hPipe);
            CloseHandle(hPipe);
        }
        else {
            CloseHandle(hThread);
        }

        hPipe = CreateNamedPipe(
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
            std::cout << "Failed to create new named pipe. Error: " << GetLastError() << "\n";
            break;
        }
    }

    return 0;
}

void cleanUp() {
    if (employeeSync != nullptr) {
        for (int i = 0; i < numberOfEmployees; i++) {
            if (employeeSync[i].readMutex != NULL)
                CloseHandle(employeeSync[i].readMutex);
            if (employeeSync[i].writeSemafore != NULL)
                CloseHandle(employeeSync[i].writeSemafore);
            if (employeeSync[i].employeeDataMutex != NULL)
                CloseHandle(employeeSync[i].employeeDataMutex);
        }
        delete[] employeeSync;
        employeeSync = nullptr;
    }

    if (hThreads != nullptr) {
        for (int i = 0; i < numberOfClients; i++) {
            if (hThreads[i] != NULL) {
                WaitForSingleObject(hThreads[i], 1000);
                CloseHandle(hThreads[i]);
            }
        }
        delete[] hThreads;
        hThreads = nullptr;
    }

    if (piArray != nullptr) {
        for (int i = 0; i < numberOfClients; i++) {
            if (piArray[i].hProcess != NULL)
                CloseHandle(piArray[i].hProcess);
            if (piArray[i].hThread != NULL)
                CloseHandle(piArray[i].hThread);
        }
        delete[] piArray;
        piArray = nullptr;
    }

    if (employees != nullptr) {
        delete[] employees;
        employees = nullptr;
    }
}

int main()
{
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

    employeeSync = new EmployeeSync[numberOfEmployees];
    for (int i = 0; i < numberOfEmployees; i++) {
        employeeSync[i].readMutex = CreateMutex(NULL, FALSE, NULL);
        employeeSync[i].writeSemafore = CreateSemaphore(NULL, 1, 1, NULL);
        employeeSync[i].employeeDataMutex = CreateMutex(NULL, FALSE, NULL);
        employeeSync[i].readerCount = 0;

        if (!employeeSync[i].readMutex || !employeeSync[i].writeSemafore || !employeeSync[i].employeeDataMutex) {
            std::cerr << "Failed to create synchronization objects\n";
            cleanUp();
            return 1;
        }
    }

    string numberOfClientsWelcome = "\nEnter number of clients: \n";
    getNumber(numberOfClients, numberOfClientsWelcome);

    std::cout << "\nCreating named pipe before starting clients...\n";

    HANDLE hListenerThread = CreateThread(NULL, 0, pipeListener, NULL, 0, NULL);
    if (hListenerThread == NULL) {
        std::cerr << "Failed to create listener thread. Error: " << GetLastError() << "\n";
        cleanUp();
        return 1;
    }

    Sleep(500);

    std::cout << "Named pipe should be ready. Starting clients...\n";

    piArray = new PROCESS_INFORMATION[numberOfClients];
    for (int i = 0; i < numberOfClients; ++i) {
        STARTUPINFO si;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        ZeroMemory(&piArray[i], sizeof(PROCESS_INFORMATION));

        std::wstring cmd = L"Client.exe";
        if (!CreateProcess(NULL, &cmd[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piArray[i])) {
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

    cleanUp();

    if (hListenerThread != NULL) {
        WaitForSingleObject(hListenerThread, 2000);
        CloseHandle(hListenerThread);
    }

    std::cout << "\nPress any key to finish the server: ";
    _getch();

    return 0;
}