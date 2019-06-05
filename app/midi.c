#include "midi.h"
#include <stdio.h>
#include <alsa/asoundlib.h>

static snd_rawmidi_t* midi_in = NULL;
static int32_t state = 0;

int32_t midi_init(void)
{
    // TODO: Merge all midi streams into one
    const char* interface = "hw:1,0,0";  // see alsarawportlist.c example program
    int32_t err;

    if ((err = snd_rawmidi_open(&midi_in, NULL, interface, SND_RAWMIDI_NONBLOCK)) < 0) {
        fprintf(stderr, "Problem opening MIDI input: %s\n", snd_strerror(err));
        return 1;
    }
}

int32_t midi_close(void)
{
    snd_rawmidi_close(midi_in);
    midi_in  = NULL;    // snd_rawmidi_close() does not clear invalid pointer,
    return 0;          // so might be a good idea to erase it after closing.
}

int32_t midi_get(midi_message_t *m)
{
    int32_t err;
    uint8_t buffer;

    if ((err = snd_rawmidi_read(midi_in, &buffer, 1)) < 0) {
        return 1;
    }

    // A status message is received if the MSB is set
    if (buffer & 0x80) {
        state = 0;
    }

    switch (state) {
    case 0:
        m->status = buffer;
        state = 1;
        err = 1;

    case 1:
        m->data[0] = buffer;
        state = 2;
        err = 1;
        break;

    case 2:
        m->data[1] = buffer;
        state = 3;
        err = 0;
        break;

    default:
        break;
    }

    return err;
}
