#include "ctrl.h"
#include "midi.h"
#include "preset.h"
#include "panel.h"

// Globals /////////////////////////////////////////////////////////////////////

// Private functions ///////////////////////////////////////////////////////////
static bool released(voice_t *v)
{
    if (panel_get(PANEL_SUSTAIN_PEDAL) != 0) {
        return false;
    }
    return true;
}

static bool tailed_off(voice_t *v)
{
    if (!adsr_is_stopped(&v->env1)) {
        return false;
    }
    if (!adsr_is_stopped(&v->env2)) {
        return false;
    }
    return true;
}

static status_t voice_stop(voice_t *v)
{
    v->state = VOICE_STATE_STOPPED;
    return STATUS_OK;
}

static status_t voice_restart(voice_t *v)
{
    adsr_stop_quick(&v->env1);
    adsr_stop_quick(&v->env2);
    v->state = VOICE_STATE_RESTARTING;
    return STATUS_OK;
}

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
    v = find_voice(m.data[0], panel_get(PANEL_ENABLE_STEALING) != 0, &stolen);
    if (v == NULL) {
        error("No voice available");
        return STATUS_ERROR;
    }

    // Store the midi message triggering the voice
    v->midi = m;

    if (stolen) {
        voice_restart(v);
        v->state = VOICE_STATE_RESTARTING;
    }
    else {
        // FIXME: If a voice is not stolen, the old voice should ring out.
        // Right now, it seems like it is quick-stopping the ADSR.

        // Prepare the voice for playback
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
        if ((v->note == m.data[0]) && (v->state != VOICE_STATE_IDLE)) {
            voice_stop(v);
        }
    }

    return STATUS_OK;
}

static status_t change_sample_collection(uint8_t id)
{
    here("Change sample");
    return STATUS_OK;
}

static status_t input(midi_message_t m)
{
    switch (m.status & 0xf0) {
    case MIDI_NOTE_ON:
        if (m.data[1] > 0) {
            note_on(m);
        }
        else {
            note_off(m);
        }
        break;

    case MIDI_NOTE_OFF:
        note_off(m);
        break;

    case MIDI_CC:
        panel_set(m.data[0], m.data[1]);

        switch (m.data[0]) {
        case PANEL_SAMPLE_COLLECTION:
            change_sample_collection(m.data[1]);
            break;
        case PANEL_PRESET:
            preset_load(panel_get(PANEL_PRESET));
            break;
        default:
            break;
        }
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

status_t ctrl_voice_tick(voice_t *v)
{
    status_t status;

    // Voice state machine
    switch (v->state) {
    case VOICE_STATE_IDLE:
        break;

    case VOICE_STATE_STARTING:
        v->note = v->midi.data[0];
        v->velocity = v->midi.data[1];

        adsr_start(&v->env1);
        adsr_start(&v->env2);
        vco_setup(&v->vco, panel_get(PANEL_SAMPLE_COLLECTION), v->note);
        vcf_setup(&v->vcf);

        v->state = VOICE_STATE_RUNNING;
        break;

    case VOICE_STATE_RUNNING:
        break;

    case VOICE_STATE_RESTARTING:
        if (tailed_off(v)) {
            v->state = VOICE_STATE_STARTING;
            break;
        }
        break;

    case VOICE_STATE_STOPPED:
        if (released(v)) {
            adsr_stop(&v->env1);
            adsr_stop(&v->env2);
            v->state = VOICE_STATE_RELEASED;
            break;
        }
        break;

    case VOICE_STATE_RELEASED:
        if (tailed_off(v)) {
            v->state = VOICE_STATE_IDLE;
            break;
        }
        break;

    default:
        error("Unhandled voice state!");
        break;
    }

    status = adsr_setup(&v->env1,
                        panel_get(PANEL_ENV1_ATTACK),
                        panel_get(PANEL_ENV1_DECAY),
                        panel_get(PANEL_ENV1_SUSTAIN),
                        panel_get(PANEL_ENV1_RELEASE));
    assert(status == STATUS_OK);

    status = adsr_tick(&v->env1);
    assert(status == STATUS_OK);

    status = adsr_setup(&v->env2,
                        panel_get(PANEL_ENV2_ATTACK),
                        panel_get(PANEL_ENV2_DECAY),
                        panel_get(PANEL_ENV2_SUSTAIN),
                        panel_get(PANEL_ENV2_RELEASE));
    assert(status == STATUS_OK);

    status = adsr_tick(&v->env2);
    assert(status == STATUS_OK);

    return STATUS_OK;
}
