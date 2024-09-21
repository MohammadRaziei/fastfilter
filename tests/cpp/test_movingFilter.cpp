#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <vector>
#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "movingFilter.h"
#include "movingFilter/medianFilter.hpp"

//#include "../../examples/common.h"

namespace fs = std::filesystem;


const fs::path CURRENT_WORK_DIR = fs::path(__FILE__).parent_path();


TEST(sortedInOut, test1) {
  const size_t size = 5;
  float a[size + 1] = {0, 0, 0, 0, 3}; // Last element is a dummy
  const float aout = 0;
  const float ain = 5;
  const float expected[size] = {0, 0, 0, 3, 5};

  // Call the function 
  filt::kernel::utils::sortedInOut(a, size, aout, ain);

  // Check the results
  ASSERT_TRUE(std::equal(expected, expected + size, a));
}






// A simple structure to hold a pair of vectors for input and output
struct TestCase {
    std::vector<float> input;
    std::vector<float> output;
    size_t windowSize;
    std::string kernel;
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
        testCase.kernel = item["kernel"].get<std::string>();

        test_cases.emplace_back(testCase);
    }

    return test_cases;
}


class MovfiltTest : public ::testing::TestWithParam<TestCase> {};


INSTANTIATE_TEST_SUITE_P(
        MovfiltTests,
        MovfiltTest,
        ::testing::ValuesIn(parse_json(
                (CURRENT_WORK_DIR.parent_path() / "movingfilter-data.json").string())));



TEST_P(MovfiltTest, MovfiltFunction) {
    auto& tc = GetParam();
    // Assuming medfilt is a function that takes a vector<int> and returns a vector<int>
    std::vector<float> output;
    filt::movingFilter(output, tc.input, tc.windowSize / 2, filt::kernels<float>[tc.kernel]);
    ASSERT_EQ(output, tc.output);
}

TEST_P(MovfiltTest, MovfiltFunctionInplace) {
    auto& tc = GetParam();
    // Assuming medfilt is a function that takes a vector<int> and returns a vector<int>
    std::vector<float> output = tc.input;
    filt::movingFilter(output, output, tc.windowSize / 2, filt::kernels<float>[tc.kernel]);
    ASSERT_EQ(output, tc.output);
}

TEST_P(MovfiltTest, MovfiltFunctionModern) {
	auto& tc = GetParam(); 
	std::vector<float> output(tc.output.size()); 
	if (tc.kernel == "median") {
		filt::MedianFilter<float> medianFilter(tc.windowSize); 
		medianFilter(output, tc.input);
		ASSERT_EQ(output, tc.output);
	}
	else {
		ASSERT_EQ(1, 2);
	}
}
