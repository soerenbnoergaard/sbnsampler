#ifndef PRESET_H
#define PRESET_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "preset_types.h"

#define NUM_PRESETS 5

extern preset_t presets[NUM_PRESETS];

int32_t preset_init(const char *path);

#endif