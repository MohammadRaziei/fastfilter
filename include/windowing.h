#ifndef WINDOWING_H
#define WINDOWING_H

#include <vector>

namespace window {
	
	namespace kernel {
		template<typename T> 
		using WindowType = T (*) (T, T);

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
	void windowFunction(T &output, 
						const T num1, 
						const T num2, 
						kernel::WindowType<T> winType
						) {
		output = winType(num1, num2); 
	}




} // end of namespace window










#endif
