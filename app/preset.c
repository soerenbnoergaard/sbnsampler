#include "preset.h"
#include "panel.h"
#include "ctrl.h"

#define INPUT_FILE "presets.csv"

// Globals /////////////////////////////////////////////////////////////////////
typedef struct {
    uint8_t id;
    uint8_t status;
    uint8_t data[2];
} record_t;

// Private functions ///////////////////////////////////////////////////////////
static status_t fetch(FILE *input_file, bool parse, record_t *output)
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
            // Ignore comment and fetch new line
            free(line);
            return fetch(input_file, parse, output);
        }

        ret = sscanf(line, "%d,%d,%d,%d", &tmp[0], &tmp[1], &tmp[2], &tmp[3]);
        if (ret == 4) {
            output->id = tmp[0];
            output->status = tmp[1];
            output->data[0] = tmp[2];
            output->data[1] = tmp[3];
        }
    }

    free(line);
    return STATUS_OK;
}

// Public functions ////////////////////////////////////////////////////////////
status_t preset_load(uint8_t id)
{
    FILE *input_file = NULL;
    status_t status;
    record_t record;

    input_file = fopen(INPUT_FILE, "r");
    if (input_file == NULL) {
        error("Preset file not found");
        goto bad_exit;
    }
    else {
        printf("Loading preset %d\n", id);
    }

    // Skip header
    status = fetch(input_file, false, &record);
    if (status != STATUS_OK) {
        goto bad_exit;
    }

    // Set all panel parameters matching the ID.
    do {
        status = fetch(input_file, true, &record);
        if (record.id == id) {
            if (record.status == MIDI_CC) {
                panel_set(record.data[0], record.data[1]);
            }
        }
    } while (status == STATUS_OK);

    fclose(input_file);
    return STATUS_OK;

bad_exit:
    if (input_file != NULL) {
        fclose(input_file);
    }
    return STATUS_ERROR;
}


