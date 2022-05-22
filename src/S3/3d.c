#include "3d.h"
#include "audio.h"
#include "resource.h"
#include "s3sound.h"
#include <math.h>

tS3_vector3 gS3_listener_position_old;
tS3_vector3 gS3_listener_position_now;
tS3_vector3 gS3_listener_vel_now;
tS3_vector3 gS3_listener_left_now;

void* gS3_listener_pos_ptr;
void* gS3_listener_vel_ptr;
void* gS3_listener_left_ptr;

int gS3_listener_pos_is_brender;
int gS3_listener_vel_is_brender;
int gS3_listener_left_is_brender;

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
    gS3_listener_position_now.x = 0.0;
    gS3_listener_position_now.y = 0.0;
    gS3_listener_position_now.z = 0.0;
    gS3_listener_vel_now.x = 0.0;
    gS3_listener_vel_now.y = 0.0;
    gS3_listener_vel_now.z = 0.0;
    gS3_listener_left_now.x = 0.0;
    gS3_listener_left_now.y = 0.0;
    gS3_listener_left_now.z = 0.0;
}

void S3BindListenerPositionBRender(br_vector3* pos) {
    gS3_listener_pos_ptr = pos;
    gS3_listener_pos_is_brender = 1;
    S3CopyBrVector(&gS3_listener_position_old, pos);
}

void S3BindListenerVelocityBRender(br_vector3* vel) {
    gS3_listener_vel_ptr = vel;
    gS3_listener_vel_is_brender = 1;
}

void S3BindListenerLeftBRender(br_vector3* left) {
    gS3_listener_left_ptr = left;
    gS3_listener_left_is_brender = 1;
}

void S3UpdateListenerVectors() {}
void S3ServiceSoundSources() {}
int S3UpdateSpatialSound(tS3_channel* chan) {
    return 1;
}

int S3BindAmbientSoundToOutlet(tS3_outlet* pOutlet, int pSound, tS3_sound_source* source, float pMax_distance, int pPeriod, int pRepeats, int pVolume, int pPitch, int pSpeed) {
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

void S3UpdateSoundSource(tS3_outlet* outlet, tS3_sound_tag tag, tS3_sound_source* src, float pMax_distance_squared, int pPeriod, tS3_repeats pAmbient_repeats, tS3_volume pVolume, int pPitch, tS3_speed pSpeed) {
    tS3_channel* chan;    // [esp+28h] [ebp-Ch]
    tS3_descriptor* desc; // [esp+2Ch] [ebp-8h]
    int is_sample;        // [esp+30h] [ebp-4h]

    is_sample = 0;
    if (!gS3_enabled || !src) {
        return;
    }
    chan = src->channel;
    if (tag != -1 && src->sound_id != tag) {
        desc = S3GetDescriptorByID(tag);
        if (desc == NULL) {
            return;
        }
        if (desc->type == eS3_ST_sample) {
            src->sound_id = tag;
            if ((desc->sound_data == NULL || (desc->flags & 2) != 0) && !S3LoadSample(tag)) {
                return;
            }
            is_sample = 1;
        }
    }
    if (pSpeed != -1) {
        if (pSpeed < 1) {
            pSpeed = 0x10000;
        }
        src->speed = pSpeed;
    }
    if (pPitch != -1) {
        if (pPitch < 1) {
            pPitch = 0x10000;
        }
        src->pitch = pPitch;
        if (chan && chan->descriptor && chan->descriptor->type == eS3_ST_sample) {
            chan->initial_pitch = S3IRandomBetweenLog(chan->descriptor->min_pitch, chan->descriptor->max_pitch, ((tS3_sample*)chan->descriptor->sound_data)->rate);
            chan->initial_pitch = ldexpf(src->pitch, -16) * chan->initial_pitch;
            chan->initial_pitch = ldexpf(src->speed, -16) * chan->initial_pitch;
        }
    }
    if (pAmbient_repeats != -1) {
        src->ambient_repeats = MAX(0, pAmbient_repeats);
        if (chan) {
            chan->repetitions = src->ambient_repeats;
        }
    }
    if (pMax_distance_squared != -1.0f) {
        src->max_distance_sq = pMax_distance_squared;
        if (chan) {
            chan->pMax_distance_squared = pMax_distance_squared;
        }
    }
    if (pPeriod != -1) {
        src->period = pPeriod;
    }
    if (pVolume != -1) {
        if (pVolume > 255) {
            pVolume = 255;
        }
        if (pVolume < 0) {
            pVolume = 128;
        }
        if (src->volume != pVolume) {
            src->volume = pVolume;
        }
        if (chan) {
            chan->initial_volume = pVolume;
        }
    }
    if (chan) {
        if (chan->sound_source_ptr != src) {
            S3StopChannel(chan);
        }
        if (!pVolume || is_sample) {
            chan->spatial_sound = 0;
            chan->needs_service = 1;
            S3StopChannel(chan);
            src->channel = 0;
            src->tag = 0;
        }
    }
}