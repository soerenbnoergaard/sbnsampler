#ifndef SAMPLEBANK_H
#define SAMPLEBANK_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "file_helpers.h"

// Types ///////////////////////////////////////////////////////////////////////

typedef struct {
    int16_t *data;
    size_t length;
    uint8_t note_root;
    uint8_t note_min;
    uint8_t note_max;
    bool loop_enabled;
    uint32_t loop_start;
    uint32_t loop_stop;
} sample_t;

// Defines, macros, and constants //////////////////////////////////////////////

#define NUM_SAMPLES 7

// Globals /////////////////////////////////////////////////////////////////////

extern sample_t samplebank[NUM_SAMPLES];

// Functions ///////////////////////////////////////////////////////////////////

int32_t samplebank_init();


#endif
