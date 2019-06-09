#ifndef POLYFILTER_H
#define POLYFILTER_H

#include <stdint.h>
#include <stdbool.h>

#include "voice.h"
#include "polyfilter_coeffs.h"

int16_t ppf_get_transposed_sample(voice_t *v);

#endif
