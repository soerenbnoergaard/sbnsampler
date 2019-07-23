#ifndef CTRL_H
#define CTRL_H

#include "utils.h"
#include "voice.h"

status_t ctrl_init(void);
status_t ctrl_close(void);
status_t ctrl_tick(void);
status_t ctrl_voice_tick(voice_t *v);

#endif
