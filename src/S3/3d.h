#ifndef _3D_H_
#define _3D_H_

#include "brender/brender.h"
#include "s3_defs.h"

void S3Set3DSoundEnvironment(float a1, float a2, float a3);

void S3UpdateListenerVectors();
void S3ServiceSoundSources();
int S3UpdateSpatialSound(tS3_channel* chan);
int S3BindAmbientSoundToOutlet(tS3_outlet* pOutlet, int pSound, tS3_sound_source* source, float pMax_distance, int pPeriod, int pRepeats, int pVolume, int pPitch, int pSpeed);
void S3UpdateSoundSource(tS3_outlet* outlet, tS3_sound_tag tag, tS3_sound_source* src, float pMax_distance_squared, int pPeriod, tS3_repeats pAmbient_repeats, tS3_volume pVolume, int pPitch, tS3_speed pSpeed);
tS3_sound_tag S3ServiceSoundSource(tS3_sound_source* src);

int S3Calculate3D(tS3_channel* chan, int pIs_ambient);

void S3CopyVector3(void* a1, void* a2, int pBrender_vector);
void S3CopyBrVector3(tS3_vector3* a1, br_vector3* a2);
void S3CopyS3Vector3(tS3_vector3* a1, tS3_vector3* a2);

#endif
