#include "s3music.h"
#include "harness/trace.h"

int gS3_midi_enabled;

// FUNCTION: CARM95 0x00411EA5
void S3DisableMIDI(void) {
    S3StopMIDIOutlets();
    gS3_midi_enabled = 0;
}

// FUNCTION: CARM95 0x004123CF
void S3StopMIDIOutlets(void) {
}

// FUNCTION: CARM95 0x00412453
void S3ReleaseMIDI(tS3_sound_tag tag) {
}

// FUNCTION: CARM95 0x00411F53
int S3PlayMIDI(tS3_channel* chan) {
    return 0;
}
// FUNCTION: CARM95 0x00411F23
int S3MIDILoadSong(tS3_channel* chan) {
    return 0;
}
// FUNCTION: CARM95 0x00411FF2
int S3SetMIDIVolume(tS3_channel* chan) {
    return 0;
}

// FUNCTION: CARM95 0x00411FA5
int S3StopMIDI(tS3_channel* chan) {
    // if (gS3_midi_enabled && chan->active && chan->type == 1) {
    //     S3SendMCICloseCommand(chan);
    //     chan->active = 0;
    // }
    return 0;
}

// FUNCTION: CARM95 0x0049DEDC
int S3IsMIDIStopped(tS3_channel* chan) {
    return 1;
}
