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

sample_t samplebank[SAMPLEBANK_NUM_SAMPLES];

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
    if ((tmp = fh_read_file("sound/volca_c0.wav")) == NULL)
        return 1;
    samplebank[0].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[0].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[0].note_root = 12;
    samplebank[0].note_max = 12;
    samplebank[0].note_min = 0;
    samplebank[0].loop_enabled = true;
    samplebank[0].loop_start = 27195;
    samplebank[0].loop_stop = 71691;

    // C1
    if ((tmp = fh_read_file("sound/volca_c1.wav")) == NULL)
        return 1;
    samplebank[1].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[1].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[1].note_root = 24;
    samplebank[1].note_max = 24;
    samplebank[1].note_min = 13;
    samplebank[1].loop_enabled = true;
    samplebank[1].loop_start = 26606;
    samplebank[1].loop_stop = 71110;

    // C2
    if ((tmp = fh_read_file("sound/volca_c2.wav")) == NULL)
        return 1;
    samplebank[2].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[2].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[2].note_root = 36;
    samplebank[2].note_max = 36;
    samplebank[2].note_min = 25;
    samplebank[2].loop_enabled = true;
    samplebank[2].loop_start = 26713;
    samplebank[2].loop_stop = 70868;

    // C3
    if ((tmp = fh_read_file("sound/volca_c3.wav")) == NULL)
        return 1;
    samplebank[3].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[3].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[3].note_root = 48;
    samplebank[3].note_max = 48;
    samplebank[3].note_min = 37;
    samplebank[3].loop_enabled = true;
    samplebank[3].loop_start = 26592;
    samplebank[3].loop_stop = 70614;

    // C4
    if ((tmp = fh_read_file("sound/volca_c4.wav")) == NULL)
        return 1;
    samplebank[4].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[4].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[4].note_root = 60;
    samplebank[4].note_max = 60;
    samplebank[4].note_min = 49;
    samplebank[4].loop_enabled = true;
    samplebank[4].loop_start = 26538;
    samplebank[4].loop_stop = 70618;

    // C5
    if ((tmp = fh_read_file("sound/volca_c5.wav")) == NULL)
        return 1;
    samplebank[5].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[5].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[5].note_root = 72;
    samplebank[5].note_max = 72;
    samplebank[5].note_min = 61;
    samplebank[5].loop_enabled = true;
    samplebank[5].loop_start = 26481;
    samplebank[5].loop_stop = 70590;

    // C6
    if ((tmp = fh_read_file("sound/volca_c6.wav")) == NULL)
        return 1;
    samplebank[6].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[6].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[6].note_root = 84;
    samplebank[6].note_max = 84;
    samplebank[6].note_min = 73;
    samplebank[6].loop_enabled = true;
    samplebank[6].loop_start = 26477;
    samplebank[6].loop_stop = 70574;

    //
    // Piano
    //

    // C0
    if ((tmp = fh_read_file("sound/piano_c0.wav")) == NULL)
        return 1;
    samplebank[7].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[7].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[7].note_root = 12;
    samplebank[7].note_max = 12;
    samplebank[7].note_min = 0;
    samplebank[7].loop_enabled = false;

    // C1
    if ((tmp = fh_read_file("sound/piano_c1.wav")) == NULL)
        return 1;
    samplebank[8].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[8].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[8].note_root = 24;
    samplebank[8].note_max = 24;
    samplebank[8].note_min = 13;
    samplebank[8].loop_enabled = false;

    // C2
    if ((tmp = fh_read_file("sound/piano_c2.wav")) == NULL)
        return 1;
    samplebank[9].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[9].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[9].note_root = 36;
    samplebank[9].note_max = 36;
    samplebank[9].note_min = 25;
    samplebank[9].loop_enabled = false;

    // C3
    if ((tmp = fh_read_file("sound/piano_c3.wav")) == NULL)
        return 1;
    samplebank[10].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[10].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[10].note_root = 48;
    samplebank[10].note_max = 48;
    samplebank[10].note_min = 37;
    samplebank[10].loop_enabled = false;

    // C4
    if ((tmp = fh_read_file("sound/piano_c4.wav")) == NULL)
        return 1;
    samplebank[11].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[11].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[11].note_root = 60;
    samplebank[11].note_max = 60;
    samplebank[11].note_min = 49;
    samplebank[11].loop_enabled = false;

    // C5
    if ((tmp = fh_read_file("sound/piano_c5.wav")) == NULL)
        return 1;
    samplebank[12].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[12].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[12].note_root = 72;
    samplebank[12].note_max = 72;
    samplebank[12].note_min = 61;
    samplebank[12].loop_enabled = false;

    // C6
    if ((tmp = fh_read_file("sound/piano_c6.wav")) == NULL)
        return 1;
    samplebank[13].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[13].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[13].note_root = 84;
    samplebank[13].note_max = 84;
    samplebank[13].note_min = 73;
    samplebank[13].loop_enabled = false;

    // C7
    if ((tmp = fh_read_file("sound/piano_c7.wav")) == NULL)
        return 1;
    samplebank[14].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[14].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[14].note_root = 96;
    samplebank[14].note_max = 96;
    samplebank[14].note_min = 85;
    samplebank[14].loop_enabled = false;

    //
    // Precision bass
    //

    // E1
    if ((tmp = fh_read_file("sound/pbass_e1.wav")) == NULL)
        return 1;
    samplebank[15].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[15].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[15].note_root = 28;
    samplebank[15].note_max = 28;
    samplebank[15].note_min = 0;
    samplebank[15].loop_enabled = false;

    // A1
    if ((tmp = fh_read_file("sound/pbass_a1.wav")) == NULL)
        return 1;
    samplebank[16].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[16].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[16].note_root = 33;
    samplebank[16].note_max = 33;
    samplebank[16].note_min = 29;
    samplebank[16].loop_enabled = false;

    // D2
    if ((tmp = fh_read_file("sound/pbass_d2.wav")) == NULL)
        return 1;
    samplebank[17].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[17].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[17].note_root = 38;
    samplebank[17].note_max = 38;
    samplebank[17].note_min = 34;
    samplebank[17].loop_enabled = false;

    // G2
    if ((tmp = fh_read_file("sound/pbass_g2.wav")) == NULL)
        return 1;
    samplebank[18].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[18].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[18].note_root = 43;
    samplebank[18].note_max = 43;
    samplebank[18].note_min = 39;
    samplebank[18].loop_enabled = false;

    // C3
    if ((tmp = fh_read_file("sound/pbass_c3.wav")) == NULL)
        return 1;
    samplebank[19].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[19].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[19].note_root = 48;
    samplebank[19].note_max = 48;
    samplebank[19].note_min = 44;
    samplebank[19].loop_enabled = false;

    // F3
    if ((tmp = fh_read_file("sound/pbass_f3.wav")) == NULL)
        return 1;
    samplebank[20].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[20].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[20].note_root = 53;
    samplebank[20].note_max = 53;
    samplebank[20].note_min = 49;
    samplebank[20].loop_enabled = false;

    // Bb3
    if ((tmp = fh_read_file("sound/pbass_bb3.wav")) == NULL)
        return 1;
    samplebank[21].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[21].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[21].note_root = 58;
    samplebank[21].note_max = 58;
    samplebank[21].note_min = 54;
    samplebank[21].loop_enabled = false;

    // Eb4
    if ((tmp = fh_read_file("sound/pbass_eb4.wav")) == NULL)
        return 1;
    samplebank[22].data = tmp->data + WAV_HEADER_SIZE;
    samplebank[22].length = (tmp->size - WAV_HEADER_SIZE)/2;
    samplebank[22].note_root = 63;
    samplebank[22].note_max = 63;
    samplebank[22].note_min = 59;
    samplebank[22].loop_enabled = false;


    return 0;
}

