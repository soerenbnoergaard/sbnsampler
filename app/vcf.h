#ifndef VCF_H
#define VCF_H

#include <stdint.h>
#include <stdbool.h>

#include "vcf_types.h"
#include "voice_types.h"

int16_t vcf_filter(int16_t x, voice_t *v);

#endif