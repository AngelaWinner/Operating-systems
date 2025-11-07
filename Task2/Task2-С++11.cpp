#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <regex>
#include <algorithm>
#include <chrono>
void getNumber(int& numberOfNotes, std::string welcome, std::regex regex) {
    std::string numberStr;
    while (true) {
        std::cout << welcome;
        if (std::cin >> numberStr) {
            if (std::regex_match(numberStr, regex)) {
                numberOfNotes = std::stoi(numberStr);
                break;
            }
            else {
                std::cout << "Error: Input must be correct.\n";
            }
        }
        else {
            std::cout << "Error: Please enter a valid input.\n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}

int main() {
    std::regex regex("\\d+");
    std::regex regexElement("^-?\\d+$");
    int arrSize = 0;
    std::string welcome = "Enter size of array: ";
    getNumber(arrSize, welcome, regex);

    std::vector<int> arr(arrSize);
    std::cout << "Enter " << arrSize << " elements of array:\n";
    for (int i = 0; i < arrSize; ++i) {
        getNumber(arr[i], "", regexElement);
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