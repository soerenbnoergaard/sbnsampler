#include "preset.h"
#include "samplebank.h"

preset_t presets[NUM_PRESETS];

static void reset(preset_t *p)
{
    p->num_samples = 0;

    p->settings.note_stealing = true;
    p->settings.sustain = false;
    p->settings.volume = 127;
    p->settings.cutoff = 127;
    p->settings.resonance = 0;
    p->settings.cutoff_velocity = 0;
    p->settings.amp_attack = 0;
    p->settings.amp_decay = 0;
    p->settings.amp_sustain = 127;
    p->settings.amp_release = 0;
    p->settings.amp_velocity = 0;
}

int32_t preset_init(void)
{
    int32_t err;
    err = samplebank_init();
    if (err != 0) {
        return 1;
    }

    // Volca Keys
    reset(&presets[0]);
    presets[0].samples[0] = &samplebank[0];
    presets[0].samples[1] = &samplebank[1];
    presets[0].samples[2] = &samplebank[2];
    presets[0].samples[3] = &samplebank[3];
    presets[0].samples[4] = &samplebank[4];
    presets[0].samples[5] = &samplebank[5];
    presets[0].samples[6] = &samplebank[6];
    presets[0].num_samples = 7;

    presets[0].settings.cutoff = 0;
    presets[0].settings.resonance = 0;
    presets[0].settings.amp_attack = 0;
    presets[0].settings.amp_decay = 0;
    presets[0].settings.amp_sustain = 127;
    presets[0].settings.amp_release = 32;
    presets[0].settings.cutoff_velocity = 127;

    // Piano
    reset(&presets[1]);
    presets[1].samples[0] = &samplebank[7];
    presets[1].samples[1] = &samplebank[8];
    presets[1].samples[2] = &samplebank[9];
    presets[1].samples[3] = &samplebank[10];
    presets[1].samples[4] = &samplebank[11];
    presets[1].samples[5] = &samplebank[12];
    presets[1].samples[6] = &samplebank[13];
    presets[1].samples[7] = &samplebank[14];
    presets[1].num_samples = 8;

    presets[1].settings.cutoff = 0;
    presets[1].settings.resonance = 0;
    presets[1].settings.amp_release = 20;
    presets[1].settings.cutoff_velocity = 127;

    // Precision bass
    reset(&presets[2]);
    presets[2].samples[0] = &samplebank[15];
    presets[2].samples[1] = &samplebank[16];
    presets[2].samples[2] = &samplebank[17];
    presets[2].samples[3] = &samplebank[18];
    presets[2].samples[4] = &samplebank[19];
    presets[2].samples[5] = &samplebank[20];
    presets[2].samples[6] = &samplebank[21];
    presets[2].samples[7] = &samplebank[22];
    presets[2].num_samples = 8;

    presets[2].settings.note_stealing = false;
    presets[2].settings.cutoff = 0;
    presets[2].settings.resonance = 0;
    presets[2].settings.amp_release = 20;
    presets[2].settings.cutoff_velocity = 127;

    return 0;
}