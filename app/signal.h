#ifndef SIGNAL_H
#define SIGNAL_H

// Signal path definitions

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int32_t signal_init_memory(void);
int32_t signal_init_hardware(void);

int32_t signal_close_hardware(void);
int32_t signal_close_memory(void);

int32_t signal_tick_hardware(void);
int32_t signal_tick_process(void);

int32_t dummy_tick_hardware(void);
int32_t dummy_note_on(uint8_t note, uint8_t velocity);
int32_t dummy_note_off(uint8_t note);

#endif