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
class MaximumFilter : public RankFilterGeneral<T> {
   public:
    MaximumFilter(uint32_t windowSize,
              enum PaddingMode mode = PaddingMode::Constant,
              const T& cval = std::numeric_limits<T>::lowest()) :
          RankFilterGeneral<T>(windowSize, &maximum, mode, cval) {
    }
    static T maximum(const T arr[], const uint32_t size) {
        return arr[size - 1];
    }
};
}  // namespace filt


