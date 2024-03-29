#include "adsr.h"
#include "utils.h"
#include <stdio.h>

int32_t adsr_setup(adsr_t *adsr, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release)
{
    adsr->attack = attack;
    adsr->decay = decay;
    adsr->sustain = sustain;
    adsr->release = release;
    return 0;
}

int32_t adsr_start(adsr_t *adsr)
{
    adsr->state = ADSR_STATE_IDLE;
    return 0;
}

int32_t adsr_restart(adsr_t *adsr)
{
    adsr->state = ADSR_STATE_QUICK_RELEASE;
    return 0;
}

int32_t adsr_stop(adsr_t *adsr)
{
    adsr->state = ADSR_STATE_RELEASE;
    return 0;
}

int32_t adsr_update(adsr_t *adsr)
{
    switch (adsr->state) {
    case ADSR_STATE_IDLE:
        adsr->value = 0;
        adsr->step = 0;

        // Calculate attack slope
        adsr->slope = 8*adsr->attack + 1;
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
            adsr->slope = 8*adsr->decay + 1;
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
            adsr->slope = 8*adsr->release + 1;
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
            adsr->state = ADSR_STATE_IDLE;
        }

        break;

    case ADSR_STATE_STOPPED:
        break;
    }

    return 0;
}