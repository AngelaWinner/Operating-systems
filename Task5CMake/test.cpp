#include <gtest/gtest.h>
#include "../Headers/Functions.h"
#include "../Headers/Employee.h"
#include <sstream>
#include <fstream>

// ==================== Тесты для enter_ID_of_employee ====================

TEST(EnterIDTest, PositiveInteger123) {
    int id = 0;
    std::stringstream input("123");
    std::cin.rdbuf(input.rdbuf());
    bool result = enter_ID_of_employee(id);
    EXPECT_TRUE(result);
    EXPECT_EQ(id, 123);
}

TEST(EnterIDTest, ZeroIsInvalid) {
    int id = 0;
    std::stringstream input("0");
    std::cin.rdbuf(input.rdbuf());
    bool result = enter_ID_of_employee(id);
    EXPECT_FALSE(result);
}

TEST(EnterIDTest, NegativeNumberMinus5) {
    int id = 0;
    std::stringstream input("-5");
    std::cin.rdbuf(input.rdbuf());
    bool result = enter_ID_of_employee(id);
    EXPECT_FALSE(result);
}

TEST(EnterIDTest, NonNumericStringAbc) {
    int id = 0;
    std::stringstream input("abc");
    std::cin.rdbuf(input.rdbuf());
    bool result = enter_ID_of_employee(id);
    EXPECT_FALSE(result);
}

TEST(EnterIDTest, FloatNumber12Dot5) {
    int id = 0;
    std::stringstream input("12.5");
    std::cin.rdbuf(input.rdbuf());
    bool result = enter_ID_of_employee(id);
    EXPECT_TRUE(result);
}

TEST(EnterIDTest, MinimumValidID1) {
    int id = 0;
    std::stringstream input("1");
    std::cin.rdbuf(input.rdbuf());
    bool result = enter_ID_of_employee(id);
    EXPECT_TRUE(result);
    EXPECT_EQ(id, 1);
}

// ==================== Тесты для getNumber ====================

TEST(GetNumberTest, ValidNumber42) {
    int number = 0;
    std::stringstream input("42");
    std::cin.rdbuf(input.rdbuf());
    getNumber(number, "");
    EXPECT_EQ(number, 42);
}

TEST(GetNumberTest, ZeroIsNotAccepted) {
    int number = 0;
    std::stringstream input("0");
    std::cin.rdbuf(input.rdbuf());
    std::stringstream input2("5");
    std::cin.rdbuf(input2.rdbuf());
    getNumber(number, "");
    EXPECT_EQ(number, 5);
}

TEST(GetNumberTest, LeadingZero012IsInvalid) {
    int number = 0;
    std::stringstream input("012");
    std::cin.rdbuf(input.rdbuf());
    std::stringstream input2("12");
    std::cin.rdbuf(input2.rdbuf());
    getNumber(number, "");
    EXPECT_EQ(number, 12);
}

TEST(GetNumberTest, SingleDigit9) {
    int number = 0;
    std::stringstream input("9");
    std::cin.rdbuf(input.rdbuf());
    getNumber(number, "");
    EXPECT_EQ(number, 9);
}

TEST(GetNumberTest, LargeNumber99999) {
    int number = 0;
    std::stringstream input("99999");
    std::cin.rdbuf(input.rdbuf());
    getNumber(number, "");
    EXPECT_EQ(number, 99999);
}

// ==================== Тесты для getDouble ====================

TEST(GetDoubleTest, Integer100) {
    double number = 0.0;
    std::stringstream input("100");
    std::cin.rdbuf(input.rdbuf());
    getDouble(number, "");
    EXPECT_DOUBLE_EQ(number, 100.0);
}

TEST(GetDoubleTest, Float12Dot5) {
    double number = 0.0;
    std::stringstream input("12.5");
    std::cin.rdbuf(input.rdbuf());
    getDouble(number, "");
    EXPECT_DOUBLE_EQ(number, 12.5);
}

