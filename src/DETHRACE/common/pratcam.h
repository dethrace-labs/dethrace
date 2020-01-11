#ifndef _PRATCAM_H_
#define _PRATCAM_H_

#include "br_types.h"
#include "dr_types.h"

// Offset: 0
// Size: 45
int PratcamGetCurrent();

// Offset: 48
// Size: 45
int PratcamGetAmbient();

// Offset: 96
// Size: 45
int PratcamGetPending();

// Offset: 144
// Size: 240
void TogglePratcam();

// Offset: 384
// Size: 1836
// EAX: pFolder_name
void LoadPratcam(char* pFolder_name);

// Offset: 2220
// Size: 495
void NextPratcamChunk();

// Offset: 2716
// Size: 83
// EAX: pSequence_index
// EDX: pStart_chunk
void NewPratcamSequence(int pSequence_index, int pStart_chunk);

// Offset: 2800
// Size: 67
// EAX: pIndex
// EDX: pStart_chunk
void ChangeAmbientPratcamNow(int pIndex, int pStart_chunk);

// Offset: 2868
// Size: 118
// EAX: pIndex
void ChangeAmbientPratcam(int pIndex);

// Offset: 2988
// Size: 57
// EAX: pIndex
void PratcamEventNow(int pIndex);

// Offset: 3048
// Size: 113
// EAX: pIndex
void PratcamEvent(int pIndex);

// Offset: 3164
// Size: 94
int HighResPratBufferWidth();

// Offset: 3260
// Size: 94
int HighResPratBufferHeight();

// Offset: 3356
// Size: 419
void InitPratcam();

// Offset: 3776
// Size: 447
void DisposePratcam();

// Offset: 4224
// Size: 1252
// EAX: pThe_time
void DoPratcam(tU32 pThe_time);

// Offset: 5476
// Size: 96
// EAX: pIndex
void TestPratCam(int pIndex);

// Offset: 5572
// Size: 41
void PratCam0();

// Offset: 5616
// Size: 44
void PratCam1();

// Offset: 5660
// Size: 44
void PratCam2();

// Offset: 5704
// Size: 44
void PratCam3();

// Offset: 5748
// Size: 44
void PratCam4();

// Offset: 5792
// Size: 44
void PratCam5();

// Offset: 5836
// Size: 44
void PratCam6();

// Offset: 5880
// Size: 44
void PratCam7();

// Offset: 5924
// Size: 44
void PratCam8();

// Offset: 5968
// Size: 44
void PratCam9();

#endif
