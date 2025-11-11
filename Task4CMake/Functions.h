#include <iostream>
#include <regex>
#include <fstream>

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