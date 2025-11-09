#include <fstream>
#include <windows.h>
#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
using std::cin;
using std::cout;
using std::fstream;
using std::string;

const int twenty = 20;
int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << "Usage: Sender.exe <binaryFileName> <senderIndex>\n";
        return 1;
    }
    string binaryFileName = argv[1];
    int senderIndex = std::stoi(argv[2]);

    string eventName = "MyStartEvent" + std::to_string(senderIndex);
    //std::wstring wideEventName = std::wstring(eventName.begin(), eventName.end());

    HANDLE hEventStarted = OpenEvent(EVENT_MODIFY_STATE, FALSE, eventName.c_str());
    if (hEventStarted == NULL)
    {
        cout << "Error opening event: " << GetLastError() << std::endl;
        return GetLastError();
    }

    HANDLE hNotesAmountSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "MyNotesAmountSemaphore");
    if (hNotesAmountSemaphore == NULL)
    {
        return GetLastError();
    }
    HANDLE hNotNotesAmountSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "MyNotNotesAmountSemaphore");
    if (hNotNotesAmountSemaphore == NULL)
    {
        return GetLastError();
    }
    HANDLE hMutex = OpenMutex(SYNCHRONIZE, FALSE, "MyMutex");

    SetEvent(hEventStarted);
    cout << "Sender " << senderIndex << " started successfully!" << std::endl;

    int choice;
    std::regex choiceRegex("^[01]$");
    string choiceStr;
    fstream file;
    while (true)
    {
        cout << "\nChoose your command: \n";
        cout << "Press \"1\" to  write message \n";
        cout << "Press \"0\" to exit\n";
        getline(cin, choiceStr);
        if (std::regex_match(choiceStr, choiceRegex)) {
            choice = std::stoi(choiceStr);
            if (choice == 0)
            {
                cout << "The process is finished.";
                break;
            }
            if (choice == 1)
            {
                cout << "Waiting for free space...\n";
                DWORD waitResult = WaitForSingleObject(hNotNotesAmountSemaphore, INFINITE);
                if (waitResult != WAIT_OBJECT_0) {
                    cout << "Error waiting for semaphores: " << GetLastError() << std::endl;
                    continue;
                }

                waitResult = WaitForSingleObject(hMutex, INFINITE);
                if (waitResult != WAIT_OBJECT_0) {
                    cout << "Error waiting for mutex: " << GetLastError() << std::endl;
                    ReleaseSemaphore(hNotNotesAmountSemaphore, 1, NULL);
                    continue;
                }

                cout << "Enter message: ";
                string inputMessage;
                getline(cin, inputMessage);

                if (inputMessage.empty()) {
                    cout << "Empty message, skipping...\n";
                    ReleaseSemaphore(hNotNotesAmountSemaphore, 1, NULL);
                    ReleaseMutex(hMutex);
                    continue;
                }

                if (inputMessage.size() >= twenty)
                {
                    inputMessage = inputMessage.substr(0, twenty - 1);
                }

                file.open(binaryFileName, std::ios::out | std::ios::app | std::ios::binary);
                if (!file.is_open()) {
                    cout << "Error opening file for writing\n";
                    ReleaseSemaphore(hNotNotesAmountSemaphore, 1, NULL);
                }
                else {
                    file.write(inputMessage.c_str(), twenty);
                    file.close();
                    ReleaseSemaphore(hNotesAmountSemaphore, 1, NULL);
                    cout << "Message sent: '" << inputMessage << "'\n";
                }
                ReleaseMutex(hMutex);
            }
        }
        else {
            cout << "Incorrect input\nTry again...\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
    CloseHandle(hEventStarted);
    CloseHandle(hNotesAmountSemaphore);
    CloseHandle(hNotNotesAmountSemaphore);
    CloseHandle(hMutex);

    return 0;
}