#include "s3music.h"
#include "harness/trace.h"

int gS3_midi_enabled;

void S3DisableMIDI(void) {
    S3StopMIDIOutlets();
    gS3_midi_enabled = 0;
}

void S3StopMIDIOutlets(void) {
}

void S3ReleaseMIDI(tS3_sound_tag tag) {
}

int S3PlayMIDI(tS3_channel* chan) {
    return 0;
}
int S3MIDILoadSong(tS3_channel* chan) {
    return 0;
}
int S3SetMIDIVolume(tS3_channel* chan) {
    return 0;
}

int S3StopMIDI(tS3_channel* chan) {
    // if (gS3_midi_enabled && chan->active && chan->type == 1) {
    //     S3SendMCICloseCommand(chan);
    //     chan->active = 0;
    // }
    return 0;
}

int S3IsMIDIStopped(tS3_channel* chan) {
    return 1;
}
