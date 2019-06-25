#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "utils.h"
#include "dac.h"
#include "midi.h"
#include "preset.h"
#include "voice.h"
#include "polyfilter.h"
#include "vcf.h"
#include "adsr.h"
#include "gpio.h"

// Types ///////////////////////////////////////////////////////////////////////

// Defines, macros, and constants //////////////////////////////////////////////

// #define MIDI_CC_SUSTAIN 64
#define MIDI_CC_CUTOFF 0x4a
#define MIDI_CC_RESONANCE 0x47

#define MIDI_CC_AMP_ATTACK 0x53
#define MIDI_CC_AMP_DECAY 0x59
#define MIDI_CC_AMP_SUSTAIN 0x55
#define MIDI_CC_AMP_RELEASE 0x52

#define MAP_VELOCITY_TO_AMPLITUDE false
#define MAP_VELOCITY_TO_CUTOFF false

// Globals /////////////////////////////////////////////////////////////////////

static preset_t *active_preset = &presets[0];
static settings_t global = {
    .sustain = false,
    .cutoff = 64,
    .resonance = 0,
    .amp_attack = 0,
    .amp_decay = 0,
    .amp_sustain = 127,
    .amp_release = 16
};
FILE *log_h;

// Functions ///////////////////////////////////////////////////////////////////

voice_t *find_voice(uint8_t note, bool *voice_takeover)
{
    // Find a suiting voice for the incoming note.
    int32_t n;
    voice_t *ret = NULL;

    for (n = 0; n < NUM_VOICES; n++) {
        if (voices[n].note == note) {
            // Take over existing note's voice
            *voice_takeover = true;
            ret = &voices[n];
            break;
        }
        else if (voices[n].state == VOICE_STATE_IDLE) {
            // Empty voice
            *voice_takeover = false;
            ret = &voices[n];
        }
    }

    return ret;
}

sample_t *find_sample(uint8_t note)
{
    // Find the sample to activate
    int32_t n;
    preset_t *p = active_preset;

    for (n = 0; n < p->num_samples; n++) {
        if ((p->samples[n]->note_min <= note) && (note <= p->samples[n]->note_max)) {
            return p->samples[n];
        }
    }
    return NULL;
}

ppf_t *find_transposition(voice_t *v)
{
    int32_t num_steps;
    num_steps = v->note - v->sample->note_root;

    // Find the necessary polyphase filter to obtain the transposition
    if ((PPF_TRANSPOSE_MIN < num_steps) && (num_steps < PPF_TRANSPOSE_MAX)) {
        return &ppf[PPF_ZERO_TRANSPOSE_OFFSET + num_steps];
    }

    return NULL;
}

