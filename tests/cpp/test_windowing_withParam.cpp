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



// A simple structure to hold a pair of vectors for input and output
struct TestDataWithParam {
    std::vector<float> input;
	std::vector<float> window;
	std::vector<float> output;
	std::vector<float> parameter; 
	std::string windowType; 
};




// Parse the JSON file and return a vector of pairs (input, window, output)
std::vector<TestDataWithParam> parseJSON(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json data;
    file >> data;


    std::vector<TestDataWithParam> test_cases;
    for (auto& item : data) {
        TestDataWithParam testCase;
        testCase.input = item["input"].get<std::vector<float>>();
        testCase.output = item["output"].get<std::vector<float>>();
		testCase.window = item["window"].get<std::vector<float>>();
		testCase.parameter = item["parameter"].get<std::vector<float>>();
        testCase.windowType = item["windowType"].get<std::string>();
        test_cases.emplace_back(testCase);
    }

    return test_cases;
}


class windowWithParamTest : public ::testing::TestWithParam<TestDataWithParam> {};


INSTANTIATE_TEST_SUITE_P(
        windowWithParamTests,
        windowWithParamTest,
        ::testing::ValuesIn(parseJSON(
                (CURRENT_WORK_DIR.parent_path() / "windowing-data-withParam.json").string())));



TEST_P(windowWithParamTest, windowFunctionWithParam) {
    auto& tc = GetParam();
    std::vector<float> output(tc.window.size());
	std::vector<float> input(tc.window.size()); 
	std::copy(tc.input.begin(), tc.input.end(), input.begin());
	float parameter = tc.parameter[0];
	window::windowFunction(output, input, output.size(), parameter, window::windowsWithParam<float>["gaussian"]);
   
	ASSERT_TRUE(compareVectors(output, tc.output));
}

