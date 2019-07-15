#include "hardware.h"
#include "process.h"

#include "preset.h"
#include "voice.h"
#include "gpio.h"
#include "dac.h"
#include "midi.h"

#define MIDI_CC_PRESET 0x50
#define MIDI_CC_VOLUME 0x46
#define MIDI_CC_CUTOFF_VELOCITY 0x0a
#define MIDI_CC_SUSTAIN 0x40
#define MIDI_CC_CUTOFF 0x4a
#define MIDI_CC_RESONANCE 0x47

#define MIDI_CC_AMP_ATTACK 0x53
#define MIDI_CC_AMP_DECAY 0x59
#define MIDI_CC_AMP_SUSTAIN 0x55
#define MIDI_CC_AMP_RELEASE 0x52

static voice_t *find_voice(uint8_t note, bool enable_stealing, bool *voice_stolen)
{
    // Find a suiting voice for the incoming note.
    int32_t n;
    int32_t n_idle = -1;
    int32_t n_steal = -1;
    int32_t n_oldest = -1;
    voice_t *v;

    for (n = 0; n < NUM_VOICES; n++) {
        v = &voices[n];

        switch (v->state) {
        case VOICE_STATE_IDLE:
            n_idle = n;
            break;

        default:
            if (v->note == note) {
                n_steal = n;
            }

            if (n_oldest < 0) {
                n_oldest = n;
            }
            else if (v->ppf_idx > voices[n_oldest].ppf_idx) {
                n_oldest = n;
            }
            break;
        }
    }

    if ((enable_stealing) && (n_steal >= 0)) {
        *voice_stolen = true;
        return &voices[n_steal];
    }
    else if (n_idle >= 0) {
        *voice_stolen = false;
        return &voices[n_idle];
    }
    else {
        *voice_stolen = true;
        return &voices[n_oldest];
    }

}

static sample_t *find_sample(uint8_t note)
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

static ppf_t *find_transposition(voice_t *v)
{
    int32_t num_steps;
    num_steps = v->note - v->sample->note_root;

    // Find the necessary polyphase filter to obtain the transposition
    if ((PPF_TRANSPOSE_MIN < num_steps) && (num_steps < PPF_TRANSPOSE_MAX)) {
        return &ppf[PPF_ZERO_TRANSPOSE_OFFSET + num_steps];
    }

    return NULL;
}


static int32_t note_on(midi_message_t m)
{
    bool voice_stolen = false;
    voice_t *v = NULL;
    sample_t *sample = NULL;
    ppf_t *transposition = NULL;

    v = find_voice(m.data[0], global.note_stealing, &voice_stolen);
    if (v == NULL) {
        fprintf(stderr, "No voice available\n");
        return 1;
    }
    v->note = m.data[0];
    v->velocity = m.data[1];

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

    if (voice_stolen) {
        v->state = VOICE_STATE_RESTARTING_NOTE;
        return 0;
    }
    else {
        v->state = VOICE_STATE_STARTING_NOTE;
        return 0;
    }
}

static int32_t note_off(midi_message_t m)
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

static int32_t handle_midi(void)
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
            note_off(m);
        }
        else {
            note_on(m);
        }
    }
    else if ((m.status & 0xf0) == 0x80) {
        note_off(m);
    }
    else if ((m.status & 0xf0) == 0xb0) {
        switch (m.data[0]) {
        case MIDI_CC_PRESET:
            if (m.data[1] < NUM_PRESETS) {
                active_preset = &presets[m.data[1]];
                global = active_preset->settings;
                for (n = 0; n < NUM_VOICES; n++) {
                    voices[n].state = VOICE_STATE_IDLE;
                }
            }
            break;

        case MIDI_CC_VOLUME:
            global.volume = m.data[1];
            break;

        case MIDI_CC_CUTOFF_VELOCITY:
            global.cutoff_velocity = m.data[1];
            break;

        case MIDI_CC_SUSTAIN:
            global.sustain = m.data[1] == 0 ? false : true;
            break;

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


int32_t hardware_init(void)
{
    int32_t err;
    midi_list();

    err = dac_init("default", SAMPLE_RATE_Hz);
    if (err != 0) {
        fprintf(stderr, "Error initializing DAC\n");
        return 1;
    }

    err = midi_init();
    if (err != 0) {
        fprintf(stderr, "Error initializing MIDI\n");
        return 1;
    }

    err = gpio_init();
    if (err != 0) {
        fprintf(stderr, "Error initializing GPIO\n");
        return 1;
    }
    return 0;
}

int32_t hardware_close(void)
{
    int32_t err;
    err = midi_close();
    if (err != 0) {
        fprintf(stderr, "Error closing MIDI\n");
        return 1;
    }

    err = dac_close();
    if (err != 0) {
        fprintf(stderr, "Error closing DAC\n");
        return 1;
    }

    return 0;
}

int32_t hardware_tick(void)
{
    int32_t err;

    // Handle input (MIDI)
    handle_midi(); 

    // Handle output (DAC)
    if (buffer_idx == BUFFER_SIZE) {
        gpio5_set();
        err = dac_write(buffer, BUFFER_SIZE);
        if (err != 0) {
            return 1;
        }
        gpio5_clear();
        buffer_idx = 0;
    }

    return 0;
}
