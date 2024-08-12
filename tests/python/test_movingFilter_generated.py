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
    inp = np.insert(a, 0, np.zeros(kernelSize // 2))
    output = scipy.signal.medfilt(inp, kernelSize)
    output = output[:a.shape[0]]
    return output

def maximumFilter(a, kernelSize):
    inp = np.insert(a, 0, -np.full(kernelSize // 2, np.inf))
    output = scipy.ndimage.maximum_filter(inp, size=kernelSize)
    output = output[:a.shape[0]]
    return output

def minimumFilter(a, kernelSize):
    inp = np.insert(a, 0, np.full(kernelSize // 2, np.inf))
    output = scipy.ndimage.minimum_filter(inp, size=kernelSize)
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


@pytest.mark.parametrize("a", testCases)
def test_maximumFilter(a):
    a = a.astype(float)
    output1 = filt.movingfilter(a, kernelSize // 2, 'maximum')
    output1 = np.array(output1).astype(np.float16)
    output2 = maximumFilter(a, kernelSize).astype(np.float16)
    assert np.array_equal(output1, output2)

@pytest.mark.parametrize("a", testCases)
def test_minimumFilter(a):
    a = a.astype(float)
    output1 = filt.movingfilter(a, kernelSize // 2, 'minimum')
    output1 = np.array(output1).astype(np.float16)
    output2 = minimumFilter(a, kernelSize).astype(np.float16)
    assert np.array_equal(output1, output2)
