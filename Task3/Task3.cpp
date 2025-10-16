#include <windows.h>
#include <iostream>
#include <vector>
#include <random>
#include <mutex>
#include <thread>
#include <stdexcept>

int arraySize;
std::vector<int>array;
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

void marker(markerThreadParams* params) {
    int threadNumber = params->index;
    int threadIndex = threadNumber - 1;
    int five = 5;
    WaitForSingleObject(startEvent, INFINITE);
    srand(threadNumber);

    int markedElementCount = 0;
    std::vector<int> markedIndices;
    while (true) {
        int randomNumber = rand() % arraySize;
        myMutex.lock();
        if (array[randomNumber] == 0) {
            Sleep(five);
            array[randomNumber] = threadNumber;
            markedElementCount++;
            markedIndices.push_back(randomNumber);
            Sleep(five);
            myMutex.unlock();
        }
        else {
            std::cout << "Thread with threadIndex " << threadNumber
                << ", marked " << markedElementCount
                << " elements, thread cant mark index " << randomNumber << std::endl;
            myMutex.unlock();

            SetEvent(cantContinueEvents[threadIndex]);
            WaitForSingleObject(continueEvents[threadIndex], INFINITE);

            if (closeThreadFlags[threadIndex]) {
                myMutex.lock();
                for (int index : markedIndices) {
                    array[index] = 0;
                }
                myMutex.unlock();
                delete params;
                return;
            }
        }
    }
    SetEvent(cantContinueEvents[threadIndex]);
    delete params;
    return;
}

int main() {
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
    array.resize(arraySize, 0);
    int markerThreadsCount;
    std::cout << "Enter number of marker threads : ";
    try {
        if (!(std::cin >> markerThreadsCount)) {
            throw std::runtime_error("Invalid input: not a number");
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

    startEvent = CreateEvent(NULL, TRUE, FALSE, NULL); //событие с ручным сбрососм, несигнальное состояние

    cantContinueEvents.resize(markerThreadsCount);
    continueEvents.resize(markerThreadsCount);
    closeThreadFlags.resize(markerThreadsCount);
    std::vector<std::thread> markerThreads;

    for (int i = 0; i < markerThreadsCount; ++i) {
        closeThreadFlags[i] = false;
    }

    for (int i = 0; i < markerThreadsCount; i++) {
        cantContinueEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL); //события с автоматическим сбросом, несигнальное состояние
        continueEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        closeThreadFlags[i] = false;

        markerThreadParams* params = new markerThreadParams(i+1);
        markerThreads.emplace_back(marker, params);
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

        myMutex.lock();
        std::cout << "Array contents : ";
        for (int i = 0; i < arraySize; i++) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;
        myMutex.unlock();

        int threadNumberToClose;
        int threadIndexToClose;
        myMutex.lock();
        do {
            std::cout << "Enter thread index to close (1-" << markerThreadsCount << ") : ";
            std::cin >> threadNumberToClose;
            threadIndexToClose = threadNumberToClose - 1;
        } while (threadNumberToClose < 1 || threadNumberToClose >= markerThreadsCount + 1);
        myMutex.unlock();

        if (!closeThreadFlags[threadIndexToClose]) {

            closeThreadFlags[threadIndexToClose] = true;
            SetEvent(continueEvents[threadIndexToClose]);
            if (markerThreads[threadIndexToClose].joinable()) {
                markerThreads[threadIndexToClose].join();
            }

            activeThreadsCount--;

            myMutex.lock();
            std::cout << "Array after thread " << threadIndexToClose << " closing : ";
            for (int i = 0; i < arraySize; i++) {
                std::cout << array[i] << " ";
            }
            std::cout << std::endl;
            myMutex.unlock();

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
        if (markerThreads[i].joinable()) {
            markerThreads[i].join();
        }
    }
    for (int i = 0; i < markerThreadsCount; i++) {
        CloseHandle(cantContinueEvents[i]);
        CloseHandle(continueEvents[i]);
    }
    CloseHandle(startEvent);
    std::cout << "All threads completed. Program finished." << std::endl;
    return 0;
}