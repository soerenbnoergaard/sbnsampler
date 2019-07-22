#include "voice.h"
#include "vca.h"

// Globals /////////////////////////////////////////////////////////////////////
static voice_t voices[NUM_VOICES];

// Private functions ///////////////////////////////////////////////////////////
int16_t datapath(voice_t *v, status_t *status)
{
    // TODO: Move parameters to panel

    int16_t x = 0;
    uint8_t param1;
    uint8_t param2;

    x = vco_get_sample(&v->vco, status);
    if (*status != STATUS_OK) {
        *status = STATUS_ERROR;
        return 0;
    }

    param1 = v->velocity;
    param2 = 0;
    x = vcf_filter(x, v->vcf.w1, param1, param2);

    param1 = adsr_get(&v->env1);
    x = vca(x, param1);

    *status = STATUS_OK;
    return x;
}

status_t control(voice_t *v)
{
    // TODO: Fetch settings from front panel
    if (adsr_setup(&v->env1, 0, 0, 127, 63) != STATUS_OK) {
        return STATUS_ERROR;
    }

    if (adsr_tick(&v->env1) != STATUS_OK) {
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

status_t get_sample(voice_t *v, status_t *status)
{
    int16_t x;

    x = datapath(v, status);
    if (*status != STATUS_OK) {
        return 0;
    }

    *status = control(v);
    if (*status != STATUS_OK) {
        return 0;
    }

    return x;
}

bool released(voice_t *v)
{
    if (!adsr_is_stopped(&v->env1)) {
        return false;
    }

    return true;
}

status_t quick_release(voice_t *v)
{
    adsr_stop_quick(&v->env1);
    return STATUS_OK;
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

status_t voice_restart(voice_t *v)
{
    adsr_stop_quick(&v->env1);
    v->state = VOICE_STATE_RESTARTING;
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
    status_t status = STATUS_OK;

    switch (v->state) {
    case VOICE_STATE_IDLE:
        break;

    case VOICE_STATE_STARTING:
        adsr_start(&v->env1);
        vco_setup(&v->vco, v->note);
        vcf_setup(&v->vcf);

        v->state = VOICE_STATE_RUNNING;

        break;

    case VOICE_STATE_RUNNING:
        x = get_sample(v, &status);
        break;

    case VOICE_STATE_STOPPED:
        if (released(v)) {
            v->state = VOICE_STATE_RELEASED;
            break;
        }

        x = get_sample(v, &status);
        break;

    case VOICE_STATE_RESTARTING:
        if (released(v)) {
            v->state = VOICE_STATE_STARTING;
            break;
        }

        x = get_sample(v, &status);
        break;

    case VOICE_STATE_RELEASED:
        v->state = VOICE_STATE_IDLE;
        break;

    default:
        error("Unhandled voice state!");
        break;
    }

    if (status != STATUS_OK) {
        /* here("status!=OK"); */
        v->state = VOICE_STATE_IDLE;
        x = 0;
    }
    return x;
}
