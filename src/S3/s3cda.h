#ifndef _S3CDA_H_
#define _S3CDA_H_

#include "s3_defs.h"

void S3DisableCDA(void);
void S3StopCDAOutlets(void);

int S3PlayCDA(tS3_channel* chan);
int S3StopCDA(tS3_channel* chan);
int S3SetCDAVolume(tS3_channel* chan, int pVolume);
int S3IsCDAPlaying(void);
int S3IsCDAPlaying2(void);

#endif
