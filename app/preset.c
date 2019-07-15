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

int32_t preset_init(const char *path)
{
    int32_t err;
    err = samplebank_init(path);
    if (err != 0) {
        fprintf(stderr, "Error initializing Sample bank\n");
        return 1;
    }

    // 0: Volca Keys
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

    // 1: Precision bass
    reset(&presets[1]);
    presets[1].samples[0] = &samplebank[15];
    presets[1].samples[1] = &samplebank[16];
    presets[1].samples[2] = &samplebank[17];
    presets[1].samples[3] = &samplebank[18];
    presets[1].samples[4] = &samplebank[19];
    presets[1].samples[5] = &samplebank[20];
    presets[1].samples[6] = &samplebank[21];
    presets[1].samples[7] = &samplebank[22];
    presets[1].num_samples = 8;

    presets[1].settings.note_stealing = false;
    presets[1].settings.cutoff = 0;
    presets[1].settings.resonance = 0;
    presets[1].settings.amp_release = 5;
    presets[1].settings.cutoff_velocity = 127;

    // 2: Nord piano
    reset(&presets[2]);
    presets[2].samples[0] = &samplebank[7];
    presets[2].samples[1] = &samplebank[8];
    presets[2].samples[2] = &samplebank[9];
    presets[2].samples[3] = &samplebank[10];
    presets[2].samples[4] = &samplebank[11];
    presets[2].samples[5] = &samplebank[12];
    presets[2].samples[6] = &samplebank[13];
    presets[2].samples[7] = &samplebank[14];
    presets[2].num_samples = 8;

    presets[2].settings.cutoff = 0;
    presets[2].settings.resonance = 0;
    presets[2].settings.amp_release = 5;
    presets[2].settings.cutoff_velocity = 127;

    // 3: FP90 grand
    reset(&presets[3]);
    presets[3].samples[0] = &samplebank[23];
    presets[3].samples[1] = &samplebank[24];
    presets[3].samples[2] = &samplebank[25];
    presets[3].samples[3] = &samplebank[26];
    presets[3].samples[4] = &samplebank[27];
    presets[3].samples[5] = &samplebank[28];
    presets[3].samples[6] = &samplebank[29];
    presets[3].samples[7] = &samplebank[30];
    presets[3].num_samples = 8;

    presets[3].settings.cutoff = 0;
    presets[3].settings.resonance = 0;
    presets[3].settings.amp_release = 5;
    presets[3].settings.cutoff_velocity = 127;

    // 4: FP90 upright
    reset(&presets[4]);
    presets[4].samples[0] = &samplebank[31];
    presets[4].samples[1] = &samplebank[32];
    presets[4].samples[2] = &samplebank[33];
    presets[4].samples[3] = &samplebank[34];
    presets[4].samples[4] = &samplebank[35];
    presets[4].samples[5] = &samplebank[36];
    presets[4].samples[6] = &samplebank[37];
    presets[4].samples[7] = &samplebank[38];
    presets[4].num_samples = 8;

    presets[4].settings.cutoff = 0;
    presets[4].settings.resonance = 0;
    presets[4].settings.amp_release = 5;
    presets[4].settings.cutoff_velocity = 127;

    return 0;
}