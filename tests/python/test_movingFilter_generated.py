#!/bin/python3
from __future__ import annotations

import pytest
import numpy as np
import scipy.signal

import medianFilter as filt # Assuming this is the module where your filter function is defined
import scipy


minSize = 10
maxSize = 200
minElement = -100
maxElement = 100
dataNum = 20
kernelSize = 5
halfWindowSize = kernelSize // 2

testCases = [np.random.randint(minElement, maxElement, np.random.randint(minSize, maxSize)) for i in range(dataNum)]

def movingAverageFilter(a, kernelSize):
    out =  np.convolve(a, np.ones(kernelSize) / kernelSize, mode='full')
    out = out[:a.shape[0]]
    return out

def medianFilter(a, kernelSize):
    input = np.insert(a, 0, np.zeros(2))
    output = scipy.signal.medfilt(input, kernelSize)
    output = output[:a.shape[0]]
    return output


@pytest.mark.parametrize("a", testCases)
def test_movingAverageFilter(a):
    output1 = filt.movingfilter(a, kernelSize // 2, 'average')
    output1 = np.array(output1).astype(np.float16)
    output2 = movingAverageFilter(a, kernelSize).astype(np.float16)
    assert np.array_equal(output1, output2)


@pytest.mark.parametrize("a", testCases)
def test_medianFilter(a):
    output1 = filt.movingfilter(a, kernelSize // 2, 'median')
    output1 = np.array(output1).astype(np.float16)
    output2 = medianFilter(a, kernelSize).astype(np.float16)
    assert np.array_equal(output1, output2)


