#ifndef VCF_H
#define VCF_H

#include <stdint.h>
#include <stdbool.h>

#include "vcf_types.h"
#include "voice_types.h"

int16_t vcf_filter(int16_t input_sample, voice_t *voice, uint8_t cutoff, uint8_t resonance);

#endif