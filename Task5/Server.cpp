#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include "../Headers/Employee.h"
#include "../Headers/Functions.h"
using std::string;

std::string fileName;
int numberOfEmployees;
Employee* employees;
int numberOfClients;
HANDLE* hSemaphore;
HANDLE* hStartedEvent;
HANDLE* hPipe;
HANDLE* hThreads;
PROCESS_INFORMATION* piArray;

DWORD WINAPI messaging(LPVOID pipe)
{
	HANDLE hPipe = (HANDLE)pipe;
	DWORD dwBytesRead;
	DWORD dwBytesWrite;

	int message;
	int chosenOption;
	Employee* employeeToShare = nullptr;
	bool successWritening;
	int ID;
	bool everythingIsRight;
	int command;

	while (true)
	{
		if (!ReadFile(hPipe, &message, sizeof(message), &dwBytesRead, NULL))
			return 0;

		ID = message / 10;
		chosenOption = message % 10;
		everythingIsRight = false;

		for (int i = 0; i < numberOfEmployees; i++)
		{
			if (employees[i].num == ID)
			{
				ID = i;
				everythingIsRight = true;
			}
		}

		WriteFile(hPipe, &everythingIsRight, sizeof(everythingIsRight), &dwBytesWrite, NULL);

		if (!everythingIsRight)
			continue;

		if (chosenOption == 1)
		{
			for (int i = 0; i < numberOfClients; i++) {
				WaitForSingleObject(hSemaphore[ID], INFINITE);
			}

			employeeToShare = new Employee();

			employeeToShare->num = employees[ID].num;
			employeeToShare->hours = employees[ID].hours;
			strcpy_s(employeeToShare->name, employees[ID].name);

			successWritening = WriteFile(hPipe, employeeToShare, sizeof(Employee), &dwBytesWrite, NULL);

			if (successWritening) std::cout << "Data to modify was sent.\n";
			else std::cout << "Data to modify wasn't sent.\n";

			ReadFile(hPipe, employeeToShare, sizeof(Employee), &dwBytesWrite, NULL);

			employees[ID].hours = employeeToShare->hours;
			strcpy_s(employees[ID].name, employeeToShare->name);

			/*std::ofstream out(fileName);

			for (int i = 0; i < numberOfEmployees; i++) {
				out << employees[i].num << " " << employees[i].name << " " << employees[i].hours << "\n";
			}

			out.close();*/

			std::ofstream out(fileName, std::ios::binary | std::ios::out);
			if (!out.is_open()) {
				std::cerr << "Error: Failed to open binary file '" << fileName << "' for writing\n";
				
			}
			out.write(reinterpret_cast<const char*>(&numberOfEmployees), sizeof(numberOfEmployees));
			for (int i = 0; i < numberOfEmployees; i++) {
				out.write(reinterpret_cast<const char*>(&employees[i]), sizeof(Employee));

				if (!out.good()) {
					std::cerr << "Error: Failed to write employee #" << i + 1 << " to binary file\n";
					out.close();
					
				}
			}
			out.close();


			ReadFile(hPipe, &command, sizeof(command), &dwBytesWrite, NULL);

			if (command == 1)
				for (int i = 0; i < numberOfClients; i++)
					ReleaseSemaphore(hSemaphore[ID], 1, NULL);
		}
		else if (chosenOption == 2)
		{
			WaitForSingleObject(hSemaphore[ID], INFINITE);

			employeeToShare = new Employee();
			employeeToShare->num = employees[ID].num;
			employeeToShare->hours = employees[ID].hours;
			strcpy_s(employeeToShare->name, employees[ID].name);

			successWritening = WriteFile(hPipe, employeeToShare, sizeof(Employee), &dwBytesWrite, NULL);

			if (successWritening) std::cout << "Data to read was sent.\n";
			else std::cout << "Data to read wasn't sent.\n";

			ReadFile(hPipe, &command, sizeof(command), &dwBytesWrite, NULL);

			if (command == 1)
				ReleaseSemaphore(hSemaphore[ID], 1, NULL);
		}
	}

	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);
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

