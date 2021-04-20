#ifndef S3_SOUND_H
#define S3_SOUND_H

#include "s3_types.h"

tS3_sound_tag S3StartSound(tS3_outlet_ptr pOutlet, tS3_sound_id pSound);
void S3StopAllOutletSounds();

#endif