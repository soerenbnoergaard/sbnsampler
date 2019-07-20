#ifndef VOICE_H
#define VOICE_H

#include "utils.h"
#include "vco.h"

#define NUM_VOICES 4

typedef enum {
    VOICE_STATE_IDLE,
    VOICE_STATE_BUSY
} voice_state_t;

// Voice type
//     A voice consists of an oscillator playing a specified note
typedef struct {
    voice_state_t state;
    vco_t *vco;
    uint8_t note;
} voice_t;

status_t voice_init(void);
status_t voice_close(void);
voice_t *voice_get_handle(int32_t n);
int16_t voice_get_sample(voice_t *v);

#endif
