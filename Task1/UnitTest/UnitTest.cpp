#include "pch.h"
#include "CppUnitTest.h"
#include "../Headers/Employee.h"

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
	};
}