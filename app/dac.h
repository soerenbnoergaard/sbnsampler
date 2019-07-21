#ifndef DAC_H
#define DAC_H

#include "utils.h"

status_t dac_init(const char *interface, uint32_t sample_rate_Hz);
status_t dac_close(void);
status_t dac_write(int16_t sample);

#endif
