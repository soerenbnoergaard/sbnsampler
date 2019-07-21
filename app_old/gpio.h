#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>

int32_t gpio_init(void);

int32_t gpio5_set(void);
int32_t gpio5_clear(void);

#endif