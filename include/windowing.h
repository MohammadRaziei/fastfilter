#ifndef WINDOWING_H
#define WINDOWING_H

#include <vector>
#include <map>
#include <cmath>

namespace window {
	
	namespace kernel {
		template<typename T> 
		using WindowType = void (*) (T [], const uint32_t);

		template<typename T>
		using WindowTypeWithParam = void (*) (T [], const uint32_t, const T);

		// 1) Triangular window
		template<typename T>
		inline void triangularWindow(//std::vector<T> &output, 
								T output[], 
								const uint32_t N
								) {
			const uint32_t halfSize = N / 2; 
			const T step = (N % 2 == 0) ? 1.0 / halfSize : 1.0 / (halfSize + 1); 
			const T bias = (N % 2 == 0) ? step / 2 : step;

			output[halfSize] = 1;
			
			for (uint32_t i = 0; i < halfSize; ++i) {
				output[i] = output[N - 1 - i] =  i * step + bias; 
			}
		} // end of triangularWindow

		// 2) Hamming window
		template<typename T>
		inline void hammingWindow(//std::vector<T> &output, 
							T output[], 
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
		inline void parzenWindow(//std::vector<T> &output,
								T output[], 
								const uint32_t N) {
			const uint32_t halfSize = N / 2; 
			const uint32_t quarterSize = N / 4; 

			const T bias = (N % 2 == 0) ? 0.5 : 1; 

			output[halfSize] = 1; 

			for (uint32_t i = 0; i < quarterSize; ++i) {
				output[N - halfSize + i] = output[halfSize - 1 - i] =  1 - 6 * std::pow((i + bias) / (N / 2.0), 2) + 6 * std::pow((i + bias) / (N / 2.0), 3); 
			}
				
			for (uint32_t i = quarterSize; i < halfSize; ++i) {
				output[N - halfSize + i] = output[halfSize - 1 - i] =  2 * std::pow(1 - (i + bias) / (N / 2.0), 3); 
			}

		} // enf of parzenWindow

		// 4) Hann window
		template<typename T>
		inline void hannWindow(//std::vector<T> &output, 
							   T output[], 
							   const uint32_t N) {
			if (N == 1) {
				output[0] = 1;
			}
			else {
				for (uint32_t i = 0; i < N; ++i) {
					output[i] = 0.5 * (1 - cos(2 * M_PI * i / (N - 1))); 
				}
			}
			
		} // end of hannWindow

		// 5) Blackman window
		template<typename T>
		inline void blackmanWindow(//std::vector<T> &output,
								   T output[], 
								   const uint32_t N) {
			const uint32_t halfSize = N / 2; 

			output[halfSize] = 1; 

			for (uint32_t i = 0; i < halfSize; ++i) {
				output[i] = output[N - 1 - i] = 0.42 - 0.5 * cos(2 * M_PI * i / (N - 1)) + 0.08 * cos(4 * M_PI * i / (N - 1));
			}
			
		} // end of blackman window

		// 6) Gaussian window
		template<typename T>
		inline void gaussianWindow(//std::vector<T> &output, 
								   T output[], 
								   const uint32_t N, 
								   const T parameter) {
			const uint32_t halfSize = N / 2; 
			
			const T bias = (N % 2 == 0) ? 0.5 : 1;

			output[halfSize] = 1;

			for (uint32_t i = 0; i < halfSize; ++i) {
				output[halfSize - 1 - i] = output[N - halfSize + i] = std::exp(-0.5 * std::pow(parameter * 2 * (i + bias) / (N - 1), 2));
			}
		} // end of gaussian window

		// 7) Tukey window
		template<typename T>
		inline void tukeyWindow(//std::vector<T> &output, 
								T output[], 
								const uint32_t N, 
								const T parameter) {
			const T point1 = parameter * (N - 1) / 2;
			const T point2 = (1 - parameter / 2) * (N - 1);

			uint32_t i; 
			for (i = 0; i < point1; ++i) {
				output[i] = 0.5 * (1 + cos(2 * M_PI / parameter * (i * 1.0 / (N - 1) - parameter / 2.0)));
			}
			for (i; i < point2; ++i) {
				output[i] = 1; 
			}
			for (i; i < N; ++i) {
				output[i] = 0.5 * (1 + cos(2 * M_PI / parameter * (i * 1.0 / (N - 1) - 1 + parameter / 2.0)));
			}
			if (N == 1) {
				output[0] = 1;
			}
		} // end of tukey window

	} // end of namespace kernel

	template<typename T>
	void windowFunction(T output[], 
						const T input[], 
						const uint32_t N, 
						kernel::WindowType<T> winType) {
		std::vector<T> window(N); 
		winType(window.data(), N);

		for (uint32_t i = 0; i < N; ++i) {
			output[i] = input[i] * window[i]; 
		}
		 
	}

	template<typename T>
	void windowFunction(std::vector<T> &output, 
						std::vector<T> &input, 
						const uint32_t N, 
						kernel::WindowType<T> winType) {
	
		return window::windowFunction(output.data(), input.data(), N, winType);
	}

	template<typename T>
	inline void windowFunction(T output[],  
							   const T input[], 
							   const uint32_t N,
							   const T parameter, 
							   kernel::WindowTypeWithParam<T> winType) {
		std::vector<T> window(N); 
		winType(window.data(), N, parameter);

		for (uint32_t i = 0; i < N; ++i) {
			output[i] = input[i] * window[i]; 
		}
	}


	template<typename T>
	inline void windowFunction(std::vector<T> &output, 
							   const std::vector<T> &input, 
							   const uint32_t N,
							   const T parameter, 
							   kernel::WindowTypeWithParam<T> winType) {

		return window::windowFunction(output.data(), input.data(), N, parameter, winType);
	}


	// mappping of window name to its function
	template<typename T>
	std::map<std::string, kernel::WindowType<T>> windows {
		{"triangular", kernel::triangularWindow}, 
		{"hamming", kernel::hammingWindow}, 
		{"parzen", kernel::parzenWindow},
		{"hann", kernel::hannWindow}, 
		{"blackman", kernel::blackmanWindow}
	};

	template<typename T>
	std::map<std::string, kernel::WindowTypeWithParam<T>> windowsWithParam {
		{"gaussian", kernel::gaussianWindow},
		{"tukey", kernel::tukeyWindow}
	};

} // end of namespace window




#endif
