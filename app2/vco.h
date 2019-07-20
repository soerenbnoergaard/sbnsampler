#ifndef VCO_H
#define VCO_H

#include "utils.h"

#define NUM_SAMPLES_PER_VCO 128
#define NUM_OSCILLATORS 1

// Sample type.
//     A sample consists of audio data valid for a range of midi notes. It may be
//     looped between two sample points.
typedef struct {
    int16_t *data;
    int32_t length;
    bool loop_enabled;
    int8_t note_root;
    int8_t note_min;
    int8_t note_max;
    int32_t loop_start;
    int32_t loop_stop;
} sample_t;

// Oscillator type.
//     An oscillator consists of a number of samples spanning the keyboard.
typedef struct {
    sample_t samples[NUM_SAMPLES_PER_VCO];
    int32_t num_samples;
} vco_t;

status_t vco_init(void);
status_t vco_close(void);
vco_t *vco_get_handle(int32_t n);
int16_t vco_get_sample(sample_t *s, int32_t index, status_t *status);

#endif
