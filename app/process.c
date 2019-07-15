#include "process.h"
#include "utils.h"
#include "preset.h"
#include "voice.h"
#include "polyfilter.h"
#include "vcf.h"
#include "adsr.h"

// Types ///////////////////////////////////////////////////////////////////////

// Defines, macros, and constants //////////////////////////////////////////////

#define VOICE_AMPLITUDE 0.2

// Globals /////////////////////////////////////////////////////////////////////

preset_t *active_preset = &presets[2];
settings_t global;
int16_t buffer[BUFFER_SIZE];
int32_t buffer_idx = 0;

// Private functions ///////////////////////////////////////////////////////////

// Public functions ////////////////////////////////////////////////////////////

int32_t process_init(const char *samplebank_path)
{
    int32_t err;
    err = preset_init(samplebank_path);
    if (err != 0) {
        fprintf(stderr, "Error initializing Preset\n");
        return 1;
    }

    err = voice_init();
    if (err != 0) {
        fprintf(stderr, "Error initializing Voices\n");
        return 1;
    }

    return 0;
}

int32_t process_close(void)
{
    // TODO: Consider freeing the sound bank here.
    return 0;
}

int32_t process_tick(void)
{
    float A;
    float x;
    float y;

    int32_t n;
    voice_t *v;

    // Generate audio from voices
    y = 0;

    for (n = 0; n < NUM_VOICES; n++) {
        v = &voices[n];

        //
        // VOICE STATE MACHINE
        //

        switch (v->state) {
        case VOICE_STATE_IDLE:
            v->amplitude_envelope.state = ADSR_STATE_IDLE;
            voices[n].settings = global;
            continue;

        case VOICE_STATE_STARTING_NOTE:

            adsr_start(&v->amplitude_envelope);
            v->state = VOICE_STATE_STARTING;

            continue;

        case VOICE_STATE_RESTARTING_NOTE:

            adsr_restart(&v->amplitude_envelope);
            v->state = VOICE_STATE_STARTING;

            break;
        
        case VOICE_STATE_STARTING:

            if (v->amplitude_envelope.state == ADSR_STATE_IDLE) {
                voice_reset(v);
                adsr_setup(&v->amplitude_envelope, global.amp_attack, global.amp_decay, global.amp_sustain, global.amp_release);
                v->state = VOICE_STATE_RUNNING;
            }

            break;

        case VOICE_STATE_RUNNING:

            // Update sound parameters
            v->settings.cutoff = global.cutoff + (((int32_t)v->velocity*(int32_t)global.cutoff_velocity)>>7);

            if (v->settings.cutoff > 127) {
                v->settings.cutoff = 127;
            }

            v->settings.resonance = global.resonance;
            v->settings.amp_velocity = global.amp_velocity;

            break;

        case VOICE_STATE_STOPPED:
            if (global.sustain) {
                break;
            }
            else {
                adsr_stop(&v->amplitude_envelope);
                v->state = VOICE_STATE_RELEASED;
                break;
            }

        case VOICE_STATE_RELEASED:
            if (v->amplitude_envelope.value <= 0) {
                v->state = VOICE_STATE_IDLE;
            }
            break;

        default:
            fprintf(stderr, "Unknown state\n");
            continue;
        }

        //
        // VOICE PLAYBACK
        //

        adsr_update(&v->amplitude_envelope);
        if (v->amplitude_envelope.state == ADSR_STATE_STOPPED) {
            v->state = VOICE_STATE_IDLE;
            continue;
        }

        // TODO; Implement mapping from velocity to amplitude
        A = v->amplitude_envelope.value;

        if (A > 127) {
            A = 127;
        }

        x = (A/128.0) * ppf_get_transposed_sample(v);

        // Apply filter (VCF)
        x = vcf_filter(x, v, v->settings.cutoff, v->settings.resonance);

        // Accumulate result of all voices!
        y += VOICE_AMPLITUDE*x;
    }

    // Write output do DAC
    buffer[buffer_idx] = (int16_t) (global.volume/128.0*y);
    buffer_idx += 1;

    return 0;
}
