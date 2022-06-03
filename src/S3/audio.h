#ifndef _DOSAUDIO_H_
#define _DOSAUDIO_H_

#include "brender/brender.h"
#include "s3_defs.h"

extern int gS3_enabled;
extern int gS3_last_error;
extern tS3_channel gS3_channel_template;
extern tS3_sound_source* gS3_sound_sources;
extern int gS3_service_time_delta;
extern int gS3_inside_cockpit;

int S3Init(char* path, int low_memory_mode);

void S3Enable();
void S3Disable();

int S3OpenOutputDevices();
int S3OpenSampleDevice();
tS3_outlet* S3CreateOutlet(int unk1, int pChannel_count);
int S3CreateOutletChannels(tS3_outlet* outlet, int pChannel_count);
void S3DisposeOutlet(tS3_outlet* outlet);
int S3UnbindChannels(tS3_outlet* outlet);
tS3_channel* S3AllocateChannel(tS3_outlet* outlet, int priority);
int S3StopChannel(tS3_channel* chan);

void S3DisposeOutlet(tS3_outlet* outlet);
int S3StopOutletSound(tS3_outlet* pOutlet);
void S3StopAllOutletSounds();

int S3DisposeDescriptor(tS3_sound_id id);

char* S3LoadSoundBankFile(char* pThe_path);
int S3LoadSoundbank(const char* pSoundbank_filename, int pLow_memory_mode);

void S3SoundBankReaderNextLine(tS3_soundbank_read_ctx* ctx);
void S3SoundBankReaderSkipWhitespace(tS3_soundbank_read_ctx* ctx);
void S3SoundBankReaderSkipToNewline(tS3_soundbank_read_ctx* ctx);
void S3SoundBankReaderAdvance(tS3_soundbank_read_ctx* buffer, int bytes_read);
int S3SoundBankReaderReadFilename(char** filename, tS3_soundbank_read_ctx* ctx, const char* dir_name);
int S3SoundBankReadEntry(tS3_soundbank_read_ctx* ctx, char* dir_name, int low_memory_mode);

tS3_descriptor* S3AllocateDescriptor();
tS3_descriptor* S3GetDescriptorByID(tS3_sound_id id);

char* S3GetCurrentDir();

void S3CalculateRandomizedFields(tS3_channel* chan, tS3_descriptor* desc);
int S3IRandomBetween(int pMin, int pMax, int pDefault);
int S3IRandomBetweenLog(int pMin, int pMax, int pDefault);
double S3FRandomBetween(double pMin, double pMax);

int S3GenerateTag(tS3_outlet* outlet);
int S3DisposeDescriptor(tS3_sound_id id);
int S3SetOutletVolume(tS3_outlet* pOutlet, tS3_volume pVolume);
tS3_channel* S3GetChannelForTag(tS3_sound_tag tag);
int S3SetTagVolume(tS3_sound_tag pTag, tS3_volume pVolume);

void S3ServiceOutlets();
int S3ServiceChannel(tS3_channel* chan);
void S3Service(int inside_cockpit, int unk1);

int S3SoundStillPlaying(tS3_sound_tag pSound);

tS3_sound_source* S3CreateSoundSourceBR(br_vector3* pPosition, br_vector3* pVelocity, tS3_outlet* pBound_outlet);
tS3_sound_source* S3CreateSoundSource(void* pPosition, void* pVelocity, tS3_outlet* pBound_outlet);

#endif
