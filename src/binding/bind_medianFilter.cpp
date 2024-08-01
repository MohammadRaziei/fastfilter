#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/string.h>

#include <stdint.h>
#include <string>
#include "medianFilter.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace nb = nanobind;
using namespace nb::literals;

template <typename T>
inline std::vector<T> movingFilterBind(std::vector<T>& arr, const uint32_t halfWindowSize, const std::string& kernel){
    filt::movingFilter(arr, arr, halfWindowSize, filt::kernels<T>[kernel]);
    return arr;
}

NB_MODULE(_core, m) {
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
            >>> import medianFilter as filt
            >>> data = [1, 2, 3, 4, 5]
            >>> filt.movingfilter(data, 1, 'median')
            [1.5, 2.5, 3.5, 4.5]
            >>> filt.movingfilter(data, 1, 'average')
            [1.5, 2.5, 3.5, 4.5]

    )pbdoc";

m.def("movingfilter", &movingFilterBind<double>, nb::arg("arr"), nb::arg("halfWindowSize"), nb::arg("kernel")="median");

#ifdef VERSION_INFO
m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
m.attr("__version__") = "dev";
#endif
}