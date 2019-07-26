#ifndef VCO_H
#define VCO_H

#include "utils.h"
#include "polyfilter_coeffs.h"

#define NUM_SAMPLES_PER_COLLECTION 128
#define NUM_COLLECTIONS 128

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

// Sample collection type.
//     A sample collection consists of a number of samples spanning the keyboard.
typedef struct {
    sample_t samples[NUM_SAMPLES_PER_COLLECTION];
    int32_t num_samples;
} sample_collection_t;

// VCO type
//    A VCO contains everything necessary for a voice to contain a sound source. 
typedef struct {
    // Sound sample
    sample_t *sample; // x
    uint32_t sample_index; // n
    int16_t sample_delay_line[PPF_NUM_TABS];

    // Note and transposition info
    uint8_t note;
    int32_t index; // m
    int32_t *coeffs; // h
    int32_t interpolation_rate; // L
    int32_t decimation_rate; // M
} vco_t;

status_t vco_init(void);
status_t vco_close(void);
status_t vco_setup(vco_t *vco, uint8_t collection_id, uint8_t note);
int16_t vco_get_sample(vco_t *vco, status_t *status, bool active);

#endif
