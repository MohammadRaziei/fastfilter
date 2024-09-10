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
class MinimumFilter : public RankFilterGeneral<T> {
   public:
    MinimumFilter(uint32_t windowSize,
          enum PaddingMode mode = PaddingMode::Constant,
          const T& cval = std::numeric_limits<T>::max()
          ) : RankFilterGeneral<T>(windowSize, &minimum, mode, cval) {
    }
    static T minimum(const T arr[], const uint32_t size) {
        return arr[0];
    }
};
}  // namespace filt


