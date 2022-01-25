#ifndef S3_SOUND_H
#define S3_SOUND_H

#include "s3_types.h"

void S3Service(int inside_cockpit, int unk1);
tS3_sound_tag S3StartSound(tS3_outlet_ptr pOutlet, tS3_sound_id pSound);
void S3StopAllOutletSounds();
int S3SoundStillPlaying(tS3_sound_tag pSound);
int S3StopSound(tS3_sound_tag pSound_tag);

#endif