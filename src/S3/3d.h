#ifndef _3D_H_
#define _3D_H_

#include "brender/brender.h"
#include "s3_types.h"

void S3Set3DSoundEnvironment(float a1, float a2, float a3);

void S3UpdateListenerVectors();
void S3ServiceSoundSources();
int S3UpdateSpatialSound(tS3_channel* chan);

#endif
