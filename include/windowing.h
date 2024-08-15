#ifndef WINDOWING_H
#define WINDOWING_H

#include <vector>

namespace window {
	
	namespace kernel {
		template<typename T> 
		using WindowType = void (*) (std::vector<T>&, const int);

		// 1) Triangular window
		template<typename T>
		inline void triangularWindow(std::vector<T> &output, 
								const int N
								) {
			const int halfSize = N / 2; 
			const T step = (N % 2 == 0) ? 1.0 / halfSize : 1.0 / (halfSize + 1); 
			const T bias = (N % 2 == 0) ? step / 2 : step;

			output[halfSize] = 1;
			
			for (uint32_t i = 0; i < halfSize; ++i) {
				output[i] = i * step + bias; 
				output[N - 1 - i] = i * step + bias; 
			}
		}



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
						const int N, 
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
		{"triangular", kernel::triangularWindow}
	};




} // end of namespace window










#endif