TEST(GetDoubleTest, ZeroIsRejected) {
    double number = 0.0;
    std::stringstream input("0");
    std::cin.rdbuf(input.rdbuf());
    std::stringstream input2("1.5");
    std::cin.rdbuf(input2.rdbuf());
    getDouble(number, "");
    EXPECT_DOUBLE_EQ(number, 1.5);
}

TEST(GetDoubleTest, NegativeMinus5Dot5) {
    double number = 0.0;
    std::stringstream input("-5.5");
    std::cin.rdbuf(input.rdbuf());
    std::stringstream input2("5.5");
    std::cin.rdbuf(input2.rdbuf());
    getDouble(number, "");
    EXPECT_DOUBLE_EQ(number, 5.5);
}

TEST(GetDoubleTest, DecimalPoint5) {
    double number = 0.0;
    std::stringstream input("0.5");
    std::cin.rdbuf(input.rdbuf());
    getDouble(number, "");
    EXPECT_DOUBLE_EQ(number, 0.5);
}

TEST(GetDoubleTest, ManyDecimals123Dot456789) {
    double number = 0.0;
    std::stringstream input("123.456789");
    std::cin.rdbuf(input.rdbuf());
    getDouble(number, "");
    EXPECT_DOUBLE_EQ(number, 123.456789);
}

// ==================== Тесты для getFileName ====================

TEST(GetFileNameTest, LettersOnlyTest) {
    std::string filename;
    std::stringstream input("test");
    std::cin.rdbuf(input.rdbuf());
    getFileName(filename);
    EXPECT_EQ(filename, "test");
}

TEST(GetFileNameTest, WithNumbersFile123) {
    std::string filename;
    std::stringstream input("file123");
    std::cin.rdbuf(input.rdbuf());
    getFileName(filename);
    EXPECT_EQ(filename, "file123");
}

TEST(GetFileNameTest, WithUnderscoreTestFile) {
    std::string filename;
    std::stringstream input("test_file");
    std::cin.rdbuf(input.rdbuf());
    getFileName(filename);
    EXPECT_EQ(filename, "test_file");
}

TEST(GetFileNameTest, WithDashTestDashFile) {
    std::string filename;
    std::stringstream input("test-file");
    std::cin.rdbuf(input.rdbuf());
    getFileName(filename);
    EXPECT_EQ(filename, "test-file");
}

TEST(GetFileNameTest, SpaceIsRejected) {
    std::string filename;
    std::stringstream input("test file");
    std::cin.rdbuf(input.rdbuf());
    std::stringstream input2("testfile");
    std::cin.rdbuf(input2.rdbuf());
    getFileName(filename);
    EXPECT_EQ(filename, "testfile");
}

TEST(GetFileNameTest, DotIsRejected) {
    std::string filename;
    std::stringstream input("test.txt");
    std::cin.rdbuf(input.rdbuf());
    std::stringstream input2("test");
    std::cin.rdbuf(input2.rdbuf());
    getFileName(filename);
    EXPECT_EQ(filename, "test");
}

TEST(GetFileNameTest, MixedValidCharsABCDash123UnderscoreXYZ) {
    std::string filename;
    std::stringstream input("AB-123_XYZ");
    std::cin.rdbuf(input.rdbuf());
    getFileName(filename);
    EXPECT_EQ(filename, "AB-123_XYZ");
}

// ==================== Тесты для showEmployees ====================

//?

TEST(ShowEmployeesTest, EmptyFileZeroRecords) {
    std::ofstream out("test_empty.dat", std::ios::binary);
    int count = 0;
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    out.close();

    std::ifstream in("test_empty.dat", std::ios::binary);
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    showEmployees(in);
    std::cout.rdbuf(old);
    in.close();

    std::string output = buffer.str();
    EXPECT_NE(output.find("Total records: 0"), std::string::npos);

    std::remove("test_empty.dat");
}

//?

