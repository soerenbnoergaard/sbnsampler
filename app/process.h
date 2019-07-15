#ifndef PROCESS_H
#define PROCESS_H

// Signal path definitions

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "utils.h"
#include "midi_types.h"
#include "preset.h"

extern preset_t *active_preset;
extern settings_t global; // Settings loaded from the panel (or a preset)
extern int16_t buffer[BUFFER_SIZE]; // Output buffer
extern int32_t buffer_idx;

int32_t process_init(const char *samplebank_path);
int32_t process_close(void);
int32_t process_tick(void);

#endif