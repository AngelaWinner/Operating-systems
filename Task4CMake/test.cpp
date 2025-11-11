#include <gtest/gtest.h>
#include <windows.h>
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