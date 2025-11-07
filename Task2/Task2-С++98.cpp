#include <windows.h>
#include <iostream>
using namespace std;

int* arr = NULL; //NULL
int arrSize = 0;
int maxValue = 0;
int minValue = 0;
double arithmeticMean = 0.0;

DWORD WINAPI min_max(LPVOID)
{
    maxValue = arr[0];
    minValue = arr[0];

    for (int i = 0; i < arrSize; i++)
    {
        if (arr[i] <= minValue)
        {
            minValue = arr[i];
        }
        Sleep(7);
        if (arr[i] >= maxValue)
        {
            maxValue = arr[i];
        }
        Sleep(7);
    }

    cout << "Minimum element of array : " << minValue << "\n";
    cout << "Maximum element of array : " << maxValue << "\n";

    return 0;
}

DWORD WINAPI average(LPVOID)
{
    for (int i = 0; i < arrSize; i++)
    {
        arithmeticMean += (double)arr[i] / arrSize; //C-style
        Sleep(12);
    }
    cout << "Arithmetic mean : " << arithmeticMean << " ~~ " << (int)arithmeticMean << "\n";

    return 0;
}

int main()
{
    int i;
    cout << "Eneter size of array : ";
    if (!(std::cin >> arrSize)) {
        std::cerr << "Invalid input. Exiting." << std::endl;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        return 1;
    }
    arr = new int[arrSize];

    cout << "Enter " << arrSize << " elements of array :\n";
    for (i = 0; i < arrSize; i++)
    {
        if (!(std::cin >> arr[i])) {
            std::cerr << "Invalid input. Exiting." << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            return 1;
        }
    }

    HANDLE min_max_hThread;
    DWORD  min_max_IDThread;
    HANDLE average_hThread;
    DWORD average_IDThread;

    min_max_hThread = CreateThread(NULL, 0, min_max, NULL, 0, &min_max_IDThread);
    average_hThread = CreateThread(NULL, 0, average, NULL, 0, &average_IDThread);

    if (min_max_hThread == NULL || average_hThread == NULL)
    {
        return GetLastError();
    }

    WaitForSingleObject(average_hThread, INFINITE);
    WaitForSingleObject(min_max_hThread, INFINITE);

    CloseHandle(average_hThread);
    CloseHandle(min_max_hThread);

    for (i = 0; i < arrSize; i++)
    {
        if (arr[i] == maxValue || arr[i] == minValue)
        {
            arr[i] = (int)arithmeticMean;
        }
    }

    cout << "Changed array : ";
    for (i = 0; i < arrSize; i++)
    {
        cout << arr[i] << " ";
    }
    cout << "\n";

    delete[] arr;
    return 0;
}