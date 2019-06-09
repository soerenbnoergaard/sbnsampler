#ifndef VOICE_H
#define VOICE_H

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "voice_types.h"

// Defines
#define NUM_VOICES 8

// Global variables (initialized elsewhere)
extern voice_t voices[NUM_VOICES];
extern voice_t voice_default;

// Prototypes
int32_t voice_init(void);
int32_t voice_reset(voice_t *v);

#endif