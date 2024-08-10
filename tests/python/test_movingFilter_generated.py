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


@pytest.mark.parametrize("a", testCases)
def movingAverageFilter(a):
    output1 = filt.movingfilter(a, kernelSize // 2, 'average')
    output2 = np.convolve(a, np.ones(kernelSize) / kernelSize, mode='same')
    assert (output1[halfWindowSize:] == output2[:-halfWindowSize]).any()


@pytest.mark.parametrize("a", testCases)
def medianFilter(a):
    output1 = filt.movingfilter(a, kernelSize // 2, 'median')
    output2 = scipy.signal.medfilt(a, kernelSize)
    assert (output1[halfWindowSize:] == output2[:-halfWindowSize]).all()


