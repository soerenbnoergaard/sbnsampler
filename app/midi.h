#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>

#include "midi_types.h"

int32_t midi_init(void);
int32_t midi_close(void);
int32_t midi_get(midi_message_t *m);
int32_t midi_list(void);

#endif
