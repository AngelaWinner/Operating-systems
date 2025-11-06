#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include "../Headers/Employee.h"

int four = 4;
int main(int argc, char* argv[]) {
	Employee employee;
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
			std::cin >> employee.num >> employee.name >> employee.hours;
			file.write((char*)&employee, sizeof(Employee));
		}
	}
	file.close();
	return 0;
}