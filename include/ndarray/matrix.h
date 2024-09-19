//
// Created by mohammad on 9/19/24.
//

#ifndef FASTFILTER_MATRIX_H
#define FASTFILTER_MATRIX_H

#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <stdint.h>
#include "array.h"



enum class OrderingAxis {
    RowMajor = 0,
    ColumnMajor = 1
};

template <typename T>
class Matrix : public Array<T> {
   private:
    uint32_t _nrows;
    uint32_t _ncols;
    const OrderingAxis _orderingAxis;

   public:
    Matrix(uint32_t rows, uint32_t cols, const T& value, OrderingAxis orderingAxis = OrderingAxis::RowMajor)
        : Array<T>(rows * cols, value), _nrows(rows), _ncols(cols), _orderingAxis(orderingAxis) {}

    Matrix(const T data[], const uint32_t rows, const uint32_t cols) :
          Array<T>(data, rows * cols), _nrows(rows), _ncols(cols) {}

    T& operator()(uint32_t row, uint32_t col) {
        if (_orderingAxis == OrderingAxis::RowMajor) {
            return Array<T>::operator[](row * _ncols + col);
        } else {
            return Array<T>::operator[](col * _nrows + row);
        }
    }

    const T& operator()(uint32_t row, uint32_t col) const {
        if (_orderingAxis == OrderingAxis::RowMajor) {
            return Array<T>::operator[](row * _ncols + col);
        } else {
            return Array<T>::operator[](col * _nrows + row);
        }
    }

    [[nodiscard]] virtual std::string str(const uint32_t max_num_elements = 0, const std::string sep = ", ") const override {
        std::string _str = "[";
        uint32_t num_rows = max_num_elements > 0? std::min(_nrows, max_num_elements) : _nrows;
        for (uint32_t i = 0; i < num_rows; ++i) {
            _str += "[";
            for (uint32_t j = 0; j < _ncols; ++j) {
                _str += std::to_string(operator()(i, j));
                if (j + 1!= _ncols) {
                    _str += sep;
                }
            }
            _str += "]";
            if (i + 1!= num_rows) {
                _str +=  "\n ";
            }
        }
        if (_nrows > max_num_elements && max_num_elements > 0) {
            _str += "\n...\n";
        }
        _str += "]";
        return _str;
    }
};


#endif  // FASTFILTER_MATRIX_H







