#ifndef MIDI_H
#define MIDI_H

#include "utils.h"

typedef struct {
    uint8_t status;
    uint8_t data[2];
} midi_message_t;

status_t midi_init(void);
status_t midi_close(void);
status_t midi_get(midi_message_t *m);
status_t midi_input(uint8_t status, uint8_t data0, uint8_t data1);

#endif
