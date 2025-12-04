#include <iostream>
#include <string>
#include <regex>
#include <set>
#include "../Headers/Employee.h"

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

void showEmployees(std::ifstream& in) {
	int fileRecordCount;
	in.read(reinterpret_cast<char*>(&fileRecordCount), sizeof(fileRecordCount));

	std::cout << "Total records: " << fileRecordCount << std::endl;

	Employee emp;
	for (int i = 0; i < fileRecordCount; i++) {
		in.read(reinterpret_cast<char*>(&emp), sizeof(Employee));

		std::cout << "\nEmployee " << i + 1 << ":" << std::endl;
		std::cout << "ID: " << emp.num << std::endl;
		std::cout << "Name: " << emp.name << std::endl;
		std::cout << "Hours: " << emp.hours << std::endl;
	}
}

void getEmployeeData(Employee& emp, int index, std::set<int>& usedIds) {
	bool validInput = false;

	while (!validInput) {
		std::cout << "Enter " << index + 1 << " employee ID: \n";

		int id;
		if (enter_ID_of_employee(id)) {
			if (usedIds.find(id) == usedIds.end()) {
				emp.num = id;
				usedIds.insert(id);
				validInput = true;
			}
			else {
				std::cout << "Error: ID " << id << " is already used. Please enter a unique ID.\n";
			}
		}
	}

	validInput = false;
	while (!validInput) {
		std::cout << "Enter employee name: \n";
		std::cin.getline(emp.name, 11);

		if (strlen(emp.name) > 0) {
			validInput = true;
		}
		else {
			std::cout << "Error: Name cannot be empty.\n";
		}
	}
}