#pragma once

#include "movingFilter/movingFilter.hpp"

namespace filt {
    template <typename T>  // Template class for moving average filter
    class MovingAverage : public MovingFilter<T> {
       public:
        MovingAverage(int windowSize) : MovingFilter<T>(windowSize, 1) {
            this->padBuffer(static_cast<T>(0));
        }
        virtual void changeAuxiliaryWindow(const T& inp, const T& out) override{
            this->auxWindow[0] += inp - out;
        }

        virtual T calculateOutputFromWindow() override{
            return this->auxWindow[0] / this->windowSize;
        }

    };

}




