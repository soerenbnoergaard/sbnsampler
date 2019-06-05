#include "midi.h"
#include <stdio.h>
#include <alsa/asoundlib.h>

static snd_rawmidi_t* midi_in = NULL;

int32_t midi_init(void)
{
    // TODO: Merge all midi streams into one
    const char* interface = "hw:1,0,0";  // see alsarawportlist.c example program
    int32_t err;

    if ((err = snd_rawmidi_open(&midi_in, NULL, interface, SND_RAWMIDI_SYNC)) < 0) {
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

midi_message_t midi_get(void)
{
    int32_t err;
    int32_t state;
    uint8_t buffer;
    midi_message_t m;

    state = 0;

    while (state < 3) {
        if ((err = snd_rawmidi_read(midi_in, &buffer, 1)) < 0) {
            fprintf(stderr, "Problem reading MIDI input: %s\n", snd_strerror(err));
        }

        switch (state) {
        case 0:
            if (buffer & 0x80) {
                m.status = buffer;
                state = 1;
            }
            break;
        case 1:
            m.data[0] = buffer;
            state = 2;
            break;
        case 2:
            m.data[1] = buffer;
            state = 3;
        default:
            break;
        }
    }

    return m;
}
