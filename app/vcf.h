#ifndef VCF_H
#define VCF_H

#include "utils.h"

// Zavalishin 2015 The Art of VA Filter Design
// Ladder filter, page 63 and 48

typedef struct {
    int32_t w1[5]; // Delayed samples
} vcf_t;

status_t vcf_setup(vcf_t *vcf);
int16_t vcf_filter(int16_t input_sample, int32_t *delay_line, uint8_t cutoff, uint8_t resonance);

#endif
