#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/string.h>

#include <stdint.h>
#include <string>
#include "movingFilter.h"
#include "movingFilter/medianFilter.hpp"
#include "movingFilter/maximumFilter.hpp"
#include "movingFilter/minimumFilter.hpp"
#include "movingFilter/movingAverage.hpp"


namespace nb = nanobind;
using namespace nb::literals;

template <typename T>
inline std::vector<T> movingFilterBind(std::vector<T>& arr, const uint32_t halfWindowSize, const std::string& kernel){
    filt::movingFilter(arr, arr, halfWindowSize, filt::kernels<T>[kernel]);
    return arr;
}
template <typename T>
inline std::vector<T> movingModernFilterBind(std::vector<T>& arr, const uint32_t WindowSize, const std::string& kernel){
    std::vector<T> output(arr.size()); 
    if (kernel == "median") { 
      filt::MedianFilter<T> medianFilter(WindowSize); 
      medianFilter(output, arr); 
    }
    else if (kernel == "maximum") { 
      filt::MaximumFilter<T> maxFilter(WindowSize); 
      maxFilter(output, arr); 
    }
    else if (kernel == "average") { 
      filt::MovingAverage<T> movingAverage(WindowSize); 
      movingAverage(output, arr); 
    }
    else if (kernel == "minimum") { 
      filt::MinimumFilter<T> minFilter(WindowSize); 
      minFilter(output, arr); 
    }
    return arr;
}



NB_MODULE(NB_MODULE_NAME, m) {
m.doc() = R"pbdoc(
        Moving Filter Module

        This module provides functionality for applying a moving filter
        to a vector of data points. The filter can be specified as either a
        'median' filter or a 'moving average' filter, operating over a sliding
        window defined by the halfWindowSize.

        Functions:
            movingfilter: Applies a moving filter to the input vector.

        Parameters:
            arr: Input vector of data points.
            halfWindowSize: Half size of the sliding window used for filtering.
            kernel: Type of kernel to use for filtering. Can be 'median' or 'average'.

        Example usage:
            >>> import fastfilter.filter as filt
            >>> data = [1, 2, 3, 4, 5]
            >>> filt.movingfilter(data, 1, 'median')
            [1.5, 2.5, 3.5, 4.5]
            >>> filt.movingfilter(data, 1, 'average')
            [1.5, 2.5, 3.5, 4.5]

    )pbdoc";

m.def("modernfilter", &movingFilterBind<double>, nb::arg("arr"), nb::arg("WindowSize"), nb::arg("kernel")="median");

}
