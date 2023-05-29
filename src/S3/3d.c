#include "3d.h"
#include "audio.h"
#include "harness/trace.h"
#include "resource.h"
#include "s3sound.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

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

void S3CopyVector3(void* a1, void* a2, int pBrender_vector) {
    if (pBrender_vector) {
        S3CopyBrVector3((tS3_vector3*)a1, (br_vector3*)a2);
    } else {
        S3CopyS3Vector3((tS3_vector3*)a1, (tS3_vector3*)a2);
    }
}

void S3CopyBrVector3(tS3_vector3* a1, br_vector3* a2) {
    a1->x = a2->v[0];
    a1->y = a2->v[1];
    a1->z = a2->v[2];
}

void S3CopyS3Vector3(tS3_vector3* a1, tS3_vector3* a2) {
    *a1 = *a2;
}

void S3BindListenerPositionBRender(br_vector3* pos) {
    gS3_listener_pos_ptr = pos;
    gS3_listener_pos_is_brender = 1;
    S3CopyBrVector3(&gS3_listener_position_old, pos);
}

void S3BindListenerVelocityBRender(br_vector3* vel) {
    gS3_listener_vel_ptr = vel;
    gS3_listener_vel_is_brender = 1;
}

void S3BindListenerLeftBRender(br_vector3* left) {
    gS3_listener_left_ptr = left;
    gS3_listener_left_is_brender = 1;
}

void S3UpdateListenerVectors(void) {
    if (gS3_listener_pos_ptr) {
        S3CopyVector3(&gS3_listener_position_now, gS3_listener_pos_ptr, gS3_listener_pos_is_brender);
    }
    if (gS3_listener_vel_ptr) {
        S3CopyVector3(&gS3_listener_vel_now, gS3_listener_vel_ptr, gS3_listener_vel_is_brender);
    } else {
        gS3_listener_vel_now.x = (gS3_listener_position_now.x - gS3_listener_position_old.x) / 1000.0 * gS3_service_time_delta;
        gS3_listener_vel_now.y = (gS3_listener_position_now.y - gS3_listener_position_old.y) / 1000.0 * gS3_service_time_delta;
        gS3_listener_vel_now.z = (gS3_listener_position_now.z - gS3_listener_position_old.z) / 1000.0 * gS3_service_time_delta;
        gS3_listener_position_old = gS3_listener_position_now;
    }
    if (gS3_listener_left_ptr) {
        S3CopyVector3(&gS3_listener_left_now, gS3_listener_left_ptr, gS3_listener_left_is_brender);
    } else {
        gS3_listener_left_now.x = gS3_listener_position_now.x + 1.0;
        gS3_listener_left_now.y = gS3_listener_position_now.y;
        gS3_listener_left_now.z = gS3_listener_position_now.z;
    }
}

void S3ServiceAmbientSoundSources(void) {
    tS3_sound_source* s; // [esp+Ch] [ebp-4h]

    for (s = gS3_sound_sources; s; s = s->next) {
        if (s->ambient == 0) {
            continue;
        }
        if (s->period > 0) {
            s->time_since_last_played += gS3_service_time_delta;
        }
        if (s->channel && s->channel->tag != s->tag) {
            S3StopChannel(s->channel);
            s->channel = NULL;
            s->tag = 0;
        }

        if (s->channel != NULL) {
            continue;
        }
        if (s->time_since_last_played <= s->period || !s->period || s->tag) {
            if ((s->ambient_repeats == 0 || s->period == 0) && s->tag == 0) {
                if (s->volume > 0 && S3ServiceSoundSource(s) == 0) {
                    s->channel = NULL;
                    s->tag = 0;
                }
                s->time_since_last_played = 0;
            }
        } else {
            if (s->volume > 0 && S3ServiceSoundSource(s) == 0) {
                s->channel = NULL;
                s->tag = 0;
            }
            s->time_since_last_played = 0;
        }
    }
}

