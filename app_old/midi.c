#include "midi.h"
#include "midi_stanford.h"
#include <stdio.h>
#include <alsa/asoundlib.h>

// Types ///////////////////////////////////////////////////////////////////////

typedef enum {
    MIDI_IN_STATE_WAITING_FOR_STATUS,
    MIDI_IN_STATE_WAITING_FOR_DATA1,
    MIDI_IN_STATE_WAITING_FOR_DATA2
} midi_in_state_t;

typedef struct {
    midi_in_state_t state;
    midi_message_t message;
    snd_rawmidi_t *handle;
} midi_in_t;

// Defines, macros, and constants //////////////////////////////////////////////

#define NUM_MIDI_IN 2
const char *midi_interface[NUM_MIDI_IN] = {
    "hw:1,0,1",
    "hw:1,0,0"
};

// Globals /////////////////////////////////////////////////////////////////////

static midi_in_t midi_in[NUM_MIDI_IN];

// Functions ///////////////////////////////////////////////////////////////////

int32_t midi_init(void)
{
    int32_t err;
    int32_t n;

    for (n = 0; n < NUM_MIDI_IN; n++) {
        printf("Selected midi port: %s\n", midi_interface[n]);

        if ((err = snd_rawmidi_open(&midi_in[n].handle, NULL, midi_interface[n], SND_RAWMIDI_NONBLOCK)) < 0) {
            fprintf(stderr, "Problem opening MIDI input: %s\n", snd_strerror(err));
            return 1;
        }

        midi_in[n].state = MIDI_IN_STATE_WAITING_FOR_STATUS;
    }

    return 0;
}

int32_t midi_close(void)
{
    int32_t n;

    for (n = 0; n < NUM_MIDI_IN; n++) {
        snd_rawmidi_close(midi_in[n].handle);
        midi_in[n].handle = NULL;
    }

    return 0;
}

int32_t midi_get(midi_message_t *m)
{
    int32_t n;
    int32_t err;
    uint8_t buffer;
    midi_in_t *h;

    for (n = 0; n < NUM_MIDI_IN; n++) {
        h = &midi_in[n];

        if (snd_rawmidi_read(h->handle, &buffer, 1) < 0) {
            err = 1;
            continue;
        }

        //
        // Default values
        //

        err = 1;

        //
        // Overwriting cases
        //

        // A status message is received if the MSB is set.
        if (buffer & 0x80) {
            h->state = MIDI_IN_STATE_WAITING_FOR_STATUS;
        }

        switch (h->state) {
        case MIDI_IN_STATE_WAITING_FOR_STATUS:
            h->message.status = buffer;
            h->state = MIDI_IN_STATE_WAITING_FOR_DATA1;
            break;

        case MIDI_IN_STATE_WAITING_FOR_DATA1:
            h->message.data[0] = buffer;
            h->state = MIDI_IN_STATE_WAITING_FOR_DATA2;
            break;

        case MIDI_IN_STATE_WAITING_FOR_DATA2:
            h->message.data[1] = buffer;
            h->state = MIDI_IN_STATE_WAITING_FOR_STATUS;
            err = 0;

            // If a message is ready, return it immideately instead of finishing
            // the midi_in loop.
            *m = h->message;
            return err;

        default:
            err = 1;
            break;
        }
    }

    return err;
}

int32_t midi_list(void)
{
    print_midi_ports();
    return 0;
}