void cleanUp() {
	if (hSemaphore != nullptr) {
		for (int i = 0; i < numberOfEmployees; i++) {
			if (hSemaphore[i] != NULL && hSemaphore[i] != INVALID_HANDLE_VALUE) {
				CloseHandle(hSemaphore[i]);
				hSemaphore[i] = NULL;
			}
		}
		delete[] hSemaphore;
		hSemaphore = nullptr;
	}

	if (hStartedEvent != nullptr) {
		for (int i = 0; i < numberOfClients; i++) {
			if (hStartedEvent[i] != NULL && hStartedEvent[i] != INVALID_HANDLE_VALUE) {
				CloseHandle(hStartedEvent[i]);
				hStartedEvent[i] = NULL;
			}
		}
		delete[] hStartedEvent;
		hStartedEvent = nullptr;
	}

	if (hThreads != nullptr) {
		for (int i = 0; i < numberOfClients; i++) {
			if (hThreads[i] != NULL && hThreads[i] != INVALID_HANDLE_VALUE) {
				WaitForSingleObject(hThreads[i], 1000);
				CloseHandle(hThreads[i]);
				hThreads[i] = NULL;
			}
		}
		delete[] hThreads;
		hThreads = nullptr;
	}

	if (hPipe != nullptr) {
		for (int i = 0; i < numberOfClients; i++) {
			if (hPipe[i] != NULL && hPipe[i] != INVALID_HANDLE_VALUE) {
				DisconnectNamedPipe(hPipe[i]);
				CloseHandle(hPipe[i]);
				hPipe[i] = NULL;
			}
		}
		delete[] hPipe;
		hPipe = nullptr;
	}

	if (piArray != nullptr) {
		for (int i = 0; i < numberOfClients; i++) {
			if (piArray[i].hProcess != NULL && piArray[i].hProcess != INVALID_HANDLE_VALUE) {
				CloseHandle(piArray[i].hProcess);
				piArray[i].hProcess = NULL;
			}
			if (piArray[i].hThread != NULL && piArray[i].hThread != INVALID_HANDLE_VALUE) {
				CloseHandle(piArray[i].hThread);
				piArray[i].hThread = NULL;
			}
		}
		delete[] piArray;
		piArray = nullptr;
	}

	if (employees != nullptr) {
		delete[] employees;
		employees = nullptr;
	}
}

