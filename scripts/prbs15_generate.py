#!/usr/bin/python3
import os
import sys
import struct
import numpy as np
import matplotlib.pyplot as plt
import scipy.io.wavfile

def relative_path(path):
    return os.path.sep.join([os.path.abspath(os.path.dirname(__file__)), path])

class PRBS(object):
    def prbs_generic(self, num_bits, seed, field_width, newbit_function):
        mask = int("1" * field_width, 2)
        a = seed
        bits = []
        for _ in range(num_bits):
            newbit = newbit_function(a)
            a = ((a << 1) | newbit) & mask
            bits.append(newbit * 2 - 1) # NRZ sequence
        return bits

    def prbs15(self, num_bits, seed=0x01):
        return self.prbs_generic(num_bits, seed, 15, lambda x: (((x >> 14) ^ (x >> 13)) & 1))

    def prbs7(self, num_bits, seed=0x01):
        return self.prbs_generic(num_bits, seed, 7, lambda x: (((x >> 6) ^ (x >> 5)) & 1))

    def prbs9(self, num_bits, seed=0x01):
        return self.prbs_generic(num_bits, seed, 9, lambda x: (((x >> 8) ^ (x >> 4)) & 1))

    def prbs15(self, num_bits, seed=0x01):
        return self.prbs_generic(num_bits, seed, 15, lambda x: (((x >> 14) ^ (x >> 13)) & 1))

    def prbs20(self, num_bits, seed=0x01):
        return self.prbs_generic(num_bits, seed, 20, lambda x: (((x >> 19) ^ (x >> 2)) & 1))

    def prbs23(self, num_bits, seed=0x01):
        return self.prbs_generic(num_bits, seed, 23, lambda x: (((x >> 22) ^ (x >> 17)) & 1))


output_file = relative_path("../app/sound/prbs15.wav")
sample_rate = 44100
N = 2**15-1

noise = PRBS()
data = (2**15-1) * np.array(noise.prbs15(N), dtype="<h")

scipy.io.wavfile.write(output_file, sample_rate, data)