int32_t handle_note_on(midi_message_t m)
{
    bool voice_takeover = false;
    voice_t *v = NULL;
    sample_t *sample = NULL;
    ppf_t *transposition = NULL;

    v = find_voice(m.data[0], &voice_takeover);
    if (v == NULL) {
        return 1;
    }
    v->note = m.data[0];
    v->velocity = m.data[1];

    if (voice_takeover) {
        v->state = VOICE_STATE_RESTARTING_NOTE;
        return 0;
    }

    sample = find_sample(v->note);
    if (sample == NULL) {
        fprintf(stderr, "Note out of range\n");
        return 1;
    }
    v->sample = sample;

    transposition = find_transposition(v);
    if (transposition == NULL) {
        fprintf(stderr, "Transposition out of ranged\n");
        return 1;
    }
    v->ppf = transposition;

    v->state = VOICE_STATE_STARTING_NOTE;
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
        // case MIDI_CC_SUSTAIN:
        //     global.sustain = m.data[1] == 0 ? false : true;
        //     break;

        case MIDI_CC_CUTOFF:
            global.cutoff = m.data[1];
            break;

        case MIDI_CC_RESONANCE:
            global.resonance = m.data[1];
            break;
        
        case MIDI_CC_AMP_ATTACK:
            global.amp_attack = m.data[1];
            break;

        case MIDI_CC_AMP_DECAY:
            global.amp_decay = m.data[1];
            break;

        case MIDI_CC_AMP_SUSTAIN:
            global.amp_sustain = m.data[1];
            break;

        case MIDI_CC_AMP_RELEASE:
            global.amp_release = m.data[1];
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
            // for (n = 0; n < NUM_VOICES; n++) {
            //     printf("%2d %2d\n", 
            //         n,
            //         (int)voices[n].state
            //     );
            // }
            // printf("\n");
        }
        summary_tick = (summary_tick + 1) % (1<<15);

        // Update voices based on MIDI input
        handle_midi(); 

        // Generate audio from voices
        y = 0;

        for (n = 0; n < NUM_VOICES; n++) {
            v = &voices[n];

            //
            // VOICE STATE MACHINE
            //

            switch (v->state) {
            case VOICE_STATE_IDLE:
                continue;

            case VOICE_STATE_STARTING_NOTE:

                adsr_start(&v->amplitude_envelope);
                v->state = VOICE_STATE_STARTING;

                continue;

            case VOICE_STATE_RESTARTING_NOTE:

                adsr_restart(&v->amplitude_envelope);
                v->state = VOICE_STATE_STARTING;

                break;
            
            case VOICE_STATE_STARTING:

                if (v->amplitude_envelope.state == ADSR_STATE_IDLE) {
                    voice_reset(v);
                    adsr_setup(&v->amplitude_envelope, global.amp_attack, global.amp_decay, global.amp_sustain, global.amp_release);
                    v->state = VOICE_STATE_RUNNING;
                }

                break;

            case VOICE_STATE_RUNNING:

                // Update sound parameters
                if (MAP_VELOCITY_TO_CUTOFF) {
                    v->settings.cutoff = (global.cutoff>>1) + (v->velocity>>1);
                }
                else {
                    v->settings.cutoff = global.cutoff;
                }
                v->settings.resonance = global.resonance;

                break;

            case VOICE_STATE_STOPPED:
                if (global.sustain) {
                    break;
                }
                else {
                    adsr_stop(&v->amplitude_envelope);
                    v->state = VOICE_STATE_RELEASED;
                    break;
                }

            case VOICE_STATE_RELEASED:
                if (v->amplitude_envelope.value <= 0) {
                    v->state = VOICE_STATE_IDLE;
                }
                break;

            default:
                fprintf(stderr, "Unknown state\n");
                continue;
            }

            //
            // VOICE PLAYBACK
            //

            adsr_update(&v->amplitude_envelope);
            if (v->amplitude_envelope.state == ADSR_STATE_STOPPED) {
                v->state = VOICE_STATE_IDLE;
                continue;
            }

            A = v->amplitude_envelope.value / 128.0;
            x = A * ppf_get_transposed_sample(v);

            // Apply filter (VCF)
            // Accumulate result of all voices!
            y += vcf_filter(x, v, v->settings.cutoff, v->settings.resonance);
        }

        // Write output do DAC
        buffer[buffer_idx] = (int16_t)y;

        if (buffer_idx == BUFFER_SIZE-1) {
            gpio5_set();
            dac_write(buffer, BUFFER_SIZE);
            gpio5_clear();
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

    // Initialize presets
    err = preset_init();
    if (err != 0) {
        fprintf(stderr, "Error initializing Preset\n");
        return 1;
    }

    // Initialize DAC
    err = dac_init("default", SAMPLE_RATE_Hz);
    if (err != 0) {
        fprintf(stderr, "Error initializing DAC\n");
        return 1;
    }

    // Initialize MIDI
    midi_list();
    err = midi_init();
    if (err != 0) {
        fprintf(stderr, "Error initializing MIDI\n");
        return 1;
    }

    // Initialize Voices
    err = voice_init();
    if (err != 0) {
        fprintf(stderr, "Error initializing Voices\n");
        return 1;
    }

    // Initialize GPIO
    err = gpio_init();
    if (err != 0) {
        fprintf(stderr, "Error initializing GPIO\n");
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
