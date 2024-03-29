#include "midi.h"

#define MAX_SIMULATION_LENGTH 30*SAMPLE_RATE_Hz

// Globals /////////////////////////////////////////////////////////////////////
static uint32_t prev_sample_counter;
static uint32_t sample_counter;
static FILE *input_file;
static struct {
    uint32_t time;
    uint8_t status;
    uint8_t data[2];
} record;

// Private functions ///////////////////////////////////////////////////////////

static status_t fetch(bool parse)
{
    char *line = NULL;
    size_t length = 0;
    int32_t ret;
    int tmp[4];

    ret = getline(&line, &length, input_file);
    if (ret < 0) {
        free(line);
        return STATUS_ERROR;
    }

    if (parse) {
        if (line[0] == '#') {
            free(line);
            // Ignore comment and fetch new line
            return fetch(parse);
        }

        ret = sscanf(line, "%d,%d,%d,%d", &tmp[0], &tmp[1], &tmp[2], &tmp[3]);
        if (ret == 4) {
            record.time = tmp[0];
            record.status = tmp[1];
            record.data[0] = tmp[2];
            record.data[1] = tmp[3];
        }
    }

    free(line);
    return STATUS_OK;
}

// Public functions ////////////////////////////////////////////////////////////
status_t midi_init(void)
{
    sample_counter = 0;

    input_file = fopen("midi.csv", "r");
    if (input_file == NULL) {
        return STATUS_ERROR;
    }

    // Skip header and read first midi message
    fetch(false);
    fetch(true);

    return STATUS_OK;
}

status_t midi_close(void)
{
    sample_counter = 0;
    fclose(input_file);
    return STATUS_OK;
}

status_t midi_get(midi_message_t *m)
{
    status_t status = STATUS_NOT_READY;

    if (prev_sample_counter <= record.time && record.time <= sample_counter) {
        prev_sample_counter = sample_counter;

        m->status = record.status;
        m->data[0] = record.data[0];
        m->data[1] = record.data[1];
        fetch(true);
        status = STATUS_OK;
    }

    sample_counter += CTRL_PRESCALER;

    if (sample_counter > MAX_SIMULATION_LENGTH) {
        error("Exceeded maximum simulation length");
        exit(1);
    }
    return status;
}

