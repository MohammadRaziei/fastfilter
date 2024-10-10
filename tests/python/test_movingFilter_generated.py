#!/bin/python3
from __future__ import annotations

import numpy as np
import pytest
import scipy
import scipy.signal

import fastfilter.filter as filt  # Assuming this is the module where your filter function is defined

maxHalfWinSize = 10
minSize = maxHalfWinSize * 2 + 1
maxSize = 200
minElement = -100
maxElement = 100
dataNum = 40

testCases = (np.random.randint(minElement, maxElement, np.random.randint(minSize, maxSize)).astype(np.float32) for i in range(dataNum))


def movingAverageFilter(a, kernelSize):
    out =  np.convolve(a, np.ones(kernelSize) / kernelSize, mode='full')
    return out[:a.shape[0]]

def medianFilter(a, kernelSize):
    inp = np.insert(a, 0, np.zeros(kernelSize // 2))
    output = scipy.ndimage.median_filter(inp, kernelSize, mode='constant')
    return output[:a.shape[0]]

def maximumFilter(a, kernelSize):
    inp = np.insert(a, 0, np.full(kernelSize // 2, -np.inf))
    output = scipy.ndimage.maximum_filter(inp, size=kernelSize)
    return output[:a.shape[0]]

def minimumFilter(a, kernelSize):
    inp = np.insert(a, 0, np.full(kernelSize // 2, np.inf))
    output = scipy.ndimage.minimum_filter(inp, size=kernelSize)
    return output[:a.shape[0]]

pyMovingFilter= {
    "median": medianFilter,
    "average": movingAverageFilter,
    "minimum": minimumFilter,
    "maximum": maximumFilter
}

@pytest.mark.parametrize("kernel", pyMovingFilter.keys())
@pytest.mark.parametrize("hafWinSize", range(1, maxHalfWinSize+1))
@pytest.mark.parametrize("arr", testCases)
def test_medianFilter(arr, kernel, hafWinSize):
    output = filt.movingfilter(arr, hafWinSize, kernel)
    expected = pyMovingFilter[kernel](arr, hafWinSize*2+1)
    assert np.allclose(output, expected, atol=1e-6)
