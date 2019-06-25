#ifndef PRESET_H
#define PRESET_H

#include <stdint.h>
#include <stdbool.h>
#include "preset_types.h"

#define NUM_PRESETS 1

extern preset_t presets[NUM_PRESETS];

int32_t preset_init(void);

#endif