#ifndef SAMPLEBANK_H
#define SAMPLEBANK_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "file_helpers.h"
#include "voice_types.h"
#include "samplebank_types.h"

// Defines, macros, and constants //////////////////////////////////////////////

#define SAMPLEBANK_NUM_SAMPLES 128

// Types ///////////////////////////////////////////////////////////////////////

// Globals /////////////////////////////////////////////////////////////////////

extern sample_t samplebank[SAMPLEBANK_NUM_SAMPLES];

// Functions ///////////////////////////////////////////////////////////////////

int32_t samplebank_init(const char *path);
int32_t samplebank_close(void);

#endif
