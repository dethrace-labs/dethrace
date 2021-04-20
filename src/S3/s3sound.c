#include "include/s3/s3sound.h"
#include "harness.h"

void S3StopAllOutletSounds() {
    Harness_Hook_S3StopAllOutletSounds();
}

tS3_sound_tag S3StartSound(tS3_outlet_ptr pOutlet, tS3_sound_id pSound) {
}