int main()
{
	getFileName(fileName);

	string numberOfEmployeesWelcome = "Enter number of employees: \n";
	getNumber(numberOfEmployees, numberOfEmployeesWelcome);

	employees = new Employee[numberOfEmployees];

	std::set<int> usedIds;
	for (int i = 0; i < numberOfEmployees; i++) {
		std::cout << "\n=== Enter data for employee " << i + 1 << " ===\n";
		getEmployeeData(employees[i], i, usedIds);
		string hoursWelcome = "Enter employee hours:\n";
		getDouble(employees[i].hours, hoursWelcome);
	}

	std::ofstream out(fileName, std::ios::binary | std::ios::out);
	if (out.fail()) {
		std::cerr << "\nError: Failed to create or open file '" << fileName << "'\n";

		delete[] employees;
		std::cout << "Press any key to exit...\n";
		_getch();
		return 1;
	}

	out.write(reinterpret_cast<const char*>(&numberOfEmployees), sizeof(numberOfEmployees));

	for (int i = 0; i < numberOfEmployees; i++) {
		//out << employees[i].num << " " << employees[i].name << " " << employees[i].hours << "\n";
		out.write(reinterpret_cast<const char*>(&employees[i]), sizeof(Employee));
	}

	out.close();

	std::ifstream in(fileName, std::ios::binary | std::ios::in);
	if (in.fail()) {
		std::cerr << "\nError: Failed to create or open binary file '" << fileName << "'\n";

		delete[] employees;
		std::cout << "Press any key to exit...\n";
		_getch();
		return 1;
	}

	int ID;
	std::string name;
	double hours;

	int fileRecordCount;
	in.read(reinterpret_cast<char*>(&fileRecordCount), sizeof(fileRecordCount));

	if (fileRecordCount != numberOfEmployees) {
		std::cout << "\nWarning: File contains " << fileRecordCount
			<< " records, expected " << numberOfEmployees << std::endl;
	}

	std::cout << "\n=== Contents of binary file '" << fileName << "' ===" << std::endl;
	std::cout << "Total records: " << fileRecordCount << std::endl;

	Employee emp;
	for (int i = 0; i < fileRecordCount; i++) {
		in.read(reinterpret_cast<char*>(&emp), sizeof(Employee));

		if (in.gcount() != sizeof(Employee)) {
			std::cerr << "Error reading employee #" << i + 1 << std::endl;
			break;
		}

		std::cout << "\nRecord #" << i + 1 << ":" << std::endl;
		std::cout << "ID of employee: " << emp.num << std::endl;
		std::cout << "Name of employee: " << emp.name << std::endl;
		std::cout << "Hours of employee: " << emp.hours << std::endl;
	}

	/*for (int i = 0; i < numberOfEmployees; i++)
	{
		in >> ID >> name >> hours;
		std::cout << "\nID of employee: " << ID << "\nName of employee: " << name << "\nHours of employee: " << hours << "\n";
	}*/

	in.close();

	string numberOfClientsWelcome = "\nEnter number of clients: \n";
	getNumber(numberOfClients, numberOfClientsWelcome);

	hStartedEvent = new HANDLE[numberOfClients];
	hSemaphore = new HANDLE[numberOfEmployees];

	for (int i = 0; i < numberOfEmployees; i++) {
		hSemaphore[i] = CreateSemaphore(NULL, numberOfClients, numberOfClients, L"hSemahpore");
	}

	piArray = new PROCESS_INFORMATION[numberOfClients];
	for (int i = 0; i < numberOfClients; ++i)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		std::string cmd = "Client.exe";
		std::wstring cmdToWString = std::wstring(cmd.begin(), cmd.end());
		LPWSTR clientCmdLine = &cmdToWString[0];
		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);

		CreateProcess(NULL, clientCmdLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piArray[i]);

		hStartedEvent[i] = CreateEvent(NULL, FALSE, FALSE, L"Process Started"); //автоматический сброс, начальное состояние не сигнальное

		//CloseHandle(pi.hProcess);
	}

	WaitForMultipleObjects(numberOfClients, hStartedEvent, TRUE, INFINITE);

	hPipe = new HANDLE[numberOfClients];
	hThreads = new HANDLE[numberOfClients];

	for (int i = 0; i < numberOfClients; i++)
	{
		hPipe[i] = CreateNamedPipe(L"\\\\.\\pipe\\pipe_name", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES,
			0, 0, INFINITE, NULL);

		if (hPipe == INVALID_HANDLE_VALUE)
		{
			std::cout << "Creation of the named pipe failed.\n The last error code: " << GetLastError() << "\n";
			cleanUp();
			std::cout << "Press any char to finish server: ";
			_getch();
			return 0;
		}

		if (!ConnectNamedPipe(hPipe[i], (LPOVERLAPPED)NULL)) //связь синхронная
		{
			std::cout << "The connection failed.\nThe last error code: " << GetLastError() << "\n";
			cleanUp();
			std::cout << "Press any char to finish the server: ";
			_getch();
			return 0;
		}

		hThreads[i] = CreateThread(NULL, 0, messaging, static_cast<LPVOID>(hPipe[i]), 0, NULL);
	}

	WaitForMultipleObjects(numberOfClients, hThreads, TRUE, INFINITE);

	std::cout << "All clients has ended their work.";

	in.open(fileName);
	if (in.fail()) {
		std::cerr << "\nError: Failed to create or open file '" << fileName << "'\n";

		delete[] employees;
		std::cout << "Press any key to exit...\n";
		_getch();
		return 1;
	}

	for (int i = 0; i < numberOfEmployees; i++)
	{
		in >> ID >> name >> hours;
		std::cout << "\nID of employee: " << ID << "\nName of employee: " << name << "\nHours of employee: " << hours << "\n";
	}

	in.close();

	cleanUp();
	std::cout << "Press any char to finish the server: \n";
	_getch();

	return 0;
}