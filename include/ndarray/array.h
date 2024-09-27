//
// Created by mohammad on 9/19/24.
//

#ifndef FASTFILTER_ARRAY_H
#define FASTFILTER_ARRAY_H

#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <algorithm>

template <typename T>
class Array {
   private:
    bool _self_ownership;
    uint32_t _size;
    T* _data;

   public:
    // Constructor

    Array(uint32_t size = 0, const T& value = static_cast<T>(0)) : _size(size), _data(nullptr), _self_ownership(true) {
        _data = (T*)malloc(size * sizeof(T));
        if (!_data) {
            throw std::runtime_error("Memory allocation failed.");
        }
        // Initialize all elements to default value (0)
        this->fill(value);
    }

    Array(const T* _begin, const T* _end, const bool ownership = true) : _size(_end - _begin), _self_ownership(ownership) {
        if (_self_ownership) {
            _data = (T*)malloc(_size * sizeof(T));
            if (!_data) {
                throw std::runtime_error("Memory allocation failed.");
            }
            for (uint32_t i = 0; i < _size; ++i) {
                _data[i] = _begin[i];
            }
        } else {
            _data = const_cast<T*>(_begin);
        }
    }


    // Constructor from array
    Array(const T data[], const uint32_t size) : Array(data, data + size, false) {}

    // Copy Constructor
    Array(const Array& other) : _size(other._size), _self_ownership(true) {
        _data = (T*)malloc(_size * sizeof(T));
        if (!_data) {
            throw std::runtime_error("Memory allocation failed.");
        }
        std::copy(other.begin(), other.end(), this->begin());
    }

    // Move Constructor
    Array(Array&& other) noexcept : _size(other._size), _data(other._data),
          _self_ownership(other._self_ownership) {
        other._data = nullptr;
        other._self_ownership = false;
    }

    // TODO: make this rvalue
    Array copy() const{
        return Array(_data, _data + _size);
    }
    // Move Assignment Operator
    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            if (_self_ownership) {
                free(_data);
            }
            _size = other._size;
            _data = other._data;
            _self_ownership = other._self_ownership;
            other._data = nullptr;
            other._self_ownership = false;
        }
        return *this;
    }

    // Destructor
    ~Array() {
        if (_self_ownership) {
            free(_data);
        }
    }

    // Add element to the end of the array
    void push_back(const T& value) {
        _size++;
        T* newData = (T*)realloc(_data, _size * sizeof(T));
        if (!newData) {
            throw std::runtime_error("Memory reallocation failed.");
        }
        _data = newData;
        _data[_size - 1] = value;
    }

    // Bracket Operator
    T& operator[](uint32_t index) {
        if (index >= _size) {
            throw std::out_of_range("Index out of range.");
        }
        return _data[index];
    }

    // Const Bracket Operator
    const T& operator[](uint32_t index) const {
        if (index >= _size) {
            throw std::out_of_range("Index out of range.");
        }
        return _data[index];
    }

    // Size
    const uint32_t size() const {
        return _size;
    }

    // Data
    T* data() {
        return _data;
    }

    // Const Data
    const T* data() const {
        return _data;
    }

    // Begin
    T* begin() {
        return _data;
    }

    // Const Begin
    const T* begin() const {
        return _data;
    }

    // End
    T* end() {
        return _data + _size;
    }

    // Const End
    const T* end() const {
        return _data + _size;
    }

    // To Vector
    std::vector<T> vector() const {
        return std::vector<T>(_data, _data + _size);
    }

    Array<T>& fill(const T& value){
        std::fill(begin(), end(), value);
        return *this;
    }

    [[nodiscard]] bool ownsData() const {
        return _self_ownership;
    }

    // To String
    [[nodiscard]] virtual std::string str(const uint32_t max_num_elements = 0, const std::string sep = ", ") const {
        std::string _str = "[";
        uint32_t num_elements = max_num_elements > 0 ? std::min(_size, max_num_elements) : _size;
        for (uint32_t i = 0; i < num_elements; ++i) {
            _str += std::to_string(_data[i]);
            if (i + 1  != num_elements) {
                _str += sep;
            }
        }
        if (_size > max_num_elements && max_num_elements > 0 ) {
            _str += sep + "...";
        }
        _str += "]";
        return _str;
    }
};


// Operator <<
template <typename T>
std::ostream& operator<<(std::ostream& os, const Array<T>& arr) {
    os << arr.str(10);
    return os;
}

#endif  // FASTFILTER_ARRAY_H
