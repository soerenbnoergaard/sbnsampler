#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <alsa/asoundlib.h>

#include "samplebank.h"

// Types ///////////////////////////////////////////////////////////////////////

// Defines, macros, and constants //////////////////////////////////////////////

#define SAMPLE_RATE_Hz 44100
#define BUFFER_SIZE 128

// Globals /////////////////////////////////////////////////////////////////////

static snd_pcm_t *playback_handle;
static snd_pcm_hw_params_t *hw_params;

// Functions ///////////////////////////////////////////////////////////////////

int32_t dac_init(const char *interface)
{
    int32_t err;
    int32_t sample_rate_Hz = SAMPLE_RATE_Hz;

    if ((err = snd_pcm_open(&playback_handle, interface, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        fprintf(stderr, "cannot open audio device %s (%s)\n", 
                 interface,
                 snd_strerror(err));
        return 1;
    }

    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",
                 snd_strerror(err));
        return 1;
    }

    if ((err = snd_pcm_hw_params_any(playback_handle, hw_params)) < 0) {
        fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n",
                 snd_strerror(err));
        return 1;
    }

    if ((err = snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf(stderr, "cannot set access type (%s)\n",
                 snd_strerror(err));
        return 1;
    }

    if ((err = snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
        fprintf(stderr, "cannot set sample format (%s)\n",
                 snd_strerror(err));
        return 1;
    }

    if ((err = snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &sample_rate_Hz, 0)) < 0) {
        fprintf(stderr, "cannot set sample rate (%s)\n",
                 snd_strerror(err));
        return 1;
    }

    if ((err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, 1)) < 0) {
        fprintf(stderr, "cannot set channel count (%s)\n",
                 snd_strerror(err));
        return 1;
    }

    if ((err = snd_pcm_hw_params(playback_handle, hw_params)) < 0) {
        fprintf(stderr, "cannot set parameters (%s)\n",
                 snd_strerror(err));
        return 1;
    }

    snd_pcm_hw_params_free(hw_params);

    if ((err = snd_pcm_prepare(playback_handle)) < 0) {
        fprintf(stderr, "cannot prepare audio interface for use (%s)\n",
                 snd_strerror(err));
        return 1;
    }

    return 0;
}

int32_t dac_close(void)
{
    return snd_pcm_close(playback_handle);
}

// Interrupts //////////////////////////////////////////////////////////////////

// Main ////////////////////////////////////////////////////////////////////////

int32_t main()
{
    int32_t err;
    int32_t i;
    int32_t n;
    int16_t buffer[BUFFER_SIZE];

    int16_t *x;
    int32_t x_length;

    // Initialize DAC
    err = dac_init("default");
    if (err != 0)
        return 1;

    // Play sound
    x = sample.start;
    x_length = sample.end - sample.start;

    for (i = 0; i < x_length - BUFFER_SIZE; i += BUFFER_SIZE) {

        // Fill buffer
        for (n = 0; n < BUFFER_SIZE; n++) {
            buffer[n] = x[i+n];
        }

        // Output to DAC
        if ((err = snd_pcm_writei(playback_handle, buffer, BUFFER_SIZE)) != BUFFER_SIZE) {
            fprintf(stderr, "write to audio interface failed (%s)\n",
                     snd_strerror(err));
            return 1;
        }
    }

    // Close DAC
    err = dac_close();
    if (err != 0)
        return 1;

    return 0;
}
