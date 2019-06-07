import os
import subprocess

from numpy import *
from matplotlib.pyplot import *
from scipy import signal
import scipy.io.wavfile


def generate_polyfilter_coefficients(L, M, N):
    # Prototype filter for combined anti-aliasing and reconstruction.
    h = signal.firwin(N*L, 0.8*min([1/L, 1/M]))
    return h

def relative_path(path):
    return os.path.sep.join([os.path.abspath(os.path.dirname(__file__)), path])

#
# Main
#

template = """\
#ifndef POLYFILTER_H
#define POLYFILTER_H

{defines:s}

typedef struct {{
    float *h;          // Polyphase filter coefficients
    uint32_t h_length; // Length of coefficients array
    uint32_t L;        // Interpolation rate
    uint32_t M;        // Decimation rate
}} ppf_t;

// Poly-phase filter coefficients for each interval
{coefficients:s}

// Poly-phase filter structures for each interval
{structures:s}
#endif
"""

intervals = [
    # Transpose down
    [2, 1],
    [15, 8],
    [16, 9],
    [5, 3],
    [8, 5],
    [3, 2],
    [45, 32],
    [4, 3],
    [5, 4],
    [6, 5],
    [9, 8],
    [16, 15],
    [1, 1], # Zero-transpose offset
    [15, 16],
    [8, 9],
    [5, 6],
    [4, 5],
    [3, 4],
    [32, 45],
    [2, 3],
    [5, 8],
    [3, 5],
    [9, 16],
    [8, 15],
    [1, 2],
]

zero_transpose_offset = intervals.index([1, 1])

N = 4
output_file = relative_path("../app/polyfilter.h")

# Generate filter coefficients for each interval
coefficients = [0]*len(intervals)
for k, (L, M) in enumerate(intervals):
    h = generate_polyfilter_coefficients(L, M, N)
    coefficients[k] = h

# Generate c code for defines
s_defines = """\
#define PPF_NUM_TABS {:d}
#define PPF_ZERO_TRANSPOSE_OFFSET {:d}
""".format(N, zero_transpose_offset)

# Generate c code for coefficients
s_coefficients = ""
for k, h in enumerate(coefficients):
    s_coefficients += "float ppf_h{n:d}[] = {{ {h:s} }};\n".format(
        n = k,
        h = ", ".join([str(X) for X in h]),
    )

# Generate c code for structures
s_structures = ""
s_structures += "ppf_t ppf[{:d}] = {{\n".format(len(intervals))
for k in range(len(intervals)):
    s_structures += "    {{ .h = ppf_h{n:<5d}, .h_length = {h_length:5d}, .L = {L:5d}, .M = {M:5d} }}{end:s}\n".format(
        n = k,
        h_length = len(coefficients[k]),
        L = intervals[k][0],
        M = intervals[k][1],
        end = "," if k != len(intervals) - 1 else "",
    )
s_structures += "};"

# Write to file
with open(output_file, "w") as f:
    tmp = template.format(
        defines = s_defines,
        coefficients = s_coefficients,
        structures = s_structures,
    )

    f.write(tmp)
print("Generated poly-phase filter coefficients in {:s}".format(output_file))