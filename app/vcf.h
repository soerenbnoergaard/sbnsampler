#ifndef VCF_H
#define VCF_H

#include <stdint.h>
#include <stdbool.h>

#include "vcf_types.h"
#include "voice.h"

int16_t vcf_filter(voice_t *v);

#endif