#include <windows.h>
#include <iostream>
#include <vector>
#include <random>
#include <mutex>
#include <thread>
#include <stdexcept>
#include <condition_variable>
#include <chrono>

int arraySize;
std::vector<int> array;
int markerThreadsCount;
std::mutex myMutex;

std::condition_variable* startCV;
bool startFlag = false;

std::vector<std::condition_variable*> threadCVs;
std::vector<bool> threadReadyFlags;
std::vector<bool> continueFlags;
std::vector<bool> closeThreadFlags;

struct markerThreadParams {
    int index;
    markerThreadParams(int indexX) : index(indexX) {};
};

void marker(markerThreadParams* params) {
    int threadNumber = params->index;
    int threadIndex = threadNumber - 1;

    {
        std::unique_lock<std::mutex> lock(myMutex);
        startCV->wait(lock, [] { return startFlag; });
    }

    srand(threadNumber);

    int markedElementCount = 0;
    std::vector<int> markedIndices;

    while (true) {
        int randomNumber = rand() % arraySize;
        bool wasMarked = false;

        if (array[randomNumber] == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            array[randomNumber] = threadNumber;
            markedElementCount++;
            markedIndices.push_back(randomNumber);

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        else {
            {
                std::lock_guard<std::mutex> lock(myMutex);
                std::cout << "Thread with threadIndex " << threadNumber
                    << ", marked " << markedElementCount
                    << " elements, thread cant mark index " << randomNumber << std::endl;
            }

            {
                std::unique_lock<std::mutex> lock(myMutex);
                threadReadyFlags[threadIndex] = true;
                threadCVs[threadIndex]->notify_one();

                threadCVs[threadIndex]->wait(lock, [&] {
                    return continueFlags[threadIndex] || closeThreadFlags[threadIndex];
                    });

                continueFlags[threadIndex] = false;
            }
                
            if (closeThreadFlags[threadIndex]) {
                std::lock_guard<std::mutex> lock(myMutex);
                for (int index : markedIndices) {
                    array[index] = 0;
                }
                delete params;
                return;
            }
        }
    }
}

int main() {
    startCV = new std::condition_variable();

    std::cout << "Enter size of array : ";
    try {
        if (!(std::cin >> arraySize)) {
            throw std::runtime_error("Invalid input: not a number");
        }
        if (arraySize <= 0) {
            std::cout << "Uncorrect enter. Try another time :)" << std::endl;
            delete startCV;
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        delete startCV;
        return 1;
    }
    array.resize(arraySize, 0);

    std::cout << "Enter number of marker threads : ";
    try {
        if (!(std::cin >> markerThreadsCount)) {
            throw std::runtime_error("Invalid input: not a number :(");
        }
        if (markerThreadsCount <= 0) {
            std::cout << "Uncorrect enter. Try another time :)" << std::endl;
            delete startCV;
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        delete startCV;
        return 1;
    }

    threadCVs.resize(markerThreadsCount);
    threadReadyFlags.resize(markerThreadsCount, false);
    continueFlags.resize(markerThreadsCount, false);
    closeThreadFlags.resize(markerThreadsCount, false);

    std::vector<std::thread> markerThreads;

    for (int i = 0; i < markerThreadsCount; i++) {
        threadCVs[i] = new std::condition_variable();
    }

    for (int i = 0; i < markerThreadsCount; i++) {
        markerThreadParams* params = new markerThreadParams(i + 1);
        markerThreads.emplace_back(marker, params);
    }

    {
        std::lock_guard<std::mutex> lock(myMutex);
        startFlag = true;
    }
    startCV->notify_all();

    int activeThreadsCount = markerThreadsCount;

    while (activeThreadsCount > 0) {
        for (int i = 0; i < markerThreadsCount; i++) {
            if (!closeThreadFlags[i]) {
                std::unique_lock<std::mutex> lock(myMutex);
                threadCVs[i]->wait(lock, [&] { return threadReadyFlags[i]; });
            }
        }

        {
            std::lock_guard<std::mutex> lock(myMutex);
            std::cout << "Array contents : ";
            for (int i = 0; i < arraySize; i++) {
                std::cout << array[i] << " ";
            }
            std::cout << std::endl;
        }

        int threadNumberToClose, threadIndexToClose;

        do {
            std::cout << "Enter thread index to close (1-" << markerThreadsCount << ") : ";
            std::cin >> threadNumberToClose;
            threadIndexToClose = threadNumberToClose - 1;
        } while (threadNumberToClose < 1 || threadNumberToClose > markerThreadsCount ||
            closeThreadFlags[threadIndexToClose]);

        if (!closeThreadFlags[threadIndexToClose]) {
            closeThreadFlags[threadIndexToClose] = true;
            threadReadyFlags[threadIndexToClose] = false;
            continueFlags[threadIndexToClose] = true;

            threadCVs[threadIndexToClose]->notify_one();

            if (markerThreads[threadIndexToClose].joinable()) {
                markerThreads[threadIndexToClose].join();
            }

            activeThreadsCount--;

            {
                std::lock_guard<std::mutex> lock(myMutex);
                std::cout << "Array after thread " << threadNumberToClose << " closing : ";
                for (int i = 0; i < arraySize; i++) {
                    std::cout << array[i] << " ";
                }
                std::cout << std::endl;
            }

            for (int i = 0; i < markerThreadsCount; i++) {
                if (!closeThreadFlags[i]) {
                    threadReadyFlags[i] = false;
                    continueFlags[i] = true;
                    threadCVs[i]->notify_one();
                }
            }
        }
    }

    for (int i = 0; i < markerThreadsCount; i++) {
        if (markerThreads[i].joinable()) {
            markerThreads[i].join();
        }
    }

    delete startCV;
    for (int i = 0; i < markerThreadsCount; i++) {
        delete threadCVs[i];
    }

    std::cout << "All threads completed. Program finished." << std::endl;
    return 0;
}