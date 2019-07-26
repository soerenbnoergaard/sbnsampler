#ifndef POLYFILTER_COEFFS_H
#define POLYFILTER_COEFFS_H

#include <stdint.h>
#define PPF_NUM_TABS 4
#define PPF_ZERO_TRANSPOSE_OFFSET 36
#define PPF_TRANSPOSE_MIN -36
#define PPF_TRANSPOSE_MAX 12


typedef struct {
    int32_t *h;        // Polyphase filter coefficients (Q15)
    uint32_t h_length; // Length of coefficients array
    uint32_t L;        // Interpolation rate
    uint32_t M;        // Decimation rate
} ppf_t;

// Poly-phase filter coefficients for each interval
extern int32_t ppf_h0[32];
extern int32_t ppf_h1[212];
extern int32_t ppf_h2[228];
extern int32_t ppf_h3[188];
extern int32_t ppf_h4[76];
extern int32_t ppf_h5[24];
extern int32_t ppf_h6[68];
extern int32_t ppf_h7[64];
extern int32_t ppf_h8[20];
extern int32_t ppf_h9[76];
extern int32_t ppf_h10[36];
extern int32_t ppf_h11[220];
extern int32_t ppf_h12[16];
extern int32_t ppf_h13[136];
extern int32_t ppf_h14[228];
extern int32_t ppf_h15[148];
extern int32_t ppf_h16[216];
extern int32_t ppf_h17[12];
extern int32_t ppf_h18[68];
extern int32_t ppf_h19[32];
extern int32_t ppf_h20[252];
extern int32_t ppf_h21[200];
extern int32_t ppf_h22[224];
extern int32_t ppf_h23[212];
extern int32_t ppf_h24[8];
extern int32_t ppf_h25[68];
extern int32_t ppf_h26[228];
extern int32_t ppf_h27[148];
extern int32_t ppf_h28[108];
extern int32_t ppf_h29[12];
extern int32_t ppf_h30[232];
extern int32_t ppf_h31[16];
extern int32_t ppf_h32[252];
extern int32_t ppf_h33[176];
extern int32_t ppf_h34[220];
extern int32_t ppf_h35[212];
extern int32_t ppf_h36[4];
extern int32_t ppf_h37[200];
extern int32_t ppf_h38[196];
extern int32_t ppf_h39[148];
extern int32_t ppf_h40[200];
extern int32_t ppf_h41[12];
extern int32_t ppf_h42[116];
extern int32_t ppf_h43[8];
extern int32_t ppf_h44[252];
extern int32_t ppf_h45[88];
extern int32_t ppf_h46[220];
extern int32_t ppf_h47[248];
extern int32_t ppf_h48[4];


// Poly-phase filter structures for each interval
extern ppf_t ppf[49];

#endif
