#ifndef S3_SOUND2_H
#define S3_SOUND2_H

#include "s3_defs.h"

extern int gS3_sample_filter_funcs_registered;
extern tS3_sample_filter* gS3_sample_filter_func;
extern tS3_sample_filter* gS3_sample_filter_disable_func;

int S3OpenSampleDevice();
int S3LoadSample(tS3_sound_id id);
void* S3LoadWavFile(char* pFile_name, tS3_sample* pSample);
int S3PlaySample(tS3_channel* chan);
int S3StopSample(tS3_channel* chan);
int S3ExecuteSampleFilterFuncs(tS3_channel* chan);
int S3SampleSyncVolume(tS3_channel* chan);

#endif