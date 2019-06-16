#ifndef ADSR_TYPES_H
#define ADSR_TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    ADSR_STATE_IDLE = 0,
    ADSR_STATE_STOPPED,
    ADSR_STATE_ATTACK,
    ADSR_STATE_DECAY,
    ADSR_STATE_SUSTAIN,
    ADSR_STATE_RELEASE,
    ADSR_STATE_QUICK_RELEASE
} adsr_state_t;

typedef struct {
    adsr_state_t state;
    uint8_t attack;
    uint8_t decay;
    uint8_t sustain;
    uint8_t release;
    int32_t step;
    int32_t slope; // Num. time steps it takes to increment/decrement the value
    int16_t value;
} adsr_t;

#endif