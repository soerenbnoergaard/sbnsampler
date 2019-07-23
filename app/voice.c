#include "voice.h"
#include "vca.h"
#include "panel.h"

// Globals /////////////////////////////////////////////////////////////////////
static voice_t voices[NUM_VOICES];

// Private functions ///////////////////////////////////////////////////////////

// Public functions ////////////////////////////////////////////////////////////
status_t voice_init(void)
{
    int32_t n;
    voice_t *v;

    for (n = 0; n < NUM_VOICES; n++) {
        v = &voices[n];
        v->state = VOICE_STATE_IDLE;
    }

    return STATUS_OK;
}

status_t voice_close(void)
{
    return STATUS_OK;
}

voice_t *voice_get_handle(int32_t n)
{
    assert((0 <= n) && (n < NUM_VOICES));
    return &voices[n];
}

/* int16_t voice_get_sample(voice_t *v) */
/* { */
/*     int16_t x = 0; */
/*     status_t status = STATUS_OK; */
/*  */
/*     switch (v->state) { */
/*     case VOICE_STATE_IDLE: */
/*         break; */
/*  */
/*     case VOICE_STATE_STARTING: */
/*         v->note = v->midi.data[0]; */
/*         v->velocity = v->midi.data[1]; */
/*  */
/*         adsr_start(&v->env1); */
/*         vco_setup(&v->vco, v->note); */
/*         vcf_setup(&v->vcf); */
/*  */
/*         v->state = VOICE_STATE_RUNNING; */
/*  */
/*         break; */
/*  */
/*     case VOICE_STATE_RUNNING: */
/*         x = get_sample(v, &status); */
/*         break; */
/*  */
/*     case VOICE_STATE_STOPPED: */
/*         if (released(v)) { */
/*             v->state = VOICE_STATE_RELEASED; */
/*             break; */
/*         } */
/*  */
/*         x = get_sample(v, &status); */
/*         break; */
/*  */
/*     case VOICE_STATE_RESTARTING: */
/*         if (released(v)) { */
/*             v->state = VOICE_STATE_STARTING; */
/*             break; */
/*         } */
/*  */
/*         x = get_sample(v, &status); */
/*         break; */
/*  */
/*     case VOICE_STATE_RELEASED: */
/*         v->state = VOICE_STATE_IDLE; */
/*         break; */
/*  */
/*     default: */
/*         error("Unhandled voice state!"); */
/*         break; */
/*     } */
/*  */
/*     if (status != STATUS_OK) { */
/*         #<{(| here("status!=OK"); |)}># */
/*         v->state = VOICE_STATE_IDLE; */
/*         x = 0; */
/*     } */
/*     return x; */
/* } */
