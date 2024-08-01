#!/bin/python3
from __future__ import annotations

import json
import pytest
from pathlib import Path
import medianFilter as filt # Assuming this is the module where your filter function is defined

# The path to the current working directory
CURRENT_WORK_DIR = Path(__file__).parent

# Parse the JSON file and return a list of tuples (input, output, windowSize)
def parse_json(filename):
    with open(filename, 'r') as file:
        data = json.load(file)

    return [(item["input"], item["output"], item["windowSize"], item["kernel"]) for item in data]

# Load test cases
test_data = parse_json(CURRENT_WORK_DIR.parent / "data.json")

# Use pytest's parametrize decorator to feed multiple sets of inputs into the test function
@pytest.mark.parametrize("input_data, expected_output, windowSize, kernel", test_data)
def test_medfilt_function(input_data, expected_output, windowSize, kernel):
    output = filt.movingfilter(input_data, windowSize//2, kernel)
    assert output == expected_output

#