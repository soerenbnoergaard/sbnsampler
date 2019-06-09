#ifndef VOICE_H
#define VOICE_H

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define NUM_VOICES 8

#include "polyfilter_coeffs.h"
#include "samplebank.h"

typedef struct {
    bool active;

    sample_t *sample;
    int32_t sample_idx; // Input sample index `n` of the poly-phase filter.
    int16_t sample_dl[PPF_NUM_TABS]; // Input sample delay line

    ppf_t *ppf;
    int32_t ppf_idx; // Output sample index `m` of the poly-phase filter.

    int16_t note; // Midi number corresponding to the active note (negative == inactive)
    uint8_t velocity; // Midi note velocity;
    bool sustained; // True when a note is being sustained.
    bool killed; // True when a note has been signaled off.
} voice_t;

// Global variables (initialized elsewhere)
extern voice_t voices[NUM_VOICES];

int32_t voice_init(void);
int32_t voice_reset(voice_t *v);

#endif