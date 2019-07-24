#include "adsr.h"
#include "utils.h"
#include <stdio.h>

// Globals /////////////////////////////////////////////////////////////////////

// Private functions ///////////////////////////////////////////////////////////
static int32_t slope(uint8_t param)
{
    return 16*param + 1;
}

// Public functions ////////////////////////////////////////////////////////////
status_t adsr_setup(adsr_t *adsr, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release)
{
    adsr->attack = attack;
    adsr->decay = decay;
    adsr->sustain = sustain;
    adsr->release = release;
    return STATUS_OK;
}

status_t adsr_start(adsr_t *adsr)
{
    adsr->state = ADSR_STATE_IDLE;
    return STATUS_OK;
}

status_t adsr_stop(adsr_t *adsr)
{
    switch (adsr->state) {
    case ADSR_STATE_STOPPED:
        break;

    default:
        adsr->slope = slope(adsr->release);
        adsr->step = 0;
        adsr->state = ADSR_STATE_RELEASE;
        break;
    }
    return STATUS_OK;
}

status_t adsr_stop_quick(adsr_t *adsr)
{
    switch (adsr->state) {
    case ADSR_STATE_STOPPED:
        break;

    default:
        adsr->step = 0;
        adsr->state = ADSR_STATE_QUICK_RELEASE;
        break;
    }
    return STATUS_OK;
}

status_t adsr_tick(adsr_t *adsr)
{
    switch (adsr->state) {
    case ADSR_STATE_IDLE:
        adsr->value = 0;
        adsr->step = 0;

        // Calculate attack slope
        adsr->slope = slope(adsr->attack);
        adsr->state = ADSR_STATE_ATTACK;
        break;

    case ADSR_STATE_ATTACK:
        if ((adsr->step % adsr->slope) == 0) {
            adsr->value += 1;
        }

        if (adsr->value < 127) {
            adsr->step += 1;
        }
        else {
            // Calculate decay slope
            adsr->slope = slope(adsr->decay);
            adsr->step = 0;
            adsr->state = ADSR_STATE_DECAY;
        }
        break;

    case ADSR_STATE_DECAY:
        if ((adsr->step % adsr->slope) == 0) {
            adsr->value -= 1;
        }

        if (adsr->value > adsr->sustain) {
            adsr->step += 1;
        }
        else if (adsr->value == 0) {
            adsr->state = ADSR_STATE_STOPPED;
        }
        else {
            // Calculate release slope
            adsr->slope = slope(adsr->release);
            adsr->step = 0;
            adsr->state = ADSR_STATE_SUSTAIN;
        }

        break;

    case ADSR_STATE_SUSTAIN:
        break;

    case ADSR_STATE_RELEASE:
        if ((adsr->step % adsr->slope) == 0) {
            adsr->value -= 1;
        }

        if (adsr->value > 0) {
            adsr->step += 1;
        }
        else {
            adsr->state = ADSR_STATE_STOPPED;
        }

        break;

    case ADSR_STATE_QUICK_RELEASE:
        if ((adsr->step % 10) == 0) {
            adsr->value -= 1;
        }

        if (adsr->value > 0) {
            adsr->step += 1;
        }
        else {
            adsr->state = ADSR_STATE_STOPPED;
        }

        break;

    case ADSR_STATE_STOPPED:
        break;
    }

    return STATUS_OK;
}

uint8_t adsr_get(adsr_t *adsr)
{
    return adsr->value;
}

bool adsr_is_stopped(adsr_t *adsr)
{
    return adsr->state == ADSR_STATE_STOPPED;
}
