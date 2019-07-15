#!/usr/bin/python3
from sbnsampler import simulator as sim
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile

sim.init()
sim.set_length(44100 * 5)
sim.run()
sim.close()

y = np.fromfile("output.bin", dtype="<h")
wavfile.write("output.wav", 44100, y)