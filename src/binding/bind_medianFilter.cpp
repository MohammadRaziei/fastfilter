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
        nanobind example plugin
        -----------------------

        .. currentmodule:: arrayProduct

        .. autosummary::
           :toctree: _generate
 
           arrayProduct
    )pbdoc";
    //

    m.def("movingfilter", &movingFilterBind<double>, nb::arg("arr"), nb::arg("halfWindowSize"), nb::arg("kernel"));


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
