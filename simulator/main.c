#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "utils.h"
#include "hardware.h"
#include "process.h"

int32_t sim_init(void)
{
    if (process_init("../app/sound") != 0) {
        return 1;
    }

    if (hardware_init() != 0) {
        return 1;
    }

    return 0;
}

int32_t sim_close(void)
{
    // FIXME: Samplebank is not free'd correctly!
    if (process_close() != 0) {
        return 1;
    }

    if (hardware_close() != 0) {
        return 1;
    }

    return 0;
}

int32_t sim_tick(void)
{
    int32_t err;
    err = 0;
    err += hardware_tick();
    err += process_tick();
    return err;
}