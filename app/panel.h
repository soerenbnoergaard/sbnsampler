#ifndef PANEL_H
#define PANEL_H

#include "utils.h"

// General
#define PANEL_PRESET 0x50
#define PANEL_ENABLE_STEALING 0x51
#define PANEL_SUSTAIN_PEDAL 0x40

// VCF
#define PANEL_CUTOFF 0x0a
#define PANEL_RESONANCE 0x47

// Envelope 1
#define PANEL_ENV1_AMOUNT 0x46
#define PANEL_ENV1_VELOCITY 0x14 // Not mapped
#define PANEL_ENV1_ATTACK 0x53
#define PANEL_ENV1_DECAY 0x59
#define PANEL_ENV1_SUSTAIN 0x55
#define PANEL_ENV1_RELEASE 0x52

// Envelope 2
#define PANEL_ENV2_AMOUNT 0x13
#define PANEL_ENV2_VELOCITY 0x4a
#define PANEL_ENV2_ATTACK 0x49
#define PANEL_ENV2_DECAY 0x4f
#define PANEL_ENV2_SUSTAIN 0x4b
#define PANEL_ENV2_RELEASE 0x48

status_t panel_init(void);
status_t panel_close(void);
status_t panel_set(uint8_t param, uint8_t value);
uint8_t panel_get(uint8_t param);

#endif