int S3UpdateSpatialSound(tS3_channel* chan) {
    int close_enough_to_play; // [esp+10h] [ebp-4h]

    if (chan->sound_source_ptr && chan->sound_source_ptr->ambient) {
        close_enough_to_play = S3Calculate3D(chan, 1);
    } else {
        close_enough_to_play = S3Calculate3D(chan, 0);
    }
    if (close_enough_to_play) {
        S3SyncSampleVolumeAndPan(chan);
        S3SyncSampleRate(chan);
    }
    return close_enough_to_play;
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
            chan->initial_pitch *= ldexpf(src->pitch, -16);
            chan->initial_pitch *= ldexpf(src->speed, -16);
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

void S3StopSoundSource(tS3_sound_source* src) {
    if (!gS3_enabled) {
        return;
    }
    if (src) {
        if (src->channel && src->tag && src->channel->tag == src->tag) {
            src->channel->termination_reason = eS3_tr_stopped;
            src->channel->spatial_sound = 0;
            S3StopChannel(src->channel);
            src->channel->sound_source_ptr = NULL;
        }
        src->ambient = 0;
    }
}

tS3_sound_tag S3ServiceSoundSource(tS3_sound_source* src) {
    tS3_channel* chan;    // [esp+30h] [ebp-10h]
    tS3_outlet* outlet;   // [esp+34h] [ebp-Ch]
    tS3_descriptor* desc; // [esp+3Ch] [ebp-4h]

    if (!src) {
        return 0;
    }
    if (!gS3_enabled || !src->ambient) {
        src->tag = 0;
        src->channel = 0;
        return 0;
    }
    outlet = src->bound_outlet;
    desc = S3GetDescriptorByID(src->sound_id);
    if (desc == NULL) {
        gS3_last_error = eS3_error_bad_id;
        return 0;
    }
    if (desc->type) {
        return 0;
    }
    memset(&gS3_channel_template, 0, sizeof(gS3_channel_template));
    gS3_channel_template.initial_volume = src->volume;
    gS3_channel_template.rate = S3IRandomBetweenLog(desc->min_pitch, desc->max_pitch, ((tS3_sample*)desc->sound_data)->rate);
    if (src->pitch < 0) {
        src->pitch = 0x10000;
    }
    if (src->speed < 0) {
        src->speed = 0x10000;
    }

    gS3_channel_template.rate = ldexp(src->pitch, -16) * gS3_channel_template.rate;
    if (!outlet->independent_pitch) {
        gS3_channel_template.rate = ldexp(src->speed, -16) * gS3_channel_template.rate;
    }
    gS3_channel_template.initial_pitch = gS3_channel_template.rate;
    gS3_channel_template.sound_source_ptr = src;
    if (!src->velocity_ptr) {
        S3CopyVector3(&gS3_channel_template.lastpos, src->position_ptr, src->brender_vector);
    }
    gS3_channel_template.pMax_distance_squared = src->max_distance_sq;
    if (S3Calculate3D(&gS3_channel_template, 1) == 0) {
        src->tag = 0;
        src->channel = NULL;
        return 0;
    }

    chan = S3AllocateChannel(outlet, desc->priority * (gS3_channel_template.right_volume + gS3_channel_template.left_volume + 1));
    if (chan == NULL) {
        gS3_last_error = eS3_error_channel_alloc;
        src->tag = 0;
        src->channel = NULL;
        return 0;
    }

    if ((desc->sound_data && (desc->flags & 2) == 0) || S3LoadSample(src->sound_id)) {
        chan->left_volume = gS3_channel_template.left_volume * chan->volume_multiplier;
        chan->right_volume = gS3_channel_template.right_volume * chan->volume_multiplier;
        chan->rate = gS3_channel_template.rate;
        chan->position.x = gS3_channel_template.position.x;
        chan->position.y = gS3_channel_template.position.y;
        chan->position.z = gS3_channel_template.position.z;
        chan->velocity.x = gS3_channel_template.velocity.x;
        chan->velocity.y = gS3_channel_template.velocity.y;
        chan->velocity.z = gS3_channel_template.velocity.z;
        chan->lastpos.x = gS3_channel_template.lastpos.x;
        chan->lastpos.y = gS3_channel_template.lastpos.y;
        chan->lastpos.z = gS3_channel_template.lastpos.z;
        chan->initial_volume = gS3_channel_template.initial_volume;
        chan->initial_pitch = gS3_channel_template.initial_pitch;
        chan->pMax_distance_squared = gS3_channel_template.pMax_distance_squared;
        chan->spatial_sound = 2;
        chan->sound_source_ptr = src;
        chan->descriptor = desc;
        chan->tag = S3GenerateTag(outlet);
        chan->repetitions = src->ambient_repeats;
        chan->needs_service = 0;
        chan->termination_reason = 0;
        S3ExecuteSampleFilterFuncs(chan);
        if (S3PlaySample(chan)) {
            src->tag = chan->tag;
            src->channel = chan;
            return chan->tag;
        } else {
            chan->needs_service = 1;
            gS3_last_error = eS3_error_start_sound;
            return 0;
        }
    } else {
        gS3_last_error = eS3_error_load_sound;
        chan->needs_service = 1;
        return 0;
    }
}

tS3_sound_tag S3StartSound3D(tS3_outlet* pOutlet, tS3_sound_id pSound, tS3_vector3* pInitial_position, tS3_vector3* pInitial_velocity, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed) {
    tS3_channel* chan;    // [esp+30h] [ebp-Ch]
    tS3_descriptor* desc; // [esp+38h] [ebp-4h]

    if (!gS3_enabled) {
        return 0;
    }
    desc = S3GetDescriptorByID(pSound);
    if (!desc) {
        gS3_last_error = eS3_error_bad_id;
        return 0;
    }
    if (desc->type != eS3_ST_sample) {
        return 0;
    }

    if ((desc->sound_data == NULL || (desc->flags & 2) != 0) && S3LoadSample(pSound) == 0) {
        gS3_last_error = eS3_error_load_sound;
        return 0;
    }
    if (pVolume > 255) {
        pVolume = 255;
    }
    if (pVolume < 0) {
        pVolume = S3IRandomBetween(desc->min_volume, desc->max_volume, 128);
    }
    memset(&gS3_channel_template, 0, sizeof(gS3_channel_template));
    gS3_channel_template.volume_multiplier = 1.0;
    gS3_channel_template.sound_source_ptr = 0;
    gS3_channel_template.pMax_distance_squared = 150.0;
    gS3_channel_template.right_volume = pVolume;
    gS3_channel_template.left_volume = pVolume;
    gS3_channel_template.initial_volume = pVolume;
    gS3_channel_template.rate = S3IRandomBetweenLog(desc->min_pitch, desc->max_pitch, ((tS3_sample*)desc->sound_data)->rate);
    if (pPitch == -1) {
        pPitch = 0x10000;
    }
    if (pSpeed == -1) {
        pSpeed = 0x10000;
    }
    gS3_channel_template.rate = ldexpf(pPitch, -16) * gS3_channel_template.rate;
    if (!pOutlet->independent_pitch) {
        gS3_channel_template.rate = ldexpf(pSpeed, -16) * gS3_channel_template.rate;
    }
    gS3_channel_template.initial_pitch = gS3_channel_template.rate;
    gS3_channel_template.position = *pInitial_position;
    gS3_channel_template.velocity = *pInitial_velocity;

    if (S3Calculate3D(&gS3_channel_template, 0) == 0) {
        return 0;
    }
    chan = S3AllocateChannel(pOutlet, desc->priority * (gS3_channel_template.right_volume + gS3_channel_template.left_volume + 1));
    if (chan) {
        chan->left_volume = gS3_channel_template.left_volume * chan->volume_multiplier;
        chan->right_volume = gS3_channel_template.right_volume * chan->volume_multiplier;
        chan->rate = gS3_channel_template.rate;
        chan->spatial_sound = 1;
        chan->sound_source_ptr = 0;
        chan->descriptor = desc;
        chan->needs_service = 0;
        chan->termination_reason = 0;
        chan->position = gS3_channel_template.position;
        chan->lastpos = gS3_channel_template.lastpos;
        chan->velocity = gS3_channel_template.velocity;
        chan->repetitions = MAX(pRepeats, 0);
        chan->tag = S3GenerateTag(pOutlet);
        chan->initial_volume = gS3_channel_template.initial_volume;
        chan->initial_pitch = gS3_channel_template.initial_pitch;
        chan->pMax_distance_squared = gS3_channel_template.pMax_distance_squared;
        S3ExecuteSampleFilterFuncs(chan);
        if (S3PlaySample(chan) != 0) {
            return chan->tag;
        } else {
            chan->needs_service = 1;
            gS3_last_error = eS3_error_start_sound;
            return 0;
        }
    } else {
        gS3_last_error = eS3_error_channel_alloc;
        return 0;
    }
}

int S3Calculate3D(tS3_channel* chan, int pIs_ambient) {
    float attenuation;                  // [esp+2Ch] [ebp-1Ch]
    float doppler_shift;                // [esp+30h] [ebp-18h]
    float vol_multiplier;               // [esp+38h] [ebp-10h]
    tS3_sound_source* sound_source_ptr; // [esp+3Ch] [ebp-Ch]
    float dist_squared;                 // [esp+40h] [ebp-8h]
    float dist;                         // [esp+44h] [ebp-4h]

    sound_source_ptr = chan->sound_source_ptr;
    if (sound_source_ptr) {
        if (sound_source_ptr->position_ptr) {
            S3CopyVector3(&chan->position, sound_source_ptr->position_ptr, sound_source_ptr->brender_vector);
        }
        if (sound_source_ptr->velocity_ptr) {
            S3CopyVector3(&chan->velocity, sound_source_ptr->velocity_ptr, sound_source_ptr->brender_vector);
        } else {
            chan->velocity.x = (chan->position.x - chan->lastpos.x) / 1000.0f * gS3_service_time_delta;
            chan->velocity.y = (chan->position.y - chan->lastpos.y) / 1000.0f * gS3_service_time_delta;
            chan->velocity.z = (chan->position.z - chan->lastpos.z) / 1000.0f * gS3_service_time_delta;
            chan->lastpos = chan->position;
        }
    }
    dist_squared = (chan->position.z - gS3_listener_position_now.z) * (chan->position.z - gS3_listener_position_now.z)
        + (chan->position.x - gS3_listener_position_now.x) * (chan->position.x - gS3_listener_position_now.x)
        + (chan->position.y - gS3_listener_position_now.y) * (chan->position.y - gS3_listener_position_now.y);
    if (dist_squared < 0) {
        dist_squared = dist_squared * -1.0;
    }

    if (chan->pMax_distance_squared < dist_squared) {
        return 0;
    }
    if (dist_squared == 0.0f) {
        dist = 0.0f;
    } else {
        dist = sqrtf(dist_squared);
    }
    if (pIs_ambient) {
        doppler_shift = 1.0f - ((chan->position.z - gS3_listener_position_now.z) * (chan->velocity.z - gS3_listener_vel_now.z) + (chan->velocity.y - gS3_listener_vel_now.y) * (chan->position.y - gS3_listener_position_now.y) + (chan->position.x - gS3_listener_position_now.x) * (chan->velocity.x - gS3_listener_vel_now.x)) / dist / flt_531D98;
        if (doppler_shift > 2.0f) {
            doppler_shift = 2.0f;
        } else if (doppler_shift < 0.5f) {
            doppler_shift = 0.5;
        }
        chan->rate = chan->initial_pitch * doppler_shift;
    } else {
        chan->rate = chan->initial_pitch;
    }

    vol_multiplier = 1.0f / (dist / 6.0f + 1.0f);
    if (!gS3_inside_cockpit) {
        vol_multiplier = vol_multiplier * 1.3f;
    }
    attenuation = (chan->position.z - gS3_listener_position_now.z) * gS3_listener_left_now.z
        + (chan->position.y - gS3_listener_position_now.y) * gS3_listener_left_now.y
        + (chan->position.x - gS3_listener_position_now.x) * gS3_listener_left_now.x;
    if (attenuation < -1.0f) {
        attenuation -= ceil(attenuation);
    }
    if (attenuation > 1.0f) {
        attenuation -= floor(attenuation);
    }
    chan->left_volume = (attenuation + 1.0f) / 2.0f * ((double)chan->initial_volume * vol_multiplier) * chan->volume_multiplier;
    if (chan->left_volume < 0) {
        chan->left_volume = 0;
    }
    chan->right_volume = (1.0f - attenuation) / 2.0f * ((double)chan->initial_volume * vol_multiplier) * chan->volume_multiplier;
    if (chan->right_volume < 0) {
        chan->right_volume = 0;
    }
    if (chan->left_volume > 255) {
        chan->left_volume = 255;
    }
    if (chan->right_volume > 255) {
        chan->right_volume = 255;
    }
    return 1;
}
