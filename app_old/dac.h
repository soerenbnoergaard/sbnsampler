#ifndef DAC_H
#define DAC_H

#include <stdint.h>

int32_t dac_init(const char *interface, uint32_t sample_rate_Hz);
int32_t dac_close(void);
int32_t dac_write(int16_t *buffer, uint32_t buffer_size);

#endif
