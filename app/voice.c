#include "voice.h"
#include "vco.h"

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

int16_t voice_get_sample(voice_t *v)
{
    int16_t x = 0;
    status_t status;

    switch (v->state) {
    case VOICE_STATE_IDLE:
        break;

    case VOICE_STATE_STARTING:
    case VOICE_STATE_RESTARTING:
        v->state = VOICE_STATE_RUNNING;
        break;

    case VOICE_STATE_RUNNING:
        x = vco_get_sample(&v->vco, &status);
        if (status == STATUS_OK) {
        }
        else {
            v->state = VOICE_STATE_IDLE;
        }
        break;

    case VOICE_STATE_STOPPED:
        v->state = VOICE_STATE_RELEASED;
        break;

    case VOICE_STATE_RELEASED:
        v->state = VOICE_STATE_IDLE;
        break;

    default:
        error("Unhandled voice state!");
        break;
    }

    return x;
}
