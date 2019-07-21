#ifndef POLYFILTER_COEFFS_H
#define POLYFILTER_COEFFS_H

#include <stdint.h>
#define PPF_NUM_TABS 4
#define PPF_ZERO_TRANSPOSE_OFFSET 36
#define PPF_TRANSPOSE_MIN -36
#define PPF_TRANSPOSE_MAX 12


typedef struct {
    float *h;          // Polyphase filter coefficients
    uint32_t h_length; // Length of coefficients array
    uint32_t L;        // Interpolation rate
    uint32_t M;        // Decimation rate
} ppf_t;

// Poly-phase filter coefficients for each interval
extern float ppf_h0[32];
extern float ppf_h1[212];
extern float ppf_h2[228];
extern float ppf_h3[188];
extern float ppf_h4[76];
extern float ppf_h5[24];
extern float ppf_h6[68];
extern float ppf_h7[64];
extern float ppf_h8[20];
extern float ppf_h9[76];
extern float ppf_h10[36];
extern float ppf_h11[220];
extern float ppf_h12[16];
extern float ppf_h13[136];
extern float ppf_h14[228];
extern float ppf_h15[148];
extern float ppf_h16[216];
extern float ppf_h17[12];
extern float ppf_h18[68];
extern float ppf_h19[32];
extern float ppf_h20[252];
extern float ppf_h21[200];
extern float ppf_h22[224];
extern float ppf_h23[212];
extern float ppf_h24[8];
extern float ppf_h25[68];
extern float ppf_h26[228];
extern float ppf_h27[148];
extern float ppf_h28[108];
extern float ppf_h29[12];
extern float ppf_h30[232];
extern float ppf_h31[16];
extern float ppf_h32[252];
extern float ppf_h33[176];
extern float ppf_h34[220];
extern float ppf_h35[212];
extern float ppf_h36[4];
extern float ppf_h37[200];
extern float ppf_h38[196];
extern float ppf_h39[148];
extern float ppf_h40[200];
extern float ppf_h41[12];
extern float ppf_h42[116];
extern float ppf_h43[8];
extern float ppf_h44[252];
extern float ppf_h45[88];
extern float ppf_h46[220];
extern float ppf_h47[248];
extern float ppf_h48[4];


// Poly-phase filter structures for each interval
extern ppf_t ppf[49];

#endif
