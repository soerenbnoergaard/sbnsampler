#include "voice.h"
#include "vca.h"

// Globals /////////////////////////////////////////////////////////////////////
static voice_t voices[NUM_VOICES];

// Private functions ///////////////////////////////////////////////////////////
int16_t datapath(voice_t *v, status_t *status)
{
    int16_t x = 0;
    uint8_t param;

    x = vco_get_sample(&v->vco, status);
    if (*status != STATUS_OK) {
        *status = STATUS_ERROR;
        return 0;
    }

    param = adsr_get(&v->env1);
    x = vca(x, param);

    *status = STATUS_OK;
    return x;
}

status_t control(voice_t *v)
{
    // TODO: Fetch settings from front panel
    if (adsr_setup(&v->env1, 10, 20, 63, 63) != STATUS_OK) {
        return STATUS_ERROR;
    }

    if (adsr_tick(&v->env1) != STATUS_OK) {
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

bool released(voice_t *v)
{
    if (!adsr_is_stopped(&v->env1)) {
        return false;
    }

    return true;
}

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

status_t voice_stop(voice_t *v)
{
    adsr_stop(&v->env1);
    v->state = VOICE_STATE_STOPPED;
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
        adsr_start(&v->env1);
        break;

    case VOICE_STATE_RUNNING:
        x = datapath(v, &status);
        if (status != STATUS_OK) {
            v->state = VOICE_STATE_IDLE;
            break;
        }

        status = control(v);
        if (status != STATUS_OK) {
            v->state = VOICE_STATE_IDLE;
            break;
        }

        break;

    case VOICE_STATE_STOPPED:
        if (released(v)) {
            v->state = VOICE_STATE_RELEASED;
        }

        x = datapath(v, &status);
        if (status != STATUS_OK) {
            v->state = VOICE_STATE_IDLE;
            break;
        }

        status = control(v);
        if (status != STATUS_OK) {
            v->state = VOICE_STATE_IDLE;
            break;
        }
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
