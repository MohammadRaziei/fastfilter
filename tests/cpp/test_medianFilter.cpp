#include <array>
#include <algorithm>
#include <gtest/gtest.h>

#include "medianFilter.h"

// class MedianFilterTest : public ::testing::Test {
// protected:
//     void SetUp() override {
//         input = {1.0, 2.0, 3.0, 4.0};
//     }

//     void TearDown() override {
//         input.clear();
//     }

//     std::vector<double> input;
// };

TEST(sortedInOut, test1) {
  const size_t size = 5;
  float a[size + 1] = {0, 0, 0, 0, 3}; // Last element is a dummy
  const float aout = 0;
  const float ain = 5;
  const float expected[size] = {0, 0, 0, 3, 5};

  // Call the function 
  utils::sortedInOut(a, size, aout, ain);

  // Check the results
  ASSERT_TRUE(std::equal(expected, expected + size, a));
}





// TEST_F(ArrayProductTest, MultiplierIs0) {
//     double multiplier = 0.0;
//     std::vector<double> expected = {0.0, 0.0, 0.0, 0.0};

//     arrayProduct(input, multiplier);

//     ASSERT_EQ(input, expected);
// }

// TEST_F(ArrayProductTest, MultiplierIsNegative) {
//     double multiplier = -1.5;
//     std::vector<double> expected = {-1.5, -3.0, -4.5, -6.0};

//     arrayProduct(input, multiplier);

//     ASSERT_EQ(input, expected);
// }
