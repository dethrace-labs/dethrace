#include "sound.h"

#include <stdlib.h>
#include <string.h>

#include "globvars.h"
#include "harness/trace.h"
#include "piping.h"
#include "s3/s3sound.h"
#include "utility.h"

int gSound_detail_level;
int gVirgin_pass;
int gOld_sound_detail_level;
int gLast_tune;
int gRandom_MIDI_tunes[3];
int gRandom_Rockin_MIDI_tunes[3];
int gRandom_CDA_tunes[7];
int gCDA_is_playing;
int gServicing_sound;
int gSong_repeat_count;
int gSound_sources_inited;
int gMusic_available;
tS3_sound_tag gCDA_tag;
int gCD_fully_installed;
tS3_outlet_ptr gEffects_outlet;
tS3_outlet_ptr gCar_outlet;
tS3_outlet_ptr gEngine_outlet;
tS3_outlet_ptr gDriver_outlet;
tS3_outlet_ptr gPedestrians_outlet;
tS3_outlet_ptr gMusic_outlet;
tS3_sound_id gMIDI_id;
tS3_outlet_ptr gIndexed_outlets[6];
tU32 gLast_sound_service;
int gCD_is_disabled;
br_vector3 gCamera_left;
br_vector3 gCamera_position;
br_vector3 gOld_camera_position;
br_vector3 gCamera_velocity;

// IDA: void __cdecl UsePathFileToDetermineIfFullInstallation()
void UsePathFileToDetermineIfFullInstallation() {
    char line2[80];
    char line3[80];
    char path_file[80];
    FILE* fp;

    strcpy(path_file, gApplication_path);
    strcat(path_file, gDir_separator);
    strcat(path_file, "PATHS.TXT");

    if (PDCheckDriveExists(path_file)) {
        fp = fopen(path_file, "rt");
        if (fp != NULL) {
            if (GetALineWithNoPossibleService(fp, line2) && GetALineWithNoPossibleService(fp, line2) && GetALineWithNoPossibleService(fp, line3) && strcmp(line3, "Full") == 0) {
                gCD_fully_installed = 1;
            }
            fclose(fp);
        }
    }
}

// IDA: void __cdecl InitSound()
void InitSound() {
    tS3_sound_tag tag;
    int engine_channel_count;
    int car_channel_count;
    int ped_channel_count;

    LOG_WARN("Not implmented");
}

// IDA: tS3_sound_tag __usercall DRS3StartSound@<EAX>(tS3_outlet_ptr pOutlet@<EAX>, tS3_sound_id pSound@<EDX>)
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
tS3_sound_tag DRS3StartSoundNoPiping(tS3_outlet_ptr pOutlet, tS3_sound_id pSound) {
    STUB();
    return 0;
}

