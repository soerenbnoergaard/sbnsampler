#include "dac.h"
#include "utils.h"
#include <stdio.h>

static FILE *output_file;

int32_t dac_init(const char *interface, uint32_t sample_rate_Hz)
{
    output_file = fopen("output.bin", "wb");
    if (output_file == NULL) {
        return 1;
    }
    return 0;
}

int32_t dac_close(void)
{
    fclose(output_file);
    return 0;
}

int32_t dac_write(int16_t *buffer, uint32_t buffer_size)
{
    if (output_file == NULL) {
        return 1;
    }
    fwrite(buffer, sizeof(buffer), sizeof(int16_t), output_file);
    return 0;
}

