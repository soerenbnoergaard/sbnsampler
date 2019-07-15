import os
from ctypes import *

_so = CDLL(
    os.path.abspath(
        os.path.join(
            os.path.dirname(__file__),
            "../simulator.so",
        )
    )
)

init = _so.sim_init
init.argtypes = []
init.restype = c_int32

set_length = _so.sim_set_length
set_length.argtypes = [c_int32]
set_length.restype = c_int32

close = _so.sim_close
close.argtypes = []
close.restype = c_int32

run = _so.sim_run
run.argtypes = []
run.restype = c_int32