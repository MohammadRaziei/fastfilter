#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <vector>
#include <gtest/gtest.h>
#include <random>

#include <nlohmann/json.hpp>

#include "movingFilter.h"
#include "movingFilter/medianFilter.hpp"
#include "movingFilter/maximumFilter.hpp"
#include "movingFilter/minimumFilter.hpp"
#include "movingFilter/movingAverage.hpp"

//#include "../../examples/common.h"

namespace fs = std::filesystem;


const fs::path CURRENT_WORK_DIR = fs::path(__FILE__).parent_path();


// A simple structure to hold a pair of vectors for input and output
struct TestCase {
    std::vector<float> input;
    std::vector<float> output;
    size_t windowSize;
    std::string kernel;
};




// Parse the JSON file and return a vector of pairs (input, output)
std::vector<TestCase> parse_json_even(const std::string& filename) {
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


class MovfiltEvenTest : public ::testing::TestWithParam<TestCase> {};


INSTANTIATE_TEST_SUITE_P(
        MovfiltEvenTests,
        MovfiltEvenTest,
        ::testing::ValuesIn(parse_json_even(
                (CURRENT_WORK_DIR.parent_path() / "movingfilter-even.json").string())));


// 3 tests) read from the json file for median, max and avg kernels for even size kernels
TEST_P(MovfiltEvenTest, MovfiltEvenFunction) {
	auto& tc = GetParam(); 
	std::vector<float> output(tc.output.size()); 
	if (tc.kernel == "median") {
		filt::MedianFilter<float> medianFilter(tc.windowSize); 
		medianFilter(output, tc.input);
		ASSERT_EQ(output, tc.output);
	}
	else if (tc.kernel == "maximum") {
		filt::MaximumFilter<float> maxFilter(tc.windowSize);
		maxFilter(output, tc.input);
		ASSERT_EQ(output, tc.output);
	}
	else if (tc.kernel == "minimum") {
		filt::MinimumFilter<float> minFilter(tc.windowSize);
		minFilter(output, tc.input);
		ASSERT_EQ(output, tc.output);
	}
	else if (tc.kernel == "average") {
		filt::MovingAverage<float> movingAverage(tc.windowSize);
		movingAverage(output, tc.input);
		ASSERT_EQ(output, tc.output);
	}
}

