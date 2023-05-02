#include "s3cda.h"
#include "harness/trace.h"

int gS3_cda_enabled;

void S3DisableCDA(void) {
    S3StopCDAOutlets();
    gS3_cda_enabled = 0;
}

void S3StopCDAOutlets(void) {
    STUB();
}

int S3PlayCDA(tS3_channel* chan) {
    return 0;
}

int S3StopCDA(tS3_channel* chan) {
    return 0;
}

int S3SetCDAVolume(tS3_channel* chan, int pVolume) {
    // if ( gS3_cda_enabled )
    // {
    //     sub_49CC70();
    // }
    return 0;
}

int S3IsCDAPlaying(void) {
    return 0;
}

int S3IsCDAPlaying2(void) {
    return S3IsCDAPlaying();
}
