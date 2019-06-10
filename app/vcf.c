#include "vcf.h"
#include <stdio.h>

static float map_midi_to_cutoff(uint8_t value)
{
    // Map cutoff to VCF parameter `g`.
    const float b = 0.01;
    const float a = (0.9-b) / (127*127*127);
    return a*value*value*value + b;
}

float map_midi_to_resonance(uint8_t value)
{
    // Map resonance to VCF parameter `k`.
    const float b = 0.5;
    const float a = (1.5-b) / 127;
    return a*value + b;
}

int16_t vcf_filter(int16_t input_sample, voice_t *voice, uint8_t cutoff, uint8_t resonance)
{
    // Per-filter inputs and outputs
    float x;
    float y;

    // Short-hand names for VCF parameters
    float *y1 = &voice->vcf.y1;
    float *w1 = voice->vcf.w1;
    float g = map_midi_to_cutoff(cutoff);
    float k = map_midi_to_resonance(resonance);

    // Various variables
    float u;
    float v;
    float G;

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