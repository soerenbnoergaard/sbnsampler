#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dac.h"
#include "samplebank.h"

// Types ///////////////////////////////////////////////////////////////////////

// Defines, macros, and constants //////////////////////////////////////////////

#define SAMPLE_RATE_Hz 44100
#define BUFFER_SIZE 128

// Globals /////////////////////////////////////////////////////////////////////

// Functions ///////////////////////////////////////////////////////////////////

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

    for (i = 0; i < length - BUFFER_SIZE; i += BUFFER_SIZE) {
        // Fill buffer
        for (n = 0; n < BUFFER_SIZE; n++) {
            buffer[n] = x[i+n];
        }

        // Output to DAC
        err = dac_write(buffer, BUFFER_SIZE);
        if (err != 0) {
            fprintf(stderr, "Error during playback...\n");
            goto clean_exit;
        }
    }

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
