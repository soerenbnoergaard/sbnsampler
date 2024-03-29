#include "dac.h"

#define WAV_HEADER_SIZE 44
#define WAV_HEADER_SUBCHUNK2SIZE_POSITION 40
const int8_t wav_header_44100_mono_16le[WAV_HEADER_SIZE] = {
    0x52, 0x49, 0x46, 0x46, 0x24, 0xB0, 0x06, 0x00,
    0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20,
    0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
    0x44, 0xAC, 0x00, 0x00, 0x88, 0x58, 0x01, 0x00,
    0x02, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61,
    0x00, 0xB0, 0x06, 0x00
};

// Globals /////////////////////////////////////////////////////////////////////
static FILE *output_file;
static uint32_t subchunk_2_size;
static int16_t buffer[BUFFER_SIZE];
static int32_t buffer_index;

// Private functions ///////////////////////////////////////////////////////////
status_t write_buffer(void)
{
    if (output_file == NULL) {
        return STATUS_ERROR;
    }
    fwrite(buffer, sizeof(int16_t), BUFFER_SIZE, output_file);
    subchunk_2_size += sizeof(int16_t)*BUFFER_SIZE;
    return STATUS_OK;
}

// Public functions ////////////////////////////////////////////////////////////
status_t dac_init(const char *interface, uint32_t sample_rate_Hz)
{
    subchunk_2_size = 0;
    output_file = fopen("dac.wav", "wb");
    if (output_file == NULL) {
        return STATUS_ERROR;
    }
    fwrite(wav_header_44100_mono_16le, sizeof(uint8_t), WAV_HEADER_SIZE, output_file);

    buffer_index = 0;
    return STATUS_OK;
}

status_t dac_close(void)
{
    uint8_t subchunk_2_size_le[4] = {
        (subchunk_2_size)       & 0xff,
        (subchunk_2_size >> 8)  & 0xff,
        (subchunk_2_size >> 16) & 0xff,
        (subchunk_2_size >> 24) & 0xff,
    };

    // Write subchunk size to header before closing file
    fseek(output_file, WAV_HEADER_SUBCHUNK2SIZE_POSITION, SEEK_SET);
    fwrite(subchunk_2_size_le, sizeof(uint8_t), 4, output_file);
    fclose(output_file);

    return STATUS_OK;
}

status_t dac_write(int16_t sample)
{
    status_t status = STATUS_OK;
    buffer[buffer_index] = sample;
    buffer_index += 1;
    if (buffer_index >= BUFFER_SIZE) {
        status = write_buffer();
        buffer_index = 0;
    }
    return status;
}

