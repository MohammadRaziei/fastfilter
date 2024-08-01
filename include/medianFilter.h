#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include <omp.h>
#include <cstring>
#include <cmath>
#include <stdint.h>
#include <algorithm>

enum class ParallelMethod {
    NONE, CPU, GPU
};

namespace utils {
    template<typename T>
    inline void movingAverageFilterKernel(T output[],
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
        sum += input[windowSize - 1];
        output[0] = sum / windowSize;
        for (uint32_t i = 0; i < len - 1; ++i) {
            sum += (input[i + windowSize] - input[i]);
            output[i + 1] = sum / windowSize;
        }
    }

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
    inline void medianFilterKernel(T output[],
                                   const T input[],
                                   const uint32_t len,
                                   const uint32_t halfWindow,
                                   const bool fromScratch = true) {
        const uint32_t windowSize = 2 * halfWindow + 1;
        T *temp = new T[windowSize + 1];
        memcpy(temp, input, windowSize * sizeof(T));

        if (!fromScratch) {
            std::sort(temp, temp + windowSize);
        }
        output[0] = temp[halfWindow];
        for (uint32_t i = 0; i < len - 1; ++i) {
            sortedInOut(temp, windowSize, input[i], input[i + windowSize]);
            output[i + 1] = temp[halfWindow];
        }
        delete[] temp;
    }

    template<typename T>
    void movingFilter(
            T output[],
            const T input[],
            const size_t vecSize,
            const uint32_t halfWindow,
            void (&filtKernel)(T[], const T[], const uint32_t, const uint32_t, const bool)){
//            const ParallelMethod &method = ParallelMethod::NONE)
        const uint32_t windowSize = 2 * halfWindow + 1;
        std::vector <T> inp(vecSize + windowSize);
        memset(inp.data(), 0, (windowSize - 1) * sizeof(T));
        memcpy(inp.data() + windowSize - 1, input, vecSize * sizeof(T));

                filtKernel(output, inp.data(), (uint32_t) vecSize,
                           halfWindow, true);
//                break;
//            }
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
            void (&filtKernel)(T[], const T[], const uint32_t, const uint32_t, const bool)){
        const size_t vectorSize = input.size();
        output.resize(vectorSize);
        return utils::movingFilter(output.data(), input.data(), vectorSize, filtKernel);
    }

}  // namespace utils

#endif