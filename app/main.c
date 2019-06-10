#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

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

#define MAP_VELOCITY_TO_AMPLITUDE true
#define MAP_VELOCITY_TO_CUTOFF true

// Globals /////////////////////////////////////////////////////////////////////

static settings_t global = {
    .sustain = false,
    .cutoff = 0,
    .resonance = 0,
};
FILE *log_h;

// Functions ///////////////////////////////////////////////////////////////////

int32_t handle_note_on(midi_message_t m)
{
    int32_t n;
    voice_t *v = NULL;
    sample_t *sample = NULL;
    int32_t transpose = 0;

    int32_t n_chosen = -1;

    // Find empty voice

    // TODO:
    // Get the insertion order-based voice selection working
    //

    for (n = 0; n < NUM_VOICES; n++) {
        if (voices[n].note == m.data[0]) {
            // Take over existing note
            n_chosen = n;
        }
        else if (voices[n].state == VOICE_STATE_IDLE) {
            // Empty voice
            n_chosen = n;
        }
    }

    if (n_chosen < 0) {
        // No voices available
        return 1;
    }
    v = &voices[n_chosen];

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
    v->state = VOICE_STATE_STARTING;

    return 0;
}

int32_t handle_note_off(midi_message_t m)
{
    int32_t n;

    // Find the active voice
    for (n = 0; n < NUM_VOICES; n++) {
        if (voices[n].note == m.data[0]) {
            voices[n].state = VOICE_STATE_STOPPED;
        }
    }

    return 0;
}

int32_t handle_midi(void)
{
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
            global.sustain = m.data[1] == 0 ? false : true;
            break;

        case MIDI_CC_CUTOFF:
            global.cutoff = m.data[1];
            break;

        case MIDI_CC_RESONANCE:
            global.resonance = m.data[1];
            break;
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

    int32_t n;
    voice_t *v;

    int32_t summary_tick = 0;

    while (1) {
        // Periodically print voice summary
        if (summary_tick == 0) {
            for (n = 0; n < NUM_VOICES; n++) {
                printf("%2d %2d\n", 
                    n,
                    (int)voices[n].state
                );
            }
            printf("\n");
        }
        summary_tick = (summary_tick + 1) % (1<<15);

        // Update voices based on MIDI input
        handle_midi(); 

        // Generate audio from voices
        y = 0;

        for (n = 0; n < NUM_VOICES; n++) {
            v = &voices[n];

            switch (v->state) {
            case VOICE_STATE_IDLE:
                continue;

            case VOICE_STATE_STARTING:

                // Reset polyphase filter and sample settings
                voice_reset(v);

                // Update sound parameters
                if (MAP_VELOCITY_TO_CUTOFF) {
                    v->settings.cutoff = (global.cutoff>>1) + (v->velocity>>1);
                }
                else {
                    v->settings.cutoff = global.cutoff;
                }
                v->settings.resonance = global.resonance;

                v->state = VOICE_STATE_RUNNING;
                continue;

            case VOICE_STATE_RUNNING:
                break;

            case VOICE_STATE_STOPPED:
                if (global.sustain) {
                    break;
                }
                else {
                    v->state = VOICE_STATE_IDLE;
                    continue;
                }

            default:
                fprintf(stderr, "Unknown state\n");
                continue;
            }

            // Break target:
            // Fetch sound for the given voice

            if (MAP_VELOCITY_TO_AMPLITUDE) {
                A = v->velocity / 128.0;
            }
            else {
                A = 1.0;
            }
            x = A * ppf_get_transposed_sample(v);

            // Apply filter (VCF)
            // Accumulate result of all voices!
            y += vcf_filter(x, v, v->settings.cutoff, v->settings.resonance);
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
