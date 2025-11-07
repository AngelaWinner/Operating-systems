#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <regex>
#include <cstring>
#include "../Headers/Employee.h"
//#include "../Headers/Functions.h"
void getString(std::string& numberOfNotes, std::string welcome, std::regex regex) {
	std::string numberStr;
	while (true) {
		std::cout << welcome;
		if (std::cin >> numberStr) {
			if (std::regex_match(numberStr, regex)) {
				numberOfNotes = numberStr;
				break;
			}
			else {
				std::cout << "Error: Input must be correct.\n";
			}
		}
		else {
			std::cout << "Error: Please enter a valid input.\n";
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}
}
void getNumber(int& number, std::string welcome) {
	std::string numberStr;
	std::regex numberRegex("^[1-9][0-9]*$");
	while (true) {
		std::cout << welcome;
		if (std::cin >> numberStr) {
			if (std::regex_match(numberStr, numberRegex)) {
				number = std::stoi(numberStr);
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
Employee createEmployee() {
	std::string numberWelcome= "Enter employee's number\n";
	int num;
	getNumber(num, numberWelcome);
	std::string hoursWelcome = "Enter employee's hours\n";
	int hours;
	getNumber(hours, hoursWelcome);
	std::string nameWelcome = "Enter employee's name (9 symbols, other will be ignored)\n";
	std::string name;
	std::regex nameRegex("^[a-zA-Z0-9]+$");
	getString(name, nameWelcome, nameRegex);
	char nameArray[10];
	int i = 0;
	for (; i < 9 && i < name.length(); i++) {
		nameArray[i] = name[i];
	}
	nameArray[i] = '\0';
	return Employee(num, nameArray, hours);
}
int four = 4;
int main(int argc, char* argv[]) {
	std::string binFileName = argv[1];
	
	int notesNumer;
	try {
		notesNumer = std::stoi(argv[2]);
	}
	catch (const std::invalid_argument& e) {
		std::cout << "the number is not a number, lets make it equal to " << four << ".\n";
		notesNumer = four;
	}
	catch (const std::out_of_range& e) {
		std::cout << "the number is too big, lets make it smaller and equal to " << four << ".\n";
		notesNumer = four;
	}

	std::ofstream file;

	file.open(binFileName, std::ios::binary);
	if (file.is_open()) {
		std::cout << "Enter number, name, hours of employee :\n";
		for (int i = 0; i < notesNumer; i++)
		{
			std::cout << "Enter employee " << i + 1 << ":\n";
			Employee employee = createEmployee();
			file.write((char*)&employee, sizeof(Employee));
		}
	}
	else {
		std::cerr << "Error - cant open file with name : " << binFileName << std::endl;
		return -1;
	}
	file.close();
	return 0;
}