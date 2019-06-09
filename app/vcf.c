#include "vcf.h"

int16_t vcf_filter(int16_t input_sample, voice_t *voice)
{
    // Per-filter inputs and outputs
    float x;
    float y;

    // Short-hand names for VCF parameters
    float *y1 = &voice->vcf.y1;
    float *w1 = voice->vcf.w1;
    float k = voice->vcf.k;
    float g = voice->vcf.g;

    // Various variables
    float u;
    float v;
    float G;

    // if (voice->active == false) {
    //     return 0;
    // }

    // Ladder filter
    u = (float)input_sample - (*y1)*k;
    G = g/(1+g);

    // 1
    x = u;
    v = (x-w1[0]) * G;
    y = v + w1[0];
    w1[0] = y + v;

    // 2
    x = y;
    v = (x-w1[1]) * G;
    y = v + w1[1];
    w1[1] = y + v;

    // 2
    x = y;
    v = (x-w1[2]) * G;
    y = v + w1[2];
    w1[2] = y + v;

    // 3
    x = y;
    v = (x-w1[3]) * G;
    y = v + w1[3];
    w1[3] = y + v;

    // Update the output delay-line
    *y1 = y;

    // Return the output sample
    return (int16_t)y;
}