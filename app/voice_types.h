#ifndef VOICE_TYPES_H
#define VOICE_TYPES_H

#include "samplebank.h"
#include "polyfilter_types.h"
#include "vcf_types.h"

typedef struct {
    bool active;
    int32_t activation_index; // At what point the voice was activated.

    sample_t *sample;
    int32_t sample_idx; // Input sample index `n` of the poly-phase filter.
    int16_t sample_dl[PPF_NUM_TABS]; // Input sample delay line

    ppf_t *ppf;
    int32_t ppf_idx; // Output sample index `m` of the poly-phase filter.

    int16_t note; // Midi number corresponding to the active note (negative == inactive)
    uint8_t velocity; // Midi note velocity;

    vcf_t vcf;

    // TODO: Replace the two below with an ADSR
    bool sustained; // True when a note is being sustained.
    bool killed; // True when a note has been signaled off.
} voice_t;


#endif