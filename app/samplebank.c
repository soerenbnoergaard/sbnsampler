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
    samplebank[0].root_note = 60; // C4

    return 0;
}

