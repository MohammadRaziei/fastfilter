<p align="center">
  <a href="https://github.com/mohammadraziei/MedianFilterCpp">
    <img src="https://github.com/MohammadRaziei/MedianFilterCpp/blob/master/docs/images/logo/logo-color.svg" alt="Logo">
  </a>
  <h3 align="center">
    High-Performance Non-Linear Filters and Window Functions Library in C++ with Python Bindings
  </h3>
  <h4 align="center">
	Comprehensive Library for Digital Signal Processing in C++, Python
  </h4>
</p>

[![mohammadraziei - MedianFilterCpp](https://img.shields.io/static/v1?label=mohammadraziei&message=fastfilter&color=white&logo=github)](https://github.com/mohammadraziei/MedianFilterCpp "Go to GitHub repo")
[![forks - MedianFilterCpp](https://img.shields.io/github/forks/mohammadraziei/MedianFilterCpp?style=social)](https://github.com/mohammadraziei/MedianFilterCpp)

![Python](https://img.shields.io/badge/Python-3.8%20%7C%203.9%20%7C%203.10%20%7C%203.11-blue)
![Cpp](https://img.shields.io/badge/C++-17-blue)


[![License](https://img.shields.io/badge/License-MIT-purple)](#license)
[![issues - MedianFilterCpp](https://img.shields.io/github/issues/mohammadraziei/MedianFilterCpp)](https://github.com/mohammadraziei/MedianFilterCpp/issues)


## About The Project

**fastfilter** is a powerful library written in C++ with Python bindings. It provides you with the capability to use commonly used window functions and non-linear filters that are utilized in digital signal processing. You can use it in both Python and C++.



### Features

Here are some key features of **fastfilter**



1. **Python Bindings**
   - **Easy Integration**: Install the project in `Python` and use the filters and window functions seamlessly.
   - **Cross-Language Compatibility**: Utilize the library in both `Python` and `C++` environments.

2. **Common Non-Linear Filters**
   - **Average Filter**: Computes the mean of the surrounding values, smoothing the signal.
   - **Median Filter**: Reduces noise by replacing each value with the median of neighboring values.
   - **Minimum Filter**: Selects the smallest value from the surrounding values, useful for edge detection.
   - **Maximum Filter**: Selects the largest value from the surrounding values, enhancing bright regions.
   - **High Performance**: Implemented using one of the fastest algorithms to ensure efficient processing.
   - **Versatile Usage**: Suitable for various signal processing projects.

3. **Window Functions**
   - **Triangular Window**: Simple and efficient, used for basic signal smoothing.
   - **Hamming Window**: Reduces the side lobes in the frequency domain, improving spectral analysis.
   - **Parzen Window**: Provides a smooth tapering of the signal, reducing spectral leakage.
   - **Hann Window**: Minimizes the first side lobe, commonly used in Fourier analysis.
   - **Blackman Window**: Offers better side lobe suppression, ideal for high-resolution spectral analysis.
   - **Gaussian Window**: Provides a smooth, bell-shaped curve, useful for time-frequency analysis.
   - **Tukey Window**: Combines rectangular and Hann windows, offering adjustable side lobe suppression.

4. **Header-Only Implementation**
   - **Ease of Use**: No need for separate compilation, simply include the headers in your project.
   - **Portability**: Easily integrate into various projects without dependency issues.

5. **Comprehensive Documentation**
   - **Detailed Guides**: Step-by-step instructions for using the filters and window functions.
   - **Examples**: Practical examples to help you get started quickly.



## Usage in python

Using `fastfilter` is intuitive and straightforward. Below are some examples to help you get started.

### Accessing Help

All classes come with a help section to guide you through their usage:
```python
import medianFilter
help(medianFilter)
print(medianFilter.__version__)
```

### Applying Filters

You can easily apply various filters to your signal data. Here are some examples:

#### Average Filter
```python
import numpy as np
import medianFilter as filt

signal = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
kernelSize = 5
output = filt.movingfilter(signal, kernelSize // 2, 'average')
print(output)
```

#### Median Filter
```python
import numpy as np
import medianFilter as filt

signal = np.array([1, 12, 7, 8, 1, 16, 2, 18, 9, 21])
kernelSize = 5
output = filt.movingfilter(signal, kernelSize // 2, 'median')
print(output)
```

#### Maximum and Minimum Filters
```python
import numpy as np
import medianFilter as filt

signal = np.array([1, 12, 7, 8, 1, 16, 2, 18, 9, 21])
kernelSize = 5

# Apply maximum filter
output_max = filt.movingfilter(signal, kernelSize // 2, 'maximum')
print(output_max)

# Apply minimum filter
output_min = filt.movingfilter(signal, kernelSize // 2, 'minimum')
print(output_min)
```

### Using Window Functions

You can also apply various window functions to your signal data. Here’s an example of using the Hamming window:
```python
import numpy as np
import windowFunctions as wf

# It should be completed here
# windowFunctions is not binded yet. 
```


## Usage in C++

Integrating `fastfilter` into your C++ projects is simple.

### Applying Filters

You can effortlessly apply various filters to your signal data.

#### Average Filter
```cpp
#include <iostream>
#include "medianFilter.h"

int main() { 
    std::vector<float> data {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Create output vector and variables
    std::vector<float> output(data.size());
    const uint32_t halfWindowSize = 2;

    // Apply median filter
    filt::movingFilter(output, data, halfWindowSize, filt::kernel::average);

    // Display the output
    for (const auto& val : output) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

#### Median Filter 
```cpp
#include <iostream>
#include "medianFilter.h"

int main() { 
    std::vector<float> data {1, 12, 7, 8, 1, 16, 2, 18, 9, 21};

    // Create output vector and variables
    std::vector<float> output(data.size());
    const uint32_t halfWindowSize = 2;

    // Apply median filter
    filt::movingFilter(output, data, halfWindowSize, filt::kernel::median);

    // Display the output
    for (const auto& val : output) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

#### Minimum and Maximum Filters
```cpp
#include <iostream>
#include "medianFilter.h"

int main() { 
    std::vector<float> data {1, 12, 7, 8, 1, 16, 2, 18, 9, 21};

    // Create output vector and variables
    std::vector<float> output(data.size());
    const uint32_t halfWindowSize = 2;

    // Apply minimum filter
    filt::movingFilter(output, data, halfWindowSize, filt::kernel::minimum);

    // Display the output
    std::cout << "Minimum Filter Output: ";
    for (const auto& val : output) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Apply maximum filter
    filt::movingFilter(output, data, halfWindowSize, filt::kernel::maximum);

    // Display the output
    std::cout << "Maximum Filter Output: ";
    for (const auto& val : output) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### Using Window Functions

#### Example 1: Basic Window Functions

```cpp
#include <iostream>
#include "windowing.h"
#include <vector>

int main(int argc, char** argv) {
    std::vector<float> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; 
    std::vector<float> output(input.size()); 

    // Apply various window functions
    window::windowFunction(output, input, input.size(), window::kernel::triangularWindow);
    window::windowFunction(output, input, input.size(), window::kernel::hammingWindow);
    window::windowFunction(output, input, input.size(), window::kernel::parzenWindow);
    window::windowFunction(output, input, input.size(), window::kernel::hannWindow);
    window::windowFunction(output, input, input.size(), window::kernel::blackmanWindow);
    window::windowFunction(output, input, input.size(), window::kernel::triangularWindow);

    return 0; 
}
```

#### Example 2: Window Functions with Parameters

```cpp
#include <iostream>
#include "windowing.h"
#include <vector>

int main(int argc, char** argv) {
    std::vector<float> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; 
    std::vector<float> output(input.size()); 

    const float parameter = 0.3; 

    // Apply window functions with parameters
    window::windowFunction(output, input, input.size(), parameter, window::kernel::gaussianWindow); 
    window::windowFunction(output, input, input.size(), parameter, window::kernel::tukeyWindow); 

    return 0; 
}
```

These examples demonstrate how to use various window functions provided by the library. The first example shows basic window functions, while the second example includes window functions that require additional parameters.


<!--
# CMAKE MATLAB PYTHON CPP EXAMPLE



```bash
cmake -B build 
cmake --build build 
ctest --test-dir build 
```
-->

