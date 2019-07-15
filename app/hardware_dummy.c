#include "hardware.h"
#include "process.h"

#include <stdio.h>

FILE *output_file;

int32_t hardware_init(void)
{
    output_file = fopen("output.bin", "wb");
    if (output_file == NULL) {
        return 1;
    }
    return 0;
}

int32_t hardware_close(void)
{
    fclose(output_file);
    return 0;
}

int32_t hardware_tick(void)
{
    // Simulate the hardware process with dummy devices

    // Handle input (MIDI)
    // TODO

    // Handle output (DAC)
    if (buffer_idx == BUFFER_SIZE) {
        fwrite(buffer, 2, BUFFER_SIZE, output_file);
        buffer_idx = 0;
    }

    return 0;
}
