#include "sound.h"

#include <stdlib.h>
#include <string.h>

#include "brender.h"
#include "controls.h"
#include "globvars.h"
#include "graphics.h"
#include "harness/trace.h"
#include "opponent.h"
#include "pd/sys.h"
#include "piping.h"
#include "replay.h"
#include "s3/s3.h"
#include "s3/s3_brender.h"
#include "utility.h"

// GLOBAL: CARM95 0x00514928
int gSound_detail_level = 1;

// GLOBAL: CARM95 0x0051492c
int gVirgin_pass = 1;

// GLOBAL: CARM95 0x00514930
int gOld_sound_detail_level = -1;

// GLOBAL: CARM95 0x00514934
int gLast_tune = -1;

// GLOBAL: CARM95 0x00514938
int gRandom_MIDI_tunes[3] = { 9500, 9501, 9502 };

// GLOBAL: CARM95 0x00514948
int gRandom_Rockin_MIDI_tunes[3] = { 9500, 9501, 9502 };

// GLOBAL: CARM95 0x00514958
int gRandom_CDA_tunes[8] = { 9600, 9601, 9602, 9603, 9604, 9605, 9606, 9607 }; /* dethrace: Changed to size 8 */

// GLOBAL: CARM95 0x0051498c
int gCDA_is_playing;

// GLOBAL: CARM95 0x0051497c
int gServicing_sound;

// GLOBAL: CARM95 0x00514980
int gSong_repeat_count;

// GLOBAL: CARM95 0x00514984
int gSound_sources_inited;

// GLOBAL: CARM95 0x00514988
int gMusic_available;

// GLOBAL: CARM95 0x00514978
tS3_sound_tag gCDA_tag;

// GLOBAL: CARM95 0x00514990
int gCD_fully_installed;

// GLOBAL: CARM95 0x00514994
tS3_outlet_ptr gEffects_outlet;

// GLOBAL: CARM95 0x00514998
tS3_outlet_ptr gCar_outlet;

// GLOBAL: CARM95 0x0051499c
tS3_outlet_ptr gEngine_outlet;

// GLOBAL: CARM95 0x005149a0
tS3_outlet_ptr gDriver_outlet;

// GLOBAL: CARM95 0x005149a4
tS3_outlet_ptr gPedestrians_outlet;

// GLOBAL: CARM95 0x005149a8
tS3_outlet_ptr gMusic_outlet;
tS3_sound_id gMIDI_id;

// GLOBAL: CARM95 0x00550a20
tS3_outlet_ptr gIndexed_outlets[6];

// GLOBAL: CARM95 0x00538284
tU32 gLast_sound_service;

// GLOBAL: CARM95 0x00550a1c
int gCD_is_disabled;

// GLOBAL: CARM95 0x00550a00
br_vector3 gCamera_left;

// GLOBAL: CARM95 0x00550a10
br_vector3 gCamera_position;

// GLOBAL: CARM95 0x00550a40
br_vector3 gOld_camera_position;

// GLOBAL: CARM95 0x00550a50
br_vector3 gCamera_velocity;

#if defined(DETHRACE_FIX_BUGS)
extern int gS3_last_error;
#endif

// IDA: void __cdecl UsePathFileToDetermineIfFullInstallation()
// FUNCTION: CARM95 0x00463fb0
void UsePathFileToDetermineIfFullInstallation(void) {
    // changed by dethrace for compatibility
    // char line1[80];
    // char line2[80];
    // char line3[80];
    // char path_file[80];
    char line1[MAX_PATH_LENGTH];
    char line2[MAX_PATH_LENGTH];
    char line3[MAX_PATH_LENGTH];
    char path_file[MAX_PATH_LENGTH];
    FILE* fp;

    strcpy(path_file, gApplication_path);
    strcat(path_file, gDir_separator);
    strcat(path_file, "PATHS.TXT");
    if (PDCheckDriveExists(path_file) != 0) {
        fp = fopen(path_file, "rt");
        if (fp) {
            line1[0] = 0;
            line2[0] = 0;
            line3[0] = 0;
            GetALineWithNoPossibleService(fp, (unsigned char*)line1);
            GetALineWithNoPossibleService(fp, (unsigned char*)line2);
            GetALineWithNoPossibleService(fp, (unsigned char*)line3);
            fclose(fp);
            if (strcmp(line3, "Full") == 0) {
                gCD_fully_installed = 1;
            }
        }
    } else {
        gCD_fully_installed = 1;
    }
}

