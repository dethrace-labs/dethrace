#ifndef _SOUND_H_
#define _SOUND_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 224
void UsePathFileToDetermineIfFullInstallation();

// Offset: 224
// Size: 976
void InitSound();

// Offset: 1200
// Size: 145
// EAX: pOutlet
// EDX: pSound
tS3_sound_tag DRS3StartSound(tS3_outlet_ptr pOutlet, tS3_sound_id pSound);

// Offset: 1348
// Size: 73
// EAX: pOutlet
// EDX: pSound
tS3_sound_tag DRS3StartSoundNoPiping(tS3_outlet_ptr pOutlet, tS3_sound_id pSound);

// Offset: 1424
// Size: 206
// EAX: pOutlet
// EDX: pSound
// EBX: pRepeats
// ECX: pLVolume
tS3_sound_tag DRS3StartSound2(tS3_outlet_ptr pOutlet, tS3_sound_id pSound, tS3_repeats pRepeats, tS3_volume pLVolume, tS3_volume pRVolume, tS3_pitch pPitch, tS3_speed pSpeed);

// Offset: 1632
// Size: 73
// EAX: pSound_tag
// EDX: pNew_volume
int DRS3ChangeVolume(tS3_sound_tag pSound_tag, tS3_volume pNew_volume);

// Offset: 1708
// Size: 77
// EAX: pSound_tag
// EDX: pNew_Lvolume
// EBX: pNew_Rvolume
int DRS3ChangeLRVolume(tS3_sound_tag pSound_tag, tS3_volume pNew_Lvolume, tS3_volume pNew_Rvolume);

// Offset: 1788
// Size: 73
// EAX: pTag
// EDX: pNew_pitch
int DRS3ChangePitch(tS3_sound_tag pTag, tS3_pitch pNew_pitch);

// Offset: 1864
// Size: 73
// EAX: pTag
// EDX: pNew_speed
int DRS3ChangeSpeed(tS3_sound_tag pTag, tS3_pitch pNew_speed);

// Offset: 1940
// Size: 73
// EAX: pTag
// EDX: pNew_pitch
int DRS3ChangePitchSpeed(tS3_sound_tag pTag, tS3_pitch pNew_pitch);

// Offset: 2016
// Size: 69
// EAX: pSound_tag
int DRS3StopSound(tS3_sound_tag pSound_tag);

// Offset: 2088
// Size: 69
// EAX: pThe_sound
int DRS3LoadSound(tS3_sound_id pThe_sound);

// Offset: 2160
// Size: 69
// EAX: pThe_sound
int DRS3ReleaseSound(tS3_sound_id pThe_sound);

// Offset: 2232
// Size: 97
void DRS3Service();

// Offset: 2332
// Size: 69
// EAX: pOutlet
int DRS3OutletSoundsPlaying(tS3_outlet_ptr pOutlet);

// Offset: 2404
// Size: 69
// EAX: pSound_tag
int DRS3SoundStillPlaying(tS3_sound_tag pSound_tag);

// Offset: 2476
// Size: 58
void DRS3ShutDown();

// Offset: 2536
// Size: 73
// EAX: pOutlet
// EDX: pVolume
int DRS3SetOutletVolume(tS3_outlet_ptr pOutlet, tS3_volume pVolume);

// Offset: 2612
// Size: 69
// EAX: pVolume
int DRS3OverallVolume(tS3_volume pVolume);

// Offset: 2684
// Size: 69
// EAX: pOutlet
int DRS3StopOutletSound(tS3_outlet_ptr pOutlet);

// Offset: 2756
// Size: 63
int DRS3StopAllOutletSounds();

// Offset: 2820
// Size: 80
void ToggleSoundEnable();

// Offset: 2900
// Size: 125
void SoundService();

// Offset: 3028
// Size: 521
void InitSoundSources();

// Offset: 3552
// Size: 411
void DisposeSoundSources();

// Offset: 3964
// Size: 192
// EAX: pOutlet
// EDX: pSound
// EBX: pInitial_position
// ECX: pInitial_velocity
tS3_sound_tag DRS3StartSound3D(tS3_outlet_ptr pOutlet, tS3_sound_id pSound, br_vector3 *pInitial_position, br_vector3 *pInitial_velocity, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed);

// Offset: 4156
// Size: 97
// EAX: pSource
// EDX: pSound
// EBX: pRepeats
// ECX: pVolume
tS3_sound_tag DRS3StartSoundFromSource3(tS3_sound_source_ptr pSource, tS3_sound_id pSound, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed);

// Offset: 4256
// Size: 79
// EAX: pSource
// EDX: pSound
tS3_sound_tag DRS3StartSoundFromSource(tS3_sound_source_ptr pSource, tS3_sound_id pSound);

// Offset: 4336
// Size: 1377
void MungeEngineNoise();

// Offset: 5716
// Size: 154
void SetSoundVolumes();

// Offset: 5872
// Size: 55
// EAX: pIndex
tS3_outlet_ptr GetOutletFromIndex(int pIndex);

// Offset: 5928
// Size: 97
// EAX: pOutlet
int GetIndexFromOutlet(tS3_outlet_ptr pOutlet);

// Offset: 6028
// Size: 295
// EAX: pCDA_id
int DRS3StartCDA(tS3_sound_id pCDA_id);

// Offset: 6324
// Size: 95
int DRS3StopCDA();

// Offset: 6420
// Size: 75
void StartMusic();

// Offset: 6496
// Size: 48
void StopMusic();

#endif
