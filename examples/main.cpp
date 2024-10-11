//
// Created by mohammad on 5/20/23.
//
#include <iostream>
#include "common.h"
#include "movingFilter.h"
#include "movingFilter/movingAverage.hpp"
#include "movingFilter/rankFilter.hpp"
#include "movingFilter/medianFilter.hpp"
#include "movingFilter/maximumFilter.hpp"
#include "movingFilter/minimumFilter.hpp"
#include "ndarray/array.h"
#include "ndarray/ndarray.h"
#include "ndarray/matrix.h"

#include <random>



////inline float myrand() { return (float)std::rand() / (RAND_MAX); }
//inline float myrand() { return float(std::rand() % 20); }

void calc_mse(std::vector<float> arr1, std::vector<float> arr2, int arr_size) {
	int mismatched = 0; 	
	float result = 0.0;
	std::vector<int> mismatched_indices; 


	for (uint32_t i = 0; i < arr_size; ++i) {
		result += (arr1[i] - arr2[i]);
		if (arr1[i] != arr2[i]) {
			mismatched ++;
			mismatched_indices.push_back(i);
		}
	}
	std::cout << "MSE: " << result << std::endl; 
	std::cout << "mismatched: " << mismatched << "/" << arr_size << std::endl; 
	for (const auto &item : mismatched_indices) {
		std::cout << item << " "; 
	}
	std::cout << std::endl; 
}

int abas(int low, int high) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(low, high);
	return int(dis(gen)); 
}


int main() {

    std::vector<float> data  {-7, 5, 13, 114, 16, 17, 18, 23, 21, 22, 26, 25, 21, 25, 21, 26, 25, 20, 22, 12, 15, 8, 9,
                              3, 2, 3, -2, -6, -7, -8, -9, -13, -10, -14, -14, -16, -12, -14, -18, -12, -9, -11, -7,
                              -10, -6, -5, -3, 3, 3, 9, 8, 5, 11, 18, 16, 14, 18, 17, 28, 29, 28, 26, 27, 21, 22, 24,
                              20, 16, 23, 16, 15, 11, 7, 6, 9, 5, -1, -7, -2, -9, -8, -15, -10, -15, -15, -15, -16, -14,
                              -10, -18, -15, -16, -8, -7, -9, -5, -1, 2, 99, 8, 10, 6, 16, 17, 19, 19, 18, 26, 24, 23,
                              22, 29, 24, 25, 26, 27, 23, 17, 19, 16, 15, 14, 7, 4, 2, 100, -4, -1, -7, -10, -13, -13,
                              -14, -16, -15, -14, -11, -10, -19, -17, -12, -15, -7, -11, -4, -3, 0, -4, 0, 7, 3, 5, 11,
                              15, 12, 14, 116, 20, 25, 21, 28, 24, 26, 22, 26, 28, 21, 18, 18, 16, 15, 13, 5, 4, 5, 5,
                              3, 0, -4, -2, -10, -11, -9, -10, -15, -11, -15, -12, -19, -19, -9, -15, -9, -11, -9, -6,
                              2, 101, 7, 7};

    std::vector<float> filtData(data.size());
	std::vector<float> filtData2(data.size());
	const uint32_t halfWindowSize = 2;
    const uint32_t windowSize = halfWindowSize * 2 + 1;

    printf("============================\n");
    show(data);

    printf(">> classic moving average\n");
    tic;
    filt::movingFilter(filtData, data, halfWindowSize, filt::kernel::average);
    toc;
    show(filtData);

    printf(">> modern moving average\n");
    tic;
    filt::MovingAverage<float> movingAverage(windowSize);
    movingAverage(filtData2, data);
    toc;
    show(filtData2);
	
	calc_mse(filtData, filtData2, data.size());
	
    printf("============================\n");
    show(data);

    printf(">> classic moving median\n");
    tic;//    mf.setDynamicPadding();
    filt::movingFilter(filtData, data, halfWindowSize, filt::kernel::median);
    toc;
    show(filtData);

    printf(">> modern moving median\n");
    tic;
    filt::MedianFilter<float> medianFilter(windowSize);
    //    mf.setDynamicPadding();
    medianFilter(filtData2, data);
    toc;
    show(filtData2);

	calc_mse(filtData, filtData2, data.size());

    printf("============================\n");
    show(data);

    printf(">> classic moving max\n");
	tic;//    mf.setDynamicPadding();
	filt::movingFilter(filtData, data, halfWindowSize, filt::kernel::maximum);
	toc;
	show(filtData);

	printf(">> modern moving maximum\n");
	tic;
	filt::MaximumFilter<float> maxFilter(windowSize);
	maxFilter(filtData2, data);
	toc;
	show(filtData2);

	calc_mse(filtData, filtData2, data.size());


	printf(">> modern rankfilter: -1\n");
	tic;
	filt::RankFilter<float> rankFilterMax(windowSize, -1);
	rankFilterMax(filtData2, data);
	toc;
	show(filtData2);

	calc_mse(filtData, filtData2, data.size());


    printf("============================\n");
    show(data);

    printf(">> classic moving min\n");
    tic;//    mf.setDynamicPadding();
    filt::movingFilter(filtData, data, halfWindowSize, filt::kernel::minimum);
    toc;
    show(filtData);

    printf(">> modern moving minimum\n");
    tic;
    filt::MinimumFilter<float> minFilter(windowSize);
    minFilter(filtData2, data);
    toc;
    show(filtData2);

	calc_mse(filtData, filtData2, data.size());


    printf(">> modern rankfilter: 0\n");
    tic;
    filt::RankFilter<float> rankFilterMin(windowSize, 0);
    rankFilterMin(filtData2, data);
    toc;
    show(filtData2);

	calc_mse(filtData, filtData2, data.size());


    printf("============================\n");

    Array<int> arr(20, 5);
    show(arr);

    Matrix<int> mat(20, 20, 2);
    show(mat);

    NDArray<int> ndarr({2,4,3}, 5);
    show((ndarr[{1, 0, 1}]));
    show(ndarr[6]);
    show(ndarr(1, 0, 1));
    show(ndarr.size(0));

    NDArray<int> ndarr2(2,4,3);
    ndarr2.fill(5);
    show(ndarr2.shape());
    show(ndarr2);
    show(ndarr2 == ndarr);

    NDArray<int> ndarr3(ndarr2.data(), {2,4,3});
    show(ndarr2 == ndarr3);


    printf("\ngood bye :)\n");
		
    return 0;
}
