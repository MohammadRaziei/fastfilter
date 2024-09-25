//
// Created by mohammad on 9/25/24.
//


#include "gtest/gtest.h"
#include "ndarray/array.h" // Assuming this is where your Array class is defined

TEST(ArrayTest, DefaultConstruction) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0u);
}

TEST(ArrayTest, ConstructionWithSize) {
    Array<int> arr(5, 10);
    EXPECT_EQ(arr.size(), 5u);
    for (int i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 10);
    }
}

TEST(ArrayTest, ConstructionFromRange) {
    int data[] = {1, 2, 3, 4, 5};
    Array<int> arr(data, data + 5);
    EXPECT_EQ(arr.size(), 5u);
    for (int i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], data[i]);
    }
}

TEST(ArrayTest, CopyConstructor) {
    Array<int> arr1(5, 10);
    Array<int> arr2(arr1);
    EXPECT_EQ(arr1.size(), arr2.size());
    for (int i = 0; i < arr2.size(); ++i) {
        EXPECT_EQ(arr1[i], arr2[i]);
    }
}

TEST(ArrayTest, MoveConstructor) {
    Array<int> arr1(5, 10);
    Array<int> arr2 = std::move(arr1);
    EXPECT_EQ(arr2.size(), 5u);
    for (int i = 0; i < arr2.size(); ++i) {
        EXPECT_EQ(arr2[i], 10);
    }
}

TEST(ArrayTest, PushBack) {
    Array<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    EXPECT_EQ(arr.size(), 2u);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST(ArrayTest, Fill) {
    Array<int> arr(5);
    arr.fill(10);
    for (int i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 10);
    }
}

TEST(ArrayTest, VectorConversion) {
    Array<int> arr(5, 10);
    std::vector<int> vec = arr.vector();
    EXPECT_EQ(vec.size(), arr.size());
    for (int i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i], 10);
    }
}

TEST(ArrayTest, StreamOperator) {
    Array<int> arr(5, 10);
    std::stringstream ss;
    ss << arr;
    EXPECT_STREQ(ss.str().c_str(), "[10, 10, 10, 10, 10]");
}


TEST(ArrayTest, ConstructionFromRawPointer) {
    int data[] = {1, 2, 3, 4, 5};
    Array<int> arr(data,  5);

    EXPECT_EQ(arr.size(), 5u);
    for (int i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], data[i]);
    }

    // Check that the array does not own the data initially
    EXPECT_FALSE(arr.ownsData());
}

TEST(ArrayTest, CopyToOwnershipMode) {
    int data[] = {1, 2, 3, 4, 5};
    Array<int> arr(data,5);

    // Copy the array to take ownership of the data
    Array<int> ownedArr = arr.copy();

    EXPECT_EQ(ownedArr.size(), 5u);
    for (int i = 0; i < ownedArr.size(); ++i) {
        EXPECT_EQ(ownedArr[i], data[i]);
    }

    // Check that the copied array now owns the data
    EXPECT_TRUE(ownedArr.ownsData());
}

TEST(ArrayTest, CopyDoesNotAffectOriginalData) {
    int data[] = {1, 2, 3, 4, 5};
    Array<int> arr(data, data + 5);

    // Copy the array to take ownership of the data
    Array<int> ownedArr = arr.copy();

    // Modify the original data
    for (int i = 0; i < 5; ++i) {
        data[i] = 0;
    }

    // Ensure the copied array data remains unchanged
    for (int i = 0; i < ownedArr.size(); ++i) {
        EXPECT_EQ(ownedArr[i], i + 1);
    }
}