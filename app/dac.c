#include "dac.h"
#include <alsa/asoundlib.h>

static snd_pcm_t *playback_handle;
static snd_pcm_hw_params_t *hw_params;

int32_t dac_init(const char *interface, uint32_t sample_rate_Hz)
{
    // https://users.suse.com/~mana/alsa090_howto.html#sect02
    int32_t err;
    uint32_t channels = 1;
    uint32_t periods = 2;
    uint32_t periodsize = 256;

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

    if ((err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, channels)) < 0) {
        fprintf(stderr, "cannot set channel count (%s)\n",
                 snd_strerror(err));
        return 1;
    }

    if ((err = snd_pcm_hw_params_set_periods(playback_handle, hw_params, periods, 0)) < 0) {
        fprintf(stderr, "cannot set periods (%s)\n",
                 snd_strerror(err));
        return 1;
    }

    // latency = periodsize * periods / (rate * bytes_per_frame)
    if ((err = snd_pcm_hw_params_set_buffer_size(playback_handle, hw_params, (periodsize * periods)>>channels)) < 0) {
        fprintf(stderr, "cannot set buffer size (%s)\n",
                 snd_strerror(err));
        return 1;
    }

    // Apply all parameters
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
    snd_pcm_close(playback_handle);
    playback_handle = NULL;
    return 0;
}

int32_t dac_write(int16_t *buffer, uint32_t buffer_size)
{
    int32_t err;

    if ((err = snd_pcm_writei(playback_handle, buffer, buffer_size)) != buffer_size) {
        fprintf(stderr, "write to audio interface failed (%s)\n",
                 snd_strerror(err));
        return 1;
    }

    return 0;
}
