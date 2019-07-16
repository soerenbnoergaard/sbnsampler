#!/usr/bin/python3
from sbnsampler import simulator as sim
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile

sim.init()
sim.add_midi(0, 0x90, 63, 127)
sim.set_length(44100 * 5)
sim.run()
sim.close()