#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define SAMPLE_RATE_Hz 44100
#define BUFFER_SIZE 128

#define error(s) fprintf(stderr, "%s\n", (s))

typedef enum {
    STATUS_OK,
    STATUS_ERROR,
} status_t;


#endif
