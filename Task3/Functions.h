#include <vector>

void initializeArray(std::vector<int>& arr, int size);
bool tryMarkElement(std::vector<int>& arr, int index, int threadIndex);
void clearMarkedElements(std::vector<int>& arr, const std::vector<int>& markedIndices);
void printArray(const std::vector<int>& arr);
int readArraySize();
int readThreadsCount();