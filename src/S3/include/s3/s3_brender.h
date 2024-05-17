#ifndef S3_S3_BRENDER_H
#define S3_S3_BRENDER_H

#include "s3.h"

#include <brender.h>

tS3_sound_source_ptr S3CreateSoundSourceBR(br_vector3* pPosition, br_vector3* pVelocity, tS3_outlet_ptr pBound_outlet);

void S3BindListenerPositionBRender(br_vector3* pos);
void S3BindListenerVelocityBRender(br_vector3* vel);
void S3BindListenerLeftBRender(br_vector3* left);

#endif /* S3_S3_BRENDER_H */
