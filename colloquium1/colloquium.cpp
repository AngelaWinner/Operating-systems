#include <iostream>
#include "Functions.h"

class Element {
public:
    int data;
    Element* next;
    Element* prev;
    Element(int value) : data(value), next(nullptr), prev(nullptr) {}
};

class List {
private:
    Element* head;
    Element* tail;
public:
    List() : head(nullptr), tail(nullptr) {}
    ~List() { clear(); }

    bool empty() {
        return head == nullptr;
    }

    void clear() {
        Element* current = head;
        while (current) {
            Element* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
        tail = nullptr;
    }

    void push_front(int value) {
        Element* newEl = new Element(value);
        if (!head) {
            head = tail = newEl;
        }
        else {
            newEl->next = head;
            head->prev = newEl;
            head = newEl;
        }
    }

    void push_back(int value) {
        Element* newEl = new Element(value);
        if (!tail) {
            head = tail = newEl;
        }
        else {
            tail->next = newEl;
            newEl->prev = tail;
            tail = newEl;
        }
    }

    void pop_front() {
        if (!head) return;
        Element* temp = head;
        head = head->next;
        if (head) {
            head->prev = nullptr;
        }
        else {
            tail = nullptr;
        }
        delete temp;
    }

    void pop_back() {
        if (!tail) return;
        Element* temp = tail;
        tail = tail->prev;
        if (tail) {
            tail->next = nullptr;
        }
        else {
            head = nullptr;
        }
        delete temp;
    }

    friend std::ostream& operator<<(std::ostream& out, const List& list) {
        Element* current = list.head;
        while (current) {
            out << current->data << " ";
            current = current->next;
        }
        return out;
    }

    friend std::istream& operator>>(std::istream& in, List& list) {
        int value;
        while (in >> value) {
            list.push_back(value);
        }
        return in;
    }

    void reverse() {
        Element* current = head;
        Element* prev = nullptr;
        tail = head;
        while (current) {
            Element* nextNode = current->next;
            current->next = prev;
            current->prev = nextNode;
            prev = current;
            current = nextNode;
        }
        head = prev;
        if (head) {
            head->prev = nullptr;
        }
    }
};
int inputNumber() {
    int n;
    std::cout << "Enter a positive number - the number of Fibonacci numbers: ";
    if (!(std::cin >> n)) {
        throw std::runtime_error("Invalid input: not a number");
    }
    if (n <= 0) {
        throw std::runtime_error("Incorrect enter");
    }
    return n;
}
void printFibonacciNumbers(const std::vector<long long>& arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
}
int main() {
    //1.  На вход натуральное число n. На выход контейнер с первыми n числами Фибоначчи.
    try {
        int n = inputNumber();
        auto FibonacciNumbers = getFibonacciNumbers(n);
        std::cout << "Fibonacci numbers:\n";
        printFibonacciNumbers(FibonacciNumbers);
        std::cout << "\n\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    //2.  На вход число. На выход – ответ является ли число палиндромом.
    int palindromNumber;
    std::cout << "Enter a positive number to check if it is a palindrome: ";
    try {
        if (!(std::cin >> palindromNumber)) {
            throw std::runtime_error("Invalid input: not a number or not an integer number");
        }
        if (palindromNumber < 0) {
            std::cout << "Incorrect enter. Try another time :)" << "\n";
            return 1;
        }
        std::cout << "Is entered number a palindrome? " << isPalindrom(palindromNumber) << "\n" << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    //3.  Развернуть связный список используя итеративный подход.
    List myList;
    myList.push_back(1);
    myList.push_back(2);
    myList.push_back(3);
    myList.push_back(4);
    myList.push_back(5);
    std::cout << "Original list: " << myList << "\n";
    myList.reverse();
    std::cout << "Reversed list: " << myList << "\n";

    return 0;
}