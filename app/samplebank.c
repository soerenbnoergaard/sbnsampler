#include "samplebank.h"

// Midi numbers for each note and octave
//     A   A#  B   C   C#  D   D#  E   F   F#  G   G#
// 0   21  22  23  12  13  14  15  16  17  18  19  20
// 1   33  34  35  24  25  26  27  28  29  30  31  32
// 2   45  46  47  36  37  38  39  40  41  42  43  44
// 3   57  58  59  48  49  50  51  52  53  54  55  56
// 4   69  70  71  60  61  62  63  64  65  66  67  68
// 5   81  82  83  72  73  74  75  76  77  78  79  80
// 6   93  94  95  84  85  86  87  88  89  90  91  92
// 7   105 106 107 96  97  98  99  100 101 102 103 104
// 8   117 118 119 108 109 110 111 112 113 114 115 116
// 9               120 121 122 123 124 125 126 127

sample_t samplebank[NUM_SAMPLES];

#define WAV_HEADER_SIZE 44
// #define NAME(note) "sound/piano_"note".wav"
#define NAME(note) "sound/volca_"note".wav"

int32_t samplebank_init(void)
{
    // Malloc is done internally (never free'd!)
    binary_data_t *tmp;

    //
    // VOLCA KEYS
    //

    // C0
    if ((tmp = fh_read_file(NAME("c0"))) == NULL)
        return 1;
    samplebank[0].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[0].length = (tmp->size/2) - WAV_HEADER_SIZE;
    samplebank[0].note_root = 12;
    samplebank[0].note_max = 12;
    samplebank[0].note_min = 0;
    samplebank[0].loop_enabled = true;
    samplebank[0].loop_start = 27195;
    samplebank[0].loop_stop = 71691;

    // C1
    if ((tmp = fh_read_file(NAME("c1"))) == NULL)
        return 1;
    samplebank[1].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[1].length = (tmp->size/2) - WAV_HEADER_SIZE;
    samplebank[1].note_root = 24;
    samplebank[1].note_max = 24;
    samplebank[1].note_min = 13;
    samplebank[1].loop_enabled = true;
    samplebank[1].loop_start = 26606;
    samplebank[1].loop_stop = 71110;

    // C2
    if ((tmp = fh_read_file(NAME("c2"))) == NULL)
        return 1;
    samplebank[2].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[2].length = (tmp->size/2) - WAV_HEADER_SIZE;
    samplebank[2].note_root = 36;
    samplebank[2].note_max = 36;
    samplebank[2].note_min = 25;
    samplebank[2].loop_enabled = true;
    samplebank[2].loop_start = 26713;
    samplebank[2].loop_stop = 70868;

    // C3
    if ((tmp = fh_read_file(NAME("c3"))) == NULL)
        return 1;
    samplebank[3].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[3].length = (tmp->size/2) - WAV_HEADER_SIZE;
    samplebank[3].note_root = 48;
    samplebank[3].note_max = 48;
    samplebank[3].note_min = 37;
    samplebank[3].loop_enabled = true;
    samplebank[3].loop_start = 26592;
    samplebank[3].loop_stop = 70614;

    // C4
    if ((tmp = fh_read_file(NAME("c4"))) == NULL)
        return 1;
    samplebank[4].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[4].length = (tmp->size/2) - WAV_HEADER_SIZE;
    samplebank[4].note_root = 60;
    samplebank[4].note_max = 60;
    samplebank[4].note_min = 49;
    samplebank[4].loop_enabled = true;
    samplebank[4].loop_start = 26538;
    samplebank[4].loop_stop = 70618;

    // C5
    if ((tmp = fh_read_file(NAME("c5"))) == NULL)
        return 1;
    samplebank[5].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[5].length = (tmp->size/2) - WAV_HEADER_SIZE;
    samplebank[5].note_root = 72;
    samplebank[5].note_max = 72;
    samplebank[5].note_min = 61;
    samplebank[5].loop_enabled = true;
    samplebank[5].loop_start = 26481;
    samplebank[5].loop_stop = 70590;

    // C6
    if ((tmp = fh_read_file(NAME("c6"))) == NULL)
        return 1;
    samplebank[6].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[6].length = (tmp->size/2) - WAV_HEADER_SIZE;
    samplebank[6].note_root = 84;
    samplebank[6].note_max = 84;
    samplebank[6].note_min = 73;
    samplebank[6].loop_enabled = true;
    samplebank[6].loop_start = 26477;
    samplebank[6].loop_stop = 70574;

    return 0;
}

