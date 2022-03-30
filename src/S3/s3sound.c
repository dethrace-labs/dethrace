#include "include/s3/s3sound.h"

// Forward declarations of harneess hook funcs. See `src/harness`
extern void Harness_Hook_S3Service(int unk1, int unk2);
extern void Harness_Hook_S3StopAllOutletSounds();

void S3Service(int inside_cockpit, int unk1) {
    Harness_Hook_S3Service(inside_cockpit, unk1);
}

void S3StopAllOutletSounds() {
    Harness_Hook_S3StopAllOutletSounds();
}

tS3_sound_tag S3StartSound(tS3_outlet_ptr pOutlet, tS3_sound_id pSound) {
    return 0;
}

int S3SoundStillPlaying(tS3_sound_tag pSound) {
    return 0;
}

int S3StopSound(tS3_sound_tag pSound_tag) {
    return 0;
}

void S3StopOutletSound(tS3_outlet_ptr pOutlet) {
    return;
}
