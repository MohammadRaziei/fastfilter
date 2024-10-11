//
// Created by mohammad on 9/29/24.
//

#include <gtest/gtest.h>

#include "ndarray/ndarray.h"


TEST(NDArrayTest, DefaultConstructor) {
    NDArray<int> arr(std::vector<uint32_t>({3, 4}));
    EXPECT_EQ(arr.dim(), 2);
    EXPECT_EQ(arr.shape(), std::vector<uint32_t>({3, 4}));
    EXPECT_EQ(arr.size(), 12);
    EXPECT_EQ(arr.size(0), 3);
    EXPECT_EQ(arr.size(1), 4);
}

TEST(NDArrayTest, ConstructorWithArray) {
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    NDArray<int> arr(data, {3, 4});
    EXPECT_EQ(arr.dim(), 2);
    EXPECT_EQ(arr.shape(), std::vector<uint32_t>({3, 4}));
    EXPECT_EQ(arr.size(), 12);
    EXPECT_EQ(arr.size(0), 3);
    EXPECT_EQ(arr.size(1), 4);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[3], 4);
    EXPECT_EQ(arr[6], 7);
    EXPECT_EQ(arr[9], 10);
}

TEST(NDArrayTest, ConstructorWithSizes) {
    NDArray<int> arr(3, 4);
    EXPECT_EQ(arr.dim(), 2);
    EXPECT_EQ(arr.shape(), std::vector<uint32_t>({3, 4}));
    EXPECT_EQ(arr.size(), 12);
    EXPECT_EQ(arr.size(0), 3);
    EXPECT_EQ(arr.size(1), 4);
}

TEST(NDArrayTest, Fill) {
    NDArray<int> arr(std::vector<uint32_t>({3, 4}));
    arr.fill(10);
    for (int i = 0; i < 12; ++i) {
        EXPECT_EQ(arr[i], 10);
    }
}

TEST(NDArrayTest, EqualityOperator) {
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    NDArray<int> arr1(data, {3, 4});
    NDArray<int> arr2(data, {3, 4});
    EXPECT_TRUE(arr1 == arr2);
}

TEST(NDArrayTest, IndexingWithSingleIndex) {
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    NDArray<int> arr(data, {3, 4});
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[3], 4);
    EXPECT_EQ(arr[6], 7);
    EXPECT_EQ(arr[9], 10);
}

TEST(NDArrayTest, IndexingMatching) {
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    NDArray<int> arr(data, {3, 4});
    for(uint32_t i{0}; i < arr.size(0); ++i){
        for(uint32_t j{0}; j < arr.size(1); ++j) {
            EXPECT_EQ((arr[{i,j}]), arr(i,j));
        }
    }
}

TEST(NDArrayTest, IndexingWithMultipleIndices) {
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    NDArray<int> arr(data, {3, 4});
    arr.setAxisMode(NDArrayOrderingAxis::RowMajor);
    EXPECT_EQ(arr(0, 0), 1);
    EXPECT_EQ(arr(0, 1), 2);
    EXPECT_EQ(arr(0, 2), 3);
    EXPECT_EQ(arr(1, 0), 4);
    EXPECT_EQ(arr(1, 1), 5);
    EXPECT_EQ(arr(1, 2), 6);
    EXPECT_EQ(arr(2, 0), 7);
    EXPECT_EQ(arr(2, 1), 8);
    EXPECT_EQ(arr(2, 2), 9);
}

TEST(NDArrayTest, IndexingWithVector) {
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    NDArray<int> arr(data, {3, 4});
    arr.setAxisMode(NDArrayOrderingAxis::RowMajor);
    std::vector<uint32_t> indices = {0, 0};
    EXPECT_EQ(arr[indices], 1);
    indices = {0, 1};
    EXPECT_EQ(arr[indices], 2);
    indices = {0, 2};
    EXPECT_EQ(arr[indices], 3);
    indices = {1, 0};
    EXPECT_EQ(arr[indices], 4);
    indices = {1, 1};
    EXPECT_EQ(arr[indices], 5);
    indices = {1, 2};
    EXPECT_EQ(arr[indices], 6);
    indices = {2, 0};
    EXPECT_EQ(arr[indices], 7);
    indices = {2, 1};
    EXPECT_EQ(arr[indices], 8);
    indices = {2, 2};
    EXPECT_EQ(arr[indices], 9);
}

TEST(NDArrayTest, OutOfRangeIndex) {
    GTEST_SKIP() << "Skipping single test";

    int data[12] = {1, 2, 3, 4};
    NDArray<int> arr(data, {3, 4});
    EXPECT_THROW((arr[0, 5]), std::out_of_range);
    EXPECT_THROW((arr[1, 5]), std::out_of_range);
    EXPECT_THROW((arr[2, 5]), std::out_of_range);
    EXPECT_THROW((arr[3]),    std::out_of_range);
    EXPECT_THROW((arr[3, 0]), std::out_of_range);
    EXPECT_THROW((arr[3, 1]), std::out_of_range);
    EXPECT_THROW((arr[3, 2]), std::out_of_range);
    EXPECT_THROW((arr[3, 3]), std::out_of_range);
    }

TEST(NDArrayTest, VicePresident) {
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    NDArray<int> arr(data, {3, 4});
    arr[0, 0] = 10;
    EXPECT_EQ((arr[0, 0]), 10);
    arr[0, 1] = 20;
    EXPECT_EQ((arr[0, 1]), 20);
    arr[0, 2] = 30;
    EXPECT_EQ((arr[0, 2]), 30);
    arr[1, 0] = 40;
    EXPECT_EQ((arr[1, 0]), 40);
    arr[1, 1] = 50;
    EXPECT_EQ((arr[1, 1]), 50);
    arr[1, 2] = 60;
    EXPECT_EQ((arr[1, 2]), 60);
    arr[2, 0] = 70;
    EXPECT_EQ((arr[2, 0]), 70);
    arr[2, 1] = 80;
    EXPECT_EQ((arr[2, 1]), 80);
    arr[2, 2] = 90;
    EXPECT_EQ((arr[2, 2]), 90);
}
