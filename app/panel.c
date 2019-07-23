#include "panel.h"

#define NUM_PARAMETERS 128

// Globals /////////////////////////////////////////////////////////////////////
static uint8_t parameters[NUM_PARAMETERS];

// Private functions ///////////////////////////////////////////////////////////

// Public functions ////////////////////////////////////////////////////////////
status_t panel_init(void)
{
    int32_t n;
    for (n = 0; n < NUM_PARAMETERS; n++) {
        parameters[n] = 0;
    }

    panel_set(PANEL_ENV1_AMOUNT, 127);
    panel_set(PANEL_PRESET, 0);
    panel_set(PANEL_ENABLE_STEALING, 127);

    panel_set(PANEL_CUTOFF, 127);

    panel_set(PANEL_ENV1_SUSTAIN, 127);
    return STATUS_OK;
}

status_t panel_close(void)
{
    return STATUS_OK;
}

status_t panel_set(uint8_t param, uint8_t value)
{
    assert(param < NUM_PARAMETERS);
    parameters[param] = value;
    return STATUS_OK;
}

uint8_t panel_get(uint8_t param)
{
    assert(param < NUM_PARAMETERS);
    return parameters[param];
}
