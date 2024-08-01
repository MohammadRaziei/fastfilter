#!/bin/python3
from __future__ import annotations

import pytest
from arrayProduct import arrayProduct

@pytest.fixture
def input_array():
    return [1.0, 2.0, 3.0, 4.0]


def test_array_product_multiplier_is_2(input_array):
    multiplier = 2.0
    expected = [2.0, 4.0, 6.0, 8.0]

    output_array = arrayProduct(multiplier, input_array)

    assert output_array == expected

def test_array_product_multiplier_is_0(input_array):
    multiplier = 0.0
    expected = [0.0, 0.0, 0.0, 0.0]

    output_array = arrayProduct(multiplier, input_array)

    assert output_array == expected

def test_array_product_multiplier_is_negative(input_array):
    multiplier = -1.5
    expected = [-1.5, -3.0, -4.5, -6.0]

    output_array = arrayProduct(multiplier, input_array)

    assert output_array == expected
