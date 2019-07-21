#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define SAMPLE_RATE_Hz 44100
#define BUFFER_SIZE 128

#define error(s) fprintf(stderr, "%s\n", (s))
#define here(s) fprintf(stdout, "%s:%d %s\n", __FILE__, __LINE__, s)

typedef enum {
    STATUS_OK,
    STATUS_ERROR,
    STATUS_NOT_READY,
    STATUS_NO_SAMPLES
} status_t;


#endif
