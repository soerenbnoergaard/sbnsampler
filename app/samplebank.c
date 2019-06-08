#include "samplebank.h"

sample_t samplebank[NUM_SAMPLES];

#define WAV_HEADER_SIZE 44

int32_t samplebank_init(void)
{
    // Malloc is done internally (never free'd!)
    binary_data_t *tmp = fh_read_file("sound/piano_c4.wav");
    if (tmp == NULL)
        return 1;

    // Skip WAV header
    samplebank[0].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[0].length = (tmp->size/2) - WAV_HEADER_SIZE;
    samplebank[0].note_root = 60; // C4
    samplebank[0].note_min = 49; // C#3
    samplebank[0].note_max = 71; // C#4

    return 0;
}

