#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include <omp.h>
#include <cstring>
#include <cmath>
#include <stdint.h>
#include <algorithm>
#include <map>

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
                    const uint32_t halfWindow,
                    T (*arrayReducerFunc)(const T arr[], const uint32_t len),
                    const bool fromScratch = true) {
                const uint32_t windowSize = 2 * halfWindow + 1;
                T *window = new T[windowSize + 1];		// for keeping sorted array
				T *buffer = new T[windowSize];			// for keeping previous window

				memset(window, 0, (windowSize + 1) * sizeof(T));
				memset(buffer, 0, (windowSize) * sizeof(T)); 

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
                    return (arr[len2] + arr[len2 - 1]) / 2;
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
                const uint32_t halfWindow,
                const bool fromScratch=true) {
            const uint32_t windowSize = 2 * halfWindow + 1;
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
                           const uint32_t halfWindow,
                           const bool fromScratch = true) {
            kernel::utils::sortBasedKernel(output, input, len, halfWindow,
                                           kernel::utils::medianArray, fromScratch);
        }

        template<typename T>
        inline void maximum(T output[],
                        const T input[],
                        const uint32_t len,
                        const uint32_t halfWindow,
                        const bool fromScratch = true) {
            kernel::utils::sortBasedKernel(output, input, len, halfWindow,
                                           kernel::utils::maxArray, fromScratch);
        }
        template<typename T>
        inline void minimum(T output[],
                        const T input[],
                        const uint32_t len,
                        const uint32_t halfWindow,
                        const bool fromScratch = true) {
            kernel::utils::sortBasedKernel(output, input, len, halfWindow,
                                           kernel::utils::minArray, fromScratch);
        }


    }

    template<typename T>
    void movingFilter(
            T output[],
            const T input[],
            const size_t vecSize,
            const uint32_t halfWindow,
            kernel::KernelType<T> filtKernel){
//            const ParallelMethod &method = ParallelMethod::NONE)
        const uint32_t windowSize = 2 * halfWindow + 1;
        filtKernel(output, input, (uint32_t) vecSize,
                   halfWindow, true);
}
//            case ParallelMethod::CPU: {
                // const uint32_t lenFrames =
                // (uint32_t)ceilf((T)vecSize / NUM_THREADS);
                // Concurrency::parallel_for(
                //     (uint32_t)0, (uint32_t)vecSize, lenFrames, [&](const
                //     uint32_t& i) {
                //       if (i + lenFrames <= vecSize)
                //         filtKernel(output.data() + i, inp.data() + i, lenFrames,
                //                    halfWindow);
                //       else
                //         filtKernel(output.data() + i, inp.data() + i,
                //                    (uint32_t)vecSize - i, halfWindow);
                //     });

//                const uint32_t lenFrames =
//                        (uint32_t) ceilf((T) vecSize / (float) NUM_THREADS);
//
//#pragma omp parallel for
//                for (uint32_t i = 0; i < vecSize; i += lenFrames) {
//                    uint32_t end = i + lenFrames;
//                    if (end > vecSize)
//                        end = vecSize;
//
//                    filtKernel(output + i, inp.data() + i, end - i,
//                               halfWindow);
//                }
//                break;
//            }
//            case ParallelMethod::GPU: {
//                break;
//            }
//        }
//        output.resize(vecSize);
//    }

    template<typename T>
    void movingFilter(
            std::vector <T> &output,
            const std::vector <T> &input,
            const uint32_t halfWindow,
            kernel::KernelType<T> filtKernel){
        const size_t vectorSize = input.size();
        output.resize(vectorSize);
        return filt::movingFilter(output.data(), input.data(), vectorSize, halfWindow, filtKernel);
    }

    template<typename T>
    std::map<std::string, kernel::KernelType<T>> kernels{
            {"median", kernel::median},
            {"average", kernel::average},
            {"maximum", kernel::maximum},
            {"minimum", kernel::minimum}
        };
}  // namespace filt


#endif
