#ifndef VOICE_H
#define VOICE_H

#include "utils.h"
#include "vco.h"

#define NUM_VOICES 4

typedef enum {
    VOICE_STATE_IDLE,
    VOICE_STATE_STARTING,
    VOICE_STATE_RESTARTING,
    VOICE_STATE_RUNNING,
    VOICE_STATE_STOPPED,
    VOICE_STATE_RELEASED
} voice_state_t;

// Voice type
//     A voice consists of an oscillator playing a specified note
typedef struct {
    voice_state_t state;
    sample_t *source;
    int32_t source_index;
    int32_t index;
    uint8_t note;
    uint8_t velocity;
} voice_t;

status_t voice_init(void);
status_t voice_close(void);
voice_t *voice_get_handle(int32_t n);
int16_t voice_get_sample(voice_t *v);

#endif
