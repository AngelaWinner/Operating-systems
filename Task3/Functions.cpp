#include "Functions.h"
#include <vector>
#include <iostream>
#include <random>

void initializeArray(std::vector<int>& arr, int size) {
    arr.resize(size, 0);
}

bool tryMarkElement(std::vector<int>& arr, int index, int threadIndex) {
    if (arr[index] == 0) {
        arr[index] = threadIndex;
        return true;
    }
    return false;
}

void clearMarkedElements(std::vector<int>& arr, const std::vector<int>& markedIndices) {
    for (int idx : markedIndices) {
        arr[idx] = 0;
    }
}

void printArray(const std::vector<int>& arr) {
    for (int value : arr) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

int readArraySize() {
    int size;
    do {
        std::cout << "Enter size of array : ";
        std::cin >> size;
        if (size <= 0) std::cout << "Uncorrect enter. Try another time :)" << std::endl;
    } while (size <= 0);
    return size;
}

int readThreadsCount() {
    int count;
    do {
        std::cout << "Enter number of marker threads : ";
        std::cin >> count;
        if (count <= 0) std::cout << "Uncorrect enter. Try another time :)" << std::endl;
    } while (count <= 0);
    return count;
}