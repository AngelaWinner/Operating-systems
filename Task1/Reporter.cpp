#include <fstream>
#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <exception>
#include <string>
#include <algorithm>
#include "../Headers/Employee.h"

int thirty = 30;
int main(int argc, char* argv[]) {
	std::string inputFileName = argv[1];
	std::string outputFileName = argv[2];

	int paymentPerHour;
	try {
		paymentPerHour = std::stoi(argv[3]);
	}
	catch (const std::invalid_argument& e) {
		std::cout << "the payment is not a number, lets make it equal to " << thirty << ".\n";
		paymentPerHour = thirty;
	}
	catch (const std::out_of_range& e) {
		std::cout << "the payment is too big, lets make it smaller and equal to " << thirty << ".\n";
		paymentPerHour = thirty;
	}

	std::ifstream inputFile;
	std::ofstream outputFile;
	std::vector<Employee> employees;
	Employee employ;

	_cputs("Reporter is open and now executing.\n");

	inputFile.open(inputFileName, std::ios::binary);
	if (inputFile.is_open()) {
		while (inputFile.read((char*)&employ, sizeof(Employee))) {
			employees.push_back(employ);
		}
		if (inputFile.eof()) {
			std::cout << "The end of file is reached." << std::endl;
		}
		else if (inputFile.fail()) {
			std::cerr << "Error in reading file." << std::endl;
		}
	}
	else {
		std::cerr << "Error - cant open file with name : " << inputFileName << std::endl;
		return -1;
	}
	inputFile.close();
	
	std::sort(employees.begin(), employees.end(),
		[](const Employee& a, const Employee& b) {
			return strcmp(a.name, b.name) < 0;
		}
	);

	outputFile.open(outputFileName);
	if (outputFile.is_open()) {

		outputFile << "Report from the file with name " << inputFileName << " :\n";
		outputFile << "Number" << "\t" << "Name" << "\t" << "Hours" << "\t" << "Payment :\n";
		for (int i = 0; i < employees.size(); i++)
		{
			outputFile << employees[i].num << "\t" << employees[i].name << "\t" << employees[i].hours
				<< "\t" << paymentPerHour * employees[i].hours << "\n";
		}
		_cputs("The data was successfully documented.\n");
	}
	else {
		std::cerr << "Error - cant open file with name : " << outputFileName << std::endl;
		return -1;
	}
	outputFile.close();
	_cputs("Press any button to finish\n");
	_getch();
	return 0;
}