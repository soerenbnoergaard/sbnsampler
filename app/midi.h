#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>

typedef struct {
    uint8_t status;
    uint8_t data[2];
} midi_message_t;

int32_t midi_init(void);
int32_t midi_close(void);
midi_message_t midi_get(void);

#endif
