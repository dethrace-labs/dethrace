#include "s3cda.h"
#include "harness/audio.h"
#include "harness/trace.h"
#include "s3.h"

int gS3_cda_enabled;

// FUNCTION: CARM95 0x004956C0
void S3EnableCDA(void) {
    gS3_cda_enabled = 1;
}

// FUNCTION: CARM95 0x004956D5
void S3DisableCDA(void) {
    S3StopCDAOutlets();
    gS3_cda_enabled = 0;
}

// FUNCTION: CARM95 0x004957BE
int S3StopCDAOutlets(void) {
    tS3_channel* chan;
    tS3_outlet* o;

    if (gS3_cda_enabled) {
        for (o = gS3_outlets; o; o = o->next) {
            for (chan = o->channel_list; chan; chan = chan->next) {
                if (chan->type == eS3_ST_cda) {
                    AudioBackend_StopCDA();
                    // S3SetMCIStopCommand(chan);
                }
            }
        }
    }
    return 1;
}

// FUNCTION: CARM95 0x00495725
int S3CDAEnabled(void) {
    return gS3_cda_enabled;
}

// FUNCTION: CARM95 0x00495753
int S3PlayCDA(tS3_channel* chan) {
    int track;
    if (gS3_cda_enabled) {
        track = strtoul(chan->descriptor->filename, NULL, 10);
        if (AudioBackend_PlayCDA(track) == eAB_error) {
            return eS3_error_start_cda;
        }
    }
    return eS3_error_none;
}

// FUNCTION: CARM95 0x0049578C
int S3StopCDA(tS3_channel* chan) {
    AudioBackend_StopCDA();
    return eS3_error_none;
}

// FUNCTION: CARM95 0x0049592E
int S3SetCDAVolume(tS3_channel* chan, int pVolume) {
    if (gS3_cda_enabled) {
        AudioBackend_SetCDAVolume(pVolume);
    }
    return 0;
}

// FUNCTION: CARM95 0x0049E278
int S3IsCDAPlaying2(void) {
    return AudioBackend_CDAIsPlaying();
}

// FUNCTION: CARM95 0x00495919
int S3IsCDAPlaying(void) {
    return S3IsCDAPlaying2();
}
