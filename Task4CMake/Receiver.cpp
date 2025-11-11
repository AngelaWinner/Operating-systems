#include <iostream>
#include <windows.h>
#include <string>
#include <regex>
#include <fstream>
using std::cin;
using std::cout;
using std::fstream;
using std::string;

const int twenty = 20;

void getNumber(int& numberOfNotes, string welcome) {
	std::regex numberRegex("^[1-9][0-9]*$");
	string numberStr;
	while (true) {
		cout << welcome;
		if (cin >> numberStr) {
			if (std::regex_match(numberStr, numberRegex)) {
				numberOfNotes = stoi(numberStr);
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

int main()
{
	int readPosition = 0;
	string binaryFileName;
	std::regex filenameRegex("^[a-zA-Z0-9_-]+$");
	while (true) {
		cout << "Enter binary file name: \n";
		cin >> binaryFileName;
		if (std::regex_match(binaryFileName, filenameRegex)) {
			break;
		}
		else {
			cout << "Error: Invalid file name. ";
			cout << "Only letters, numbers, _ and - sumbols are allowed.\n";
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
	}
	int numberOfNotes;
	string numberWelcome = "Enter amount of notes: \n";
	getNumber(numberOfNotes, numberWelcome);

	fstream file;
	file.open(binaryFileName, std::ios::out | std::ios::binary | std::ios::trunc);
	file.close();

	int numberOfSenders;
	string SendersWelcome = "Enter amount of senders processes: \n";
	getNumber(numberOfSenders, SendersWelcome);

	HANDLE hMutex = CreateMutex(NULL, 0, "MyMutex");
	HANDLE hNotesAmountSemaphore = CreateSemaphore(NULL, 0, numberOfNotes, "MyNotesAmountSemaphore");
	if (hNotesAmountSemaphore == NULL)
	{
		cout << "Error creating notes semaphore: " << GetLastError() << std::endl;
		return GetLastError();
	}
	HANDLE hNotNotesAmountSemaphore = CreateSemaphore(NULL, numberOfNotes, numberOfNotes, "MyNotNotesAmountSemaphore");
	if (hNotNotesAmountSemaphore == NULL)
	{
		cout << "Error creating space semaphore: " << GetLastError() << std::endl;
		return GetLastError();
	}

	string SenderString;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE* hEventStarted = new HANDLE[numberOfSenders];
	for (int i = 0; i < numberOfSenders; i++)
	{
		string eventName = "MyStartEvent" + std::to_string(i); //                            Sender' 
		std::wstring wideEventName = std::wstring(eventName.begin(), eventName.end());

		hEventStarted[i] = CreateEventW(NULL, FALSE, FALSE, wideEventName.c_str());
		if (hEventStarted[i] == NULL)
		{
			cout << "Error creating event " << i << ": " << GetLastError() << std::endl;
			return GetLastError();
		}
	}
	for (int i = 0; i < numberOfSenders; i++)
	{
		SenderString = "cmd.exe /c start Sender.exe " + binaryFileName + " " + std::to_string(i); //                
		//std::wstring SenderLPWSTRConverting = std::wstring(SenderString.begin(), SenderString.end());
		//wchar_t* commandLine = &SenderLPWSTRConverting[0];

		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);

		if (!CreateProcess(SenderString.c_str(), NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
		{
			std::cout << "The Sender process is not created.\n";
			return GetLastError();
		}
		CloseHandle(pi.hProcess);
	}
	cout << "Waiting for all senders to start..." << std::endl;
	WaitForMultipleObjects(numberOfSenders, hEventStarted, TRUE, INFINITE); //        
	cout << "All senders started successfully!" << std::endl;

	int choice;
	std::regex choiceRegex("^[01]$");
	string choiceStr;
	while (true)
	{
		cout << "\nChoose your command: \n";
		cout << "Press \"1\" to  read message \n";
		cout << "Press \"0\" to exit\n";
		cin >> choiceStr;
		if (std::regex_match(choiceStr, choiceRegex)) {
			choice = std::stoi(choiceStr);
			if (choice == 0)
			{
				cout << "The process is finished.";
				break;
			}
			if (choice == 1)
			{
				WaitForSingleObject(hNotesAmountSemaphore, INFINITE);
				WaitForSingleObject(hMutex, INFINITE);

				file.open(binaryFileName, std::ios::in | std::ios::binary);
				if (!file.is_open()) {
					cout << "Error opening file for reading" << std::endl;
				}
				else {

					char buffer[twenty] = { 0 };
					file.seekg(readPosition);
					file.read(buffer, twenty);
					std::cout << "Message: " << buffer << std::endl;

					readPosition += twenty;
					if (readPosition >= numberOfNotes * twenty) {
						readPosition = 0;
					}
				}

				ReleaseSemaphore(hNotNotesAmountSemaphore, 1, NULL);
				ReleaseMutex(hMutex);
			}
		}
		else
		{
			cout << "Incorrect input\nTry again...";
		}
	}

	for (int i = 0; i < numberOfSenders; i++)
	{
		CloseHandle(hEventStarted[i]);
	}
	CloseHandle(hNotesAmountSemaphore);
	CloseHandle(hNotNotesAmountSemaphore);
	CloseHandle(hMutex);
	return 0;
}