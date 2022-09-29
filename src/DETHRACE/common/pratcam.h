#ifndef _PRATCAM_H_
#define _PRATCAM_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern tS3_sound_tag gWhirr_noise;
extern tFlic_descriptor gPrat_flic;
extern tPrat_sequence* gPratcam_sequences;
extern tPrat_flic_spec* gPratcam_flics;
extern int gCurrent_pratcam_chunk;
extern int gPending_ambient_prat;
extern int gCurrent_pratcam_index;
extern br_pixelmap* gPrat_buffer;
extern int gNumber_of_prat_sequences;
extern int gNumber_of_prat_flics;
extern tU32 gLast_pratcam_frame_time;
extern int gCurrent_pratcam_precedence;
extern int gCurrent_ambient_prat_sequence;
extern int gCurrent_pratcam_alternative;

int PratcamGetCurrent(void);

int PratcamGetAmbient(void);

int PratcamGetPending(void);

void TogglePratcam(void);

void LoadPratcam(char* pFolder_name);

void NextPratcamChunk(void);

void NewPratcamSequence(int pSequence_index, int pStart_chunk);

void ChangeAmbientPratcamNow(int pIndex, int pStart_chunk);

void ChangeAmbientPratcam(int pIndex);

void PratcamEventNow(int pIndex);

void PratcamEvent(int pIndex);

int HighResPratBufferWidth(void);

int HighResPratBufferHeight(void);

void InitPratcam(void);

void DisposePratcam(void);

void DoPratcam(tU32 pThe_time);

void TestPratCam(int pIndex);

void PratCam0(void);

void PratCam1(void);

void PratCam2(void);

void PratCam3(void);

void PratCam4(void);

void PratCam5(void);

void PratCam6(void);

void PratCam7(void);

void PratCam8(void);

void PratCam9(void);

#endif
