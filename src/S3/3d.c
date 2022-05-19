#include "3d.h"
#include "audio.h"
#include "s3sound.h"
#include <math.h>

br_vector3 gS3_listener_position_copy;
br_vector3 gS3_listener_vel_copy;
br_vector3 gS3_listener_left_copy;
float dword_531D9C;
float flt_531D7C;
float flt_531D98;

void S3Set3DSoundEnvironment(float a1, float a2, float a3) {
    float tmp;

    if (a1 == -1.0f) {
        a1 = 0.25f;
    }
    if (a2 == -1.0f) {
        a2 = 1.2f;
    }
    if (a3 == -1.0f) {
        a3 = 130000.0f;
    }
    dword_531D9C = a1;
    tmp = sqrtf(a3 / a2);
    flt_531D7C = tmp;
    flt_531D98 = tmp * dword_531D9C;
    gS3_listener_position_copy.v[2] = 0.0;
    gS3_listener_position_copy.v[1] = 0.0;
    gS3_listener_position_copy.v[0] = 0.0;
    gS3_listener_vel_copy.v[2] = 0.0;
    gS3_listener_vel_copy.v[1] = 0.0;
    gS3_listener_vel_copy.v[0] = 0.0;
    gS3_listener_left_copy.v[2] = 0.0;
    gS3_listener_left_copy.v[1] = 0.0;
    gS3_listener_left_copy.v[0] = 0.0;
}

void S3UpdateListenerVectors() {}
void S3ServiceSoundSources() {}
int S3UpdateSpatialSound(tS3_channel* chan) {
    return 1;
}

int S3BindAmbientSource(tS3_outlet* pOutlet, int pSound, tS3_sound_source* source, float pMax_distance, int pPeriod, int pRepeats, int pVolume, int pPitch, int pSpeed) {
    tS3_descriptor* desc; // [esp+Ch] [ebp-4h]

    if (!gS3_enabled) {
        return 0;
    }
    if (!source) {
        return eS3_error_nonexistant_source;
    }
    desc = S3GetDescriptorByID(pSound);
    if (!desc) {
        return eS3_error_bad_id;
    }
    if (desc->type != eS3_ST_sample) {
        return 0;
    }
    if ((!desc->sound_data || (desc->flags & 2) != 0) && !S3LoadSample(pSound)) {
        return eS3_error_load_sound;
    }
    if (pVolume > 255) {
        pVolume = 255;
    }
    if (pVolume < 0) {
        pVolume = 128;
    }
    if (pPitch < 0) {
        pPitch = 0x10000;
    }
    if (pSpeed < 0) {
        pSpeed = 0x10000;
    }
    source->bound_outlet = pOutlet;
    source->sound_id = pSound;
    source->volume = pVolume;
    source->max_distance_sq = pMax_distance;
    source->period = pPeriod;
    source->pitch = pPitch;
    source->speed = pSpeed;
    source->ambient = 1;

    source->ambient_repeats = pRepeats < 0 ? 0 : pRepeats;
    source->time_since_last_played = pPeriod;
    source->channel = 0;
    source->tag = 0;
    return 0;
}