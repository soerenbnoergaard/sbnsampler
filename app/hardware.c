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
            process_note_off(m);
        }
        else {
            process_note_on(m);
        }
    }
    else if ((m.status & 0xf0) == 0x80) {
        process_note_off(m);
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

    err = dac_init("default", SAMPLE_RATE_Hz);
    if (err != 0) {
        fprintf(stderr, "Error initializing DAC\n");
        return 1;
    }
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
