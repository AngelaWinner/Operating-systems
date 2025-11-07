#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <regex>
#include <conio.h>
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
int main()
{
	std::regex filenameRegex("^[a-zA-Z0-9_-]+$");
	std::regex numberRegex("^[1-9][0-9]*$");
	std::string binFileName;
	std::string notesNumber;
	std::string binFileWelcome = "Enter name of a binary file : ";
	std::string notesNumberWelcome = "Eneter a number of notes : ";
	getString(binFileName, binFileWelcome, filenameRegex);
	getString(notesNumber, notesNumberWelcome, numberRegex);

	std::string creatorString = "Creator.exe " + binFileName + " " + notesNumber;
	std::wstring creatorLPWSTR = std::wstring(creatorString.begin(), creatorString.end());
	LPWSTR lpszCreatorProcess = &creatorLPWSTR[0];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(NULL, lpszCreatorProcess, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		_cputs("The Creator process is not running.\n");
		return GetLastError();
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	std::ifstream inputFile;
	std::vector<Employee> employees;
	Employee employ;

	inputFile.open(binFileName, std::ios::binary);
	if (inputFile.is_open()) {
		Employee employ;
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
		std::cerr << "Error - cant open file with name : " << binFileName << std::endl;
		return -1;
	}
	inputFile.close();

	std::cout << "Information : \nNumber\tName\tHours\n";
	for (int i = 0; i < employees.size(); ++i)
	{
		std::cout << employees[i].num << "\t" << employees[i].name << "\t" << employees[i].hours << "\n";
	}

	std::string textFileName;
	std::string paymentPerHour;

	std::cout << "Enter name of the report file : ";
	std::cin >> textFileName;
	std::cout<<"Enter the payment per hour : ";
	std::cin >> paymentPerHour;
	std::cout << "\n";

	std::string reporterString = "Reporter.exe " + binFileName + " " + textFileName + " " + paymentPerHour;
	std::wstring reporterLPWSTR = std::wstring(reporterString.begin(), reporterString.end());
	LPWSTR lpszReporterProcess = &reporterLPWSTR[0];

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcessW(NULL, lpszReporterProcess, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		_cputs("The Reporter process is not running.\n");
		return GetLastError();
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	std::ifstream outputFile;
	outputFile.open(textFileName);
	if (outputFile.is_open()) {
		std::string line;
		while (std::getline(outputFile, line)) {
			std::cout << line << "\n";
		}
	}
	else {
		std::cerr << "Error - cant open file with name : " << textFileName << std::endl;
		return -1;
	}
	outputFile.close();

	_cputs("Press any button to finish.\n");
	_getch();
	
	return 0;
}