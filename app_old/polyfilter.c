#include "polyfilter.h"
#include "voice.h"
#include <stdio.h>

// static int16_t get_squarewave_sample(void)
// {
//     static int32_t n = 0;
//     int16_t x;

//     x = (n < 50) * 32767;
//     n = (n + 1) % 100;

//     return x;
// }

int16_t ppf_get_transposed_sample(voice_t *v)
{
    // Polyphase re-sampling filter
    // Variable names after Lyons - Understanding Digital Signal Processing.

    float y;
    int32_t k; // Sub-filter coefficient selector
    int32_t l; // Delay-line index
    uint32_t N = PPF_NUM_TABS;

    // Short-hand names for voice variables
    int16_t *x = v->sample_dl;
    int32_t n = v->sample_idx;
    int32_t m = v->ppf_idx;
    int32_t M = v->ppf->M;
    int32_t L = v->ppf->L;
    float *h = v->ppf->h;

    // Compute input buffer index
    n = (m * M) / L;

    // Update delay line if `n` has changed.
    // Index into delay line == number of delays
    if (n != v->sample_idx) {
        for (l = N-1; l >= 1; l--) {
            x[l] = x[l-1];
        }

        if (v->sample->loop_enabled) {
            // LOOPED

            if (n > v->sample->loop_stop) {
                m -= ((v->sample->loop_stop - v->sample->loop_start) * L) / M;
            }
        }
        else {
            // NOT LOOPED

            if (n >= v->sample->length - 1) {
                // No more input-samples
                v->state = VOICE_STATE_IDLE;
                return 0;
            }
        }

        x[0] = v->sample->data[n];
    }

    // Compute polyphase sub-filter selector
    k = (m * M) % L;

    // Execute filter difference equation
    y = 0;
    for (l = 0; l < N; l++) {
        y += (L * h[L*l + k] * (float)x[l]);
    }

    // Update voice names from internal variables
    // Increase output buffer index, `m`
    v->sample_idx = n;
    v->ppf_idx = m + 1;

    // Return resulting sample
    return (int16_t)y;
}