#ifndef SAMPLEBANK_H
#define SAMPLEBANK_H

#include <stdint.h>

#define expand_sample(name)

extern const uint8_t _binary_sound_piano_c4_wav_start[];
extern const uint8_t _binary_sound_piano_c4_wav_end[];

typedef struct {
    uint16_t *start;
    uint16_t *end;
} sample_t;

sample_t sample = {
    .start = (((int16_t *)&_binary_sound_piano_c4_wav_start) + 44),
    .end = ((int16_t *)&_binary_sound_piano_c4_wav_end)
};

#endif
