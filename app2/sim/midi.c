#include "midi.h"

// Globals /////////////////////////////////////////////////////////////////////
static int32_t sample_counter;
static int32_t sim_midi_counter;
static midi_message_t sim_midi_message;
static bool sim_midi_available;

// Private functions ///////////////////////////////////////////////////////////

// Public functions ////////////////////////////////////////////////////////////
status_t midi_input(uint8_t status, uint8_t data0, uint8_t data1)
{
    sim_midi_message.status = status;
    sim_midi_message.data[0] = data0;
    sim_midi_message.data[1] = data1;
    sim_midi_available = true;
    return STATUS_OK;
}

status_t midi_init(void)
{
    sample_counter = 0;
    sim_midi_counter = 0;
    return STATUS_OK;
}

status_t midi_close(void)
{
    sample_counter = 0;
    return STATUS_OK;
}

status_t midi_get(midi_message_t *m)
{
    int32_t ret;

    // Return midi messages at certain points in time
    if (sim_midi_available) {
        *m = sim_midi_message;
        sim_midi_available = false;
        return STATUS_OK;
    }
    else {
        return STATUS_NOT_READY;
    }

    return ret;
}

int32_t midi_list(void)
{
    return 0;
}
