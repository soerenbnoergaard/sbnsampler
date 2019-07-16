#include "midi.h"
#include "midi_stanford.h"
#include <stdio.h>

static int32_t counter;

int32_t midi_init(void)
{
    counter = 0;
    return 0;
}

int32_t midi_close(void)
{
    counter = 0;
    return 0;
}

int32_t midi_get(midi_message_t *m)
{
    int32_t ret;

    // Return midi messages at certain points in time
    switch (counter) {
    case 0:
        m->status = 0x90;
        m->data[0] = 48;
        m->data[1] = 127;
        ret = 0;
        break;
    case 22050:
        m->status = 0x80;
        m->data[0] = 48;
        m->data[1] = 0;
        ret = 0;
        break;
    default:
        ret = 1;
        break;
    }

    counter += 1;
    return ret;
}

int32_t midi_list(void)
{
    return 0;
}
