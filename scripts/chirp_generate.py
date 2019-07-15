#!/usr/bin/python3
import os
import sys
import struct
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal
import scipy.io.wavfile

SAMPLE_RATE_Hz = 44100

def relative_path(path):
    return os.path.sep.join([os.path.abspath(os.path.dirname(__file__)), path])

def get_chirp(fs=SAMPLE_RATE_Hz, fstart=20, fstop=20e3, amplitude=2**14-1, duration_s=20, method="linear"):
    t = np.arange(duration_s*fs)/fs
    x = amplitude * scipy.signal.chirp(t, f0=fstart, f1=fstop, t1=max(t), method=method)
    return np.array(x, dtype=np.int16)

data = get_chirp(method="linear")
scipy.io.wavfile.write(relative_path("../app/sound/chirp_lin.wav"), SAMPLE_RATE_Hz, data)

data = get_chirp(method="logarithmic")
scipy.io.wavfile.write(relative_path("../app/sound/chirp_log.wav"), SAMPLE_RATE_Hz, data)
