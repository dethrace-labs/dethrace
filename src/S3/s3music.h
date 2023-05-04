#ifndef _S3MUSIC_H_
#define _S3MUSIC_H_

#include "s3_defs.h"

void S3DisableMIDI(void);
void S3StopMIDIOutlets(void);
void S3DisposeMIDIChannel(tS3_sound_tag);
int S3PlayMIDI(tS3_channel* chan);
int S3MIDILoadSong(tS3_channel* chan);
int S3SetMIDIVolume(tS3_channel* chan);
int S3StopMIDI(tS3_channel* chan);

int S3IsMIDIStopped(tS3_channel* chan);

#endif
