#include <iostream>
#include <string>
#include <regex>
#include <set>

bool enter_ID_of_employee(int& ID) {
	std::string idStr;
	std::cin >> idStr;
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');

	try {
		ID = std::stoi(idStr);
		if (ID <= 0) {
			std::cout << "Error: ID must be positive number.\n";
			return false;
		}
		return true;
	}
	catch (const std::exception&) {
		std::cout << "Error: Please enter a valid integer number.\n";
		return false;
	}
}

void getNumber(int& numberOfNotes, std::string welcome) {
	std::regex numberRegex("^[1-9][0-9]*$");
	std::string numberStr;
	while (true) {
		std::cout << welcome;
		if (std::cin >> numberStr) {
			if (std::regex_match(numberStr, numberRegex)) {
				numberOfNotes = stoi(numberStr);
				break;
			}
			else {
				std::cout << "Error: Number must be positive number.\n";
			}
		}
		else {
			std::cout << "Error: Please enter a valid integer positive number.\n";
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}
}

void getDouble(double& number, const std::string& welcome) {
	std::regex doubleRegex("^[0-9]+(\.[0-9]+)?$");
	std::string numberStr;
	while (true) {
		std::cout << welcome;
		if (std::cin >> numberStr) {
			if (std::regex_match(numberStr, doubleRegex)) {
				try {
					number = std::stod(numberStr);
					if (number > 0) {
						break;
					}
					else {
						std::cout << "Error: Number must be positive.\n";
					}
				}
				catch (const std::exception&) {
					std::cout << "Error: Invalid number format.\n";
				}
			}
			else {
				std::cout << "Error: Please enter a valid positive number.\n";
			}
		}
		else {
			std::cout << "Error: Please enter a valid number.\n";
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}
}

void getFileName(std::string& fileName) {
	std::regex filenameRegex("^[a-zA-Z0-9_-]+$");
	while (true) {
		std::cout << "Enter file name: \n";
		std::cin >> fileName;
		if (std::regex_match(fileName, filenameRegex)) {
			break;
		}
		else {
			std::cout << "Error: Invalid file name. ";
			std::cout << "Only letters, numbers, _ and - symbols are allowed.\n";
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}
}