TEST(ShowEmployeesTest, SingleEmployeeJohn) {
    Employee emp(1, "John", 40.5);
    std::ofstream out("test_single.dat", std::ios::binary);
    int count = 1;
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    out.write(reinterpret_cast<const char*>(&emp), sizeof(Employee));
    out.close();

    std::ifstream in("test_single.dat", std::ios::binary);
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    showEmployees(in);
    std::cout.rdbuf(old);
    in.close();

    std::string output = buffer.str();
    EXPECT_NE(output.find("Total records: 1"), std::string::npos);
    EXPECT_NE(output.find("ID: 1"), std::string::npos);

    std::remove("test_single.dat");
}

TEST(ShowEmployeesTest, TwoEmployeesAliceAndBob) {
    Employee emps[2] = {
        Employee(1, "Alice", 35.0),
        Employee(2, "Bob", 42.5)
    };
    std::ofstream out("test_two.dat", std::ios::binary);
    int count = 2;
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (int i = 0; i < 2; i++) {
        out.write(reinterpret_cast<const char*>(&emps[i]), sizeof(Employee));
    }
    out.close();

    std::ifstream in("test_two.dat", std::ios::binary);
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    showEmployees(in);
    std::cout.rdbuf(old);
    in.close();

    std::string output = buffer.str();
    EXPECT_NE(output.find("Total records: 2"), std::string::npos);

    std::remove("test_two.dat");
}

// ==================== Тесты для getEmployeeData ====================

TEST(GetEmployeeDataTest, ValidDataID123NameJohn) {
    Employee emp;
    std::set<int> usedIds;
    std::stringstream input("123\nJohn\n");
    std::cin.rdbuf(input.rdbuf());
    getEmployeeData(emp, 0, usedIds);
    EXPECT_EQ(emp.num, 123);
    EXPECT_EQ(std::string(emp.name), "John");
    EXPECT_EQ(usedIds.size(), 1);
}

TEST(GetEmployeeDataTest, DuplicateID123Rejected) {
    Employee emp;
    std::set<int> usedIds;
    usedIds.insert(123);
    std::stringstream input("123\n456\nBob\n");
    std::cin.rdbuf(input.rdbuf());
    getEmployeeData(emp, 1, usedIds);
    EXPECT_EQ(emp.num, 456);
    EXPECT_EQ(std::string(emp.name), "Bob");
}

TEST(GetEmployeeDataTest, EmptyNameRejectedThenValid) {
    Employee emp;
    std::set<int> usedIds;
    std::stringstream input("789\n\nAlice\n");
    std::cin.rdbuf(input.rdbuf());
    getEmployeeData(emp, 0, usedIds);
    EXPECT_EQ(emp.num, 789);
    EXPECT_EQ(std::string(emp.name), "Alice");
}

TEST(GetEmployeeDataTest, LongNameAlexanderTruncated) {
    Employee emp;
    std::set<int> usedIds;
    std::stringstream input("999\nAlexanderTheGreat\n");
    std::cin.rdbuf(input.rdbuf());
    getEmployeeData(emp, 0, usedIds);
    EXPECT_EQ(emp.num, 999);
    EXPECT_EQ(std::string(emp.name), "Alexander");
}

// ==================== Тесты для Employee структуры ====================

TEST(EmployeeStructTest, DefaultConstructorValues) {
    Employee emp;
    EXPECT_EQ(emp.num, 0);
    EXPECT_EQ(std::string(emp.name), "");
    EXPECT_DOUBLE_EQ(emp.hours, 0.0);
}

TEST(EmployeeStructTest, ParameterizedConstructorID1NameTestHours40Dot5) {
    Employee emp(1, "Test", 40.5);
    EXPECT_EQ(emp.num, 1);
    EXPECT_EQ(std::string(emp.name), "Test");
    EXPECT_DOUBLE_EQ(emp.hours, 40.5);
}

TEST(EmployeeStructTest, LongNameVeryLongNameTruncated) {
    Employee emp(2, "VeryLongNameThatExceedsLimit", 35.0);
    EXPECT_EQ(strlen(emp.name), 9);
    EXPECT_EQ(std::string(emp.name), "VeryLongN");
}

