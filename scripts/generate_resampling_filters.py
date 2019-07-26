import os
import subprocess

from numpy import *
from matplotlib.pyplot import *
from scipy import signal
import scipy.io.wavfile


def generate_polyfilter_coefficients(L, M, N):
    # Prototype filter for combined anti-aliasing and reconstruction.
    h = signal.firwin(N*L, 0.8*min([1/L, 1/M]))
    h = array(h*2**15, dtype=int)
    return h

def relative_path(path):
    return os.path.sep.join([os.path.abspath(os.path.dirname(__file__)), path])

#
# Main
#

template_h = """\
#ifndef POLYFILTER_COEFFS_H
#define POLYFILTER_COEFFS_H

#include <stdint.h>
{defines:s}

typedef struct {{
    int32_t *h;        // Polyphase filter coefficients (Q15)
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

template_c = """\
#include "{h_file:s}"

{coefficients:s}

{structures:s}
"""

intervals_0 = [
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

intervals = [
    [8, 1],
    [53, 7],
    [57, 8],
    [47, 7],
    [19, 3],
    [6, 1],
    [17, 3],
    [16, 3],
    [5, 1],
    [19, 4],
    [9, 2],
    [55, 13],
    [4, 1],
    [34, 9],
    [57, 16],
    [37, 11],
    [54, 17],
    [3, 1],
    [17, 6],
    [8, 3],
    [63, 25],
    [50, 21],
    [56, 25],
    [53, 25],
    [2, 1],
    [17, 9],
    [57, 32],
    [37, 22],
    [27, 17],
    [3, 2],
    [58, 41],
    [4, 3],
    [63, 50],
    [44, 37],
    [55, 49],
    [53, 50],
    [1, 1],
    [50, 53],
    [49, 55],
    [37, 44],
    [50, 63],
    [3, 4],
    [29, 41],
    [2, 3],
    [63, 100],
    [22, 37],
    [55, 98],
    [62, 117],
    [1, 2],
]

zero_transpose_offset = intervals.index([1, 1])
transpose_min = - zero_transpose_offset
transpose_max = len(intervals) - 1 - zero_transpose_offset

N = 4
output_file_h = relative_path("../app/polyfilter_coeffs.h")
output_file_c = relative_path("../app/polyfilter_coeffs.c")

# Generate filter coefficients for each interval
coefficients = [0]*len(intervals)
for k, (L, M) in enumerate(intervals):
    h = generate_polyfilter_coefficients(L, M, N)
    coefficients[k] = h

# Generate c code for defines
s_defines = """\
#define PPF_NUM_TABS {:d}
#define PPF_ZERO_TRANSPOSE_OFFSET {:d}
#define PPF_TRANSPOSE_MIN {:d}
#define PPF_TRANSPOSE_MAX {:d}
""".format(N, zero_transpose_offset, transpose_min, transpose_max)

# Generate c code for coefficients
s_coefficients_h = ""
s_coefficients_c = ""
for k, h in enumerate(coefficients):
    s_coefficients_h += "extern int32_t ppf_h{n:d}[{length:d}];\n".format(n = k, length = len(h))
    s_coefficients_c += "int32_t ppf_h{n:d}[] = {{ {h:s} }};\n".format(
        n = k,
        h = ", ".join([str(X) for X in h]),
    )

# Generate c code for structures
s_structures_h = ""
s_structures_c = ""
s_structures_h += "extern ppf_t ppf[{length:d}];\n".format(length = len(intervals))
s_structures_c += "ppf_t ppf[{:d}] = {{\n".format(len(intervals))
for k in range(len(intervals)):
    s_structures_c += "    {{ .h = ppf_h{n:<5d}, .h_length = {h_length:5d}, .L = {L:5d}, .M = {M:5d} }}{end:s}\n".format(
        n = k,
        h_length = len(coefficients[k]),
        L = intervals[k][0],
        M = intervals[k][1],
        end = "," if k != len(intervals) - 1 else "",
    )
s_structures_c += "};"

# Write to file
with open(output_file_h, "w") as f:
    tmp = template_h.format(
        defines = s_defines,
        coefficients = s_coefficients_h,
        structures = s_structures_h,
    )
    f.write(tmp)

with open(output_file_c, "w") as f:
    tmp = template_c.format(
        h_file = os.path.basename(output_file_h),
        coefficients = s_coefficients_c,
        structures = s_structures_c,
    )
    f.write(tmp)

print("Generated poly-phase filter coefficients in {:s} and {:s}".format(output_file_h, output_file_c))
