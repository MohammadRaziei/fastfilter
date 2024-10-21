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


// 3 tests) read from the json file for median, max and avg kernels
TEST_P(MovfiltTest, MovfiltFunction) {
    auto& tc = GetParam();
    // Assuming medfilt is a function that takes a vector<int> and returns a vector<int>
    std::vector<float> output;
    filt::movingFilter(output, tc.input, tc.windowSize, filt::kernels<float>[tc.kernel]);
    ASSERT_EQ(output, tc.output);
}

// 3 tests) read from the json file for median, max and avg kernels in inplace mode
TEST_P(MovfiltTest, MovfiltFunctionInplace) {
    auto& tc = GetParam();
    // Assuming medfilt is a function that takes a vector<int> and returns a vector<int>
    std::vector<float> output = tc.input;
    filt::movingFilter(output, output, tc.windowSize, filt::kernels<float>[tc.kernel]);
    ASSERT_EQ(output, tc.output);
}

// 3 tests) read from the json file for median, max and avg kernels for modern mode
TEST_P(MovfiltTest, MovfiltFunctionModern) {
	auto& tc = GetParam(); 
	std::vector<float> output(tc.output.size()); 
	if (tc.kernel == "median") {
		filt::MedianFilter<float> medianFilter(tc.windowSize); 
		medianFilter(output, tc.input);
	}
	else if (tc.kernel == "maximum") {
		filt::MaximumFilter<float> maxFilter(tc.windowSize);
		maxFilter(output, tc.input);
	}
	else if (tc.kernel == "average") {
		filt::MovingAverage<float> movingAverage(tc.windowSize);
		movingAverage(output, tc.input);
	}
  else if (tc.kernel == "minimum") {
    filt::MinimumFilter<float> minFilter(tc.windowSize);
    minFilter(output, tc.input); 
  }
    else {
        throw std::runtime_error("unsupported filter");
    }
    EXPECT_EQ(output, tc.output);
}



int generate_random_int1(int low, int high) {
	static std::random_device rd; 
	static std::mt19937 gen(rd()); 
	std::uniform_real_distribution<> dis(low, high); 
	return int(dis(gen)); 
}

std::vector<TestCase> generate_random_test_cases1(uint32_t num_cases, 
		uint32_t vec_size 
		) {
	std::vector<TestCase> test_cases;
	std::vector<std::string> kernels = {"average", "median", "maximum", "minimum"};
	int low = -100; 
	int high = 100;
	
	TestCase test1; 
	test1.windowSize = (rand() % 5) * 2 + 1; 

	for (uint32_t i = 0; i < num_cases; ++i) {
		test1.kernel = kernels[i % kernels.size()]; 
		for (uint32_t j = 0; j < vec_size; ++j) {
			test1.input.push_back(generate_random_int1(low, high));
		}
		test_cases.push_back(test1);
		test1.input.clear();
	}
	return test_cases; 
}

class MovfiltRandomTest : public ::testing::TestWithParam<TestCase> {}; 

INSTANTIATE_TEST_SUITE_P(
	MovfiltRandomTests, 
	MovfiltRandomTest, 
	::testing::ValuesIn(generate_random_test_cases1(20, 200))); 

TEST_P(MovfiltRandomTest, MovfiltRandomT) {
	auto& tc = GetParam(); 	
	std::vector<float> classic_output(200);
	std::vector<float> modern_output(200);
	
	if (tc.kernel == "average") {
		filt::movingFilter(classic_output, tc.input, tc.windowSize, filt::kernel::average); 

		filt::MovingAverage<float> movingAverage(tc.windowSize); 
		movingAverage(modern_output, tc.input);

		ASSERT_EQ(classic_output, modern_output); 
	}
	else if (tc.kernel == "median") {
		filt::movingFilter(classic_output, tc.input, tc.windowSize, filt::kernel::median); 

		filt::MedianFilter<float> medianFilter(tc.windowSize); 
		medianFilter(modern_output, tc.input);

		ASSERT_EQ(classic_output, modern_output);
	}
	else if (tc.kernel == "maximum") {
		filt::movingFilter(classic_output, tc.input, tc.windowSize, filt::kernel::maximum); 

		filt::MaximumFilter<float> maxFilter(tc.windowSize); 
		maxFilter(modern_output, tc.input);

		ASSERT_EQ(classic_output, modern_output);
	}
	else if (tc.kernel == "minimum") {
		filt::movingFilter(classic_output, tc.input, tc.windowSize, filt::kernel::minimum); 

		filt::MinimumFilter<float> minFilter(tc.windowSize); 
		minFilter(modern_output, tc.input); 

		ASSERT_EQ(classic_output, modern_output); 
	}
	
}
