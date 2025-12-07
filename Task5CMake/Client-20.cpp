#include <conio.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <thread>
#include "../Headers/Employee.h"
#include "../Headers/Functions.h"

bool connectToServer(HANDLE& hPipe, int& attempts) {
    const wchar_t* PIPE_NAME = L"\\\\.\\pipe\\pipe_name";
    const int MAX_ATTEMPTS = 5;

    while (attempts < MAX_ATTEMPTS) {
        hPipe = CreateFileW(
            PIPE_NAME,
            GENERIC_WRITE | GENERIC_READ,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hPipe != INVALID_HANDLE_VALUE) {
            DWORD mode = PIPE_READMODE_MESSAGE;
            SetNamedPipeHandleState(hPipe, &mode, NULL, NULL);
            return true;
        }

        DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND) {
            attempts++;
            std::cout << "Server not ready yet (attempt " << attempts << " of " << MAX_ATTEMPTS << "). Waiting...\n";
            Sleep(500);
        }
        else {
            std::cout << "Cannot connect to server. Error: " << error << "\n";
            break;
        }
    }

    return false;
}

void handleOperation(char operation, int employeeId) {
    HANDLE hPipe = INVALID_HANDLE_VALUE;
    int attempts = 0;

    if (!connectToServer(hPipe, attempts)) {
        std::cout << "Failed to connect to server after " << attempts << " attempts.\n";
        std::cout << "Press any key to continue...\n";
        _getch();
        return;
    }

    DWORD dwBytesWritten;
    DWORD dwBytesRead;
    bool success;
    Employee employeeTemp;

    success = WriteFile(hPipe, &operation, sizeof(operation), &dwBytesWritten, NULL);
    if (!success) {
        std::cout << "Failed to send operation. Error: " << GetLastError() << "\n";
        CloseHandle(hPipe);
        return;
    }

    success = WriteFile(hPipe, &employeeId, sizeof(employeeId), &dwBytesWritten, NULL);
    if (!success) {
        std::cout << "Failed to send employee ID. Error: " << GetLastError() << "\n";
        CloseHandle(hPipe);
        return;
    }

    bool employeeFound;
    success = ReadFile(hPipe, &employeeFound, sizeof(employeeFound), &dwBytesRead, NULL);
    if (!success) {
        std::cout << "Failed to receive response from server. Error: " << GetLastError() << "\n";
        CloseHandle(hPipe);
        return;
    }

    if (!employeeFound) {
        std::cout << "Employee with ID " << employeeId << " not found on server.\n";
        CloseHandle(hPipe);
        return;
    }

    success = ReadFile(hPipe, &employeeTemp, sizeof(Employee), &dwBytesRead, NULL);
    if (!success) {
        std::cout << "Failed to receive employee data. Error: " << GetLastError() << "\n";
        CloseHandle(hPipe);
        return;
    }

    std::cout << "\n=== Employee Information ===\n";
    std::cout << "ID: " << employeeTemp.num << "\n";
    std::cout << "Name: " << employeeTemp.name << "\n";
    std::cout << "Hours: " << employeeTemp.hours << "\n";
    std::cout << "===========================\n";

    if (operation == '1') {
        std::cout << "\nPress any key to start modifying...\n";
        _getch();

        std::cout << "\n=== Modify Employee Data ===\n";

        std::cout << "Enter new name (max 9 chars): ";
        std::string newName;
        std::cin.ignore(INT_MAX, '\n');
        while (true) {
            std::getline(std::cin, newName);
            if (!newName.empty()) {
                break;
            }
            std::cout << "Name cannot be empty! Enter name again: ";
        }
        if (newName.length() > 9) {
            newName = newName.substr(0, 9);
            std::cout << "Note: Name truncated to 9 characters.\n";
        }
        strncpy_s(employeeTemp.name, 10, newName.c_str(), _TRUNCATE);

        std::cout << "Enter new hours: ";
        getDouble(employeeTemp.hours, "");

        std::cout << "\nPress any key to send modified record to Server...\n";
        _getch();

        success = WriteFile(hPipe, &employeeTemp, sizeof(Employee), &dwBytesWritten, NULL);
        if (!success) {
            std::cout << "Failed to send modified data. Error: " << GetLastError() << "\n";
            CloseHandle(hPipe);
            return;
        }

        std::cout << "Modified data sent to server.\n";
    }

    std::cout << "\nPress any key to end the selected operation...\n";
    _getch();

    char finishSignal = '0';
    WriteFile(hPipe, &finishSignal, sizeof(finishSignal), &dwBytesWritten, NULL);

    CloseHandle(hPipe);

    std::ofstream loggingOut("clientLogging.txt", std::ios::app);
    if (loggingOut.is_open()) {
        loggingOut << "Operation: " << (operation == '2' ? "READ" : "MODIFY")
            << ", Employee ID: " << employeeId
            << ", Status: SUCCESS\n";
        loggingOut.close();
    }
}

void showLog() {
    std::ifstream loggingIn("clientLogging.txt");
    if (loggingIn.is_open()) {
        std::cout << "\n=== Client Log ===\n";
        std::string line;
        while (std::getline(loggingIn, line)) {
            std::cout << line << "\n";
        }
        loggingIn.close();
    }
}

int main() {
    std::cout << "Client started. Waiting for server to be ready...\n";
    Sleep(500);

    while (true) {
        std::cout << "\n=======================================\n";
        std::cout << "Choose operation: \n";
        std::cout << "1. Modify employee data\n";
        std::cout << "2. Read employee data\n";
        std::cout << "3. Exit\n";
        std::cout << "Your choice: ";

        std::string chosenOptionStr;
        std::cin >> chosenOptionStr;

        std::regex chosenOptionRegex("^[123]$");
        if (!std::regex_match(chosenOptionStr, chosenOptionRegex)) {
            std::cout << "Incorrect input! Please enter 1, 2 or 3.\n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            continue;
        }

        int chosenOption = std::stoi(chosenOptionStr);

        if (chosenOption == 3) {
            HANDLE hPipe = INVALID_HANDLE_VALUE;
            int attempts = 0;

            if (connectToServer(hPipe, attempts)) {
                char operation = '3';
                DWORD dwBytesWritten;
                WriteFile(hPipe, &operation, sizeof(operation), &dwBytesWritten, NULL);
                CloseHandle(hPipe);
            }

            std::cout << "Exiting client...\n";
            break;
        }

        char operation = (chosenOption == 1) ? '1' : '2';

        std::cout << "Enter employee ID: ";
        int employeeId;
        bool validId = false;
        while (!validId) {
            validId = enter_ID_of_employee(employeeId);
        }

        handleOperation(operation, employeeId);
    }

    showLog();

    std::cout << "\nClient terminated. Press any key to exit...\n";
    _getch();

    return 0;
}