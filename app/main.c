#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dac.h"
#include "samplebank.h"
#include "polyfilter.h"

// Types ///////////////////////////////////////////////////////////////////////

typedef struct {
    sample_t *x; // Input sample buffer
    uint32_t n; // Index into sample buffer
    ppf_t *ppf; // Poly-phase filter
} voice_t;

// Defines, macros, and constants //////////////////////////////////////////////

#define SAMPLE_RATE_Hz 44100
#define BUFFER_SIZE 128

// Globals /////////////////////////////////////////////////////////////////////

voice_t voices[] = {
    {
        .x = &samplebank[0],
        .n = 0,
        .ppf = &ppf1,
    },
    {
        .x = &samplebank[0],
        .n = 0,
        .ppf = &ppf12,
    },
};

// Functions ///////////////////////////////////////////////////////////////////

void resampler() 
{
    // Variable names after Lyons - Understanding Digital Signal Processing.

    // Output buffer
    int16_t buffer[BUFFER_SIZE];
    int32_t buffer_idx = 0;

    float y;
    int32_t m; // Output sample index, e.g. y[m]
    int32_t k; // Sub-filter coefficient selector
    int32_t l; // Delay-line index

    uint32_t N = PPF_NUM_TABS;

    // Expand active voice
    voice_t *v = &voices[1];

    for (m = 0; m < v->x->length; m++) {
        buffer_idx = (m % BUFFER_SIZE);

        //
        // Polyphase re-sampling filter
        //

        v->n = (m * v->ppf->M) / v->ppf->L;
        k    = (m * v->ppf->M) % v->ppf->L;

        y = 0;

        for (l = 0; l < N; l++) {
            // Skip unknown input sample values
            if (v->n - l < 0) {
                continue;
            }

            y += 
                v->ppf->L *                  // Scaling factor
                v->ppf->h[l*v->ppf->L + k] * // Filter coefficient
                (float)v->x->data[v->n - l]; // Input sample
        }

        //
        // Other tasks
        //


        //
        // Output buffer
        //

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

    resampler();

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
