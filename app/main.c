#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dac.h"
#include "samplebank.h"
#include "polyfilter.h"

// Types ///////////////////////////////////////////////////////////////////////

// Defines, macros, and constants //////////////////////////////////////////////

#define SAMPLE_RATE_Hz 44100
#define BUFFER_SIZE 128

// Globals /////////////////////////////////////////////////////////////////////

// Functions ///////////////////////////////////////////////////////////////////

void resampler(int16_t *x, uint32_t length) 
{
    float y;
    int16_t buffer[BUFFER_SIZE];
    int32_t buffer_idx = 0;

    int32_t m;
    int32_t k;
    int32_t n;
    int32_t l;

    uint32_t N = SUBFILTER_LENGTH;

    const float *h = h4;
    uint32_t h_length = (sizeof(h4)/sizeof(float));
    uint32_t L = L4;
    uint32_t M = M4;

    for (m = 0; m < length; m++) {
        buffer_idx = (m % BUFFER_SIZE);

        n = (m*M) / L;
        k = (m*M) % L;

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
            y += L * h[l*L + k] * (float)x[n - l];
        }

        // Add the input sample to the output, to hear the interval
        y = (y/2) + (x[m]/2);

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

    /* for (i = 0; i < length - BUFFER_SIZE; i += BUFFER_SIZE) { */
    /*     // Fill buffer */
    /*     for (n = 0; n < BUFFER_SIZE; n++) { */
    /*         buffer[n] = x[i+n]; */
    /*     } */
    /*  */
    /*     // Output to DAC */
    /*     err = dac_write(buffer, BUFFER_SIZE); */
    /*     if (err != 0) { */
    /*         fprintf(stderr, "Error during playback...\n"); */
    /*         goto clean_exit; */
    /*     } */
    /* } */

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