// IDA: void __cdecl InitSound()
// FUNCTION: CARM95 0x00464117
void InitSound(void) {
    tPath_name the_path;
    tS3_sound_tag tag;
    int engine_channel_count;
    int car_channel_count;
    int ped_channel_count;

    if (gVirgin_pass) {
        PathCat(the_path, gApplication_path, "SOUND");
        if (gProgram_state.sausage_eater_mode) {
            PathCat(the_path, the_path, "KSOUND.TXT");
        } else {
            PathCat(the_path, the_path, "SOUND.TXT");
        }
        if (gSound_override) {
            gSound_enabled = 0;
            gSound_available = 0;
        } else {
            gSound_enabled = S3Init(the_path, gAusterity_mode) == 0;
            gSound_available = gSound_enabled;
        }
        S3Set3DSoundEnvironment(1.0f / WORLD_SCALE, -1.0, -1.0);
        gVirgin_pass = 0;
        gCD_is_disabled = 0;
        UsePathFileToDetermineIfFullInstallation();
    }
    if (gSound_available == 0) {
        return;
    }
    switch (gSound_detail_level) {
    case 0:
        engine_channel_count = 2;
        car_channel_count = 2;
        ped_channel_count = 3;
        break;
    case 1:
        engine_channel_count = 2;
        car_channel_count = 3;
        ped_channel_count = 4;
        break;
    case 2:
        engine_channel_count = 6;
        car_channel_count = 4;
        ped_channel_count = 5;
        break;
    default:
        TELL_ME_IF_WE_PASS_THIS_WAY();
    }
    if (gDriver_outlet == NULL) {
        gDriver_outlet = S3CreateOutlet(1, 1);
        gIndexed_outlets[0] = gDriver_outlet;
        if (gDriver_outlet == NULL) {
            gSound_available = 0;
            return;
        }
    }
    if (!gMusic_outlet) {
        gMusic_outlet = S3CreateOutlet(1, 1);
        gIndexed_outlets[2] = gMusic_outlet;
        gMusic_available = gMusic_outlet != 0;
        DRS3SetOutletVolume(gMusic_outlet, 42 * gProgram_state.music_volume);
    }
    if (gSound_detail_level != gOld_sound_detail_level) {
        if (gCar_outlet) {
            S3ReleaseOutlet(gCar_outlet);
            gCar_outlet = 0;
        }
        if (gPedestrians_outlet) {
            S3ReleaseOutlet(gPedestrians_outlet);
            gPedestrians_outlet = 0;
        }
        if (gEngine_outlet) {
            S3ReleaseOutlet(gEngine_outlet);
            gEngine_outlet = 0;
        }
        if (gEngine_outlet == NULL) {
            gEngine_outlet = S3CreateOutlet(engine_channel_count, engine_channel_count);
            gIndexed_outlets[1] = gEngine_outlet;
            if (!gEngine_outlet) {
                gSound_available = 0;
                return;
            }
            DRS3SetOutletVolume(gEngine_outlet, 42 * gProgram_state.effects_volume);
        }
        if (gCar_outlet == NULL) {
            gCar_outlet = S3CreateOutlet(car_channel_count, car_channel_count);
            gIndexed_outlets[3] = gCar_outlet;
            if (!gCar_outlet) {
                gSound_available = 0;
                return;
            }
            DRS3SetOutletVolume(gCar_outlet, 42 * gProgram_state.effects_volume);
        }
        if (gPedestrians_outlet == NULL) {
            gPedestrians_outlet = S3CreateOutlet(ped_channel_count, ped_channel_count);
            gIndexed_outlets[4] = gPedestrians_outlet;
            if (!gPedestrians_outlet) {
                gSound_available = 0;
                return;
            }
            DRS3SetOutletVolume(gPedestrians_outlet, 42 * gProgram_state.effects_volume);
        }
    }
    if (gEffects_outlet == NULL) {
        gEffects_outlet = S3CreateOutlet(2, 2);
        gIndexed_outlets[5] = gEffects_outlet;
        if (!gEffects_outlet) {
            gSound_available = 0;
            return;
        }
        DRS3SetOutletVolume(gEffects_outlet, 42 * gProgram_state.effects_volume);
    }
    gOld_sound_detail_level = gSound_detail_level;
    SetSoundVolumes();
}

