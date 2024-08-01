#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <vector>
#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "medianFilter.h"

//#include "../../examples/common.h"

namespace fs = std::filesystem;


const fs::path CURRENT_WORK_DIR = fs::path(__FILE__).parent_path();


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






// A simple structure to hold a pair of vectors for input and output
struct TestCase {
    std::vector<float> input;
    std::vector<float> output;
    size_t windowSize;
};




// Parse the JSON file and return a vector of pairs (input, output)
std::vector<TestCase> parse_json(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json data;
    file >> data;


    std::vector<TestCase> test_cases;
    for (auto& item : data) {
        std::vector<int> input = item["input"];
        std::vector<int> output = item["output"];

        TestCase testCase;
        testCase.input = item["input"].get<std::vector<float>>();
        testCase.output = item["output"].get<std::vector<float>>();
        testCase.windowSize = item["windowSize"].get<size_t>();

        test_cases.emplace_back(testCase);
    }

    return test_cases;
}


class MedfiltTest : public ::testing::TestWithParam<TestCase> {};


INSTANTIATE_TEST_SUITE_P(
        MedfiltTests,
        MedfiltTest,
        ::testing::ValuesIn(parse_json(
                (CURRENT_WORK_DIR.parent_path() / "data.json").string())));



TEST_P(MedfiltTest, MedfiltFunction) {
    auto& tc = GetParam();
    // Assuming medfilt is a function that takes a vector<int> and returns a vector<int>
    std::vector<float> output;
    filt::movingFilter(output, tc.input, tc.windowSize/2, filt::kernel::median);
    ASSERT_EQ(output, tc.output);
}

TEST_P(MedfiltTest, MedfiltFunctionInplace) {
    auto& tc = GetParam();
    // Assuming medfilt is a function that takes a vector<int> and returns a vector<int>
    std::vector<float> output = tc.input;
    filt::movingFilter(output, output, tc.windowSize / 2, filt::kernel::median);
    ASSERT_EQ(output, tc.output);
}
