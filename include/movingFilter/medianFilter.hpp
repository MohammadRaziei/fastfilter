#pragma once

#include <cstring>
#include <cmath>
#include <stdint.h>
#include <algorithm>
#include <map>
#include <limits>

#include "movingFilter/rankFilter.hpp"
#include "ndarray/matrix.h"


namespace filt {

template <typename T>  // Template class for moving average filter
class MedianFilter : public RankFilterGeneral<T> {
   public:
    MedianFilter(uint32_t windowSize,
                 enum PaddingMode mode = PaddingMode::Constant,
                 const T& cval = 0) :
          RankFilterGeneral<T>(windowSize, &median, mode, cval) {
    }
    static T median(const T arr[], const uint32_t size) {
        const uint32_t size2 = size >> 1;
        if(size % 2 == 0)
            //return (arr[size2 - 1] + arr[size2]) / 2;
	    return (arr[size2]);
        else
            return arr[size2];
    }
};

template <typename T>  // Template class for moving average filter
class RadixMedianFilter : public  MovingFilter<T> {
   public:
    RadixMedianFilter(uint32_t windowSize, uint32_t numMaxBits,
                 enum PaddingMode mode = PaddingMode::Constant,
                 const T& cval = 0) :
          MovingFilter<T>(windowSize, 0, mode, cval) {
    }
    static T median(const T arr[], uint32_t numMaxBits, const uint32_t size) {
    }
    virtual void fillBuffer(const T& value) override{
        MovingFilter<T>::fillBuffer(value);
    }

   protected:
        Matrix<T> alpha;
};


}  // namespace filt


