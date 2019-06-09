#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "dac.h"
#include "midi.h"
#include "samplebank.h"
#include "voice.h"
#include "polyfilter.h"
#include "vcf.h"

// Types ///////////////////////////////////////////////////////////////////////

// Defines, macros, and constants //////////////////////////////////////////////

#define SAMPLE_RATE_Hz 44100
#define BUFFER_SIZE 128

#define MIDI_CC_SUSTAIN 64
#define MIDI_CC_CUTOFF 74
#define MIDI_CC_RESONANCE 71

#define MAP_VELOCITY_TO_AMPLITUDE false
#define MAP_VELOCITY_TO_CUTOFF true

// Globals /////////////////////////////////////////////////////////////////////

FILE *log_h;

// Functions ///////////////////////////////////////////////////////////////////

float map_midi_to_cutoff(uint8_t value)
{
    // Map cutoff to VCF parameter `g`.
    const float b = 0.01;
    const float a = (0.9-b) / (127*127*127);
    return a*value*value*value + b;
}

int32_t handle_cutoff(midi_message_t m)
{
    // FIXME: At the moment, the cutoff is either controlled by velocity or
    // manually - not both.
    if (MAP_VELOCITY_TO_CUTOFF) {
        return 0;
    }
    else {
        int32_t n;
        float g = map_midi_to_cutoff(m.data[1]);

        for (n = 0; n < NUM_VOICES; n++) {
            voice_default.vcf.g = g;
            voices[n].vcf.g = g;
        }

        return 0;
    }
}

float map_midi_to_resonance(uint8_t value)
{
    // Map resonance to VCF parameter `k`.
    const float b = 0.5;
    const float a = (1.5-b) / 127;
    return a*value + b;
}

int32_t handle_resonance(midi_message_t m)
{
    // Map cutoff to VCF parameter `k`.

    int32_t n;
    float k = map_midi_to_resonance(m.data[1]);

    for (n = 0; n < NUM_VOICES; n++) {
        voice_default.vcf.k = k;
        voices[n].vcf.k = k;
    }

    return 0;
}

int32_t handle_note_on(midi_message_t m)
{
    int32_t n;
    voice_t *v = NULL;
    sample_t *sample = NULL;
    int32_t transpose = 0;

    int32_t n_chosen = -1;
    int32_t n_min = 0; 
    int32_t idx_min = 99999;
    int32_t idx_max = -1;

    // Find empty voice

    // TODO:
    // Get the insertion order-based voice selection working
    //

    for (n = 0; n < NUM_VOICES; n++) {
        if (voices[n].note == m.data[0]) {
            // Take over existing note
            n_chosen = n;
        }
        else if (voices[n].active == false) {
            // Empty voice
            n_chosen = n;
        }

        // Store the activation index extremes
        if (idx_max < voices[n].activation_index) {
            idx_max = voices[n].activation_index;
        }
        if (idx_min > voices[n].activation_index) {
            idx_min = voices[n].activation_index;
            n_min = n;
        }
    }

    // If there is no empty slots, select the one with the lowest activation
    // index.
    if (n_chosen < 0) {
        // No voices available
        n_chosen = n_min;
    }
    printf("%02d @ %02d -> %02d\n", idx_min, n_min, n_chosen);
    v = &voices[n_chosen];
    v->activation_index = idx_max+1;

    // List voices
    for (n = 0; n < NUM_VOICES; n++) {
        printf("%2d %02d %s\n",
            n,
            voices[n].activation_index, 
            n == n_chosen ? "<" :
                voices[n].active ? "x" : " ");
    }

    // Find the sample to activate
    for (n = 0; n < NUM_SAMPLES; n++) {
        if ((samplebank[n].note_min <= m.data[0]) && (m.data[0] <= samplebank[n].note_max)) {
            sample = &samplebank[n];
        }
    }
    if (sample == NULL) {
        fprintf(stderr, "Note out of range\n");
        return 1;
    }

    // Find transposition interval
    transpose = m.data[0] - sample->note_root;

    // Find the necessary polyphase filter to obtain the transposition
    if ((transpose < PPF_TRANSPOSE_MIN) || (PPF_TRANSPOSE_MAX < transpose)) {
        fprintf(stderr, "Transposition out of range: %d\n", transpose);
        return 1;
    }

    // Activate voice
    v->sample = sample;
    v->ppf = &ppf[PPF_ZERO_TRANSPOSE_OFFSET + transpose];
    v->note = m.data[0];
    v->velocity = m.data[1];
    if (MAP_VELOCITY_TO_CUTOFF) {
        v->vcf.g = map_midi_to_cutoff(v->velocity);
    }
    v->killed = false;
    v->active = true;

    return 0;
}

int32_t handle_note_off(midi_message_t m)
{
    int32_t n;

    // Find the active voice
    for (n = 0; n < NUM_VOICES; n++) {
        if (voices[n].note == m.data[0]) {
            // Signal the voice to be killed when sustain is over.
            voices[n].killed = true;
        }
    }

    return 0;
}

int32_t handle_sustain(midi_message_t m)
{
    int32_t n;

    if (m.data[1] == 0) {
        // Sustain off
        for (n = 0; n < NUM_VOICES; n++) {
            voices[n].sustained = false;
        }
    }
    else {
        // Sustain on
        for (n = 0; n < NUM_VOICES; n++) {
            voices[n].sustained = true;
        }
    }
    return 0;
}


int32_t handle_midi(void)
{
    int32_t n;
    int32_t err;
    midi_message_t m;

    err = midi_get(&m);

    if (err != 0) {
        return 1;
    }

    printf("%02x %02x %02x\n", m.status, m.data[0], m.data[1]);

    if ((m.status & 0xf0) == 0x90) {
        if (m.data[1] == 0) {
            handle_note_off(m);
        }
        else {
            handle_note_on(m);
        }
    }
    else if ((m.status & 0xf0) == 0x80) {
        handle_note_off(m);
    }
    else if ((m.status & 0xf0) == 0xb0) {
        switch (m.data[0]) {
        case MIDI_CC_SUSTAIN:
            handle_sustain(m);
            break;

        case MIDI_CC_CUTOFF:
            handle_cutoff(m);
            break;

        case MIDI_CC_RESONANCE:
            handle_resonance(m);
            break;
        }
    }

    // Kill notes that are no longer sustained
    for (n = 0; n < NUM_VOICES; n++) {
        if ((voices[n].killed == true) && (voices[n].sustained == false)) {
            voice_reset(&voices[n]);
        }
    }

    return 0;
}

void loop() 
{
    // Output buffer
    int16_t buffer[BUFFER_SIZE];
    int32_t buffer_idx = 0;

    float A;
    float x;
    float y;

    int32_t i;
    voice_t *v;

    while (1) {

        // Update voices based on MIDI input
        handle_midi(); 

        // Generate audio from voices
        y = 0;

        for (i = 0; i < NUM_VOICES; i++) {
            v = &(voices[i]);
            if (v->active == false) {
                continue;
            }

            if (MAP_VELOCITY_TO_AMPLITUDE) {
                A = v->velocity / 128.0;
            }
            else {
                A = 1.0;
            }
            x = A * ppf_get_transposed_sample(v);

            // Apply filter (VCF)
            // Accumulate result of all voices!
            y += vcf_filter(x, v);
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

    // Initialize log
    log_h = fopen("log.log", "w");

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

    // Initialize Voices
    err = voice_init();
    if (err != 0) {
        fprintf(stderr, "Error initializin Voices\n");
        return 1;
    }

    //
    // Begin program
    //

    printf("Ready!\n");
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
