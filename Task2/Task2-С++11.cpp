#include <windows.h>
#include <iostream>
using namespace std;

int* arr = nullptr; //nullptr
size_t arrSize = 0;
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
    for (int i = 0; i < arrSize; ++i)
    {
        arithmeticMean += static_cast<double>(arr[i]) / arrSize; //static_cast<...>
        Sleep(12);
    }
    cout << "Arithmetic mean : " << arithmeticMean << " ~~ " << static_cast<int>(arithmeticMean) << "\n";

    return 0;
}

int main()
{
    int i;
    cout << "Eneter size of array : ";
    cin >> arrSize;
    arr = new int[arrSize];

    cout << "Enter " << arrSize << " elements of array :\n";
    for (i = 0; i < arrSize; i++)
    {
        cin >> arr[i];
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
            arr[i] = static_cast<int>(arithmeticMean);
        }
    }

    cout << "Changed array : ";
    std::for_each(arr, arr + arrSize, [](int element) { //foreach
        std::cout << element << " ";
        });
    cout << "\n";

    delete[] arr;
    return 0;
}