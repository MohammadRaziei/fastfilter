#pragma once

#include <limits>
#include "movingFilter/movingFilter.hpp"

namespace filt {
template <typename T>  // Template class for moving average filter
class RankFilter : public MovingFilter<T> {
   public:
    using ArrayReducerFunc = T(*)(const T[], const uint32_t);
    static void sortedInOut(T sortedData[], const uint32_t len, const T &outValue, const T &inValue);
   public:
    RankFilter(uint32_t windowSize, ArrayReducerFunc reducerFunc, T* paddingValuePtr = nullptr) :
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
inline void filt::RankFilter<T>::sortedInOut(T sortedData[],
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
class MedianFilter : public RankFilter<T> {
   public:
    MedianFilter(uint32_t windowSize) :
      RankFilter<T>(windowSize, &median) {
        this->padBuffer(static_cast<T>(0));
    }
    static T median(const T arr[], const uint32_t size) {
        const uint32_t size2 = size >> 1;
        if(size % 2 == 0)
            return (arr[size2 - 1] + arr[size2]) / 2;
        else
            return arr[size2];
    }
};

template <typename T>  // Template class for moving average filter
class MaximumFilter : public RankFilter<T> {
   public:
    MaximumFilter(uint32_t windowSize) :
          RankFilter<T>(windowSize, &maximum) {
        this->padBuffer(std::numeric_limits<T>::lowest());
    }
    static T maximum(const T arr[], const uint32_t size) {
        return arr[size - 1];
    }
};

template <typename T>  // Template class for moving average filter
class MinimumFilter : public RankFilter<T> {
   public:
    MinimumFilter(uint32_t windowSize) :
          RankFilter<T>(windowSize, &minimum) {
        this->padBuffer(std::numeric_limits<T>::max());
    }
    static T minimum(const T arr[], const uint32_t size) {
        return arr[0];
    }
};
}








