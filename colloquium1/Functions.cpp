#include "Functions.h"
std::string isPalindrom(int palindromNumber) {
    std::string str = std::to_string(palindromNumber);
    std::string reverseStr(str.rbegin(), str.rend());
    if (str == reverseStr) {
        return "Yes";
    }
    else {
        return "No";
    }
}
std::vector<long long> getFibonacciNumbers(int numberOfFibNums) {
    if (numberOfFibNums <= 0) {
        throw std::invalid_argument("Number of Fibonacci numbers must be positive");
    }
    std::vector<long long> FibonacciNumbers(numberOfFibNums);
    FibonacciNumbers[0] = 0;
    if (numberOfFibNums > 1) {
        FibonacciNumbers[1] = 1;
        for (int i = 2; i < numberOfFibNums; ++i) {
            FibonacciNumbers[i] = FibonacciNumbers[i - 1] + FibonacciNumbers[i - 2];
        }
    }
    return FibonacciNumbers;
}