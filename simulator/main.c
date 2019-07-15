#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "utils.h"
#include "hardware.h"
#include "process.h"

#define MAX_NUM_RETRIES 512

int32_t main(void)
{
    int32_t err;
    int32_t num_fails = 0;

    if (process_init("../app/sound") != 0) {
        return 1;
    }

    if (hardware_init() != 0) {
        return 1;
    }

    do {
        err = 0;
        err += hardware_tick();
        err += process_tick();

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

    if (process_close() != 0) {
        return 1;
    }

    return 0;
}
