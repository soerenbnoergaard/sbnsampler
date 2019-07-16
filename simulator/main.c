#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "utils.h"
#include "hardware.h"
#include "process.h"

#define MAX_NUM_RETRIES 512

static int32_t simulation_length = 0;

int32_t sim_add_midi(int32_t position, uint8_t status, uint8_t data0, uint8_t data1)
{
    return 0;
}

int32_t sim_init(void)
{
    if (process_init("../app/sound") != 0) {
        return 1;
    }
    return 0;
}

int32_t sim_set_length(int32_t length)
{
    simulation_length = length;
    return 0;
}

int32_t sim_close(void)
{
    // FIXME: Samplebank is not free'd correctly!
    if (process_close() != 0) {
        return 1;
    }
    return 0;
}

int32_t sim_run(void)
{
    int32_t err;
    int32_t num_fails = 0;
    int32_t num_ticks = 0;

    if (hardware_init() != 0) {
        return 1;
    }

    do {
        err = 0;
        err += hardware_tick();
        err += process_tick();

        if (num_ticks++ > simulation_length) {
            break;
        }

        if (err == 0) {
            continue;
        }

        // Re-initialize hardware if the signal path fails
        num_fails += 1;
        if (hardware_close() != 0) {
            return 1;
        }
        if (hardware_init() != 0) {
            return 1;
        }

    } while (num_fails < MAX_NUM_RETRIES);
    return 0;
}

int32_t main(void)
{
    if (sim_init() != 0) {
        return 1;
    }

    if (sim_set_length(44100 * 5) != 0) {
        return 1;
    }

    if (sim_run() != 0) {
        return 1;
    }

    return 0;
}
