#include "midi.h"
#include "midi_stanford.h"
#include <stdio.h>

static int32_t sample_counter;
static int32_t sim_midi_counter;
static midi_message_t sim_midi_messages[128];
static int32_t sim_midi_positions[128];

// SIMULATION ONLY FUNCTION - EXPOSED THROUGH THE DLL!
int32_t _midi_enqueue(int32_t index, int32_t position, uint8_t status, uint8_t data0, uint8_t data1)
{
    sim_midi_positions[index] = position;
    sim_midi_messages[index].status = status;
    sim_midi_messages[index].data[0] = data0;
    sim_midi_messages[index].data[1] = data1;
    return 0;
}

int32_t midi_init(void)
{
    int32_t n;

    sample_counter = 0;
    sim_midi_counter = 0;
    for (n = 0; n < 128; n++) {
        sim_midi_positions[n] = -1;
    }
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
