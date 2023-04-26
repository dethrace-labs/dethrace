#ifndef _S3CDA_H_
#define _S3CDA_H_

#include "s3_defs.h"

void S3DisableCDA();
void S3StopCDAOutlets();

int S3PlayCDA(tS3_channel* chan);
int S3StopCDA(tS3_channel* chan);
int S3SetCDAVolume(tS3_channel* chan, int pVolume);
int S3IsCDAPlaying();
int S3IsCDAPlaying2();

#endif
