#ifndef _PRATCAM_H_
#define _PRATCAM_H_

#include "br_types.h"
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

int PratcamGetCurrent();

int PratcamGetAmbient();

int PratcamGetPending();

void TogglePratcam();

void LoadPratcam(char* pFolder_name);

void NextPratcamChunk();

void NewPratcamSequence(int pSequence_index, int pStart_chunk);

void ChangeAmbientPratcamNow(int pIndex, int pStart_chunk);

void ChangeAmbientPratcam(int pIndex);

void PratcamEventNow(int pIndex);

void PratcamEvent(int pIndex);

int HighResPratBufferWidth();

int HighResPratBufferHeight();

void InitPratcam();

void DisposePratcam();

void DoPratcam(tU32 pThe_time);

void TestPratCam(int pIndex);

void PratCam0();

void PratCam1();

void PratCam2();

void PratCam3();

void PratCam4();

void PratCam5();

void PratCam6();

void PratCam7();

void PratCam8();

void PratCam9();

#endif
