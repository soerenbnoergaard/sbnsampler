#include <stdio.h>
#include "utils.h"
#include "voice.h"
#include "dac.h"

static status_t init(void)
{
    if (vco_init() != STATUS_OK) {
        error("Error initializing VCO");
        return STATUS_ERROR;
    }
    if (voice_init() != STATUS_OK) {
        error("Error initializing voice");
        return STATUS_ERROR;
    }
    if (dac_init("default", SAMPLE_RATE_Hz) != STATUS_OK) {
        error("Error initializing DAC");
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
        error("Error closing voice");
        return STATUS_ERROR;
    }
    if (dac_close() != STATUS_OK) {
        error("Error closing DAC");
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
    status_t st;

    if (init() != STATUS_OK) {
        return 1;
    }

    // Data path
    for (i = 0; i < 1000; i++) {
        x = 0;

        // Voice data paths
        for (n = 0; n < NUM_VOICES; n++) {
            v = voice_get_handle(n);
            x += voice_get_sample(v);
        }

        // Accumulated data path
        st = dac_write(x);
        if (st != STATUS_OK) {
            error("Error writing to DAC");
            break;
        }
    }

    if (close() != STATUS_OK) {
        return 1;
    }

    return 0;
}
