#include "midi.h"
#include "midi_stanford.h"
#include <stdio.h>

static int32_t sample_counter;

int32_t midi_init(void)
{
    sample_counter = 0;
    return 0;
}

int32_t midi_close(void)
{
    sample_counter = 0;
    return 0;
}

int32_t midi_get(midi_message_t *m)
{
    int32_t ret = 1;

    sample_counter += 1;
    return ret;
}

int32_t midi_list(void)
{
    return 0;
}
