#ifndef S3_SOUND_H
#define S3_SOUND_H

// External typedefs

typedef float tF32;
typedef char* tS3_sound_source_ptr;
typedef int tS3_sound_tag;
typedef int tS3_sound_id;
typedef int tS3_type;
typedef int tS3_repeats;
typedef int tS3_volume;
typedef int tS3_effect_tag;
typedef long tS3_priority;
typedef long tS3_pitch;
typedef long tS3_speed;
typedef char* tS3_outlet_ptr;

typedef void tS3_sample_filter(tS3_effect_tag, tS3_sound_tag);

typedef struct tS3_vector3 {
    tF32 x;
    tF32 y;
    tF32 z;
} tS3_vector3;

int S3Init(char* path, int low_memory_mode);

void S3Disable();
void S3Enable();

int S3Set3DSoundEnvironment(float a1, float a2, float a3);
tS3_sound_source_ptr S3CreateSoundSourceBR(br_vector3* pPosition, br_vector3* pVelocity, tS3_outlet_ptr pBound_outlet);
int S3BindAmbientSoundToOutlet(tS3_outlet_ptr pOutlet, int pSound, tS3_sound_source_ptr source, float pMax_distance, int pPeriod, int pRepeats, int pVolume, int pPitch, int pSpeed);

tS3_sound_tag S3StartSound3D(tS3_outlet_ptr pOutlet, tS3_sound_id pSound, tS3_vector3* pInitial_position, tS3_vector3* pInitial_velocity, tS3_repeats pRepeats, tS3_volume pVolume, tS3_pitch pPitch, tS3_speed pSpeed);

tS3_outlet_ptr S3CreateOutlet(int unk1, int pChannel_count);
void S3DisposeOutlet(tS3_outlet_ptr outlet);
int S3DisposeDescriptor(tS3_sound_id id);
int S3DisposeSoundSource(tS3_sound_source_ptr src);

int S3SetTagVolume(tS3_sound_tag pTag, tS3_volume pVolume);

void S3Service(int inside_cockpit, int unk1);

int S3LoadSample(tS3_sound_id id);
tS3_sound_tag S3StartSound(tS3_outlet_ptr pOutlet, tS3_sound_id pSound);
tS3_sound_tag S3StartSound2(tS3_outlet_ptr pOutlet, tS3_sound_id pSound, tS3_repeats pRepeats, tS3_volume pLVolume, tS3_volume pRVolume, tS3_pitch pPitch, tS3_speed pSpeed);
void S3StopAllOutletSounds();
int S3SoundStillPlaying(tS3_sound_tag pSound);
int S3StopSound(tS3_sound_tag pSound_tag);
int S3StopOutletSound(tS3_outlet_ptr pOutlet);
int S3SetOutletVolume(tS3_outlet_ptr pOutlet, tS3_volume pVolume);
void S3UpdateSoundSource(tS3_outlet_ptr outlet, tS3_sound_tag tag, tS3_sound_source_ptr src, float pMax_distance_squared, int pPeriod, tS3_repeats pAmbient_repeats, tS3_volume pVolume, int pPitch, tS3_speed pSpeed);

void S3BindListenerPositionBRender(br_vector3* pos);
void S3BindListenerVelocityBRender(br_vector3* vel);
void S3BindListenerLeftBRender(br_vector3* left);

int S3IsCDAPlaying2();

#endif
