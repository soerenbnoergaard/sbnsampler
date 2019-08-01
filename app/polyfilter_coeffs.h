#ifndef POLYFILTER_COEFFS_H
#define POLYFILTER_COEFFS_H

#include <stdint.h>
#define PPF_NUM_TABS 4
#define PPF_ZERO_TRANSPOSE_OFFSET 48
#define PPF_TRANSPOSE_MIN -48
#define PPF_TRANSPOSE_MAX 24


typedef struct {
    int32_t *h;        // Polyphase filter coefficients (Q15)
    uint32_t h_length; // Length of coefficients array
    uint32_t L;        // Interpolation rate
    uint32_t M;        // Decimation rate
} ppf_t;

// Poly-phase filter coefficients for each interval
extern int32_t ppf_h0[64];
extern int32_t ppf_h1[60];
extern int32_t ppf_h2[228];
extern int32_t ppf_h3[108];
extern int32_t ppf_h4[152];
extern int32_t ppf_h5[48];
extern int32_t ppf_h6[136];
extern int32_t ppf_h7[128];
extern int32_t ppf_h8[40];
extern int32_t ppf_h9[76];
extern int32_t ppf_h10[36];
extern int32_t ppf_h11[68];
extern int32_t ppf_h12[32];
extern int32_t ppf_h13[212];
extern int32_t ppf_h14[228];
extern int32_t ppf_h15[188];
extern int32_t ppf_h16[76];
extern int32_t ppf_h17[24];
extern int32_t ppf_h18[68];
extern int32_t ppf_h19[64];
extern int32_t ppf_h20[20];
extern int32_t ppf_h21[76];
extern int32_t ppf_h22[36];
extern int32_t ppf_h23[220];
extern int32_t ppf_h24[16];
extern int32_t ppf_h25[136];
extern int32_t ppf_h26[228];
extern int32_t ppf_h27[148];
extern int32_t ppf_h28[216];
extern int32_t ppf_h29[12];
extern int32_t ppf_h30[68];
extern int32_t ppf_h31[32];
extern int32_t ppf_h32[252];
extern int32_t ppf_h33[200];
extern int32_t ppf_h34[224];
extern int32_t ppf_h35[212];
extern int32_t ppf_h36[8];
extern int32_t ppf_h37[68];
extern int32_t ppf_h38[228];
extern int32_t ppf_h39[148];
extern int32_t ppf_h40[108];
extern int32_t ppf_h41[12];
extern int32_t ppf_h42[232];
extern int32_t ppf_h43[16];
extern int32_t ppf_h44[252];
extern int32_t ppf_h45[176];
extern int32_t ppf_h46[220];
extern int32_t ppf_h47[212];
extern int32_t ppf_h48[4];
extern int32_t ppf_h49[200];
extern int32_t ppf_h50[196];
extern int32_t ppf_h51[148];
extern int32_t ppf_h52[200];
extern int32_t ppf_h53[12];
extern int32_t ppf_h54[116];
extern int32_t ppf_h55[8];
extern int32_t ppf_h56[252];
extern int32_t ppf_h57[88];
extern int32_t ppf_h58[220];
extern int32_t ppf_h59[248];
extern int32_t ppf_h60[4];
extern int32_t ppf_h61[168];
extern int32_t ppf_h62[196];
extern int32_t ppf_h63[148];
extern int32_t ppf_h64[100];
extern int32_t ppf_h65[256];
extern int32_t ppf_h66[140];
extern int32_t ppf_h67[4];
extern int32_t ppf_h68[160];
extern int32_t ppf_h69[44];
extern int32_t ppf_h70[220];
extern int32_t ppf_h71[196];
extern int32_t ppf_h72[4];


// Poly-phase filter structures for each interval
extern ppf_t ppf[73];

#endif
