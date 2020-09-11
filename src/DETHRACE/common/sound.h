#ifndef _SOUND_H_
#define _SOUND_H_

#include "br_types.h"
#include "dr_types.h"

extern tS3_outlet_ptr gIndexed_outlets[6];
extern int gRandom_CDA_tunes[7];
extern tU32 gLast_sound_service;
extern int gSong_repeat_count;
extern int gServicing_sound;
extern int gCD_fully_installed;
extern int gCD_is_disabled;
extern int gCDA_is_playing;
extern int gSound_sources_inited;
extern tS3_sound_tag gCDA_tag;
extern tS3_outlet_ptr gEngine_outlet;
extern int gRandom_Rockin_MIDI_tunes[3];
extern int gOld_sound_detail_level;
extern int gLast_tune;
extern int gVirgin_pass;
extern br_vector3 gCamera_left;
extern int gRandom_MIDI_tunes[3];
extern br_vector3 gCamera_position;
extern br_vector3 gOld_camera_position;
extern br_vector3 gCamera_velocity;
extern int gSound_detail_level;
extern tS3_outlet_ptr gDriver_outlet;
extern tS3_outlet_ptr gPedestrians_outlet;
extern tS3_outlet_ptr gCar_outlet;
extern tS3_sound_id gMIDI_id;
extern int gMusic_available;
extern tS3_outlet_ptr gMusic_outlet;
extern tS3_outlet_ptr gEffects_outlet;

void UsePathFileToDetermineIfFullInstallation();

void InitSound();

tS3_sound_tag DRS3StartSound(tS3_outlet_ptr pOutlet, tS3_sound_id pSound);

tS3_sound_tag DRS3StartSoundNoPiping(tS3_outlet_ptr pOutlet, tS3_sound_id pSound);

tS3_sound_tag DRS3StartSound2(tS3_outlet_ptr pOutlet, tS3_sound_id pSound, tS3_repeats pRepeats, tS3_volume pLVolume, tS3_volume pRVolume, tS3_pitch pPitch, tS3_speed pSpeed);

int DRS3ChangeVolume(tS3_sound_tag pSound_tag, tS3_volume pNew_volume);

int DRS3ChangeLRVolume(tS3_sound_tag pSound_tag, tS3_volume pNew_Lvolume, tS3_volume pNew_Rvolume);

int DRS3ChangePitch(tS3_sound_tag pTag, tS3_pitch pNew_pitch);

int DRS3ChangeSpeed(tS3_sound_tag pTag, tS3_pitch pNew_speed);

int DRS3ChangePitchSpeed(tS3_sound_tag pTag, tS3_pitch pNew_pitch);

int DRS3StopSound(tS3_sound_tag pSound_tag);

int DRS3LoadSound(tS3_sound_id pThe_sound);

int DRS3ReleaseSound(tS3_sound_id pThe_sound);

void DRS3Service();

int DRS3OutletSoundsPlaying(tS3_outlet_ptr pOutlet);

int DRS3SoundStillPlaying(tS3_sound_tag pSound_tag);

void DRS3ShutDown();

int DRS3SetOutletVolume(tS3_outlet_ptr pOutlet, tS3_volume pVolume);

int DRS3OverallVolume(tS3_volume pVolume);

int DRS3StopOutletSound(tS3_outlet_ptr pOutlet);

int DRS3StopAllOutletSounds();

void ToggleSoundEnable();

void SoundService();

void InitSoundSources();

void DisposeSoundSources();

tS3_sound_tag DRS3StartSound3D(tS3_outlet_ptr pOutlet, tS3_sound_id pSound, br_vector3* pInitial_position, br_vector3* pInitial_velocity, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed);

tS3_sound_tag DRS3StartSoundFromSource3(tS3_sound_source_ptr pSource, tS3_sound_id pSound, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed);

tS3_sound_tag DRS3StartSoundFromSource(tS3_sound_source_ptr pSource, tS3_sound_id pSound);

void MungeEngineNoise();

void SetSoundVolumes();

tS3_outlet_ptr GetOutletFromIndex(int pIndex);

int GetIndexFromOutlet(tS3_outlet_ptr pOutlet);

int DRS3StartCDA(tS3_sound_id pCDA_id);

int DRS3StopCDA();

void StartMusic();

void StopMusic();

#endif
