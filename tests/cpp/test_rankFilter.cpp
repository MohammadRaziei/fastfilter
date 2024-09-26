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
#include "movingFilter/maximumFilter.hpp"
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
std::vector<TestCase> parse_json_file(const std::string& filename) {
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


class RankfiltTest : public ::testing::TestWithParam<TestCase> {};


INSTANTIATE_TEST_SUITE_P(
        RankfiltTests,
        RankfiltTest,
        ::testing::ValuesIn(parse_json_file(
                (CURRENT_WORK_DIR.parent_path() / "rankfilter-data.json").string())));

// 1 tests) read from the json file for max and min kernel for rankfilter modern mode
TEST_P(RankfiltTest, RankfiltFunctionModern) {
	auto& tc = GetParam(); 
	std::vector<float> output(tc.output.size()); 
	if (tc.kernel == "maximum") {
		filt::RankFilter<float> rankFilterMax(tc.windowSize, -1);
		rankFilterMax(output, tc.input);
		ASSERT_EQ(output, tc.output);
	}
	else if (tc.kernel == "minimum") {
		filt::RankFilter<float> rankFilterMin(tc.windowSize, 0); 
		rankFilterMin(output, tc.input); 
		ASSERT_EQ(output, tc.output);
	}
}
