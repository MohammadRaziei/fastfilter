#ifndef WINDOWING_H
#define WINDOWING_H

#include <vector>
#include <map>
#include <cmath>

namespace window {
	
	namespace kernel {
		template<typename T> 
		using WindowType = void (*) (std::vector<T>&, const uint32_t);

		// 1) Triangular window
		template<typename T>
		inline void triangularWindow(std::vector<T> &output, 
								const uint32_t N
								) {
			const uint32_t halfSize = N / 2; 
			const T step = (N % 2 == 0) ? 1.0 / halfSize : 1.0 / (halfSize + 1); 
			const T bias = (N % 2 == 0) ? step / 2 : step;

			output[halfSize] = 1;
			
			for (uint32_t i = 0; i < halfSize; ++i) {
				output[i] = i * step + bias; 
				output[N - 1 - i] = i * step + bias; 
			}
		} // end of triangularWindow

		// 2) Hamming window
		template<typename T>
		inline void hammingWindow(std::vector<T> &output, 
							const uint32_t N) {
			if (N == 1) {
				output[0] = 1; 
			}
			else {
				for (uint32_t i = 0; i < N; ++i) {
					output[i] = 0.54 - 0.46 * cos(2 * M_PI * i / (N - 1)); 
				}	
			}
		} // end of hammingWindow

		// 3) Parzen window
		template<typename T>
		inline void parzenWindow(std::vector<T> &output,
								const uint32_t N) {
			const uint32_t halfSize = N / 2; 
			const uint32_t quarterSize = N / 4; 

			const T bias = (N % 2 == 0) ? 0.5 : 1; 

			output[halfSize] = 1; 

			for (uint32_t i = 0; i < quarterSize; ++i) {
				output[N - halfSize + i] = 1 - 6 * std::pow((i + bias) / (N / 2.0), 2) + 6 * std::pow((i + bias) / (N / 2.0), 3); 
				output[halfSize - 1 - i] = 1 - 6 * std::pow((i + bias) / (N / 2.0), 2) + 6 * std::pow((i + bias) / (N / 2.0), 3);
			}
				
			for (uint32_t i = quarterSize; i < halfSize; ++i) {
				output[N - halfSize + i] = 2 * std::pow(1 - (i + bias) / (N / 2.0), 3); 
				output[halfSize - 1 - i] = 2 * std::pow(1 - (i + bias) / (N / 2.0), 3); 
			}

		} // enf of parzenWindow




		template<typename T>
		T mul(T num1, T num2) {
			return num1 * num2;
		}
		
		template<typename T>
		T add(T num1, T num2) {
			return num1 + num2; 
		}

		template<typename T>
		T sub(T num1, T num2) {
			return num1 - num2; 
		}

		template<typename T>
		T dev(T num1, T num2) {
			return num1 / num2; 
		}
		
		template<typename T>
		void vectorAddition(std::vector<T> &output,
							std::vector<T> input1,
							std::vector<T> input2, 
							int arrSize
							) {
			for (int i = 0; i < arrSize; ++i) {
				output[i] = input1[i] + input2[i]; 
			}
		}

	} // end of namespace kernel

	template<typename T>
	void windowFunction(std::vector<T> &output, 
						std::vector<T> &input, 
						const uint32_t N, 
						kernel::WindowType<T> winType
						) {
		std::vector<T> window(N); 
		winType(window, N);

		for (uint32_t i = 0; i < N; ++i) {
			output[i] = input[i] * window[i]; 
		}
		 
	}


	// mappping of window name to its function
	template<typename T>
	std::map<std::string, kernel::WindowType<T>> windows {
		{"triangular", kernel::triangularWindow}, 
		{"hamming", kernel::hammingWindow}, 
		{"parzen", kernel::parzenWindow}
	};




} // end of namespace window










#endif
