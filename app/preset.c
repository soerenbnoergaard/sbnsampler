#include "preset.h"
#include "samplebank.h"

preset_t presets[NUM_PRESETS];

int32_t preset_init(void)
{
    int32_t err;
    err = samplebank_init();
    if (err != 0) {
        return 1;
    }

    // Volca Keys
    presets[0].samples[0] = &samplebank[0];
    presets[0].samples[1] = &samplebank[1];
    presets[0].samples[2] = &samplebank[2];
    presets[0].samples[3] = &samplebank[3];
    presets[0].samples[4] = &samplebank[4];
    presets[0].samples[5] = &samplebank[5];
    presets[0].samples[6] = &samplebank[6];
    presets[0].samples[7] = &samplebank[7];
    presets[0].num_samples = 8;

    return 0;
}