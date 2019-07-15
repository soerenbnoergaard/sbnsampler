#include "midi.h"
#include "midi_stanford.h"
#include <stdio.h>

int32_t midi_init(void)
{
    return 0;
}

int32_t midi_close(void)
{
    return 0;
}

int32_t midi_get(midi_message_t *m)
{
    m->status = 0x00;
    m->data[0] = 0x00;
    m->data[1] = 0x00;
    return 1;
}

int32_t midi_list(void)
{
    return 0;
}
