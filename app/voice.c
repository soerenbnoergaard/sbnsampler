#include "voice.h"

voice_t voices[NUM_VOICES];
voice_t voice_default = {
    .active = false,
    .activation_index = 0,
    .sample = &samplebank[0],
    .sample_idx = 0,
    .sample_dl[0] = 0,
    .sample_dl[1] = 0,
    .sample_dl[2] = 0,
    .sample_dl[3] = 0,
    .ppf = &ppf[0],
    .ppf_idx = 0,
    .note = -1,
    .velocity = 0,
    .vcf = {
        .g = 0.9,
        .k = 0.5,
        .w1 = {0, 0, 0, 0},
        .y1 = 0
    },
    .sustained = false,
    .killed = false,
};

int32_t voice_reset(voice_t *v)
{
    *v = voice_default;
    return 0;
}

int32_t voice_init(void)
{
    int32_t n;
    for (n = 0; n < NUM_VOICES; n++) {
        voice_reset(&voices[n]);
    }

    return 0;
}