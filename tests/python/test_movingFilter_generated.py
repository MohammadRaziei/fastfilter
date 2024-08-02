#!/bin/python3
from __future__ import annotations

import pytest
import numpy as np
import scipy.signal

import medianFilter as filt # Assuming this is the module where your filter function is defined

def movingAverageFilter(x, windowSize):
    return np.convolve(x, np.ones(windowSize) / windowSize, mode='same')


def medianFilter(x, windoSize):
    return scipy.signal.medfilt(x, windoSize)
