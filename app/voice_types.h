#ifndef VOICE_TYPES_H
#define VOICE_TYPES_H

#include "samplebank.h"
#include "polyfilter_types.h"
#include "vcf_types.h"

typedef struct {
    bool sustain;
    uint8_t cutoff;
    uint8_t resonance;
} settings_t;

typedef enum {
    VOICE_STATE_IDLE = 0,
    VOICE_STATE_STARTING,
    VOICE_STATE_RUNNING,
    VOICE_STATE_STOPPED
} voice_state_t;

typedef struct {
    voice_state_t state;
    settings_t settings; // Local variant of global settings

    sample_t *sample;
    int32_t sample_idx; // Input sample index `n` of the poly-phase filter.
    int16_t sample_dl[PPF_NUM_TABS]; // Input sample delay line

    ppf_t *ppf;
    int32_t ppf_idx; // Output sample index `m` of the poly-phase filter.

    int16_t note; // Midi number corresponding to the active note (negative == inactive)
    uint8_t velocity; // Midi note velocity;

    vcf_t vcf;
} voice_t;

#endif