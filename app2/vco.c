#include "vco.h"

#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>

#define WAV_HEADER_SIZE 44

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
static vco_t oscillators[NUM_OSCILLATORS];

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

    puts("HERE1");

    size -= WAV_HEADER_SIZE;
    destination->data = (int16_t *)malloc(size);

    puts("HERE2");

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

status_t reset(vco_t *osc)
{
    int32_t n;
    sample_t *s;
    for (n = 0; n < osc->num_samples; n++) {
        s = &osc->samples[n];
        if (s->data != NULL) {
            free(s->data);
        }
        s->data = NULL;
        s->loop_enabled = false;
    }
    return STATUS_OK;
}

static status_t load_vco_volca(void)
{
    vco_t *vco = &vco[0];
    sample_t *s;

    s = &vco->samples[0]; 
    if (load("../app/sound/volca_c4.wav", s) != STATUS_OK) {
        return STATUS_ERROR;
    }
    s->note_root = 60;
    s->note_max = 60;
    s->note_min = 49;
    s->loop_enabled = true;
    s->loop_start = 26538;
    s->loop_stop = 70618;
    return STATUS_OK;
}

// Public functions ////////////////////////////////////////////////////////////
status_t vco_init(void)
{
    int32_t n;

    // Reset all sample slots
    for (n = 0; n < NUM_OSCILLATORS; n++) {
        reset(&oscillators[n]);
    }

    if (load_vco_volca() != STATUS_OK) {
        return STATUS_ERROR;
    };

    return STATUS_OK;
}

status_t vco_close(void)
{
    int32_t n;

    // Free all allocated slots
    for (n = 0; n < NUM_OSCILLATORS; n++) {
        reset(&oscillators[n]);
    }

    return STATUS_OK;
}

vco_t *vco_get_handle(int32_t n)
{
    assert((0 <= n) && (n < NUM_OSCILLATORS));
    return &oscillators[n];
}

int16_t vco_get_sample(vco_t *osc)
{
    return 0;
}
