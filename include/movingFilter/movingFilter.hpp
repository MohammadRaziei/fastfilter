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

    enum class PaddingMode {
        None, // 0
        Constant, // 1
        Nearest, // 2
    };

    template<typename T>
    class MovingFilter{
       public:
        MovingFilter() = default;
        MovingFilter(const uint32_t windowSize, const uint32_t auxWindowSize = 1,
                     enum PaddingMode mode = PaddingMode::Nearest,
                     const T& cval = static_cast<T>(0)) :
              windowSize(windowSize) {
            this->buffer.resize(windowSize);
            this->auxWindow.resize(auxWindowSize);
            setPaddingMode(mode, cval);
        };

        void padBuffer(const T paddingValue) {
            this->cval = paddingValue;
            this->buffer.fill(paddingValue);
            std::fill(this->auxWindow.begin(), this->auxWindow.end(), paddingValue);
            this->fillbuffer = true;
        }

        void setPaddingMode(PaddingMode mode, const T& cval = static_cast<T>(0)) {
            this->fillbuffer = false;
            this->mode = mode;
            if (mode == PaddingMode::Constant) {
                padBuffer(cval);
            }
        }

        void operator()(T output[], const T input[], const uint32_t size) {
            if (size == 0)
                return;
            if (!fillbuffer && (mode == PaddingMode::Nearest)) {
                padBuffer(input[0]);
            }
            for (uint32_t i = 0; i < size; ++i) {
                process_one(output[i], input[i]);
            }
        }

        void operator()(std::vector<T>& output, const std::vector<T>& input) {
            output.resize(input.size());
            operator()(output.data(), input.data(), input.size());
        }
        void operator()(T& output, const T input) {
            if (!fillbuffer & (mode == PaddingMode::Nearest)) {
                buffer.fill(input);
                std::fill(auxWindow.begin(), auxWindow.end(), input);
                fillbuffer = true;
            }
            process_one(output, input);
        }


        std::vector<T> operator()(const std::vector<T>& input) {
            std::vector<T> output;
            output.resize(input.size());
            operator()(output.data(), input.data(), input.size());
            return output;
        }

        T operator()(const T input) {
            T output;
            operator()(output, input);
            return output;
        }


        virtual void changeAuxiliaryWindow(const T& inp, const T& out){
            auxWindow[0] = inp;
        }

        virtual T calculateOutputFromWindow(){
            return auxWindow[0];
        }
       private:
        void process_one(T& output, const T& input) {
            changeAuxiliaryWindow(input, buffer.top());
            buffer.push(input);
            output = calculateOutputFromWindow();
        }

       protected:
        InputBuffer<T> buffer;
        std::vector<T> auxWindow;
        const uint32_t windowSize = 1;
        PaddingMode mode;
        T cval = 0;
       private:
        bool fillbuffer = false;
    };

}  // namespace filt


