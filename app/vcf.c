#include "vcf.h"
#include "gpio.h"
#include <stdio.h>

#define VCF_ENABLE_RESONANCE 0
#define VCF_ENABLE_FOUR_POLES 1

// Globals /////////////////////////////////////////////////////////////////////

// Private functions ///////////////////////////////////////////////////////////
static float map_midi_to_cutoff(uint8_t value)
{
    // Map cutoff to VCF parameter `g`.
    const float b = 0.02;
    const float a = (0.8-b) / (127*127);
    return a*value*value + b;
}

#if VCF_ENABLE_RESONANCE
static float map_midi_to_resonance(uint8_t value)
{
    // Map resonance to VCF parameter `k`.
    const float b = 0.5;
    const float a = (3.5-b) / 127;
    return a*value + b;
}
#endif

static int32_t vcf_onepole(int32_t x, float g, int32_t x1, int32_t y1)
{
    int32_t v = (int32_t)(0.8*(float)x + 0.2*(float)x1);
    return (v-y1)*g + y1;
}

// Public functions ////////////////////////////////////////////////////////////
status_t vcf_setup(vcf_t *vcf)
{
    int32_t n;
    for (n = 0; n < 5; n++) {
        vcf->w1[0] = 0;
    }
    return STATUS_OK;
}

int16_t vcf_filter(int16_t input_sample, int32_t *delay_line, uint8_t cutoff, uint8_t resonance)
{
    // Per-filter inputs and outputs
    int32_t x;
    int32_t y;

    // Short-hand names for VCF parameters
    int32_t n;
    int32_t w[5]; // Current samples
    int32_t *w1 = delay_line; // Delayed samples

    float g = map_midi_to_cutoff(cutoff);

#if VCF_ENABLE_RESONANCE
    float k = map_midi_to_resonance(resonance);
#endif

    // Apply four one-pole filters
    x = input_sample;

#if VCF_ENABLE_RESONANCE
    w[0] = x - ((int32_t)((float)w1[4]*k) - (x>>1)); // x>>1 = G_comp=0.5 from Valimaki
#else
    w[0] = x;
#endif

    w[1] = vcf_onepole(w[0], g, w1[0], w1[1]);
    w[2] = vcf_onepole(w[1], g, w1[1], w1[2]);

#if VCF_ENABLE_FOUR_POLES
    w[3] = vcf_onepole(w[2], g, w1[2], w1[3]);
    w[4] = vcf_onepole(w[3], g, w1[3], w1[4]);
    y = w[4];
#else
    y = w[2];
#endif

    // Update delay line
    for (n = 0; n < 5; n++) {
        w1[n] = w[n];
    }

    // Return the output sample
#if VCF_ENABLE_RESONANCE
    y = y >> 4;
#else
    y = y;
#endif
    return (int16_t)y;
}
