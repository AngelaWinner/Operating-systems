#include "pch.h"
#include <windows.h>
#include <iostream>
#include <regex>
#include <fstream>
#include "Functions.h"

using std::cout;
using std::cin;
using std::string;
using std::fstream;

bool validatePositiveNumber(const std::string& input) {
    std::regex numberRegex("^[1-9][0-9]*$");
    return std::regex_match(input, numberRegex);
}

bool validateFilename(const std::string& filename) {
    std::regex filenameRegex("^[a-zA-Z0-9_-]+$");
    return std::regex_match(filename, filenameRegex);
}

bool validateChoice(const std::string& choice) {
    std::regex choiceRegex("^[01]$");
    return std::regex_match(choice, choiceRegex);
}

void getNumber(int& number, const std::string& welcomeMessage) {
    string numberStr;
    while (true) {
        cout << welcomeMessage;
        if (cin >> numberStr) {
            if (validatePositiveNumber(numberStr)) {
                number = std::stoi(numberStr);
                break;
            }
            else {
                cout << "Error: Number must be positive number.\n";
            }
        }
        else {
            cout << "Error: Please enter a valid integer positive number.\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

void getFilename(std::string& filename) {
    while (true) {
        cout << "Enter binary file name: \n";
        cin >> filename;
        if (validateFilename(filename)) {
            break;
        }
        else {
            cout << "Error: Invalid file name. ";
            cout << "Only letters, numbers, _ and - symbols are allowed.\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

void getChoice(int& choice, const std::string& options) {
    string choiceStr;
    while (true) {
        cout << options;
        std::getline(cin, choiceStr);
        if (validateChoice(choiceStr)) {
            choice = std::stoi(choiceStr);
            break;
        }
        else {
            cout << "Incorrect input. Try again...\n";
        }
    }
}

HANDLE createMutex(const std::wstring& name) {
    HANDLE mutex = CreateMutex(NULL, FALSE, name.c_str());
    if (mutex == NULL) {
        cout << "Error creating mutex";
    }
    return mutex;
}

HANDLE createSemaphore(const std::wstring& name, int initialCount, int maxCount) {
    HANDLE semaphore = CreateSemaphore(NULL, initialCount, maxCount, name.c_str());
    if (semaphore == NULL) {
        cout << "Error creating semaphore";
    }
    return semaphore;
}

HANDLE openMutex(const std::wstring& name) {
    HANDLE mutex = OpenMutex(SYNCHRONIZE, FALSE, name.c_str());
    if (mutex == NULL) {
        cout << "Error opening mutex";
    }
    return mutex;
}

HANDLE openSemaphore(const std::wstring& name) {
    HANDLE semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, name.c_str());
    if (semaphore == NULL) {
        cout << "Error opening semaphore";
    }
    return semaphore;
}

bool waitForMultipleHandles(HANDLE* handles, int count, DWORD timeout) {
    DWORD result = WaitForMultipleObjects(count, handles, TRUE, timeout);
    return result == WAIT_OBJECT_0;
}

bool waitForSingleHandle(HANDLE handle, DWORD timeout) {
    DWORD result = WaitForSingleObject(handle, timeout);
    return result == WAIT_OBJECT_0;
}