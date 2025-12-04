#pragma once
#include <iostream>
#include <fstream>
#include <cstring>

struct Employee
{
    int num;
    char name[10];
    double hours;
    Employee(int n, const char* nm, double h) : num(n), hours(h) {
        if (nm != nullptr) {
            int i = 0;
            for (; i < 9 && nm[i] != '\0'; ++i) {
                name[i] = nm[i];
            }
            name[i] = '\0';
        }
        else {
            name[0] = '\0';
        }
    }
    Employee() : num(0), hours(0.0) {
        name[0] = '\0';
    }
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