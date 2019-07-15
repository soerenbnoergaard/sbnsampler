#!/usr/bin/python3
from sbnsampler import simulator as sim

sim.init()
sim.set_length(44100 * 5)
sim.run()
sim.close()
