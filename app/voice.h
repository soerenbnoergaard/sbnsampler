#ifndef VOICE_H
#define VOICE_H

#include "utils.h"
#include "midi.h"
#include "vco.h"
#include "vcf.h"
#include "adsr.h"

#define NUM_VOICES 10

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
    midi_message_t midi;

    vco_t vco;
    vcf_t vcf;
    adsr_t env1;
    adsr_t env2;

    uint8_t note;
    uint8_t velocity;
} voice_t;

status_t voice_init(void);
status_t voice_close(void);
/* status_t voice_stop(voice_t *v); */
/* status_t voice_restart(voice_t *v); */
voice_t *voice_get_handle(int32_t n);
/* int16_t voice_get_sample(voice_t *v); */

#endif
