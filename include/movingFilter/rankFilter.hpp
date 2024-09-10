#pragma once

#include <limits>
#include "movingFilter/movingFilter.hpp"
#include <functional>

namespace filt {
template <typename T>  // Template class for moving average filter
class RankFilterGeneral : public MovingFilter<T> {
   public:
//    using ArrayReducerFunc = T(*)(const T[], const uint32_t);
    using ArrayReducerFunc = std::function<T(const T[], const uint32_t)>;
    static void sortedInOut(T sortedData[], const uint32_t len, const T &outValue, const T &inValue);
   public:
    RankFilterGeneral(const uint32_t windowSize, ArrayReducerFunc reducerFunc,
                      enum PaddingMode mode = PaddingMode::Constant,
                      const T& cval = 0) :
          MovingFilter<T>(windowSize, windowSize + 1),
          reducerFunc(reducerFunc) {
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
template <typename T>
class RankFilter : public RankFilterGeneral<T> {
   public:
    RankFilter(uint32_t windowSize, int32_t rank,
               enum PaddingMode mode = PaddingMode::Constant,
               const T& cval = 0) :
          RankFilterGeneral<T>(windowSize,
                               std::bind(&RankFilter<T>::rankReducer, this, std::placeholders::_1, std::placeholders::_2, rank),
                               mode, cval) {
    }
   private:
    T rankReducer(const T arr[], const uint32_t size, int32_t rank) {
        return arr[rank % size];
    }
};
}
// https://docs.scipy.org/doc/scipy/reference/generated/scipy.ndimage.rank_filter.html







