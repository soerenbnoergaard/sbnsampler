#include <stdio.h>
#include "utils.h"
#include "vco.h"
#include "voice.h"
#include "dac.h"
#include "midi.h"
#include "gpio.h"
#include "ctrl.h"
#include "vca.h"

// Globals /////////////////////////////////////////////////////////////////////
static int32_t duration;
static bool simulation;

// Private functions ///////////////////////////////////////////////////////////
static status_t init_on_crash(void)
{
    if (dac_init("default", SAMPLE_RATE_Hz) != STATUS_OK) {
        error("Error initializing DAC");
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

static status_t close_on_crash(void)
{
    if (dac_close() != STATUS_OK) {
        error("Error closing DAC");
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

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
    if (midi_init() != STATUS_OK) {
        error("Error initializing MIDI");
        return STATUS_ERROR;
    }
    if (gpio_init() != STATUS_OK) {
        error("Error initializing GPIO");
        return STATUS_ERROR;
    }
    if (ctrl_init() != STATUS_OK) {
        error("Error initializing control path");
        return STATUS_ERROR;
    }

    if (init_on_crash() != STATUS_OK) {
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
    if (midi_close() != STATUS_OK) {
        error("Error closing MIDI");
        return STATUS_ERROR;
    }
    if (gpio_close() != STATUS_OK) {
        error("Error closing GPIO");
        return STATUS_ERROR;
    }
    if (ctrl_close() != STATUS_OK) {
        error("Error closing control path");
        return STATUS_ERROR;
    }

    if (close_on_crash() != STATUS_OK) {
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

static status_t run(void)
{
    int32_t i;
    int32_t n;
    int16_t x;
    int16_t y;
    voice_t *v;
    status_t status;

    // Data path
    for (i = 0; (!simulation) || (i < duration); i++) {
        y = 0;

        // Control path
        ctrl_tick();

        // Voice data paths
        for (n = 0; n < NUM_VOICES; n++) {
            v = voice_get_handle(n);

            x = voice_get_sample(v);
            x = vca(x, 31);
            y += x;
        }

        // Accumulated data path
        status = dac_write(y);
        if (status != STATUS_OK) {
            return STATUS_ERROR;
        }
    }
    return STATUS_OK;
}

// Public functions ////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    // First argument: Number of samples to stop after.
    // If no argument is supplied, run forever.
    if (argc > 1) {
        if (sscanf(argv[1], "%d", &duration) != 1) {
            error("Could not duration based on argument");
            return 1;
        }
        printf("Simulation: %d samples\n", duration);
        simulation = true;
    }
    else {
        duration = 0;
        simulation = false;
    }

    if (init() != STATUS_OK) {
        return 1;
    }

    // Do crash handling when running on hardware.
    // Exit directly when running as simulation.
    do {
        if (run() != STATUS_OK) {
            close_on_crash();
            init_on_crash();
        }
    } while(!simulation);

    if (close() != STATUS_OK) {
        return 1;
    }

    return 0;
}
