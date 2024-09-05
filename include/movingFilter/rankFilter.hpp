#pragma once

#include <limits>
#include "movingFilter/movingFilter.hpp"

namespace filt {
template <typename T>  // Template class for moving average filter
class RankFilterGeneral : public MovingFilter<T> {
   public:
    using ArrayReducerFunc = T(*)(const T[], const uint32_t);
    static void sortedInOut(T sortedData[], const uint32_t len, const T &outValue, const T &inValue);
   public:
    RankFilterGeneral(uint32_t windowSize, ArrayReducerFunc reducerFunc, const T* paddingValuePtr = nullptr) :
          MovingFilter<T>(windowSize, windowSize + 1),
              reducerFunc(reducerFunc) {
        if(paddingValuePtr) this->padBuffer(*paddingValuePtr);
    }

    virtual void changeAuxiliaryWindow(const T& inp, const T& out) override{
        sortedInOut(this->auxWindow.data(), this->windowSize, out, inp);
    }
    virtual T calculateOutputFromWindow() override{
        return reducerFunc(this->auxWindow.data(), this->windowSize);
    }

   protected:
    ArrayReducerFunc reducerFunc;
   };
}





template<typename T>
inline void filt::RankFilterGeneral<T>::sortedInOut(T sortedData[],
                                       const uint32_t len,
                                       const T &outValue,
                                       const T &inValue) {
    bool TasksOut = true, TasksIn = true;
    if (outValue == inValue)
        return;
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



namespace filt {
template <typename T>  // Template class for moving average filter
class MedianFilter : public RankFilterGeneral<T> {
   public:
    MedianFilter(uint32_t windowSize) :
          RankFilterGeneral<T>(windowSize, &median, &paddingValue) {
    }
    static T median(const T arr[], const uint32_t size) {
        const uint32_t size2 = size >> 1;
        if(size % 2 == 0)
            return (arr[size2 - 1] + arr[size2]) / 2;
        else
            return arr[size2];
    }
    static constexpr T paddingValue = static_cast<T>(0);
};

template <typename T>  // Template class for moving average filter
class MaximumFilter : public RankFilterGeneral<T> {
   public:
    MaximumFilter(uint32_t windowSize) :
          RankFilterGeneral<T>(windowSize, &maximum, &paddingValue) {
    }
    static T maximum(const T arr[], const uint32_t size) {
        return arr[size - 1];
    }
    static constexpr T paddingValue = std::numeric_limits<T>::lowest();
};

template <typename T>  // Template class for moving average filter
class MinimumFilter : public RankFilterGeneral<T> {
   public:
    MinimumFilter(uint32_t windowSize) :
          RankFilterGeneral<T>(windowSize, &minimum, &paddingValue) {
    }
    static T minimum(const T arr[], const uint32_t size) {
        return arr[0];
    }
    static constexpr T paddingValue = std::numeric_limits<T>::max();
};

template <typename T>  // Template class for moving average filter
class RankFilter : public RankFilterGeneral<T> {
   public:
    RankFilter(uint32_t windowSize, int32_t rank) :
          RankFilter<T>(windowSize, [rank](const T arr[], const uint32_t size) {return arr[rank % size];}) {
    }
};
}
// https://docs.scipy.org/doc/scipy/reference/generated/scipy.ndimage.rank_filter.html







