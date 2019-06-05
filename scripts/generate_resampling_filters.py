import subprocess

from numpy import *
from matplotlib.pyplot import *
from scipy import signal
import scipy.io.wavfile

intervals = [
    [1, 1],
    [16, 15],
    [9, 8],
    [6, 5],
    [5, 4],
    [4, 3],
    [45, 32],
    [3, 2],
    [8, 5],
    [5, 3],
    [16, 9],
    [15, 8],
    [2, 1],
]

def generate_polyfilter_coefficients(L, M, N):
    # Prototype filter for combined anti-aliasing and reconstruction.
    h = signal.firwin(N*L, 0.8*min([1/L, 1/M]))
    return h

def resampler(x, L, M, N):
    """
    Parameters:
    x = Input signal
    L = Interpolation rate
    M = Decimation rate
    N = Sub-filter length
    
    Returns:
    y = Resampled version of x
    """
    
    # Prototype filter for combined anti-aliasing and reconstruction.
    h = signal.firwin(N*L, 0.8*min([1/L, 1/M]))

    y = zeros((L*len(x))//M)
    for m in range(0, len(y)):
        n = (m*M) // L
        k = (m*M) % L

        for l in range(N):
            # Skip unknown input samples
            if n-l < 0:
                continue

            y[m] += h[l*L + k] * x[n - l]

    # Compensate for change in amplitude
    y *= L

    return y

if __name__ == "__main__":
    N = 4
    output_file = "../app/polyfilter.h"
    with open(output_file, "w") as f:
        f.write("#ifndef POLYFILTER_H\n")
        f.write("#define POLYFILTER_H\n")
        f.write("#define PPF_NUM_TABS {:d}\n".format(N))
        f.write("""
typedef struct {
    const float *h; // Polyphase filter coefficients
    uint32_t h_length; // Length of coefficients array
    uint32_t L; // Interpolation rate
    uint32_t M; // Decimation rate
} ppf_t;
""")
        for n, (L, M) in enumerate(intervals):
            h = generate_polyfilter_coefficients(L, M, N)

            f.write("""
const float ppf_h{n:d}[] = {{ {h:s} }};
const ppf_t ppf{n:d} = {{
    .h = ppf_h{n:d},
    .h_length = {h_length:d},
    .L = {L:d},
    .M = {M:d}
}};
""".format(
    n = n,
    h = ", ".join([str(X) for X in h]),
    h_length = len(h),
    L = L,
    M = M,
))

            # f.write("const float h{:d}[] = {:s}\n".format(
            #     n,
            #     "{" + ", ".join([str(X) for X in h]) + "};"
            # ))
            # f.write("uint32_t L{:d} = {:d};\n".format(n, L))
            # f.write("uint32_t M{:d} = {:d};\n".format(n, M))
        f.write("#endif\n")

