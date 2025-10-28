#include "pch.h"
#include "CppUnitTest.h"
#include "..\Functions.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unitTests
{
	TEST_CLASS(unitTests)
	{
	public:
		
        TEST_METHOD(TestPalindrom1)
        {
            Assert::AreEqual(std::string("Yes"), isPalindrom(121));
        }

        TEST_METHOD(TestPalindrom2)
        {
            Assert::AreEqual(std::string("Yes"), isPalindrom(0));
        }

        TEST_METHOD(TestPalindrom3)
        {
            Assert::AreEqual(std::string("Yes"), isPalindrom(7));
        }

        TEST_METHOD(TestPalindrom4)
        {
            Assert::AreEqual(std::string("Yes"), isPalindrom(1221));
        }

        TEST_METHOD(TestPalindrom5)
        {
            Assert::AreEqual(std::string("No"), isPalindrom(123));
        }

        TEST_METHOD(TestPalindrom6)
        {
            Assert::AreEqual(std::string("No"), isPalindrom(1214));
        }
		TEST_METHOD(TestFibonacci1)
        {
            auto fibonacciNumbers = getFibonacciNumbers(5);
            Assert::AreEqual(size_t(5), fibonacciNumbers.size());
            Assert::AreEqual(0LL, fibonacciNumbers[0]);
            Assert::AreEqual(1LL, fibonacciNumbers[1]);
            Assert::AreEqual(1LL, fibonacciNumbers[2]);
            Assert::AreEqual(2LL, fibonacciNumbers[3]);
            Assert::AreEqual(3LL, fibonacciNumbers[4]);
        }

        TEST_METHOD(TestFibonacci2)
        {
            auto fibonacciZero = getFibonacciNumbers(1);
            Assert::AreEqual(size_t(1), fibonacciZero.size());
            Assert::AreEqual(0LL, fibonacciZero[0]);
        }

        TEST_METHOD(TestFibonacci3)
        {
            Assert::ExpectException<std::invalid_argument>([] { getFibonacciNumbers(0); });
        }

        TEST_METHOD(TestFibonacci4)
        {
            Assert::ExpectException<std::invalid_argument>([] { getFibonacciNumbers(-5); });
        }
	};
}
