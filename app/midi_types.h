#ifndef MIDI_TYPES_H
#define MIDI_TYPES_H

typedef struct {
    uint8_t status;
    uint8_t data[2];
} midi_message_t;

#endif