#include "vcf.h"
#include "gpio.h"
#include <stdio.h>

static float map_midi_to_cutoff(uint8_t value)
{
    // Map cutoff to VCF parameter `g`.
    const float b = 0.02;
    const float a = (0.8-b) / (127*127);
    return a*value*value + b;
}

float map_midi_to_resonance(uint8_t value)
{
    // Map resonance to VCF parameter `k`.
    const float b = 0.5;
    const float a = (3.5-b) / 127;
    return a*value + b;
}

int16_t vcf_onepole(int16_t x, float g, int16_t x1, int16_t y1)
{
    int16_t v = (13*(x>>4)) + (3*(x1>>4));
    return (v-y1)*g + y1;
}

int16_t vcf_filter(int16_t input_sample, voice_t *voice, uint8_t cutoff, uint8_t resonance)
{
    // Per-filter inputs and outputs
    int16_t x;
    int16_t y;

    // Short-hand names for VCF parameters
    int32_t n;
    int16_t *w1 = voice->vcf.w1; // Delayed samples
    int16_t w[5];

    float g = map_midi_to_cutoff(cutoff);
    float k = map_midi_to_resonance(resonance);

    // Apply four one-pole filters
    x = input_sample >> 4;
    w[0] = x - ((int16_t)((float)w1[4]*k) - (x>>1)); // x>>1 = G_comp=0.5 from Valimaki
    w[1] = vcf_onepole(w[0], g, w1[0], w1[1]);
    w[2] = vcf_onepole(w[1], g, w1[1], w1[2]);
    w[3] = vcf_onepole(w[2], g, w1[2], w1[3]);
    w[4] = vcf_onepole(w[3], g, w1[3], w1[4]);

    // Update delay line
    for (n = 0; n < 5; n++) {
        w1[n] = w[n];
    }

    // Return the output sample
    y = w[4];
    return (int16_t)y;
}