#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define error(s) fprintf(stderr, "%s\n", s)
#define SAMPLE_RATE_Hz 44100

typedef enum {
    STATUS_OK,
    STATUS_ERROR,
} status_t;


#endif