// IDA: tS3_sound_tag __usercall DRS3StartSound@<EAX>(tS3_outlet_ptr pOutlet@<EAX>, tS3_sound_id pSound@<EDX>)
// FUNCTION: CARM95 0x0046458b
tS3_sound_tag DRS3StartSound(tS3_outlet_ptr pOutlet, tS3_sound_id pSound) {
    if (!gSound_enabled) {
        return 0;
    }
    if (pSound != 1000 && (pSound < 3000 || pSound > 3007) && (pSound < 5300 || pSound > 5320)) {
        PipeSingleSound(pOutlet, pSound, 0, 0, -1, 0);
    }
    return S3StartSound(pOutlet, pSound);
}

// IDA: tS3_sound_tag __usercall DRS3StartSoundNoPiping@<EAX>(tS3_outlet_ptr pOutlet@<EAX>, tS3_sound_id pSound@<EDX>)
// FUNCTION: CARM95 0x0046461d
tS3_sound_tag DRS3StartSoundNoPiping(tS3_outlet_ptr pOutlet, tS3_sound_id pSound) {
    if (!gSound_enabled) {
        return 0;
    }
    return S3StartSound(pOutlet, pSound);
}

// IDA: tS3_sound_tag __usercall DRS3StartSound2@<EAX>(tS3_outlet_ptr pOutlet@<EAX>, tS3_sound_id pSound@<EDX>, tS3_repeats pRepeats@<EBX>, tS3_volume pLVolume@<ECX>, tS3_volume pRVolume, tS3_pitch pPitch, tS3_speed pSpeed)
// FUNCTION: CARM95 0x00464656
tS3_sound_tag DRS3StartSound2(tS3_outlet_ptr pOutlet, tS3_sound_id pSound, tS3_repeats pRepeats, tS3_volume pLVolume, tS3_volume pRVolume, tS3_pitch pPitch, tS3_speed pSpeed) {

    if (!gSound_enabled) {
        return 0;
    }
    if (pOutlet != gMusic_outlet
        && pSound != 1000
        && (pSound < 3000 || pSound > 3007)
        && (pSound < 5300 || pSound > 5320)
        && (pLVolume || pRVolume)) {
        PipeSingleSound(pOutlet, pSound, pLVolume, pRVolume, pPitch, 0);
    }
    return S3StartSound2(pOutlet, pSound, pRepeats, pLVolume, pRVolume, pPitch, pSpeed);
}

// IDA: int __usercall DRS3ChangeVolume@<EAX>(tS3_sound_tag pSound_tag@<EAX>, tS3_volume pNew_volume@<EDX>)
// FUNCTION: CARM95 0x00464724
int DRS3ChangeVolume(tS3_sound_tag pSound_tag, tS3_volume pNew_volume) {

    if (gSound_enabled == 0) {
        return 0;
    }
    return S3ChangeVolume(pSound_tag, pNew_volume);
}

