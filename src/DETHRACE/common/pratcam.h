#ifndef _PRATCAM_H_
#define _PRATCAM_H_

#include "br_types.h"
#include "dr_types.h"

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
