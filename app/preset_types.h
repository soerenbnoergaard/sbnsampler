#ifndef PRESET_TYPES_H
#define PRESET_TYPES_H

#define PRESET_MAX_NUM_SAMPLES 128

#include <stdbool.h>
#include <stdint.h>
#include "samplebank_types.h"

typedef struct {
    uint8_t volume;
    bool sustain;
    uint8_t cutoff;
    uint8_t resonance;
    uint8_t cutoff_velocity;
    uint8_t amp_attack;
    uint8_t amp_decay;
    uint8_t amp_sustain;
    uint8_t amp_release;
    uint8_t amp_velocity;
} settings_t;

typedef struct {
    sample_t *samples[PRESET_MAX_NUM_SAMPLES];
    int32_t num_samples;
    settings_t settings;
} preset_t;


#endif