#include <stdio.h>
#include "utils.h"
#include "vco.h"
#include "voice.h"
#include "dac.h"
#include "midi.h"
#include "ctrl.h"

static int32_t duration;

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
    if (midi_init() != STATUS_OK) {
        error("Error initializing MIDI");
        return STATUS_ERROR;
    }
    if (ctrl_init() != STATUS_OK) {
        error("Error initializing control path");
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
    if (midi_close() != STATUS_OK) {
        error("Error closing MIDI");
        return STATUS_ERROR;
    }
    if (ctrl_close() != STATUS_OK) {
        error("Error closing control path");
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

static status_t run(void)
{
    int32_t i;
    int32_t n;
    int16_t x;
    voice_t *v;
    status_t status;

    // Data path
    for (i = 0; (duration == 0) || (i < duration); i++) {
        x = 0;

        // Control path
        ctrl_tick();

        // Voice data paths
        for (n = 0; n < NUM_VOICES; n++) {
            v = voice_get_handle(n);
            x += voice_get_sample(v);
        }

        // Accumulated data path
        status = dac_write(x);
        if (status != STATUS_OK) {
            error("Error writing to DAC");
            // TODO: Restart DAC if this happens
            break;
        }
    }
    return STATUS_OK;
}

int main(int argc, char *argv[])
{
    // First argument: Number of samples to stop after.
    // If no argument is supplied, run forever.
    if (argc > 1) {
        if (sscanf(argv[1], "%d", &duration) != 1) {
            error("Could not duration based on argument");
        }
        printf("Fixed duration: %d samples\n", duration);
    }
    else {
        duration = 0;
    }

    if (init() != STATUS_OK) {
        return 1;
    }

    if (run() != STATUS_OK) {
        return 1;
    }

    if (close() != STATUS_OK) {
        return 1;
    }

    return 0;
}
