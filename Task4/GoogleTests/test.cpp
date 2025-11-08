#include "pch.h"
#include "Functions.h"

TEST(ValidationTest, ValidatePositiveNumber_ValidNumbers) {
    EXPECT_TRUE(validatePositiveNumber("1"));
    EXPECT_TRUE(validatePositiveNumber("10"));
    EXPECT_TRUE(validatePositiveNumber("100"));
    EXPECT_TRUE(validatePositiveNumber("999"));
}

TEST(ValidationTest, ValidatePositiveNumber_InvalidNumbers) {
    EXPECT_FALSE(validatePositiveNumber("0"));
    EXPECT_FALSE(validatePositiveNumber("-1"));
    EXPECT_FALSE(validatePositiveNumber("01"));
    EXPECT_FALSE(validatePositiveNumber("1a"));
    EXPECT_FALSE(validatePositiveNumber(""));
    EXPECT_FALSE(validatePositiveNumber(" 1"));
}

TEST(ValidationTest, ValidateFilename_ValidFilenames) {
    EXPECT_TRUE(validateFilename("test"));
    EXPECT_TRUE(validateFilename("test123"));
    EXPECT_TRUE(validateFilename("test-file"));
    EXPECT_TRUE(validateFilename("test_file"));
    EXPECT_TRUE(validateFilename("123"));
    EXPECT_TRUE(validateFilename("a-b_c123"));
}

TEST(ValidationTest, ValidateFilename_InvalidFilenames) {
    EXPECT_FALSE(validateFilename("test file"));
    EXPECT_FALSE(validateFilename("test.file"));
    EXPECT_FALSE(validateFilename("test@file"));
    EXPECT_FALSE(validateFilename("test+file"));
    EXPECT_FALSE(validateFilename(""));
    EXPECT_FALSE(validateFilename(" "));
    EXPECT_FALSE(validateFilename("../test"));
}

TEST(ValidationTest, ValidateChoice_ValidChoices) {
    EXPECT_TRUE(validateChoice("0"));
    EXPECT_TRUE(validateChoice("1"));
}

TEST(ValidationTest, ValidateChoice_InvalidChoices) {
    EXPECT_FALSE(validateChoice(""));
    EXPECT_FALSE(validateChoice("2"));
    EXPECT_FALSE(validateChoice("01"));
    EXPECT_FALSE(validateChoice("a"));
    EXPECT_FALSE(validateChoice(" "));
    EXPECT_FALSE(validateChoice("-1"));
}

TEST(SynchronizationTest, CreateMutex_ValidName) {
    HANDLE mutex = createMutex(L"TestMutex");
    EXPECT_NE(mutex, nullptr);
    EXPECT_NE(mutex, INVALID_HANDLE_VALUE);
    if (mutex) {
        CloseHandle(mutex);
    }
}

TEST(SynchronizationTest, CreateSemaphore_ValidParameters) {
    HANDLE semaphore = createSemaphore(L"TestSemaphore", 1, 5);
    EXPECT_NE(semaphore, nullptr);
    EXPECT_NE(semaphore, INVALID_HANDLE_VALUE);
    if (semaphore) {
        CloseHandle(semaphore);
    }
}

TEST(SynchronizationTest, OpenMutex_NonExistent) {
    // Попытка открыть несуществующий мьютекс должна вернуть ошибку
    HANDLE mutex = openMutex(L"NonExistentMutex12345");
    EXPECT_EQ(mutex, nullptr);
}

TEST(SynchronizationTest, OpenSemaphore_NonExistent) {
    HANDLE semaphore = openSemaphore(L"NonExistentSemaphore12345");
    EXPECT_EQ(semaphore, nullptr);
}

TEST(WaitTest, WaitForSingleHandle_InvalidHandle) {
    EXPECT_FALSE(waitForSingleHandle(INVALID_HANDLE_VALUE, 0));
}

TEST(WaitTest, WaitForMultipleHandles_EmptyArray) {
    EXPECT_FALSE(waitForMultipleHandles(nullptr, 0, 0));
}

TEST(WaitTest, WaitForMultipleHandles_InvalidHandles) {
    HANDLE invalidHandles[] = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };
    EXPECT_FALSE(waitForMultipleHandles(invalidHandles, 2, 0));
}

TEST(EdgeCaseTest, EmptyStrings) {
    EXPECT_FALSE(validatePositiveNumber(""));
    EXPECT_FALSE(validateFilename(""));
    EXPECT_FALSE(validateChoice(""));
}

TEST(EdgeCaseTest, SpecialCharacters) {
    EXPECT_FALSE(validatePositiveNumber("1.5"));
    EXPECT_FALSE(validateFilename("file@name"));
    EXPECT_FALSE(validateChoice("1.0"));
}