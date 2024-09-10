#pragma once

#include <cstring>
#include <cmath>
#include <stdint.h>
#include <algorithm>
#include <map>
#include <limits>

#include "movingFilter/rankFilter.hpp"


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
            return (arr[size2 - 1] + arr[size2]) / 2;
        else
            return arr[size2];
    }
};

}  // namespace filt