// IDA: tS3_sound_tag __usercall DRS3StartSound2@<EAX>(tS3_outlet_ptr pOutlet@<EAX>, tS3_sound_id pSound@<EDX>, tS3_repeats pRepeats@<EBX>, tS3_volume pLVolume@<ECX>, tS3_volume pRVolume, tS3_pitch pPitch, tS3_speed pSpeed)
tS3_sound_tag DRS3StartSound2(tS3_outlet_ptr pOutlet, tS3_sound_id pSound, tS3_repeats pRepeats, tS3_volume pLVolume, tS3_volume pRVolume, tS3_pitch pPitch, tS3_speed pSpeed) {
    LOG_TRACE("(%d, %d, %d, %d, %d, %d, %d)", pOutlet, pSound, pRepeats, pLVolume, pRVolume, pPitch, pSpeed);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3ChangeVolume@<EAX>(tS3_sound_tag pSound_tag@<EAX>, tS3_volume pNew_volume@<EDX>)
int DRS3ChangeVolume(tS3_sound_tag pSound_tag, tS3_volume pNew_volume) {
    LOG_TRACE("(%d, %d)", pSound_tag, pNew_volume);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3ChangeLRVolume@<EAX>(tS3_sound_tag pSound_tag@<EAX>, tS3_volume pNew_Lvolume@<EDX>, tS3_volume pNew_Rvolume@<EBX>)
int DRS3ChangeLRVolume(tS3_sound_tag pSound_tag, tS3_volume pNew_Lvolume, tS3_volume pNew_Rvolume) {
    LOG_TRACE("(%d, %d, %d)", pSound_tag, pNew_Lvolume, pNew_Rvolume);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3ChangePitch@<EAX>(tS3_sound_tag pTag@<EAX>, tS3_pitch pNew_pitch@<EDX>)
int DRS3ChangePitch(tS3_sound_tag pTag, tS3_pitch pNew_pitch) {
    LOG_TRACE("(%d, %d)", pTag, pNew_pitch);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3ChangeSpeed@<EAX>(tS3_sound_tag pTag@<EAX>, tS3_pitch pNew_speed@<EDX>)
int DRS3ChangeSpeed(tS3_sound_tag pTag, tS3_pitch pNew_speed) {
    LOG_TRACE("(%d, %d)", pTag, pNew_speed);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3ChangePitchSpeed@<EAX>(tS3_sound_tag pTag@<EAX>, tS3_pitch pNew_pitch@<EDX>)
int DRS3ChangePitchSpeed(tS3_sound_tag pTag, tS3_pitch pNew_pitch) {
    LOG_TRACE("(%d, %d)", pTag, pNew_pitch);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3StopSound@<EAX>(tS3_sound_tag pSound_tag@<EAX>)
int DRS3StopSound(tS3_sound_tag pSound_tag) {
    LOG_TRACE("(%d)", pSound_tag);

    if (gSound_enabled) {
        return S3StopSound(pSound_tag);
    } else {
        return 0;
    }
}

// IDA: int __usercall DRS3LoadSound@<EAX>(tS3_sound_id pThe_sound@<EAX>)
int DRS3LoadSound(tS3_sound_id pThe_sound) {
    LOG_TRACE("(%d)", pThe_sound);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3ReleaseSound@<EAX>(tS3_sound_id pThe_sound@<EAX>)
int DRS3ReleaseSound(tS3_sound_id pThe_sound) {
    LOG_TRACE("(%d)", pThe_sound);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DRS3Service()
void DRS3Service() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3OutletSoundsPlaying@<EAX>(tS3_outlet_ptr pOutlet@<EAX>)
int DRS3OutletSoundsPlaying(tS3_outlet_ptr pOutlet) {
    LOG_TRACE("(%d)", pOutlet);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3SoundStillPlaying@<EAX>(tS3_sound_tag pSound_tag@<EAX>)
int DRS3SoundStillPlaying(tS3_sound_tag pSound_tag) {
    LOG_TRACE("(%d)", pSound_tag);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DRS3ShutDown()
void DRS3ShutDown() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3SetOutletVolume@<EAX>(tS3_outlet_ptr pOutlet@<EAX>, tS3_volume pVolume@<EDX>)
int DRS3SetOutletVolume(tS3_outlet_ptr pOutlet, tS3_volume pVolume) {
    LOG_TRACE("(%d, %d)", pOutlet, pVolume);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3OverallVolume@<EAX>(tS3_volume pVolume@<EAX>)
int DRS3OverallVolume(tS3_volume pVolume) {
    LOG_TRACE("(%d)", pVolume);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3StopOutletSound@<EAX>(tS3_outlet_ptr pOutlet@<EAX>)
int DRS3StopOutletSound(tS3_outlet_ptr pOutlet) {
    STUB();
    return 0;
}

// IDA: int __cdecl DRS3StopAllOutletSounds()
int DRS3StopAllOutletSounds() {
    LOG_TRACE("()");
    STUB();
    return 0;
}

// IDA: void __cdecl ToggleSoundEnable()
void ToggleSoundEnable() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SoundService()
void SoundService() {
    br_matrix34 mat;
    STUB_ONCE();
}

// IDA: void __cdecl InitSoundSources()
void InitSoundSources() {
    int cat;
    int car_count;
    int i;
    int toggle;
    tCar_spec* the_car;
    LOG_TRACE("()");

    STUB();
}

// IDA: void __cdecl DisposeSoundSources()
void DisposeSoundSources() {
    int cat;
    int car_count;
    int i;
    int toggle;
    tCar_spec* the_car;
    LOG_TRACE("()");

    STUB();
}

// IDA: tS3_sound_tag __usercall DRS3StartSound3D@<EAX>(tS3_outlet_ptr pOutlet@<EAX>, tS3_sound_id pSound@<EDX>, br_vector3 *pInitial_position@<EBX>, br_vector3 *pInitial_velocity@<ECX>, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed)
tS3_sound_tag DRS3StartSound3D(tS3_outlet_ptr pOutlet, tS3_sound_id pSound, br_vector3* pInitial_position, br_vector3* pInitial_velocity, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed) {
    tS3_sound_tag tag;
    LOG_TRACE("(%d, %d, %p, %p, %d, %d, %d, %d)", pOutlet, pSound, pInitial_position, pInitial_velocity, pRepeats, pVolume, pPitch, pSpeed);
    NOT_IMPLEMENTED();
}

// IDA: tS3_sound_tag __usercall DRS3StartSoundFromSource3@<EAX>(tS3_sound_source_ptr pSource@<EAX>, tS3_sound_id pSound@<EDX>, tS3_repeats pRepeats@<EBX>, tS3_volume pVolume@<ECX>, tS3_pitch pPitch, tS3_speed pSpeed)
tS3_sound_tag DRS3StartSoundFromSource3(tS3_sound_source_ptr pSource, tS3_sound_id pSound, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed) {
    tS3_sound_tag tag;
    LOG_TRACE("(%d, %d, %d, %d, %d, %d)", pSource, pSound, pRepeats, pVolume, pPitch, pSpeed);
    NOT_IMPLEMENTED();
}

// IDA: tS3_sound_tag __usercall DRS3StartSoundFromSource@<EAX>(tS3_sound_source_ptr pSource@<EAX>, tS3_sound_id pSound@<EDX>)
tS3_sound_tag DRS3StartSoundFromSource(tS3_sound_source_ptr pSource, tS3_sound_id pSound) {
    tS3_sound_tag tag;
    LOG_TRACE("(%d, %d)", pSource, pSound);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MungeEngineNoise()
void MungeEngineNoise() {
    tU32 pitch;
    int vol;
    int cat;
    int car_count;
    int i;
    int stop_all;
    int type_of_engine_noise;
    tS3_sound_id engine_noise;
    STUB_ONCE();
}

// IDA: void __cdecl SetSoundVolumes()
void SetSoundVolumes() {
    STUB();
}

// IDA: tS3_outlet_ptr __usercall GetOutletFromIndex@<EAX>(int pIndex@<EAX>)
tS3_outlet_ptr GetOutletFromIndex(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GetIndexFromOutlet@<EAX>(tS3_outlet_ptr pOutlet@<EAX>)
int GetIndexFromOutlet(tS3_outlet_ptr pOutlet) {
    int i;
    LOG_TRACE("(%d)", pOutlet);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRS3StartCDA@<EAX>(tS3_sound_id pCDA_id@<EAX>)
int DRS3StartCDA(tS3_sound_id pCDA_id) {
    LOG_TRACE("(%d)", pCDA_id);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl DRS3StopCDA()
int DRS3StopCDA() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl StartMusic()
void StartMusic() {
}

// IDA: void __cdecl StopMusic()
void StopMusic() {
}
