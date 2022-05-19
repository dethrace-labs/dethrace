#ifndef _S3CDA_H_
#define _S3CDA_H_

#include "s3_types.h"

void S3DisableCDA();
void S3StopCDAOutlets();
int S3OpenCDADevice();

int S3PlayCDA(tS3_channel* chan);
int S3StopCDA(tS3_channel* chan);
int S3SetCDAVolume(tS3_channel* chan, int pVolume);
int S3IsCDAPlaying();

#endif