TEST(EmployeeStructTest, OperatorLessThanTrue) {
    Employee emp1(1, "Alice", 40.0);
    Employee emp2(2, "Bob", 35.0);
    EXPECT_TRUE(emp1 < emp2);
}

TEST(EmployeeStructTest, OperatorLessThanFalse) {
    Employee emp1(1, "Alice", 40.0);
    Employee emp2(2, "Bob", 35.0);
    EXPECT_FALSE(emp2 < emp1);
}

TEST(EmployeeStructTest, OperatorEqualsTrue) {
    Employee emp1(1, "Alice", 40.0);
    Employee emp3(1, "Alice", 40.0);
    EXPECT_TRUE(emp1 == emp3);
}

TEST(EmployeeStructTest, OperatorEqualsFalse) {
    Employee emp1(1, "Alice", 40.0);
    Employee emp2(2, "Bob", 35.0);
    EXPECT_FALSE(emp1 == emp2);
}

TEST(EmployeeStructTest, StreamOutputOperator) {
    Employee emp(5, "Charlie", 42.5);
    std::stringstream ss;
    ss << emp;
    std::string output = ss.str();
    EXPECT_NE(output.find("5"), std::string::npos);
    EXPECT_NE(output.find("Charlie"), std::string::npos);
}

TEST(EmployeeStructTest, StreamInputOperator) {
    std::stringstream ss("5 Charlie 42.5");
    Employee emp;
    ss >> emp;
    EXPECT_EQ(emp.num, 5);
    EXPECT_EQ(std::string(emp.name), "Charlie");
    EXPECT_DOUBLE_EQ(emp.hours, 42.5);
}

// ==================== Граничные случаи ====================

TEST(BoundaryTest, EmployeeNameExactly9CharsAlexander) {
    Employee emp;
    std::set<int> usedIds;
    std::stringstream input("777\nAlexander\n");
    std::cin.rdbuf(input.rdbuf());
    getEmployeeData(emp, 0, usedIds);
    EXPECT_EQ(strlen(emp.name), 9);
    EXPECT_EQ(std::string(emp.name), "Alexander");
}

TEST(BoundaryTest, EmployeeName10CharsTruncated) {
    Employee emp(3, "0123456789", 30.0);
    EXPECT_EQ(strlen(emp.name), 9);
}

TEST(BoundaryTest, HoursPrecisionTest) {
    double number = 0.0;
    std::stringstream input("123.456");
    std::cin.rdbuf(input.rdbuf());
    getDouble(number, "");
    EXPECT_NEAR(number, 123.456, 0.0001);
}

TEST(BoundaryTest, FileNameMaxLengthTest) {
    std::string filename;
    std::stringstream input("abcdefghijklmnopqrstuvwxyz");
    std::cin.rdbuf(input.rdbuf());
    std::stringstream input2("validName");
    std::cin.rdbuf(input2.rdbuf());
    getFileName(filename);
    EXPECT_EQ(filename, "validName");
}

// ==================== Ошибки ввода ====================

TEST(ErrorTest, GetNumberWithLetters) {
    int number = 0;
    std::stringstream input("abc");
    std::cin.rdbuf(input.rdbuf());
    std::stringstream input2("10");
    std::cin.rdbuf(input2.rdbuf());
    getNumber(number, "");
    EXPECT_EQ(number, 10);
}

TEST(ErrorTest, GetDoubleWithCommaInsteadOfDot) {
    double number = 0.0;
    std::stringstream input("12,5");
    std::cin.rdbuf(input.rdbuf());
    std::stringstream input2("12.5");
    std::cin.rdbuf(input2.rdbuf());
    getDouble(number, "");
    EXPECT_DOUBLE_EQ(number, 12.5);
}

TEST(ErrorTest, EmptyFileNameRejected) {
    std::string filename;
    std::stringstream input("");
    std::cin.rdbuf(input.rdbuf());
    std::stringstream input2("valid");
    std::cin.rdbuf(input2.rdbuf());
    getFileName(filename);
    EXPECT_EQ(filename, "valid");
}