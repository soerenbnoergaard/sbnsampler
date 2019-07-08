#ifndef VCF_TYPES_H
#define VCF_TYPES_H

// Zavalishin 2015 The Art of VA Filter Design
// Ladder filter, page 63 and 48

typedef struct {
    int32_t w1[5]; // Delayed samples
} vcf_t;

#endif