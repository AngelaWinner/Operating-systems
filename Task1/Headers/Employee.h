#pragma once
#include <iostream>
#include <fstream>
#include <cstring>

struct Employee
{
    int num;
    char name[10];
    double hours;

    bool operator<(const Employee& other) const {
        return num < other.num;
    }
    bool operator==(const Employee& other) const {
        return num == other.num &&
            strcmp(name, other.name) == 0 &&
            hours == other.hours;
    }
    void writeText(std::ostream& out) const {
        out << num << " " << name << " " << hours << "\n";
    }
    void readText(std::istream& in) {
        in >> num >> name >> hours;
    }
};
std::ostream& operator<<(std::ostream& out, const Employee& emp) {
    emp.writeText(out);
    return out;
}
std::istream& operator>>(std::istream& in, Employee& emp) {
    emp.readText(in);
    return in;
}