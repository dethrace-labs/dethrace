#include "sound.h"

#include <stdlib.h>
#include <string.h>

#include "globvars.h"
#include "utility.h"

tS3_outlet_ptr gIndexed_outlets[6];
int gRandom_CDA_tunes[7];
tU32 gLast_sound_service;
int gSong_repeat_count;
int gServicing_sound;
int gCD_fully_installed;
int gCD_is_disabled;
int gCDA_is_playing;
int gSound_sources_inited;
tS3_sound_tag gCDA_tag;
tS3_outlet_ptr gEngine_outlet;
int gRandom_Rockin_MIDI_tunes[3];
int gOld_sound_detail_level;
int gLast_tune;
int gVirgin_pass;
br_vector3 gCamera_left;
int gRandom_MIDI_tunes[3];
br_vector3 gCamera_position;
br_vector3 gOld_camera_position;
br_vector3 gCamera_velocity;
int gSound_detail_level;
tS3_outlet_ptr gDriver_outlet;
tS3_outlet_ptr gPedestrians_outlet;
tS3_outlet_ptr gCar_outlet;
tS3_sound_id gMIDI_id;
int gMusic_available;
tS3_outlet_ptr gMusic_outlet;
tS3_outlet_ptr gEffects_outlet;

void UsePathFileToDetermineIfFullInstallation() {
    char line2[80];
    char line3[80];
    char path_file[80];
    FILE* fp;

    strcpy(path_file, gApplication_path);
    strcat(path_file, gDir_separator);
    strcat(path_file, "PATHS.TXT");

    if (PDCheckDriveExists(path_file) == 0) {
        fp = fopen(path_file, "rt");
        if (fp != NULL) {
            if (GetALineWithNoPossibleService(fp, line2) && GetALineWithNoPossibleService(fp, line2) && GetALineWithNoPossibleService(fp, line3) && strcmp(line3, "Full") == 0) {

                gCD_fully_installed = 1;
            }
            fclose(fp);
        }
    }
}

void InitSound() {
    tS3_sound_tag tag;
    int engine_channel_count;
    int car_channel_count;
    int ped_channel_count;

    LOG_WARN("Not implmented");
}

tS3_sound_tag DRS3StartSound(tS3_outlet_ptr pOutlet, tS3_sound_id pSound) {
    STUB();
}

tS3_sound_tag DRS3StartSoundNoPiping(tS3_outlet_ptr pOutlet, tS3_sound_id pSound) {
    STUB();
}

tS3_sound_tag DRS3StartSound2(tS3_outlet_ptr pOutlet, tS3_sound_id pSound, tS3_repeats pRepeats, tS3_volume pLVolume, tS3_volume pRVolume, tS3_pitch pPitch, tS3_speed pSpeed) {
    NOT_IMPLEMENTED();
}

int DRS3ChangeVolume(tS3_sound_tag pSound_tag, tS3_volume pNew_volume) {
    NOT_IMPLEMENTED();
}

int DRS3ChangeLRVolume(tS3_sound_tag pSound_tag, tS3_volume pNew_Lvolume, tS3_volume pNew_Rvolume) {
    NOT_IMPLEMENTED();
}

int DRS3ChangePitch(tS3_sound_tag pTag, tS3_pitch pNew_pitch) {
    NOT_IMPLEMENTED();
}

int DRS3ChangeSpeed(tS3_sound_tag pTag, tS3_pitch pNew_speed) {
    NOT_IMPLEMENTED();
}

int DRS3ChangePitchSpeed(tS3_sound_tag pTag, tS3_pitch pNew_pitch) {
    NOT_IMPLEMENTED();
}

int DRS3StopSound(tS3_sound_tag pSound_tag) {
    NOT_IMPLEMENTED();
}

int DRS3LoadSound(tS3_sound_id pThe_sound) {
    NOT_IMPLEMENTED();
}

int DRS3ReleaseSound(tS3_sound_id pThe_sound) {
    NOT_IMPLEMENTED();
}

void DRS3Service() {
    NOT_IMPLEMENTED();
}

int DRS3OutletSoundsPlaying(tS3_outlet_ptr pOutlet) {
    NOT_IMPLEMENTED();
}

int DRS3SoundStillPlaying(tS3_sound_tag pSound_tag) {
    NOT_IMPLEMENTED();
}

void DRS3ShutDown() {
    NOT_IMPLEMENTED();
}

int DRS3SetOutletVolume(tS3_outlet_ptr pOutlet, tS3_volume pVolume) {
    NOT_IMPLEMENTED();
}

int DRS3OverallVolume(tS3_volume pVolume) {
    NOT_IMPLEMENTED();
}

int DRS3StopOutletSound(tS3_outlet_ptr pOutlet) {
    STUB();
}

int DRS3StopAllOutletSounds() {
    NOT_IMPLEMENTED();
}

void ToggleSoundEnable() {
    NOT_IMPLEMENTED();
}

void SoundService() {
    br_matrix34 mat;
    SILENT_STUB();
}

void InitSoundSources() {
    int car_count;
    int i;
    int toggle;
    tCar_spec* the_car;
    NOT_IMPLEMENTED();
}

void DisposeSoundSources() {
    int car_count;
    int i;
    int toggle;
    tCar_spec* the_car;
    NOT_IMPLEMENTED();
}

tS3_sound_tag DRS3StartSound3D(tS3_outlet_ptr pOutlet, tS3_sound_id pSound, br_vector3* pInitial_position, br_vector3* pInitial_velocity, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed) {
    tS3_sound_tag tag;
    NOT_IMPLEMENTED();
}

tS3_sound_tag DRS3StartSoundFromSource3(tS3_sound_source_ptr pSource, tS3_sound_id pSound, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed) {
    tS3_sound_tag tag;
    NOT_IMPLEMENTED();
}

tS3_sound_tag DRS3StartSoundFromSource(tS3_sound_source_ptr pSource, tS3_sound_id pSound) {
    tS3_sound_tag tag;
    NOT_IMPLEMENTED();
}

void MungeEngineNoise() {
    tU32 pitch;
    int vol;
    int cat;
    int car_count;
    int i;
    int stop_all;
    int type_of_engine_noise;
    tS3_sound_id engine_noise;
    STUB();
}

void SetSoundVolumes() {
    STUB();
}

tS3_outlet_ptr GetOutletFromIndex(int pIndex) {
    NOT_IMPLEMENTED();
}

int GetIndexFromOutlet(tS3_outlet_ptr pOutlet) {
    int i;
    NOT_IMPLEMENTED();
}

int DRS3StartCDA(tS3_sound_id pCDA_id) {
    NOT_IMPLEMENTED();
}

int DRS3StopCDA() {
    NOT_IMPLEMENTED();
}

void StartMusic() {
}

void StopMusic() {
}