// IDA: int __usercall DRS3ChangeLRVolume@<EAX>(tS3_sound_tag pSound_tag@<EAX>, tS3_volume pNew_Lvolume@<EDX>, tS3_volume pNew_Rvolume@<EBX>)
int DRS3ChangeLRVolume(tS3_sound_tag pSound_tag, tS3_volume pNew_Lvolume, tS3_volume pNew_Rvolume) {
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3ChangePitch@<EAX>(tS3_sound_tag pTag@<EAX>, tS3_pitch pNew_pitch@<EDX>)
int DRS3ChangePitch(tS3_sound_tag pTag, tS3_pitch pNew_pitch) {
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3ChangeSpeed@<EAX>(tS3_sound_tag pTag@<EAX>, tS3_pitch pNew_speed@<EDX>)
int DRS3ChangeSpeed(tS3_sound_tag pTag, tS3_pitch pNew_speed) {
    NOT_IMPLEMENTED();
    return 0;
}

// IDA: int __usercall DRS3ChangePitchSpeed@<EAX>(tS3_sound_tag pTag@<EAX>, tS3_pitch pNew_pitch@<EDX>)
// FUNCTION: CARM95 0x0046480c
int DRS3ChangePitchSpeed(tS3_sound_tag pTag, tS3_pitch pNew_pitch) {

    if (!gSound_enabled) {
        return 0;
    }
    return S3ChangePitchSpeed(pTag, pNew_pitch);
}

// IDA: int __usercall DRS3StopSound@<EAX>(tS3_sound_tag pSound_tag@<EAX>)
// FUNCTION: CARM95 0x00464845
int DRS3StopSound(tS3_sound_tag pSound_tag) {

    if (!gSound_enabled) {
        return 0;
    }
    return S3StopSound(pSound_tag);
}

// IDA: int __usercall DRS3LoadSound@<EAX>(tS3_sound_id pThe_sound@<EAX>)
// FUNCTION: CARM95 0x0046487a
int DRS3LoadSound(tS3_sound_id pThe_sound) {

    if (!gSound_enabled) {
        return 0;
    }
    return S3LoadSample(pThe_sound);
}

// IDA: int __usercall DRS3ReleaseSound@<EAX>(tS3_sound_id pThe_sound@<EAX>)
// FUNCTION: CARM95 0x004648af
int DRS3ReleaseSound(tS3_sound_id pThe_sound) {

    if (gSound_enabled == 0) {
        return 0;
    }
    return S3ReleaseSound(pThe_sound);
}

// IDA: void __cdecl DRS3Service()
// FUNCTION: CARM95 0x004648e4
void DRS3Service(void) {

    if (gSound_enabled) {
        if (gProgram_state.cockpit_on && gProgram_state.cockpit_image_index >= 0) {
            S3Service(1, 1);
        } else {
            S3Service(0, 1);
        }
    }
}

// IDA: int __usercall DRS3OutletSoundsPlaying@<EAX>(tS3_outlet_ptr pOutlet@<EAX>)
// FUNCTION: CARM95 0x0046493a
int DRS3OutletSoundsPlaying(tS3_outlet_ptr pOutlet) {
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3SoundStillPlaying@<EAX>(tS3_sound_tag pSound_tag@<EAX>)
// FUNCTION: CARM95 0x0046496f
int DRS3SoundStillPlaying(tS3_sound_tag pSound_tag) {

    return gSound_enabled && S3SoundStillPlaying(pSound_tag);
}

// IDA: void __cdecl DRS3ShutDown()
// FUNCTION: CARM95 0x004649a4
void DRS3ShutDown(void) {

    if (gSound_enabled) {
        gSound_enabled = 0;
        S3Shutdown();
    }
}

// IDA: int __usercall DRS3SetOutletVolume@<EAX>(tS3_outlet_ptr pOutlet@<EAX>, tS3_volume pVolume@<EDX>)
// FUNCTION: CARM95 0x004649cb
int DRS3SetOutletVolume(tS3_outlet_ptr pOutlet, tS3_volume pVolume) {

    if (!gSound_enabled) {
        return 0;
    }
    return S3SetOutletVolume(pOutlet, pVolume);
}

// IDA: int __usercall DRS3OverallVolume@<EAX>(tS3_volume pVolume@<EAX>)
int DRS3OverallVolume(tS3_volume pVolume) {
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3StopOutletSound@<EAX>(tS3_outlet_ptr pOutlet@<EAX>)
// FUNCTION: CARM95 0x00464a39
int DRS3StopOutletSound(tS3_outlet_ptr pOutlet) {
    if (!gSound_enabled) {
        return 0;
    }
    return S3StopOutletSound(pOutlet);
}

// IDA: int __cdecl DRS3StopAllOutletSounds()
// FUNCTION: CARM95 0x00464a6e
int DRS3StopAllOutletSounds(void) {

    if (gSound_enabled) {
        S3StopAllOutletSounds();
    }
    return 0;
}

// IDA: void __cdecl ToggleSoundEnable()
// FUNCTION: CARM95 0x00464a9c
void ToggleSoundEnable(void) {

    if (gSound_enabled) {
        S3StopAllOutletSounds();
        S3Disable();
        gSound_enabled = 0;
    } else {
        S3Enable();
        gSound_enabled = 1;
    }
}

// IDA: void __cdecl SoundService()
// FUNCTION: CARM95 0x00464adc
void SoundService(void) {
    br_matrix34 mat;

    if (gSound_enabled && !gServicing_sound) {
        gServicing_sound = 1;
        gLast_sound_service = PDGetTotalTime();
        if (gCDA_tag) {
            if (!S3IsCDAPlaying()) {
                StopMusic();
                StartMusic();
            }
        }
        DRS3Service();
        gServicing_sound = 0;
    }
}

// IDA: void __cdecl InitSoundSources()
// FUNCTION: CARM95 0x00464b51
void InitSoundSources(void) {
    int cat;
    int car_count;
    int i;
    int toggle;
    tCar_spec* the_car;

    toggle = 0;
    if (!gSound_available) {
        return;
    }
    if (!gSound_enabled) {
        ToggleSoundEnable();
        toggle = 1;
    }
    gCamera_position = *(br_vector3*)&gCamera_to_world.m[3][0];
    gCamera_left.v[0] = gCamera_to_world.m[0][0] * -1.0;
    gCamera_left.v[1] = gCamera_to_world.m[0][1] * -1.0;
    gCamera_left.v[2] = gCamera_to_world.m[0][2] * -1.0;
    S3BindListenerPositionBRender(&gCamera_position);
    S3BindListenerVelocityBRender(&gCamera_velocity);
    S3BindListenerLeftBRender(&gCamera_left);
    if (!gSound_sources_inited) {
        for (cat = eVehicle_self; cat <= eVehicle_rozzer; ++cat) {
            if (cat) {
                car_count = GetCarCount(cat);
            } else {
                car_count = 1;
            }
            for (i = 0; i < car_count; i++) {
                PossibleService();
                if (cat) {
                    the_car = GetCarSpec(cat, i);
                } else {
                    the_car = &gProgram_state.current_car;
                }
                if (the_car->driver == eDriver_local_human || gSound_detail_level == 2 || cat == eVehicle_rozzer) {
                    the_car->sound_source = S3CreateSoundSourceBR(&the_car->pos, &the_car->velocity_bu_per_sec, gEngine_outlet);
                    if (the_car->sound_source) {
                        if (cat == eVehicle_rozzer) {
                            S3BindAmbientSoundToOutlet(gEngine_outlet, 5350, the_car->sound_source, 250.0, 0, 0, 0, 0x10000, 0x10000);
                        } else {
                            S3BindAmbientSoundToOutlet(gEngine_outlet, the_car->engine_noises[0], the_car->sound_source, 250.0, 0, 0, 0, 0x10000, 0x10000);
                        }
                    }
                }
            }
        }
        gSound_sources_inited = 1;
    }
    if (toggle) {
        ToggleSoundEnable();
    }
}

// IDA: void __cdecl DisposeSoundSources()
// FUNCTION: CARM95 0x00464d79
void DisposeSoundSources(void) {
    int cat;
    int car_count;
    int i;
    int toggle;
    tCar_spec* the_car;

    toggle = 0;
    if (gSound_available == 0) {
        return;
    }
    if (!gSound_enabled) {
        ToggleSoundEnable();
        toggle = 1;
    }
    if (gSound_sources_inited) {
        DRS3StopOutletSound(gEngine_outlet);
        if (gProgram_state.cockpit_on && gProgram_state.cockpit_image_index >= 0) {
            S3Service(1, 0);
        } else {
            S3Service(0, 0);
        }
        for (cat = eVehicle_self; cat <= eVehicle_rozzer; cat++) {
            if (cat) {
                car_count = GetCarCount(cat);
            } else {
                car_count = 1;
            }
            for (i = 0; i < car_count; ++i) {
                if (cat) {
                    the_car = GetCarSpec(cat, i);
                } else {
                    the_car = &gProgram_state.current_car;
                }
                if (the_car->driver == eDriver_local_human || gSound_detail_level == 2 || cat == eVehicle_rozzer) {
                    if (the_car->sound_source) {
                        S3UpdateSoundSource(gEngine_outlet, -1, the_car->sound_source, 0.0f, 0, 0, 0, 0x10000, 0x10000);
                        S3ReleaseSoundSource(the_car->sound_source);
                    }
                    the_car->sound_source = NULL;
                }
            }
        }
        gSound_sources_inited = 0;
    }
    if (toggle) {
        ToggleSoundEnable();
    }
}

// IDA: tS3_sound_tag __usercall DRS3StartSound3D@<EAX>(tS3_outlet_ptr pOutlet@<EAX>, tS3_sound_id pSound@<EDX>, br_vector3 *pInitial_position@<EBX>, br_vector3 *pInitial_velocity@<ECX>, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed)
// FUNCTION: CARM95 0x00464f39
tS3_sound_tag DRS3StartSound3D(tS3_outlet_ptr pOutlet, tS3_sound_id pSound, br_vector3* pInitial_position, br_vector3* pInitial_velocity, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed) {
    tS3_sound_tag tag;

    if (!gSound_enabled) {
        return 0;
    }
    if (pVolume && pSound != 1000 && (pSound < 3000 || pSound > 3007) && (pSound < 5300 || pSound > 5320)) {
        PipeSingleSound(pOutlet, pSound, pVolume, 0, pPitch, pInitial_position);
    }
    return S3StartSound3D(pOutlet, pSound, (tS3_vector3*)pInitial_position, (tS3_vector3*)pInitial_velocity, pRepeats, pVolume, pPitch, pSpeed);
}

// IDA: tS3_sound_tag __usercall DRS3StartSoundFromSource3@<EAX>(tS3_sound_source_ptr pSource@<EAX>, tS3_sound_id pSound@<EDX>, tS3_repeats pRepeats@<EBX>, tS3_volume pVolume@<ECX>, tS3_pitch pPitch, tS3_speed pSpeed)
tS3_sound_tag DRS3StartSoundFromSource3(tS3_sound_source_ptr pSource, tS3_sound_id pSound, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed) {
    tS3_sound_tag tag;
    NOT_IMPLEMENTED();
}

// IDA: tS3_sound_tag __usercall DRS3StartSoundFromSource@<EAX>(tS3_sound_source_ptr pSource@<EAX>, tS3_sound_id pSound@<EDX>)
tS3_sound_tag DRS3StartSoundFromSource(tS3_sound_source_ptr pSource, tS3_sound_id pSound) {
    tS3_sound_tag tag;
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MungeEngineNoise()
// FUNCTION: CARM95 0x00465090
void MungeEngineNoise(void) {
    tCar_spec* the_car;
    tU32 pitch;
    int vol;
    int cat;
    int car_count;
    int i;
    int stop_all;
    int type_of_engine_noise;
    tS3_sound_id engine_noise;

    // added by dethrace
    tU32 frame_period;
    frame_period = gFrame_period;

#ifdef DETHRACE_FIX_BUGS
    // At framerates higher than 30, `gCamera_velocity` is not stable enough and causes the player car audio to stumble
    // as the pitch shifts up and down while applying doppler effect in `S3Calculate3D`.
    // We avoid the issue by only updating the engine sounds every MUNGE_ENGINE_INTERVAL ms
    static tU32 dethrace_last_executed = 0;

    tU32 now = GetTotalTime();
    if (now - dethrace_last_executed < MUNGE_ENGINE_INTERVAL) {
        return;
    }
    frame_period = now - dethrace_last_executed;
    dethrace_last_executed = now;
#endif

    type_of_engine_noise = 0;
    if (gSound_available == 0 || gProgram_state.racing == 0) {
        return;
    }

    BrVector3Copy(&gCamera_position, (br_vector3*)&gCamera_to_world.m[3][0]);
    BrVector3Negate(&gCamera_left, (br_vector3*)&gCamera_to_world.m[0][0]);
    BrVector3Sub(&gCamera_velocity, &gCamera_position, &gOld_camera_position);
    if (frame_period) {
        BrVector3InvScale(&gCamera_velocity, &gCamera_velocity, ((float)frame_period / 1000.0f));
    } else {
        BrVector3Set(&gCamera_velocity, 0.0f, 0.0f, 0.0f);
    }
    BrVector3Copy(&gOld_camera_position, &gCamera_position);
    stop_all = (gAction_replay_mode && (fabs(GetReplayRate()) > 1.0f || GetReplayRate() == 0.0f)) || gFaded_palette || gPalette_fade_time;
    for (cat = eVehicle_self; cat <= eVehicle_rozzer; cat++) {
        if (cat) {
            car_count = GetCarCount(cat);
        } else {
            car_count = 1;
        }
        for (i = 0; i < car_count; i++) {
            if (cat) {
                the_car = GetCarSpec(cat, i);
            } else {
                the_car = &gProgram_state.current_car;
            }
            if (the_car->driver == eDriver_local_human || gSound_detail_level == 2 || cat == eVehicle_rozzer) {
                if (stop_all || !the_car->active || the_car->knackered || (cat == eVehicle_rozzer && BrVector3LengthSquared(&the_car->v) < 0.0001f)) {
                    vol = 0;
                    pitch = 0x10000;
                } else {
                    BrVector3InvScale(&the_car->velocity_bu_per_sec, &the_car->v, WORLD_SCALE);
                    if (cat == eVehicle_rozzer) {
                        vol = 255;
                        pitch = 0x10000;
                    } else {
                        if (the_car->last_special_volume) {
                            type_of_engine_noise = the_car->last_special_volume->engine_noise_index;
                        }
                        pitch = the_car->revs * 10.0f + 40960.0f;
                        if (gAction_replay_mode) {
                            pitch = fabs(GetReplayRate()) * pitch;
                        }
                        if (type_of_engine_noise == 1) {
                            pitch = pitch * 0.75f;
                        } else if (type_of_engine_noise == 2) {
                            pitch = pitch * 0.55f;
                        }

                        pitch = MAX(pitch, 4096);
                        pitch = MIN(pitch, 131072);

                        vol = the_car->revs * 0.001f + 64.0f;
                        if (type_of_engine_noise == 1) {
                            vol = vol * 5.0f;
                        } else if (type_of_engine_noise == 2) {
                            vol = vol * 2.0f;
                        } else {
                            vol = vol * 2.5f;
                        }
                        vol = MIN(vol, 255);
                    }
                }
                S3UpdateSoundSource(gEngine_outlet, -1, the_car->sound_source, gAction_replay_mode == 0 ? 250.0f : 300.0f, 0, 0, vol, pitch, 0x10000);
            }
        }
    }
    SoundService();
}

// IDA: void __cdecl SetSoundVolumes()
// FUNCTION: CARM95 0x004655d8
void SetSoundVolumes(void) {
    if (gSound_enabled) {
        if (gEffects_outlet != NULL) {
            DRS3SetOutletVolume(gEffects_outlet, 42 * gProgram_state.effects_volume);
        }
        DRS3SetOutletVolume(gCar_outlet, 42 * gProgram_state.effects_volume);
        DRS3SetOutletVolume(gEngine_outlet, 42 * gProgram_state.effects_volume);
        DRS3SetOutletVolume(gDriver_outlet, 42 * gProgram_state.effects_volume);
        DRS3SetOutletVolume(gPedestrians_outlet, 42 * gProgram_state.effects_volume);
        DRS3SetOutletVolume(gMusic_outlet, 42 * gProgram_state.music_volume);
    }
}

// IDA: tS3_outlet_ptr __usercall GetOutletFromIndex@<EAX>(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004656b1
tS3_outlet_ptr GetOutletFromIndex(int pIndex) {
    return gIndexed_outlets[pIndex];
}

// IDA: int __usercall GetIndexFromOutlet@<EAX>(tS3_outlet_ptr pOutlet@<EAX>)
// FUNCTION: CARM95 0x004656cb
int GetIndexFromOutlet(tS3_outlet_ptr pOutlet) {
    int i;

    for (i = 0; i < COUNT_OF(gIndexed_outlets); i++) {
        if (gIndexed_outlets[i] == pOutlet) {
            return i;
        }
    }
    return 0;
}

// IDA: int __usercall DRS3StartCDA@<EAX>(tS3_sound_id pCDA_id@<EAX>)
// FUNCTION: CARM95 0x00465719
int DRS3StartCDA(tS3_sound_id pCDA_id) {

    if (!gCD_is_disabled && gMusic_available) {
        if (!gCDA_is_playing && !gCDA_tag) {
            if (S3CDAEnabled()) {
                S3StopOutletSound(gMusic_outlet);
                if (gSound_enabled) {
                    if (gProgram_state.cockpit_on && gProgram_state.cockpit_image_index >= 0) {
                        S3Service(1, 0);
                    } else {
                        S3Service(0, 0);
                    }
#if defined(DETHRACE_FIX_BUGS)
                    int random_track = pCDA_id == 9999;
                    int retries_remaining = 5;
retry_start_music:
#endif
                    if (pCDA_id == 9999) {
                        do {
                            pCDA_id = gRandom_CDA_tunes[IRandomBetween(0, 7)];
                        } while (pCDA_id == gLast_tune);
                    }
                    gLast_tune = pCDA_id;

                    gCDA_tag = DRS3StartSoundNoPiping(gMusic_outlet, pCDA_id);
#if defined(DETHRACE_FIX_BUGS)
                    if (gCDA_tag == 0 && gS3_last_error == eS3_error_bad_id && random_track && retries_remaining > 0) {
                        retries_remaining--;
                        // Retry with a random tune if the CD track is missing, instead of disabling music completely
                        pCDA_id = 9999;
                        goto retry_start_music;
                    }
#endif
#if defined(DETHRACE_FIX_BUGS)
                    // Initial CD music volume was not set correctly
                    DRS3SetOutletVolume(gMusic_outlet, 42 * gProgram_state.music_volume);
#endif
                    gCDA_is_playing = gCDA_tag != 0;
                    if (gCDA_tag == 0) {
                        gCD_is_disabled = 1;
                        S3DisableCDA();
                        printf("CD music disabled - no CD or CD drive found.\n");
                        fflush(stdout);
                    }
                    gSong_repeat_count = 0;
                }
            }
        }
    }
    return gCDA_is_playing;
}

// IDA: int __cdecl DRS3StopCDA()
// FUNCTION: CARM95 0x00465848
int DRS3StopCDA(void) {

    if (gMusic_available && gCDA_tag != 0) {
        S3StopSound(gCDA_tag);
        gCDA_is_playing = 0;
        gCDA_tag = 0;
    }
    return gCDA_is_playing;
}

// IDA: void __cdecl StartMusic()
// FUNCTION: CARM95 0x00465899
void StartMusic(void) {
    if (gCD_fully_installed) {
        gCDA_is_playing = DRS3StartCDA(9999);
    }
}

// IDA: void __cdecl StopMusic()
// FUNCTION: CARM95 0x004658c3
void StopMusic(void) {
    if (gCD_fully_installed) {
        DRS3StopCDA();
    }
}
