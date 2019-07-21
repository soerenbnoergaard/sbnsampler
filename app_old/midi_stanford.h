#ifndef MIDI_STANFORD_H
#define MIDI_STANFORD_H

#include <alsa/asoundlib.h>     /* Interface to the ALSA system */

// function declarations:
void  print_midi_ports          (void);
void  print_card_list           (void);
void  list_midi_devices_on_card (int card);
void  list_subdevice_info       (snd_ctl_t *ctl, int card, int device);
int   is_input                  (snd_ctl_t *ctl, int card, int device, int sub);
int   is_output                 (snd_ctl_t *ctl, int card, int device, int sub);
void  error                     (const char *format, ...);

#endif