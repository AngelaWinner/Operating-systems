#include "pch.h"
#include "CppUnitTest.h"
#include "C:\Users\LENOVO\Desktop\универАнж\ОС\Задание 2\Task2\Task2\Functions.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(testMethod1)
		{
			std::vector<int> vec = { 5, 2, 12, 1, 3 };
			int minValue, maxValue;

			min_max1(minValue, maxValue, vec);

			Assert::AreEqual(minValue, 1);
			Assert::AreEqual(maxValue, 12);
		}
		TEST_METHOD(testMethod2)
		{
			std::vector<int> vec = { -5, 0, -9, -1, -3 };
			int minValue, maxValue;

			min_max1(minValue, maxValue, vec);

			Assert::AreEqual(minValue, -9);
			Assert::AreEqual(maxValue, 0);
		}
		TEST_METHOD(testMethod3)
		{
			std::vector<int> vec = { 1,1,1 };
			int minValue, maxValue;

			min_max1(minValue, maxValue, vec);

			Assert::AreEqual(minValue, 1);
			Assert::AreEqual(maxValue, 1);
		}
		TEST_METHOD(testMethod4)
		{
			std::vector<int> vec = { 3, 1, 4, 1, 5, 9 };
			int averageValue = 0;

			average1(averageValue, vec);

			Assert::AreEqual(averageValue, 1);
		}
		TEST_METHOD(testMethod5)
		{
			std::vector<int> vec = { 10000, 3 };
			int averageValue = 0;

			average1(averageValue, vec);

			Assert::AreEqual(averageValue, 5001);
		}
	};
}
