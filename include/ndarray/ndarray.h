//
// Created by mohammad on 9/25/24.
//

#ifndef FASTFILTER_NDARRAY_H
#define FASTFILTER_NDARRAY_H

#include <numeric>
#include <stdint.h>
#include <tuple>
#include <array>

#include "array.h"


template<typename T>
class NDArray : public Array<T> {
public:
    NDArray(const std::vector<uint32_t> &shape, const T &value = 0) : Array<T>(
            std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<T>()), value
    ), _dim(shape.size()), _shape(shape) {
    }

    template<typename... Sizes>
    NDArray(Sizes... sizes) : NDArray(std::vector<uint32_t>{static_cast<uint32_t>(sizes)...}) {
        static_assert((std::is_integral_v<Sizes> &&...), "Indices must be of an integral type");
    }

    NDArray(const T arr[], const std::vector<uint32_t> &shape) : Array<T>(arr,
                                                                          std::accumulate(shape.begin(), shape.end(), 1,
                                                                                          std::multiplies<T>())),
                                                                 _dim(shape.size()), _shape(shape) {
    }

    const uint32_t dim() const {
        return _dim;
    }

    const std::vector<uint32_t> shape() const {
        return _shape;
    }

    const uint32_t size() const {
        return Array<T>::size();
    }

    const uint32_t size(uint32_t axis) const {
        if (axis >= _dim) {
            throw std::out_of_range("Axis index out of range.");
        }
        return _shape[axis];
    }

    // Indexing with multiple indices
    void fill(const T &value) {
        Array<T>::fill(value);
    }

    bool operator==(const NDArray<T>& other) const noexcept{
        if (this == &other) return true;
        if (_dim != other._dim || _shape != other._shape){
            return false;
        }
        return Array<T>::operator==(other);
    }

    const uint32_t calculateIndex(const std::vector<uint32_t> &indices) const {
        if (indices.size() != _dim) {
            throw std::out_of_range("Number of indices does not match dimension");
        }
        uint32_t index = 0;
        uint32_t stride = 1;
        for (size_t i = 0; i < indices.size(); ++i) {
            index += indices[i] * stride;
            stride *= _shape[i];
        }
        if (index >= this->size()) {
            throw std::out_of_range("Index out of range");
        }
        return index;
    }

    T &operator[](const uint32_t index) {
        return Array<T>::operator[](index);
    }

    const T &operator[](const uint32_t index) const {
        return Array<T>::operator[](index);
    }

    T &operator[](const std::vector<uint32_t> &indices) {
        uint32_t index = calculateIndex(indices);
        return Array<T>::operator[](index);
    }

    const T &operator[](const std::vector<uint32_t> &indices) const {
        uint32_t index = calculateIndex(indices);
        return Array<T>::operator[](index);
    }

    template<typename... Indices>
    T &operator()(Indices... indices) {
        static_assert((std::is_integral_v<Indices> &&...), "Indices must be of an integral type");
        std::vector<uint32_t> idx{static_cast<uint32_t>(indices)...};
        return operator[](idx);
    }

    template<typename... Indices>
    const T &operator()(Indices... indices) const {
        static_assert((std::is_integral_v<Indices> &&...), "Indices must be of an integral type");
        std::vector<uint32_t> idx{static_cast<uint32_t>(indices)...};
        return operator[](idx);
    }


private:
    uint32_t _dim;
    std::vector<uint32_t> _shape;
};

#endif  // FASTFILTER_NDARRAY_H
