#include <conio.h>
#include <windows.h>
#include <iostream>
#include "../Headers/Employee.h"
#include "../Headers/Functions.h"

int main(int argc, char* argv)
{
	int chosenOption;
	DWORD dwBytesWritten;
	DWORD dwBytesReaden;
	int ID;
	int commandToSend;
	bool successWritening;
	Employee* employee = nullptr;
	

	HANDLE hStartEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"Process Started");

	if (hStartEvent == NULL)
	{
		std::cout << "Open event failed. \nEnter any char to exit.\n";
		_getch();
		return GetLastError();
	}
	SetEvent(hStartEvent);

	HANDLE hPipe = CreateFile(L"\\\\.\\pipe\\pipe_name", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		std::cout << "Creation of the named pipe failed.\n The last error code: " << GetLastError() << "\n";
		std::cout << "Press any char to finish server: ";
		_getch();
		return 0;
	}

	std::string chosenOptionStr;
	std::regex chosenOptionRegex("^[123]$");

	while (true)
	{
		ID = 0;
		std::cout << "Choose option:\n 1. Modify data\n 2. Read data\n 3. Exit\n";
		std::cin >> chosenOptionStr;
		if (std::regex_match(chosenOptionStr, chosenOptionRegex)) {
			chosenOption = std::stoi(chosenOptionStr);

			if (chosenOption == 1)
			{
				std::cout << "Enter ID of employee: \n";
				bool enter = enter_ID_of_employee(ID);

				commandToSend = ID * 10 + chosenOption;

				successWritening = WriteFile(hPipe, &commandToSend, sizeof(commandToSend), &dwBytesWritten, NULL);

				if (successWritening) std::cout << "Message was sent.\n";
				else std::cout << "Message wasn't sent.\n";

				bool writeningIsCorrect;

				ReadFile(hPipe, &writeningIsCorrect, sizeof(writeningIsCorrect), &dwBytesReaden, NULL);

				if (!writeningIsCorrect)
				{
					std::cout << "ID is incorrect. Try again.\n";
					continue;
				}

				employee = new Employee();

				if (!ReadFile(hPipe, employee, sizeof(Employee), &dwBytesReaden, NULL))
				{
					std::cout << "Data reading from the named pipe failed.\n" << "The last error code: " << GetLastError() << "\n";
					std::cout << "Press any char to finish server: \n";
					_getch();
					return 0;
				}

				std::cout << "ID of employee: " << employee->num << "\nName of employee: " << employee->name << "\nHours of employee: " << employee->hours << "\n";

				std::cout << "Enter new Name:\n";
				std::cin.getline(employee->name, 11);

				std::string hoursWelcome = "Enter employee hours:\n";
				getDouble(employee->hours, hoursWelcome);

				std::cout << "Press any key to send modified record to Server\n";
				_getch();

				successWritening = WriteFile(hPipe, employee, sizeof(Employee), &dwBytesWritten, NULL);

				if (successWritening) std::cout << "Message was sent.\n";
				else std::cout << "Message wasn't sent.\n";

				std::cout << "Press any key to end the selected option\n";
				_getch();

				commandToSend = 1;
				WriteFile(hPipe, &commandToSend, sizeof(commandToSend), &dwBytesWritten, NULL);
			}
			else if (chosenOption == 2)
			{
				std::cout << "Enter ID of employee: \n";
				bool enter = enter_ID_of_employee(ID);

				commandToSend = ID * 10 + chosenOption;
				successWritening = WriteFile(hPipe, &commandToSend, sizeof(commandToSend), &dwBytesWritten, NULL);

				if (successWritening) std::cout << "Message was sent.\n";
				else std::cout << "Message wasn't sent.\n";

				bool writeningIsCorrect;

				ReadFile(hPipe, &writeningIsCorrect, sizeof(writeningIsCorrect), &dwBytesReaden, NULL);

				if (!writeningIsCorrect)
				{
					std::cout << "ID is incorrect. Try again.\n";
					continue;
				}

				employee = new Employee();

				if (!ReadFile(hPipe, employee, sizeof(Employee), &dwBytesReaden, NULL))
				{
					std::cout << "Data reading from the named pipe failed.\n" << "The last error code: " << GetLastError() << "\n";
					std::cout << "Press any char to finish server: \n";
					_getch();
					return 0;
				}

				std::cout << "ID of employee: " << employee->num << "\nName of employee: " << employee->name << "\nHours of employee: " << employee->hours << "\n";
				std::cout << "Press any key to end the selected option\n";
				_getch();

				commandToSend = 1;
				WriteFile(hPipe, &commandToSend, sizeof(commandToSend), &dwBytesWritten, NULL);
			}
			else  if (chosenOption == 3) break;
		}
		else {
			std::cout << "Incorrect input\nTry again...";
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}

	return 0;
}
