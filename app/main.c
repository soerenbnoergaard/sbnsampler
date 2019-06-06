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
    bool active; // Whether the voice is active or not
    sample_t *x; // Input sample buffer
    uint32_t n; // Index into sample buffer
    uint32_t m; // Index into output buffer
    ppf_t *ppf; // Poly-phase filter
} voice_t;

// Defines, macros, and constants //////////////////////////////////////////////

#define SAMPLE_RATE_Hz 44100
#define BUFFER_SIZE 128
#define NUM_VOICES 4

// Globals /////////////////////////////////////////////////////////////////////

voice_t voices[NUM_VOICES] = {
    {
        .active = false,
        .x = &samplebank[0],
        .n = 0,
        .m = 0,
        .ppf = &ppf0,
    },
    {
        .active = false,
        .x = &samplebank[0],
        .n = 0,
        .m = 0,
        .ppf = &ppf5,
    },
    {
        .active = false,
        .x = &samplebank[0],
        .n = 0,
        .m = 0,
        .ppf = &ppf8,
    },
    {
        .active = false,
        .x = &samplebank[0],
        .n = 0,
        .m = 0,
        .ppf = &ppf12,
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

    // Return zero if the voice is inactive
    if (v->active == false) {
        v->n = 0;
        v->m = 0;
        return 0;
    }

    // Compute input buffer index (de-activate the voice if there are no more samples)
    v->n = (v->m * v->ppf->M) / v->ppf->L;

    if (v->n >= v->x->length - 1) {
        v->active = false;
        return 0;
    }

    // Compute polyphase sub-filter selector
    k = (v->m * v->ppf->M) % v->ppf->L;

    // Execute filter difference equation
    y = 0;
    for (l = 0; l < N; l++) {
        // Skip unknown input sample values
        if (v->n - l < 0) {
            continue;
        }

        y += 
            v->ppf->L / NUM_VOICES *     // Scaling factor
            v->ppf->h[l*v->ppf->L + k] * // Filter coefficient
            (float)v->x->data[v->n - l]; // Input sample
    }

    // Update output buffer index
    v->m += 1;
    return (int16_t)y;
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
            if (m.status == 0x90) {
                voices[0].active = false;
                voices[1].active = true;
                voices[2].active = false;
                voices[3].active = false;
            }
            else if (m.status == 0x80) {
                voices[0].active = false;
                voices[1].active = false;
                voices[2].active = false;
                voices[3].active = false;
            }
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
