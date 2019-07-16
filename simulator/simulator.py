#!/usr/bin/python3
from sbnsampler import simulator as sim
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile

sim.init()

sim.enqueue(0, 0, 0x90, 48, 127)
sim.enqueue(1, 22050, 0x80, 48, 0)

sim.enqueue(2, 44100, 0x90, 36, 127)
sim.enqueue(3, 66150, 0x80, 36, 0)

sim.set_length(44100 * 5)
sim.run()
sim.close()