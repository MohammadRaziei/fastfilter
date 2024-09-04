#pragma once

#include <cstring>
#include <cmath>
#include <stdint.h>
#include <algorithm>
#include <map>
#include <limits>



namespace filt {
//    template<typename T>
//    void movingFilter(
//        T output[],
//        const T input[],
//        const size_t vecSize,
//        const uint32_t halfWindow,
//        kernel::KernelType<T> filtKernel){
//        const uint32_t windowSize = 2 * halfWindow + 1;
//        filtKernel(output, input, (uint32_t) vecSize,
//                   halfWindow, true);
//    }
//
//    template<typename T>
//    void movingFilter(
//        std::vector <T> &output,
//        const std::vector <T> &input,
//        const uint32_t halfWindow,
//        kernel::KernelType<T> filtKernel){
//        const size_t vectorSize = input.size();
//        output.resize(vectorSize);
//        return filt::movingFilter(output.data(), input.data(), vectorSize, halfWindow, filtKernel);
//    }


//            const T input[],
    //                const uint32_t len,
    //                const uint32_t halfWindow,
    //                const bool fromScratch = true
    //				) {
    //
    //            const uint32_t windowSize = 2 * halfWindow + 1;
    //            T sum = 0;
    //            if (!fromScratch) {
    //                for (uint32_t i = 1; i < windowSize - 1; ++i)
    //                    sum += input[i];
    //            }
    //
    //			T *buffer = new T[windowSize];
    //			memset(buffer, 0, (windowSize) * sizeof(T));
    //
    //			// main part process
    //			for (uint32_t i = 0; i < len; ++i) {
    //				sum += (input[i] - buffer[i % windowSize]);
    //				buffer[i % windowSize] = input[i];
    //				output[i] = sum / windowSize;
    //			}
    //			delete[] buffer;
    //        }

//            for (uint32_t i = 0; i < len; ++i) {
//					utils::sortedInOut(window, windowSize, buffer[i % windowSize], input[i]);
//					buffer[i % windowSize] = input[i];
//                    output[i] = arrayReducerFunc(window, windowSize);
//				}


    template<typename T>
    class InputBuffer{
       public:
        InputBuffer()  = default;
        InputBuffer(const uint32_t buffer_size) :
            buffer_size(buffer_size)
        {
            }

        void resize(const uint32_t size) {
            buffer.resize(size);
            buffer_size = size;
        }

        void fill(const T& value) {
            std::fill(buffer.begin(), buffer.end(), value);
        }

        void push(const T& value) {
            buffer[counter % buffer_size] = value;
            counter = (counter+1) % buffer_size;
        }

        T top() const {
            return buffer[counter % buffer_size];
        }

        T pop(const T value) {
            const uint32_t index = counter % buffer_size;
            counter = (counter+1) % buffer_size;
            const T b = buffer[index];
            buffer[index] = value;
            return b;
        }

       protected:
        std::vector<T> buffer;
        uint32_t buffer_size;
        uint32_t counter = 0;
    };


    template<typename T>
    class MovingFilter{
       public:
        MovingFilter() = default;
        MovingFilter(const uint32_t halfWindow, const uint32_t auxWindowSize = 1) :
              windowSize(2 * halfWindow + 1) {
            buffer.resize(windowSize);
            auxWindow.resize(auxWindowSize);
        };

        void padBuffer(const T paddingValue) {
            buffer.fill(paddingValue);
            std::fill(auxWindow.begin(), auxWindow.end(), paddingValue);
            fillbuffer = true;
        }

        void setDynamicPadding() {
            this->fillbuffer = false;
        }

        void operator()(T output[], const T input[], const uint32_t size) {
                for (uint32_t i = 0; i < size; ++i) {
                    operator()(output[i], input[i]);
            }
        }
        void operator()(std::vector<T>& output, const std::vector<T>& input) {
            output.resize(input.size());
            operator()(output.data(), input.data(), input.size());
        }
        void operator()(T& output, const T input) {
            if (!fillbuffer) {
                buffer.fill(input);
                std::fill(auxWindow.begin(), auxWindow.end(), input);
                fillbuffer = true;
            }
            changeAuxiliaryWindow(input, buffer.pop(input));
            output = calculateOutputFromWindow();
        }

        virtual void changeAuxiliaryWindow(const T& inp, const T& out){
            auxWindow[0] = inp;
        }

        virtual T calculateOutputFromWindow(){
            return auxWindow[0];
        }
       protected:
        InputBuffer<T> buffer;
        std::vector<T> auxWindow;
        const uint32_t windowSize = 1;
       private:
        bool fillbuffer = false;
//        bool dynamic_padding = false;
    };

}  // namespace filt


