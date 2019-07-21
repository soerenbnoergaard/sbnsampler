#include "voice.h"

voice_t voices[NUM_VOICES];

int32_t voice_init(void)
{
    return 0;
}

int32_t voice_reset(voice_t *v)
{
    int32_t n;

    // Reset indecies
    v->ppf_idx = 0;
    v->sample_idx = 0;

    // Reset delay lines
    for (n = 0; n < PPF_NUM_TABS; n++) {
        v->sample_dl[n] = 0;
    }

    for (n = 0; n < 5; n++) {
        v->vcf.w1[n] = 0;
    }

    return 0;
}