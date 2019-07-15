#ifndef SAMPLEBANK_TYPES_H
#define SAMPLEBANK_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "file_helpers.h"

typedef struct {
    binary_data_t *base_address;
    int16_t *data;
    size_t length;
    uint8_t note_root;
    uint8_t note_min;
    uint8_t note_max;
    bool loop_enabled;
    uint32_t loop_start;
    uint32_t loop_stop;
} sample_t;

#endif