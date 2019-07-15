#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "signal.h"
#include "utils.h"

// FIXME: Sustain pedal related note repeat: (1) change to preset 4
// [SOLVED?]
//        (2) hold sustain
//        (3) press D (middle)
//        (4) press C (middle)
//        (5) release D.
//        Issue: After (5), D is restarting
//        This is not very reproducable!

#define MAX_NUM_RETRIES 512

int32_t main(void)
{
    int32_t err;
    int32_t num_fails = 0;

    if (signal_init_memory() != 0) {
        return 1;
    }

    if (signal_init_hardware() != 0) {
        return 1;
    }

    do {
        err = 0;
        err += signal_tick_hardware();
        err += signal_tick_process();

        if (err == 0) {
            continue;
        }

        // Re-initialize hardware if the signal path fails
        num_fails += 1;
        if (signal_close_hardware() != 0) {
            return 1;
        }
        if (signal_init_hardware() != 0) {
            return 1;
        }

    } while (num_fails < MAX_NUM_RETRIES);

    if (signal_close_memory() != 0) {
        return 1;
    }

    return 0;
}
