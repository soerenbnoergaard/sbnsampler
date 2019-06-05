#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dac.h"
#include "samplebank.h"
#include "polyfilter.h"

// Types ///////////////////////////////////////////////////////////////////////

typedef struct {
    int16_t x[PPF_NUM_TABS]; // Input delay line
} voice_t;

// Defines, macros, and constants //////////////////////////////////////////////

#define SAMPLE_RATE_Hz 44100
#define BUFFER_SIZE 128

// Globals /////////////////////////////////////////////////////////////////////

// Functions ///////////////////////////////////////////////////////////////////

void resampler(int16_t *x, uint32_t length) 
{
    // Variable names after Lyons - Understanding Digital Signal Processing.

    float y;
    int16_t buffer[BUFFER_SIZE];
    int32_t buffer_idx = 0;

    int32_t m; // Output sample index, e.g. y[m]
    int32_t n; // Input sample index, e.g. x[n]
    int32_t k; // Sub-filter coefficient selector
    int32_t l; // Delay-line index

    uint32_t N = PPF_NUM_TABS;
    const ppf_t ppf = ppf4;

    for (m = 0; m < length; m++) {
        buffer_idx = (m % BUFFER_SIZE);

        n = (m*ppf.M) / ppf.L;
        k = (m*ppf.M) % ppf.L;

        if (n > length) {
            fprintf(stderr, "Not enough input samples\n");
            return;
        }

        y = 0;

        for (l = 0; l < N; l++) {
            // Skip unknown input samples
            if (n - l < 0) {
                continue;
            }

            // Poly-phase filter
            y += ppf.L * ppf.h[l*ppf.L + k] * (float)x[n - l];
        }

        buffer[buffer_idx] = (int16_t)y;

        if (buffer_idx == BUFFER_SIZE-1) {
            dac_write(buffer, BUFFER_SIZE);
        }
    }
}

// Interrupts //////////////////////////////////////////////////////////////////

// Main ////////////////////////////////////////////////////////////////////////

int32_t main(void)
{
    int32_t err;
    int32_t i;
    int32_t n;
    int16_t buffer[BUFFER_SIZE];

    int16_t *x;
    int32_t length;

    // Initialize sample bank
    err = samplebank_init();
    if (err != 0) {
        fprintf(stderr, "Error initializin Sample Bank\n");
        return 1;
    }

    // Initialize DAC
    err = dac_init("default", SAMPLE_RATE_Hz);
    if (err != 0) {
        fprintf(stderr, "Error initializin DAC\n");
        return 1;
    }

    // Play sound
    x = samplebank[0].data;
    length = samplebank[0].length;

    resampler(x, length);

clean_exit:
    // Close DAC
    err = dac_close();
    if (err != 0) {
        fprintf(stderr, "Error closing DAC\n");
        return 1;
    }

    // TODO: Consider freeing the sound bank here.

    return 0;
}
