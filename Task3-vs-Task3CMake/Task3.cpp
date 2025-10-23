#include <windows.h>
#include <iostream>
#include <vector>
#include <random>

int* array;
int arraySize;
int markerThreadsCount;

CRITICAL_SECTION cs;
HANDLE startEvent;
HANDLE* cantContinueEvents;
HANDLE* continueEvents;
bool* closeThreadFlags;

struct markerThreadParams {
    int index;
    markerThreadParams(int indexX) : index(indexX) {};
};

DWORD WINAPI marker(LPVOID param) {
    markerThreadParams* params = (markerThreadParams*)param;
    int threadNumber = params->index;
    int threadIndex = threadNumber - 1;
    int five = 5;
    WaitForSingleObject(startEvent, INFINITE);
    srand(threadNumber);

    int markedElementCount = 0;
    std::vector<int> markedIndices;

    while (true) {
        int randomNumber = rand() % arraySize;

        EnterCriticalSection(&cs);
        if (array[randomNumber] == 0) {
            Sleep(five);
            array[randomNumber] = threadNumber;
            markedElementCount++;
            markedIndices.push_back(randomNumber);
            Sleep(five);
            LeaveCriticalSection(&cs);
            continue;
        }
        if (array[randomNumber] != 0) {
            std::cout << "Thread with threadIndex " << threadNumber
                << ", marked " << markedElementCount
                << " elements, thread cant mark index " << randomNumber << std::endl;
            LeaveCriticalSection(&cs);

            SetEvent(cantContinueEvents[threadIndex]);
            WaitForSingleObject(continueEvents[threadIndex], INFINITE);

            if (closeThreadFlags[threadIndex]) {
                EnterCriticalSection(&cs);
                for (int index : markedIndices) {
                    array[index] = 0;
                }
                LeaveCriticalSection(&cs);
                delete params;
                return 0;
            }
        }
    }
    delete params;
    return 0;
}

int main() {
    InitializeCriticalSection(&cs);

    std::cout << "Enter size of array : ";
    try {
        if (!(std::cin >> arraySize)) {
            throw std::runtime_error("Invalid input: not a number");
        }
        if (arraySize <= 0) {
            std::cout << "Uncorrect enter. Try another time :)" << std::endl;
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    array = new int[arraySize]();

    std::cout << "Enter number of marker threads : ";
    try {
        if (!(std::cin >> markerThreadsCount)) {
            throw std::runtime_error("Invalid input: not a number :(");
        }
        if (markerThreadsCount <= 0) {
            std::cout << "Uncorrect enter. Try another time :)" << std::endl;
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    cantContinueEvents = new HANDLE[markerThreadsCount];
    continueEvents = new HANDLE[markerThreadsCount];
    closeThreadFlags = new bool[markerThreadsCount]();

    HANDLE* markerThreads = new HANDLE[markerThreadsCount];

    for (int i = 0; i < markerThreadsCount; i++) {
        cantContinueEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        continueEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        closeThreadFlags[i] = false;

        markerThreadParams* params = new markerThreadParams(i+1);
        markerThreads[i] = CreateThread(NULL, 0, marker, params, 0, NULL);
    }

    SetEvent(startEvent);

    int activeThreadsCount = markerThreadsCount;
    while (activeThreadsCount > 0) {
        std::vector<HANDLE> activeCantContinueEvents;
        for (int i = 0; i < markerThreadsCount; i++) {
            if (!closeThreadFlags[i]) {
                activeCantContinueEvents.push_back(cantContinueEvents[i]);
            }
        }

        WaitForMultipleObjects(activeCantContinueEvents.size(),
            activeCantContinueEvents.data(),
            TRUE, INFINITE);

        EnterCriticalSection(&cs);
        std::cout << "Array contents : ";
        for (int i = 0; i < arraySize; i++) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;
        LeaveCriticalSection(&cs);
        int threadNumberToClose;
        do {
            std::cout << "Enter thread index to close (1-" << markerThreadsCount << ") : ";
            std::cin >> threadNumberToClose;
        } while (threadNumberToClose < 1 || threadNumberToClose > markerThreadsCount);
        int threadIndexToClose = threadNumberToClose-1;
        if (!closeThreadFlags[threadIndexToClose]) {

            closeThreadFlags[threadIndexToClose] = true;
            SetEvent(continueEvents[threadIndexToClose]);
            WaitForSingleObject(markerThreads[threadIndexToClose], INFINITE);
            
            activeThreadsCount--;

            EnterCriticalSection(&cs);
            std::cout << "Array after thread " << threadNumberToClose << " closing : ";
            for (int i = 0; i < arraySize; i++) {
                std::cout << array[i] << " ";
            }
            std::cout << std::endl;
            LeaveCriticalSection(&cs);

            for (int i = 0; i < markerThreadsCount; i++) {
                if (!closeThreadFlags[i]) {
                    ResetEvent(cantContinueEvents[i]);
                    SetEvent(continueEvents[i]);
                }
            }
        }
        else {
            std::cout << "Thread have already been closed!" << std::endl;

            for (int i = 0; i < markerThreadsCount; i++) {
                if (!closeThreadFlags[i]) {
                    ResetEvent(cantContinueEvents[i]);
                    SetEvent(continueEvents[i]);
                }
            }
        }
    }

    for (int i = 0; i < markerThreadsCount; i++) {
        CloseHandle(markerThreads[i]);
        CloseHandle(cantContinueEvents[i]);
        CloseHandle(continueEvents[i]);
    }
    CloseHandle(startEvent);
    DeleteCriticalSection(&cs);
    delete[] array;
    delete[] markerThreads;
    delete[] cantContinueEvents;
    delete[] continueEvents;
    delete[] closeThreadFlags;
    std::cout << "All threads completed. Program finished." << std::endl;
    return 0;
}