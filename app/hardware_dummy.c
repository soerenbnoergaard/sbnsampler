#include "hardware.h"
#include "process.h"

int32_t hardware_init(void)
{
    return 0;
}

int32_t hardware_close(void)
{
    return 0;
}

int32_t hardware_tick(void)
{
    // Simulate the hardware process with dummy devices

    // Handle input (MIDI)
    // TODO

    // Handle output (DAC)
    if (buffer_idx == BUFFER_SIZE) {
        buffer_idx = 0;
    }

    return 0;
}
