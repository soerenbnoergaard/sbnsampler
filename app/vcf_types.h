#ifndef VCF_TYPES_H
#define VCF_TYPES_H

// Zavalishin 2015 The Art of VA Filter Design
// Ladder filter, page 63 and 48

typedef struct {
    float g; // Proportional to cutoff frequency
    float k; // Proportional to resonance
    float w1[4]; // Delayed sum of y[n] and x[n]*G
    float y1; // Delayed output
} vcf_t;

#endif