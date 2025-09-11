#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <conio.h>
#include "../Headers/Employee.h"

int main()
{
	std::string binFileName;
	std::string notesNumber;

	std::cout << "Enter name of a binary file : ";
	std::cin >> binFileName;
	std::cout << "Eneter a number of notes : ";
	std::cin >> notesNumber;

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
	outputFile.close();

	_cputs("Press any button to finish.\n");
	_getch();
	
	return 0;
}