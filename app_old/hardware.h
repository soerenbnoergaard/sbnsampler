#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int32_t hardware_init(void);
int32_t hardware_close(void);
int32_t hardware_tick(void);

#endif