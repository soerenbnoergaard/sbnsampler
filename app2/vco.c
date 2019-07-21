#include "vco.h"

#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>

#define WAV_HEADER_SIZE 44
#define MAX_FILENAME_LENGTH 1024
#define SAMPLE_FILE "samples.csv"

// Midi numbers for each note and octave
//     C   C#  D   D#  E   F   F#  G   G#  A   A#  B  
// 0   12  13  14  15  16  17  18  19  20  21  22  23 
// 1   24  25  26  27  28  29  30  31  32  33  34  35 
// 2   36  37  38  39  40  41  42  43  44  45  46  47 
// 3   48  49  50  51  52  53  54  55  56  57  58  59 
// 4   60  61  62  63  64  65  66  67  68  69  70  71 
// 5   72  73  74  75  76  77  78  79  80  81  82  83 
// 6   84  85  86  87  88  89  90  91  92  93  94  95 
// 7   96  97  98  99  100 101 102 103 104 105 106 107
// 8   108 109 110 111 112 113 114 115 116 117 118 119
// 9   120 121 122 123 124 125 126 127                

// Types ///////////////////////////////////////////////////////////////////////

// Globals /////////////////////////////////////////////////////////////////////
static sample_collection_t sample_collections[NUM_COLLECTIONS];

// Private functions ///////////////////////////////////////////////////////////
static status_t filesize(const char *filename, int32_t *filesize)
{
    struct stat st;

    if (stat(filename, &st) == 0) {
        *filesize = (int32_t)(st.st_size);
        return STATUS_OK;
    }

    return STATUS_ERROR;
}

static status_t load(const char *filename, sample_t *destination)
{
    int32_t size;
    FILE *f;

    // Determine file size and allocate memory
    if (filesize(filename, &size) != STATUS_OK) {
        return STATUS_ERROR;
    }

    size -= WAV_HEADER_SIZE;
    destination->data = (int16_t *)malloc(size);

    if (destination->data == NULL) {
        error("Failed to allocate memory");
        return STATUS_ERROR;
    }

    // Read data
    // WARNING: assuming Little Endian target!
    f = fopen(filename, "rb");
    fseek(f, WAV_HEADER_SIZE, SEEK_SET);
    destination->length = fread(destination->data, sizeof(int16_t), size/2, f);
    fclose(f);

    if (destination->length != size/2) {
        error("Failed to read the whole file");
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

status_t load_from_csv(FILE *input_file, bool parse)
{
    char *line = NULL;
    size_t length = 0;
    int32_t ret;
    int32_t N;
    sample_collection_t *collection;
    sample_t *s;

    // Scanf target
    struct {
        int collection_id;
        int sample_id;
        char filename[MAX_FILENAME_LENGTH];
        int note_root;
        int note_max;
        int note_min;
        int loop_start;
        int loop_stop;
    } record;
    const int32_t record_size = 8;

    ret = getline(&line, &length, input_file);
    if (ret < 0) {
        free(line);
        return STATUS_ERROR;
    }

    if (parse) {
        ret = sscanf(line, "%d,%d,%[^,],%d,%d,%d,%d,%d", 
                     &record.collection_id,
                     &record.sample_id,
                     record.filename,
                     &record.note_root,
                     &record.note_max,
                     &record.note_min,
                     &record.loop_start,
                     &record.loop_stop);
        free(line);

        if (ret == record_size) {
            printf("Loading sample: %s\n", record.filename);

            // Verify inputs
            N = record.collection_id;
            if ((N < 0) || (NUM_COLLECTIONS <= N)) {
                error("Collection ID out of range");
                return STATUS_ERROR;
            }

            N = record.sample_id;
            if ((N < 0) || (NUM_SAMPLES_PER_COLLECTION <= N)) {
                error("Sample ID out of range");
                return STATUS_ERROR;
            }

            // Load sample
            collection = &sample_collections[record.collection_id];
            s = &collection->samples[record.sample_id];

            if (load(record.filename, s) != STATUS_OK) {
                return STATUS_ERROR;
            }

            s->note_root = record.note_root;
            s->note_max = record.note_max;
            s->note_min = record.note_min;
            s->loop_enabled = record.loop_start < record.loop_stop ? true : false;
            s->loop_start = record.loop_start;
            s->loop_stop = record.loop_stop;

            collection->num_samples += 1;
        }
        else {
            error("Unknown format in sample CSV file");
        }
    }

    return STATUS_OK;
}

status_t reset(sample_collection_t *collection)
{
    int32_t n;
    sample_t *s;
    for (n = 0; n < collection->num_samples; n++) {
        s = &collection->samples[n];
        if (s->data != NULL) {
            free(s->data);
        }
        s->data = NULL;
        s->loop_enabled = false;
    }
    collection->num_samples = 0;
    return STATUS_OK;
}

// Public functions ////////////////////////////////////////////////////////////
status_t vco_init(void)
{
    int32_t n;
    FILE *input_file;

    // Reset all sample slots
    for (n = 0; n < NUM_COLLECTIONS; n++) {
        reset(&sample_collections[n]);
    }

    // Load samples from CSV file
    input_file = fopen(SAMPLE_FILE, "r");
    if (input_file == NULL) {
        error("Could not file sample CSV file");
        return STATUS_ERROR;
    }

    // Skip header
    load_from_csv(input_file, false);
    while(load_from_csv(input_file, true) == STATUS_OK) {
        // Load all records from the CSV file
    }

    fclose(input_file);

    return STATUS_OK;
}

status_t vco_close(void)
{
    int32_t n;

    // Free all allocated slots
    for (n = 0; n < NUM_COLLECTIONS; n++) {
        reset(&sample_collections[n]);
    }

    return STATUS_OK;
}

status_t vco_setup(vco_t *vco, uint8_t note)
{
    int32_t n;
    sample_collection_t *collection = &sample_collections[0];
    sample_t *sample;

    // Find sample in sample collection
    for (n = 0; n < collection->num_samples; n++) {
        sample = &collection->samples[n];
        if ((sample->note_min <= note) && (note <= sample->note_max)) {
            break;
        }
        else {
            sample = NULL;
        }
    }

    if (sample == NULL) {
        error("Sample not found in sample collection");
        return STATUS_ERROR;
    }

    // Initialize VCO
    vco->sample = sample;
    vco->sample_index = 0;
    vco->note = note;

    return STATUS_OK;
}

int16_t vco_get_sample(vco_t *vco, status_t *status)
{
    // TODO: Transpose the sample
    // TODO: Loop the sample

    int16_t x;

    if (vco->sample_index < vco->sample->length) {
        *status = STATUS_OK;
        x = vco->sample->data[vco->sample_index];
        vco->sample_index += 1;
    }
    else {
        *status = STATUS_NO_SAMPLES;
        x = 0;
    }

    return x;
}
