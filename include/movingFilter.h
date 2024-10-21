#ifndef MOVINGILTER_H
#define MOVINGILTER_H

#include <omp.h>
#include <cstring>
#include <cmath>
#include <stdint.h>
#include <algorithm>
#include <map>
#include <limits>

//enum class ParallelMethod {
//    NONE, CPU, GPU
//};



namespace filt {

    namespace kernel {
        template<typename T>
        using KernelType = void (*)(T[], const T[], const uint32_t, const uint32_t, const bool);


        namespace utils{
            template<typename T>
            inline void sortedInOut(T sortedData[],
                    const uint32_t len,
                    const T &outValue,
                    const T &inValue) {
                bool TasksOut = true, TasksIn = true;
                if (outValue == inValue)
                    return;
                //    std::vector<T> sortedData(len + 1);
                //    std::copy_n(sortedData2, len + 1, sortedData.data());
                sortedData[len] = inValue;
                T value = sortedData[0], saveValue;
                for (uint32_t j = 0, i = 0; (TasksOut || TasksIn) && i < len; ++i) {
                    if (TasksOut && value == outValue) {
                        value = sortedData[++j];
                        TasksOut = false;
                    }
                    if (TasksIn && value >= inValue) {
                        sortedData[i] = inValue;
                        TasksIn = false;
                    } else {
                        saveValue = value;
                        value = sortedData[++j];

                        sortedData[i] = saveValue;
                    }
                }
            }



            template<typename T>
            inline void sortBasedKernel(T output[],
                    const T input[],
                    const uint32_t len,
                    const uint32_t windowSize,
                    T (*arrayReducerFunc)(const T arr[], const uint32_t len),
                    const bool fromScratch = true, 
					const T paddingVal = std::numeric_limits<T>::infinity(), 
          const bool even = true 
					) {

                T *window = new T[windowSize + 1];		// for keeping sorted array
				T *buffer = new T[windowSize];			// for keeping previous window

				std::fill(window, window + windowSize, paddingVal); 
				std::fill(buffer, buffer + windowSize, paddingVal); 

                if (!fromScratch) {
                    std::sort(window, window + windowSize);
                }

				// main part process
                for (uint32_t i = 0; i < len; ++i) {
					utils::sortedInOut(window, windowSize, buffer[i % windowSize], input[i]); 
					buffer[i % windowSize] = input[i]; 
                    output[i] = arrayReducerFunc(window, windowSize);
				}
                delete[] window;
				delete[] buffer; 
            }

            template <typename T>
            T medianArray(const T arr[], const uint32_t len){
                const auto len2 = len / 2;
                if (len & 1){
                    return arr[len2];
                } else {
                    //return (arr[len2] + arr[len2 - 1]) / 2;
                    return (arr[len2]); 
                }
            }

            template <typename T>
            T maxArray(const T arr[], const uint32_t len){
                return arr[len - 1];

            }

            template <typename T>
            T minArray(const T arr[], const uint32_t len){
                return arr[0];

            }
        }

        template<typename T>
        inline void average(T output[],
                const T input[],
                const uint32_t len,
                const uint32_t windowSize,
                const bool fromScratch = true 
				) {

            T sum = 0;
            if (!fromScratch) {
                for (uint32_t i = 1; i < windowSize - 1; ++i)
                    sum += input[i];
            }
				
			T *buffer = new T[windowSize]; 
			memset(buffer, 0, (windowSize) * sizeof(T)); 

			// main part process
			for (uint32_t i = 0; i < len; ++i) { 
				sum += (input[i] - buffer[i % windowSize]); 
				buffer[i % windowSize] = input[i]; 
				output[i] = sum / windowSize; 
			}
			delete[] buffer; 
        }

        template<typename T>
        inline void median(T output[],
                           const T input[],
                           const uint32_t len,
                           const uint32_t windowSize,
                           const bool fromScratch = true) {
            kernel::utils::sortBasedKernel(output, input, len, windowSize,
                                           kernel::utils::medianArray, fromScratch, 
										   static_cast<T>(0));
        }

        template<typename T>
        inline void maximum(T output[],
                        const T input[],
                        const uint32_t len,
                        const uint32_t windowSize,
                        const bool fromScratch = true) { 
            kernel::utils::sortBasedKernel(output, input, len, windowSize,
                                           kernel::utils::maxArray, fromScratch,
										   std::numeric_limits<T>::lowest());
        }
        template<typename T>
        inline void minimum(T output[],
                        const T input[],
                        const uint32_t len,
                        const uint32_t windowSize,
                        const bool fromScratch = true) {  
            kernel::utils::sortBasedKernel(output, input, len, windowSize,
                                           kernel::utils::minArray, fromScratch, 
										   std::numeric_limits<T>::max());
        }


    }

    template<typename T>
    void movingFilter(
            T output[],
            const T input[],
            const size_t vecSize,
//             const uint32_t halfWindow,
            const uint32_t windowSize, 
            kernel::KernelType<T> filtKernel) { 
//             const bool even = false){ 
        filtKernel(output, input, (uint32_t) vecSize,
                   windowSize, true);
    }



    template<typename T>
    void movingFilter(
            std::vector <T> &output,
            const std::vector <T> &input,
            const uint32_t windowSize, 
//             const uint32_t halfWindow,
            kernel::KernelType<T> filtKernel) {
//             const bool even = false){
             
        const size_t vectorSize = input.size();
        output.resize(vectorSize);
        return filt::movingFilter(output.data(), input.data(), vectorSize, windowSize, filtKernel);
    }

    template<typename T>
    std::map<std::string, kernel::KernelType<T>> kernels{
            {"median", kernel::median},
            {"average", kernel::average},
            {"maximum", kernel::maximum},
            {"minimum", kernel::minimum}
        };
}  // namespace filt


#endif // MOVINGILTER_H
