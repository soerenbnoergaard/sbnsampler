#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "dac.h"
#include "midi.h"
#include "samplebank.h"
#include "polyfilter.h"

// Types ///////////////////////////////////////////////////////////////////////

typedef struct {
    bool active;

    sample_t *sample;
    uint32_t sample_idx; // Input sample index `n` of the poly-phase filter.

    ppf_t *ppf;
    uint32_t pff_idx; // Output sample index `m` of the poly-phase filter.
} voice_t;

// Defines, macros, and constants //////////////////////////////////////////////

#define SAMPLE_RATE_Hz 44100
#define BUFFER_SIZE 128
#define NUM_VOICES 4

// Globals /////////////////////////////////////////////////////////////////////

voice_t voices[NUM_VOICES] = {
    {
        .active = false,
        .sample = &samplebank[0],
        .sample_idx = 0,
        .pff_idx = 0,
        .ppf = &ppf[0],
    },
    {
        .active = false,
        .sample = &samplebank[0],
        .sample_idx = 0,
        .pff_idx = 0,
        .ppf = &ppf[5],
    },
    {
        .active = false,
        .sample = &samplebank[0],
        .sample_idx = 0,
        .pff_idx = 0,
        .ppf = &ppf[8],
    },
    {
        .active = false,
        .sample = &samplebank[0],
        .sample_idx = 0,
        .pff_idx = 0,
        .ppf = &ppf[12],
    },
};

// Functions ///////////////////////////////////////////////////////////////////

int16_t get_transposed_sample(voice_t *v)
{
    // Polyphase re-sampling filter
    // Variable names after Lyons - Understanding Digital Signal Processing.

    float y;
    int32_t k; // Sub-filter coefficient selector
    int32_t l; // Delay-line index
    uint32_t N = PPF_NUM_TABS;

    // Short-hand names for voice variables
    int16_t *x = v->sample->data;
    uint32_t n = v->sample_idx;
    uint32_t m = v->pff_idx;
    uint32_t M = v->ppf->M;
    uint32_t L = v->ppf->L;
    float *h = v->ppf->h;

    // Return zero if the voice is inactive
    if (v->active == false) {
        goto exit_zero_sample;
    }

    // Compute input buffer index (de-activate the voice if there are no more samples)
    n = (m * M) / L;

    // FIXME: Workaround because zero-transposition does not work
    if ((L == 1) && (M == 1)) {
        goto exit_no_transpose;
    }

    // No more input-samples
    if (n >= v->sample->length - 1) {
        v->active = false;
        goto exit_zero_sample;
    }

    // Compute polyphase sub-filter selector
    k = (m * M) % L;

    // Execute filter difference equation
    y = 0;
    for (l = 0; l < N; l++) {
        // Skip unknown input sample values
        if (n - l < 0) {
            continue;
        }

        y += 
            L / NUM_VOICES * h[L*l + k] * (float)x[n - l];
    }

    // Update voice names from internal variables
    // Increase output buffer index, `m`
    v->sample_idx = n;
    v->pff_idx = m + 1;

    // Return resulting sample
    return (int16_t)y;

exit_zero_sample:
    v->sample_idx = 0;
    v->pff_idx = 0;
    return 0;

exit_no_transpose:
    v->sample_idx = n;
    v->pff_idx = m + 1;
    return x[n];
}

int16_t get_squarewave_sample()
{
    static int32_t n = 0;
    int16_t x;

    x = (n < 50) * 32767;
    n = (n + 1) % 100;

    return x;
}

int32_t handle_midi(void)
{
    int32_t err;
    midi_message_t m;

    err = midi_get(&m);

    if (err == 0) {
        printf("%02x %02x %02x\n", m.status, m.data[0], m.data[1]);

        if (m.data[0] == 0x30) {
            if (m.status == 0x90) 
                voices[3].active = true;
            else if (m.status == 0x80)
                voices[3].active = false;
        }
        else if (m.data[0] == 0x34) {
            if (m.status == 0x90) 
                voices[2].active = true;
            else if (m.status == 0x80)
                voices[2].active = false;
        }
        else if (m.data[0] == 0x37) {
            if (m.status == 0x90) 
                voices[1].active = true;
            else if (m.status == 0x80)
                voices[1].active = false;
        }
        else if (m.data[0] == 0x3c) {
            if (m.status == 0x90) 
                voices[0].active = true;
            else if (m.status == 0x80)
                voices[0].active = false;
        }
    }

    return err;
}

void loop() 
{
    // Output buffer
    int16_t buffer[BUFFER_SIZE];
    int32_t buffer_idx = 0;

    float y;

    int32_t i;
    voice_t *v;

    while (1) {

        // Update voices based on MIDI input
        handle_midi(); 

        // Generate audio from voices
        y = 0;

        for (i = 0; i < NUM_VOICES; i++) {
            v = &voices[i];
            y += get_transposed_sample(v);

            // if (v->active)
            //     y += get_squarewave_sample();
        }

        // Write output do DAC
        buffer[buffer_idx] = (int16_t)y;

        if (buffer_idx == BUFFER_SIZE-1) {
            dac_write(buffer, BUFFER_SIZE);
            buffer_idx = 0;
        }
        else {
            buffer_idx += 1;
        }
    }
}

// Interrupts //////////////////////////////////////////////////////////////////

// Main ////////////////////////////////////////////////////////////////////////

int32_t main(void)
{
    int32_t err;

    // Initialize sample bank
    err = samplebank_init();
    if (err != 0) {
        fprintf(stderr, "Error initializin Sample Bank\n");
        return 1;
    }

    // Initialize DAC
    err = dac_init("default", SAMPLE_RATE_Hz);
    if (err != 0) {
        fprintf(stderr, "Error initializin DAC\n");
        return 1;
    }

    // Initialize MIDI
    err = midi_init();
    if (err != 0) {
        fprintf(stderr, "Error initializin MIDI\n");
        return 1;
    }

    //
    // Begin program
    //

    loop();

    //
    // End program
    //

    // Close DAC
    err = dac_close();
    if (err != 0) {
        fprintf(stderr, "Error closing DAC\n");
        return 1;
    }

    // Close MIDI
    err = midi_close();
    if (err != 0) {
        fprintf(stderr, "Error closing MIDI\n");
        return 1;
    }

    // TODO: Consider freeing the sound bank here.

    return 0;
}
