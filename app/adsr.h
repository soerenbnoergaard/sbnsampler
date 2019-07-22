#ifndef ADSR_H
#define ADSR_H

#include "utils.h"

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

status_t adsr_setup(adsr_t *adsr, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release);
status_t adsr_start(adsr_t *adsr);
status_t adsr_stop(adsr_t *adsr);
status_t adsr_stop_quick(adsr_t *adsr);
status_t adsr_tick(adsr_t *adsr);
uint8_t adsr_get(adsr_t *adsr);
bool adsr_is_stopped(adsr_t *adsr);

#endif
