#include <stdio.h>
#include "utils.h"
#include "voice.h"

static status_t init(void)
{
    if (vco_init() != STATUS_OK) {
        error("Error initializing VCO");
        return STATUS_ERROR;
    }
    if (voice_init() != STATUS_OK) {
        error("Error initializing voices");
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

static status_t close(void)
{
    if (vco_close() != STATUS_OK) {
        error("Error closing VCO");
        return STATUS_ERROR;
    }
    if (voice_close() != STATUS_OK) {
        error("Error closing voices");
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

int main(void)
{
    int32_t i;
    int32_t n;
    int16_t x;
    voice_t *v;

    if (init() != STATUS_OK) {
        return 1;
    }
    return 0;

    // Data path
    for (i = 0; i < 100; i++) {
        x = 0;
        for (n = 0; n < NUM_VOICES; n++) {
            v = voice_get_handle(n);
            x += voice_get_sample(v);
        }
    }

    if (close() != STATUS_OK) {
        return 1;
    }

    return 0;
}
