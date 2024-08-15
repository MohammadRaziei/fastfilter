#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <vector>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "windowing.h"


namespace fs = std::filesystem;


const fs::path CURRENT_WORK_DIR = fs::path(__FILE__).parent_path();


template<typename T>
bool compareNums(T a, T b, T epsilon = 0.0001f) {
    return fabs(a - b) < epsilon;
}

template<typename T>
bool compareVectors(const std::vector<T>& vec1, const std::vector<T>& vec2, T epsilon = 0.0001f) {
    if (vec1.size() != vec2.size()) {
        return false;
    }
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (!compareNums(vec1[i], vec2[i], epsilon)) {
            return false;
        }
    }
    return true;
}





TEST(windowSub, test1) {
	const float num1 = 14.0; 
	const float num2 = 8.0; 
	float result; 
	const float expected = 6.0; 
	result = window::kernel::sub(num1, num2);
	
	ASSERT_EQ(expected, result); 
}



// A simple structure to hold a pair of vectors for input and output
struct TestData {
    std::vector<float> input;
	std::vector<float> window;
	std::vector<float> output;
	std::string windowType; 
};




// Parse the JSON file and return a vector of pairs (input, window, output)
std::vector<TestData> parseJson(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json data;
    file >> data;


    std::vector<TestData> test_cases;
    for (auto& item : data) {
        TestData testCase;
        testCase.input = item["input"].get<std::vector<float>>();
        testCase.output = item["output"].get<std::vector<float>>();
		testCase.window = item["window"].get<std::vector<float>>(); 
        testCase.windowType = item["windowType"].get<std::string>();
        test_cases.emplace_back(testCase);
    }

    return test_cases;
}


class windowTest : public ::testing::TestWithParam<TestData> {};


INSTANTIATE_TEST_SUITE_P(
        windowTests,
        windowTest,
        ::testing::ValuesIn(parseJson(
                (CURRENT_WORK_DIR.parent_path() / "windowing-data.json").string())));



TEST_P(windowTest, windowFunction) {
    auto& tc = GetParam();
    std::vector<float> output(tc.window.size());
	std::vector<float> input(tc.window.size()); 
	std::copy(tc.input.begin(), tc.input.end(), input.begin());
	window::windowFunction(output, input, output.size(), window::windows<float>[tc.windowType]);
   
	ASSERT_TRUE(compareVectors(output, tc.output));
}


/*
TEST_P(MovfiltTest, MovfiltFunctionInplace2) {
    auto& tc = GetParam();
    // Assuming medfilt is a function that takes a vector<int> and returns a vector<int>
    std::vector<float> output = tc.input;
    filt::movingFilter(output, output, tc.windowSize / 2, filt::kernels<float>[tc.kernel]);
    ASSERT_EQ(output, tc.output);
}
*/

