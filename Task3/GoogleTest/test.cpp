#include "pch.h"
#include "Functions.cpp"
#include <gtest/gtest.h>

TEST(ArrayTest, Initialization) {
    std::vector<int> arr;
    initializeArray(arr, 10);
    EXPECT_EQ(arr.size(), 10);
    for (int val : arr) {
        EXPECT_EQ(val, 0);
    }
}

TEST(MarkTest, MarkUnmarked) {
    std::vector<int> arr = { 0, 0, 0 };
    bool result = tryMarkElement(arr, 1, 5);
    EXPECT_TRUE(result);
    EXPECT_EQ(arr[1], 5);
}

TEST(MarkTest, MarkAlreadyMarked) {
    std::vector<int> arr = { 0, 3, 0 };
    bool result = tryMarkElement(arr, 1, 5);
    EXPECT_FALSE(result);
}

TEST(ClearTest, ClearMarked) {
    std::vector<int> arr = { 1, 2, 3, 4 };
    std::vector<int> indices = { 1, 3 };
    clearMarkedElements(arr, indices);
    EXPECT_EQ(arr[1], 0);
    EXPECT_EQ(arr[3], 0);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[2], 3);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}