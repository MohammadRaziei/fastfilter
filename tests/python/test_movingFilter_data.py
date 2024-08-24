#!/bin/python3
from __future__ import annotations

import json
from pathlib import Path

import numpy as np
import pytest
import scipy

import fastfilter.filter as filt  # Assuming this is the module where your filter function is defined

# The path to the current working directory
CURRENT_WORK_DIR = Path(__file__).parent

def movingAverageFilter(a, kernelSize):
    out =  np.convolve(a, np.ones(kernelSize) / kernelSize, mode='full')
    return out[:a.shape[0]]

def medianFilter(a, kernelSize):
    input = np.insert(a, 0, np.zeros(2))
    output = scipy.signal.medfilt(input, kernelSize)
    return output[:a.shape[0]]


# Parse the JSON file and return a list of tuples (input, output, windowSize)
def parse_json(filename: Path):
    with filename.open() as file:
        data = json.load(file)

    return [(item["input"], item["output"], item["windowSize"], item["kernel"]) for item in data]

# Load test cases
test_data = parse_json(CURRENT_WORK_DIR.parent / "movingfilter-data.json")

# Use pytest's parametrize decorator to feed multiple sets of inputs into the test function
@pytest.mark.parametrize(("input_data", "expected_output", "windowSize", "kernel"), test_data)
def test_movfilt_function(input_data, expected_output, windowSize, kernel):
    output = filt.movingfilter(input_data, windowSize//2, kernel)
    assert output == expected_output
#
@pytest.mark.parametrize(("input_data", "expected_output", "windowSize", "kernel"), test_data)
def test_movfilt_function_inplace(input_data, expected_output, windowSize, kernel):
    input_save = input_data.copy()
    output = filt.movingfilter(input_data, windowSize//2, kernel)
    assert input_save != output or input_data == input_save

#
@pytest.mark.parametrize(("input_data", "expected_output", "windowSize", "kernel"), test_data)
def test_withPythonAvgFilter(input_data, expected_output, windowSize, kernel):
    input_data = np.array(input_data)
    expected_output = np.array(expected_output).astype(np.float16)

    if kernel == 'median':
        output2 = medianFilter(input_data, windowSize).astype(np.float16)
        assert np.array_equal(expected_output, output2)

    elif kernel == 'average':
        output2 = movingAverageFilter(input_data, windowSize).astype(np.float16)
        assert np.array_equal(expected_output, output2)
