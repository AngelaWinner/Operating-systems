#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <chrono>

int main() {
    int arrSize = 0;
    std::cout << "Enter size of array: ";
    std::cin >> arrSize;

    std::vector<int> arr(arrSize);
    std::cout << "Enter " << arrSize << " elements of array:\n";
    for (int i = 0; i < arrSize; ++i) {
        std::cin >> arr[i];
    }

    int maxValue = arr[0];
    int minValue = arr[0];
    double arithmeticMean = 0.0;

    auto min_max = [&arr, &minValue, &maxValue]() {
        minValue = arr[0];
        maxValue = arr[0];

        for (const auto& value : arr) {
            if (value <= minValue) minValue = value;
            std::this_thread::sleep_for(std::chrono::milliseconds(7));
            if (value >= maxValue) maxValue = value;
            std::this_thread::sleep_for(std::chrono::milliseconds(7));
        }

        std::cout << "Minimum element of array : " << minValue << "\n";
        std::cout << "Maximum element of array : " << maxValue << "\n";
        };

    auto average = [&arr, &arithmeticMean]() {
        for (int i = 0; i < arr.size(); ++i) {
            arithmeticMean += static_cast<double>(arr[i]) / arr.size();
            std::this_thread::sleep_for(std::chrono::milliseconds(12));
        }
        std::cout << "Arithmetic mean : " << arithmeticMean << " ~~ " << static_cast<int>(arithmeticMean) << "\n";
        };

    std::thread min_max_thread(min_max);
    std::thread average_thread(average);

    min_max_thread.join();
    average_thread.join();

    for (auto& element : arr) {
        if (element == maxValue || element == minValue) {
            element = static_cast<int>(arithmeticMean);
        }
    }

    std::cout << "Changed array : ";
    std::for_each(arr.begin(), arr.end(), [](int element) {
        std::cout << element << " ";
        });
    std::cout << "\n";

    return 0;
}