#ifndef CTRL_H
#define CTRL_H

#include "utils.h"
#include "voice.h"

#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CC 0xb0

status_t ctrl_init(void);
status_t ctrl_close(void);
status_t ctrl_tick(void);
status_t ctrl_voice_tick(voice_t *v);

#endif
