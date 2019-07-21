#ifndef GPIO_H
#define GPIO_H

#include "utils.h"

status_t gpio_init(void);
status_t gpio_close(void);

status_t gpio5_set(void);
status_t gpio5_clear(void);

#endif
