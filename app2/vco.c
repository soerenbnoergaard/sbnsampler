#include "vco.h"

#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>

#define WAV_HEADER_SIZE 44
#define NUM_SAMPLES 128

// Types ///////////////////////////////////////////////////////////////////////
typedef struct {
    int16_t *data;
    int32_t length;
} sample_t;

// Globals /////////////////////////////////////////////////////////////////////
static sample_t samples[NUM_SAMPLES];

// Private functions ///////////////////////////////////////////////////////////
static status_t filesize(const char *filename, int32_t *filesize)
{
    struct stat st;

    if (stat(filename, &st) == 0) {
        *filesize = (int32_t)(st.st_size);
        return STATUS_OK;
    }

    return STATUS_ERROR;
}

static status_t load(const char *filename, sample_t *destination)
{
    int32_t size;
    FILE *f;

    // Determine file size and allocate memory
    if (filesize(filename, &size) != STATUS_OK) {
        return STATUS_ERROR;
    }

    size -= WAV_HEADER_SIZE;
    destination->data = (int16_t *)malloc(size);

    if (destination->data == NULL) {
        error("Failed to allocate memory");
        return STATUS_ERROR;
    }

    // Read data
    // WARNING: assuming Little Endian target!
    f = fopen(filename, "rb");
    fseek(f, WAV_HEADER_SIZE, SEEK_SET);
    destination->length = fread(destination->data, sizeof(int16_t), size/2, f);
    fclose(f);

    if (destination->length != size/2) {
        error("Failed to read the whole file");
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

// Public functions ////////////////////////////////////////////////////////////
status_t vco_init(void)
{
    int32_t n;

    // Reset all sample slots
    for (n = 0; n < NUM_SAMPLES; n++) {
        samples[n].data = NULL;
        samples[n].length = 0;
    }

    // Load sounds into sample slots
    if (load("../app/sound/volca_c4.wav", &samples[0]) != STATUS_OK) {
        return STATUS_ERROR;
    };

    return STATUS_OK;
}

status_t vco_close(void)
{
    int32_t n;

    // Free all allocated slots
    for (n = 0; n < NUM_SAMPLES; n++) {
        if (samples[n].data != NULL) {
            free(samples[n].data);
            samples[n].data = NULL;
            samples[n].length = 0;
        }
    }

    return STATUS_OK;
}
