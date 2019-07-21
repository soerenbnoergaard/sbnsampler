#include "ctrl.h"
#include "midi.h"
#include "voice.h"

#define ENABLE_STEALING true

#define CC_PRESET 0x50
#define CC_VOLUME 0x46
#define CC_CUTOFF_VELOCITY 0x0a
#define CC_SUSTAIN 0x40
#define CC_CUTOFF 0x4a
#define CC_RESONANCE 0x47

#define CC_AMP_ATTACK 0x53
#define CC_AMP_DECAY 0x59
#define CC_AMP_SUSTAIN 0x55
#define CC_AMP_RELEASE 0x52

#define NOTE_ON 0x90
#define NOTE_OFF 0x80

// Globals /////////////////////////////////////////////////////////////////////

// Private functions ///////////////////////////////////////////////////////////
static voice_t *find_voice(uint8_t note, bool enable_stealing, bool *voice_stolen)
{
    // Find a suiting voice for the incoming note.
    int32_t n;
    int32_t n_idle = -1;
    int32_t n_steal = -1;
    int32_t n_oldest = -1;
    voice_t *v;

    for (n = 0; n < NUM_VOICES; n++) {
        v = voice_get_handle(n);

        switch (v->state) {
        case VOICE_STATE_IDLE:
            n_idle = n;
            break;

        default:
            if (v->note == note) {
                n_steal = n;
            }

            if (n_oldest < 0) {
                n_oldest = n;
            }
            else if (v->vco.index > voice_get_handle(n_oldest)->vco.index) {
                n_oldest = n;
            }
            break;
        }
    }

    if ((enable_stealing) && (n_steal >= 0)) {
        *voice_stolen = true;
        return voice_get_handle(n_steal);
    }
    else if (n_idle >= 0) {
        *voice_stolen = false;
        return voice_get_handle(n_idle);
    }
    else {
        *voice_stolen = true;
        return voice_get_handle(n_oldest);
    }
    return NULL;
}

static status_t note_on(midi_message_t m)
{
    voice_t *v;
    bool stolen;

    // Find an available voice
    v = find_voice(m.data[0], ENABLE_STEALING, &stolen);
    if (v == NULL) {
        error("No voice available");
        return STATUS_ERROR;
    }

    // Prepare the voice for playback
    // TODO: consider moving to `voice_setup()`
    v->note = m.data[0];
    v->velocity = m.data[1];
    if (vco_setup(&v->vco, v->note) != STATUS_OK) {
        return STATUS_ERROR;
    }

    if (stolen) {
        v->state = VOICE_STATE_RESTARTING;
    }
    else {
        v->state = VOICE_STATE_STARTING;
    }

    return STATUS_OK;
}

static status_t note_off(midi_message_t m)
{
    int32_t n;
    voice_t *v;

    // Find the voice with the note active and stop it.
    for (n = 0; n < NUM_VOICES; n++) {
        v = voice_get_handle(n);
        if (v->note == m.data[0]) {
            v->state = VOICE_STATE_STOPPED;
        }
    }

    return STATUS_OK;
}

static status_t input(midi_message_t m)
{
    switch (m.status & 0xf0) {
    case NOTE_ON:
        if (m.data[1] > 0) {
            note_on(m);
        }
        else {
            note_off(m);
        }
        break;

    case NOTE_OFF:
        note_off(m);
        break;

    default:
        break;
    }

    return STATUS_OK;
}

// Public functions ////////////////////////////////////////////////////////////
status_t ctrl_init(void)
{
    return STATUS_OK;
}

status_t ctrl_close(void)
{
    return STATUS_OK;
}

status_t ctrl_tick(void)
{
    status_t status;
    midi_message_t m;

    // Check if there is any input to handle
    status = midi_get(&m);
    if (status == STATUS_OK) {
        printf("%02x %02x %02x\n", m.status, m.data[0], m.data[1]);
        input(m);
    }

    return status;
}

