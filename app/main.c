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
    int32_t sample_idx; // Input sample index `n` of the poly-phase filter.

    ppf_t *ppf;
    int32_t ppf_idx; // Output sample index `m` of the poly-phase filter.

    int16_t note; // Midi number corresponding to the active note (negative == inactive)
    uint8_t velocity; // Midi note velocity;
    bool sustained; // True when a note is being sustained.
    bool killed; // True when a note has been signaled off.
} voice_t;

// Defines, macros, and constants //////////////////////////////////////////////

#define SAMPLE_RATE_Hz 44100
#define BUFFER_SIZE 128
#define NUM_VOICES 8
#define AMPLITUDE_DIVIDER (4)

// Globals /////////////////////////////////////////////////////////////////////

voice_t voices[NUM_VOICES];

// Functions ///////////////////////////////////////////////////////////////////

int32_t voice_reset(voice_t *v)
{
    voice_t voice_default = {
        .active = false,
        .sample = &samplebank[0],
        .sample_idx = 0,
        .ppf = &ppf[0],
        .ppf_idx = 0,
        .note = -1,
        .velocity = 0,
        .sustained = false,
        .killed = false,
    };

    *v = voice_default;
    return 0;
}

int32_t voices_init(void)
{
    int32_t n;
    for (n = 0; n < NUM_VOICES; n++) {
        voice_reset(&voices[n]);
    }

    return 0;
}

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
    int32_t n = v->sample_idx;
    int32_t m = v->ppf_idx;
    int32_t M = v->ppf->M;
    int32_t L = v->ppf->L;
    float *h = v->ppf->h;

    // Return zero if the voice is inactive
    if (v->active == false) {
        goto exit_zero_sample;
    }

    // Compute input buffer index (de-activate the voice if there are no more samples)
    n = (m * M) / L;

    // No more input-samples
    if (n >= v->sample->length - 1) {
        voice_reset(v);
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

        y += (L * h[L*l + k] * (float)x[n - l]) / AMPLITUDE_DIVIDER;
    }

    // Update voice names from internal variables
    // Increase output buffer index, `m`
    v->sample_idx = n;
    v->ppf_idx = m + 1;

    // Return resulting sample
    return (int16_t)y;

exit_zero_sample:
    v->sample_idx = 0;
    v->ppf_idx = 0;
    return 0;
}

int16_t get_squarewave_sample()
{
    static int32_t n = 0;
    int16_t x;

    x = (n < 50) * 32767;
    n = (n + 1) % 100;

    return x;
}

int32_t handle_note_on(midi_message_t m)
{
    int32_t n;
    voice_t *v = NULL;
    sample_t *sample = NULL;
    int32_t transpose = 0;

    // Find empty voice
    for (n = 0; n < NUM_VOICES; n++) {
        if (voices[n].note == m.data[0]) {
            // Take over existing note
            v = &voices[n];
            break;
        }
        else if (voices[n].active == false) {
            // Choose new voice
            v = &voices[n];
            break;
        }
    }

    if (v == NULL) {
        // No voices available
        return 1;
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
    v->sample_idx = 0;
    v->ppf_idx = 0;
    v->ppf = &ppf[PPF_ZERO_TRANSPOSE_OFFSET + transpose];
    v->note = m.data[0];
    v->velocity = m.data[1];
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

int32_t handle_sustain_on()
{
    int32_t n;
    for (n = 0; n < NUM_VOICES; n++) {
        voices[n].sustained = true;
    }
    return 0;
}

int32_t handle_sustain_off()
{
    int32_t n;
    for (n = 0; n < NUM_VOICES; n++) {
        voices[n].sustained = false;
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
        if (m.data[0] == 0x40) {
            if (m.data[1] == 0) {
                handle_sustain_off();
            }
            else {
                handle_sustain_on();
            }
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
            y += (v->velocity * get_transposed_sample(v)) / 128;

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

    // Initialize Voices
    err = voices_init();
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
