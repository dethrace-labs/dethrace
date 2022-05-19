#ifndef S3_SOUND_H
#define S3_SOUND_H

// External typedefs

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

int S3Init(char* path, int low_memory_mode);

int S3Set3DSoundEnvironment(float a1, float a2, float a3);
tS3_sound_source_ptr S3CreateSoundSourceBR(br_vector3* pPosition, br_vector3* pVelocity, tS3_outlet_ptr pBound_outlet);
int S3BindAmbientSource(tS3_outlet_ptr pOutlet, int pSound, tS3_sound_source_ptr source, float pMax_distance, int pPeriod, int pRepeats, int pVolume, int pPitch, int pSpeed);

tS3_outlet_ptr S3CreateOutlet(int unk1, int pChannel_count);
void S3DisposeOutlet(tS3_outlet_ptr outlet);

void S3Service(int inside_cockpit, int unk1);
tS3_sound_tag S3StartSound(tS3_outlet_ptr pOutlet, tS3_sound_id pSound);
void S3StopAllOutletSounds();
int S3SoundStillPlaying(tS3_sound_tag pSound);
int S3StopSound(tS3_sound_tag pSound_tag);
void S3StopOutletSound(tS3_outlet_ptr pOutlet);
int S3SetOutletVolume(tS3_outlet_ptr pOutlet, tS3_volume pVolume);

#endif
