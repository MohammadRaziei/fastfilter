#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

#include <string>
#include "medianFilter.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace nb = nanobind;
using namespace nb::literals;

inline std::vector<double> arrayProductBind(double scale, std::vector<double> arr){
    arrayProduct(arr, scale);
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

    m.def("arrayProduct", &arrayProductBind, nb::arg("scale"), nb::arg("arr"));


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
