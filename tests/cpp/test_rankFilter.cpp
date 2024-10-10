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

// 2 tests) read from the json file for max and min kernel for rankfilter modern mode
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




int generate_random_int(int low, int high) {
	static std::random_device rd; 
	static std::mt19937 gen(rd()); 
	std::uniform_real_distribution<> dis(low, high); 
	return int(dis(gen)); 
}

std::vector<TestCase> generate_random_test_cases(uint32_t num_cases, 
		uint32_t vec_size 
		) {
	std::vector<TestCase> test_cases;
	std::vector<std::string> kernels = {"maximum", "minimum"};
	int low = -100; 
	int high = 100;
	
	TestCase test1; 
	test1.windowSize = (rand() % 5) * 2 + 1; 

	for (uint32_t i = 0; i < num_cases; ++i) {
		test1.kernel = kernels[i % kernels.size()]; 
		for (uint32_t j = 0; j < vec_size; ++j) {
			test1.input.push_back(generate_random_int(low, high));
		}
		test_cases.push_back(test1);
	}
	return test_cases; 
}

class RankfiltRandomTest : public ::testing::TestWithParam<TestCase> {}; 

INSTANTIATE_TEST_SUITE_P(
	RankfiltRandomTests, 
	RankfiltRandomTest, 
	::testing::ValuesIn(generate_random_test_cases(20, 200))); 

TEST_P(RankfiltRandomTest, RankfiltRandomT) {
	auto& tc = GetParam(); 	
	std::vector<float> classic_output(200);
	std::vector<float> modern_output(200);
	if (tc.kernel == "maximum") {
		filt::RankFilter<float> rankFilterMax(tc.windowSize, -1); 
		rankFilterMax(modern_output, tc.input);

		filt::movingFilter(classic_output, tc.input, tc.windowSize / 2, filt::kernel::maximum);
		ASSERT_EQ(classic_output, modern_output); 
	}
	else if (tc.kernel == "minimum") {
		filt::RankFilter<float> rankFilterMin(tc.windowSize, 0);  
		rankFilterMin(modern_output, tc.input); 

		filt::movingFilter(classic_output, tc.input, tc.windowSize / 2, filt::kernel::minimum); 
		ASSERT_EQ(classic_output, modern_output);
	}
}
