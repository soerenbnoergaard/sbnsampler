#ifndef ADSR_H
#define ADSR_H

#include <stdint.h>
#include "adsr_types.h"

int32_t adsr_start(adsr_t *adsr, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release);
int32_t adsr_stop(adsr_t *adsr);
int32_t adsr_update(adsr_t *adsr);

#endif
