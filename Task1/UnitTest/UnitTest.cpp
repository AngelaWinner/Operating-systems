#include "pch.h"
#include "CppUnitTest.h"
#include "../Headers/Employee.h"
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
        TEST_METHOD(TestComparisonLessOperator)
        {
            Employee emp1{ 1, "Ivanov", 8.0 };
            Employee emp2{ 2, "Petrov", 7.5 };

            Assert::IsTrue(emp1 < emp2);
            Assert::IsFalse(emp2 < emp1);
            Assert::IsFalse(emp1 < emp1);
        }
        TEST_METHOD(TestComparisonEqualOperator)
        {
            Employee emp1{ 1, "Ivanov", 8.0 };
            Employee emp2{ 1, "Ivanov", 8.0 };
            Employee emp3{ 1, "Petrov", 8.0 };
            Employee emp4{ 2, "Ivanov", 8.0 };
            Employee emp5{ 1, "Ivanov", 7.5 };

            Assert::IsTrue(emp1 == emp2);
            Assert::IsFalse(emp1 == emp3);
            Assert::IsFalse(emp1 == emp4);
            Assert::IsFalse(emp1 == emp5);
        }
        TEST_METHOD(TestWriteTextMethod)
        {
            Employee emp{ 123, "Sidorov", 40.5 };
            std::ostringstream oss;

            emp.writeText(oss);

            std::string result = oss.str();
            Assert::AreEqual(std::string("123 Sidorov 40.5\n"), result);
        }
        TEST_METHOD(TestReadTextMethod)
        {
            std::istringstream iss("456 Smirnov 35.75");
            Employee emp;

            emp.readText(iss);

            Assert::AreEqual(456, emp.num);
            Assert::AreEqual(std::string("Smirnov"), std::string(emp.name));
            Assert::AreEqual(35.75, emp.hours);
        }
        TEST_METHOD(TestOutputOperator)
        {
            Employee emp{ 789, "Kuznetsov", 42.0 };
            std::ostringstream oss;

            oss << emp;

            std::string result = oss.str();
            Assert::AreEqual(std::string("789 Kuznetsov 42\n"), result);
        }
        TEST_METHOD(TestInputOperator)
        {
            std::istringstream iss("111 Orlov 38.25");
            Employee emp;

            iss >> emp;

            Assert::AreEqual(111, emp.num);
            Assert::AreEqual(std::string("Orlov"), std::string(emp.name));
            Assert::AreEqual(38.25, emp.hours);
        }
        TEST_METHOD(TestWriteReadConsistency)
        {
            Employee original{ 999, "Popov", 45.5 };
            std::stringstream ss;
            ss << original;
            Employee restored;
            ss >> restored;

            Assert::IsTrue(original == restored);
        }
        TEST_METHOD(TestMultipleEmployeesInStream)
        {
            Employee emp1{ 1, "Ivanov", 8.0 };
            Employee emp2{ 2, "Petrov", 7.5 };
            std::stringstream ss;

            ss << emp1 << emp2;
            Employee read1, read2;
            ss >> read1 >> read2;

            Assert::IsTrue(emp1 == read1);
            Assert::IsTrue(emp2 == read2);
        }
	};
}