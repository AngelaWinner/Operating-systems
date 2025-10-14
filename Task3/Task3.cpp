#include <windows.h>
#include <iostream>
#include <vector>
#include <random>
#include <mutex>

std::vector<int> array;
int arraySize;
int markerThreadsCount;
std::mutex myMutex;

HANDLE startEvent;
std::vector<HANDLE> cantContinueEvents;
std::vector<HANDLE> continueEvents;
std::vector<bool> closeThreadFlags;
struct markerThreadParams {
    int index;
    markerThreadParams(int indexX) : index(indexX) {};
};

DWORD WINAPI marker(LPVOID param) {
    markerThreadParams* params = (markerThreadParams*)param;
    int threadIndex = params->index;

    int five = 5;
    WaitForSingleObject(startEvent, INFINITE);
    srand(threadIndex);

    int markedElementCount = 0;
    std::vector<int> markedIndices;
    int counter = 0;
    while (counter <= 2 * arraySize) {
        int randomNumber = rand() % arraySize;
        counter++;
        myMutex.lock(); // Захватываю мьютекс
        if (array[randomNumber] == 0) {
            Sleep(five);
            array[randomNumber] = threadIndex;
            markedElementCount++;
            markedIndices.push_back(randomNumber);
            Sleep(five);
            myMutex.unlock(); // Освобождаю мьютекс
        }
        else {
            std::cout << "Thread with threadIndex " << threadIndex
                << ", marked " << markedElementCount
                << " elements, thread cant mark index " << randomNumber << std::endl;
            myMutex.unlock();

            SetEvent(cantContinueEvents[threadIndex]);
            WaitForSingleObject(continueEvents[threadIndex], INFINITE);
            //markedElementCount = 0;
            
            if (closeThreadFlags[threadIndex]) {
                myMutex.lock();
                for (int index : markedIndices) {
                    array[index] = 0;
                }
                myMutex.unlock();
                delete params;
                return 0;
            }
        }
    }
    SetEvent(cantContinueEvents[threadIndex]);
    delete params;
    return 0;
}

int main() {
    do {
        std::cout << "Enter size of array : ";
        std::cin >> arraySize;
        if (arraySize <= 0) std::cout << "Uncorrect enter. Try anothet time :)" << std::endl;
    } while (arraySize <= 0);
    array.resize(arraySize, 0);
    int markerThreadsCount;
    do {
        std::cout << "Enter number of marker threads : ";
        std::cin >> markerThreadsCount;
        if (markerThreadsCount <= 0) std::cout << "Uncorrect enter. Try anothet time :)" << std::endl;
    } while (markerThreadsCount <= 0);

    startEvent = CreateEvent(NULL, TRUE, FALSE, NULL); //событие с ручным сбрососм, несигнальное состояние

    cantContinueEvents.resize(markerThreadsCount);
    continueEvents.resize(markerThreadsCount);
    closeThreadFlags.resize(markerThreadsCount);
    std::vector<HANDLE> markerThreads(markerThreadsCount);

    for (int i = 0; i < markerThreadsCount; ++i) {
        closeThreadFlags[i] = false;
    }

    for (int i = 0; i < markerThreadsCount; i++) {
        cantContinueEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL); //события с автоматическим сбросом, несигнальное состояние
        continueEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        closeThreadFlags[i] = false;

        markerThreadParams* params = new markerThreadParams(i);
        markerThreads[i] = CreateThread(NULL, 0, marker, params, 0, NULL);
    }

    SetEvent(startEvent);

    int activeThreadsCount = markerThreadsCount; //те, которые еще остались незавершенные
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

        std::cout << "Array contents : ";
        for (int i = 0; i < arraySize; i++) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;

        int threadIndexToClose;
        do {
            std::cout << "Enter thread index to close (0-" << markerThreadsCount - 1 << ") : ";
            std::cin >> threadIndexToClose;
        } while (threadIndexToClose < 0 || threadIndexToClose >= markerThreadsCount);

        if (!closeThreadFlags[threadIndexToClose]) {

            closeThreadFlags[threadIndexToClose] = true;
            SetEvent(continueEvents[threadIndexToClose]);
            WaitForSingleObject(markerThreads[threadIndexToClose], INFINITE);

            activeThreadsCount--;

            std::cout << "Array after thread " << threadIndexToClose << " closing : ";
            for (int i = 0; i < arraySize; i++) {
                std::cout << array[i] << " ";
            }
            std::cout << std::endl;

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
    std::cout << "All threads completed. Program finished." << std::endl;
    return 0;
}