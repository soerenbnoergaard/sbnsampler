#include "midi.h"
#include "midi_stanford.h"
#include <stdio.h>

static int32_t sample_counter;
static int32_t sim_midi_counter;
midi_message_t sim_midi_messages[128];
int32_t sim_midi_positions[128];

int32_t midi_init(void)
{
    int32_t n;

    sample_counter = 0;
    sim_midi_counter = 0;
    for (n = 0; n < 128; n++) {
        sim_midi_positions[n] = -1;
    }

    // Add messages
    sim_midi_positions[0] = 0;
    sim_midi_messages[0].status = 0x90;
    sim_midi_messages[0].data[0] = 63;
    sim_midi_messages[0].data[1] = 127;

    sim_midi_positions[1] = 22050;
    sim_midi_messages[1].status = 0x80;
    sim_midi_messages[1].data[0] = 63;
    sim_midi_messages[1].data[1] = 0;
    return 0;
}

int32_t midi_close(void)
{
    sample_counter = 0;
    return 0;
}

int32_t midi_get(midi_message_t *m)
{
    int32_t ret;

    // Return midi messages at certain points in time
    if (sample_counter == sim_midi_positions[sim_midi_counter]) {
        *m = sim_midi_messages[sim_midi_counter];
        sim_midi_counter += 1;
        ret = 0;
    }
    else {
        ret = 1;
    }

    sample_counter += 1;
    return ret;
}

int32_t midi_list(void)
{
    return 0;
}
