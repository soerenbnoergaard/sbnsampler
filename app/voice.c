#include "voice.h"
#include "vca.h"
#include "panel.h"


// Globals /////////////////////////////////////////////////////////////////////
static voice_t voices[NUM_VOICES];

// Private functions ///////////////////////////////////////////////////////////

static uint8_t add_and_saturate(uint8_t x, uint8_t y)
{
    return (x+y>127) ? 127 : x+y;
}

static uint8_t multiply(uint8_t x, uint8_t y)
{
    uint16_t acc = (uint16_t)x * (uint16_t)y;
    return acc>>7;
}

static int16_t datapath(voice_t *v, status_t *status)
{
    int16_t x = 0;
    uint8_t param1;
    uint8_t param2;

    x = vco_get_sample(&v->vco, status);
    if (*status != STATUS_OK) {
        *status = STATUS_ERROR;
        return 0;
    }

    param1 = add_and_saturate(panel_get(PANEL_CUTOFF), multiply(panel_get(PANEL_CUTOFF_VELOCITY), v->velocity));
    param2 = 0;

    x = vcf_filter(x, v->vcf.w1, param1, param2);

    param1 = adsr_get(&v->env1);
    x = vca(x, param1);

    *status = STATUS_OK;
    return x;
}

static status_t control(voice_t *v)
{
    status_t status;
    status = adsr_setup(&v->env1,
                        panel_get(PANEL_ENV1_ATTACK),
                        panel_get(PANEL_ENV1_DECAY),
                        panel_get(PANEL_ENV1_SUSTAIN),
                        panel_get(PANEL_ENV1_RELEASE));

    if (status != STATUS_OK) {
        return STATUS_ERROR;
    }

    if (adsr_tick(&v->env1) != STATUS_OK) {
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

static status_t get_sample(voice_t *v, status_t *status)
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

static bool released(voice_t *v)
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
        v->note = v->midi.data[0];
        v->velocity = v->midi.data[1];

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
