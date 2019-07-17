#include "midi.h"
#include "midi_stanford.h"
#include <stdio.h>
#include <stdbool.h>

static int32_t sample_counter;
static int32_t sim_midi_counter;
static midi_message_t sim_midi_message;
static bool sim_midi_available;

// SIMULATION ONLY FUNCTION - EXPOSED THROUGH THE DLL!
int32_t _midi_enqueue(uint8_t status, uint8_t data0, uint8_t data1)
{
    sim_midi_message.status = status;
    sim_midi_message.data[0] = data0;
    sim_midi_message.data[1] = data1;
    sim_midi_available = true;
    return 0;
}

int32_t midi_init(void)
{
    int32_t n;

    sample_counter = 0;
    sim_midi_counter = 0;
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
    if (sim_midi_available) {
        *m = sim_midi_message;
        sim_midi_available = false;
        ret = 0;
    }
    else {
        ret = 1;
    }

    return ret;
}

int32_t midi_list(void)
{
    return 0;
}
