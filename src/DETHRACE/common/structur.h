#ifndef _STRUCTUR_H_
#define _STRUCTUR_H_

#include "br_types.h"
#include "dr_types.h"

// Offset: 0
// Size: 124
int NumberOfOpponentsLeft();

// Offset: 124
// Size: 419
// EAX: pReason
void RaceCompleted(tRace_over_reason pReason);

// Offset: 544
// Size: 79
// EAX: pCheckpoint_index
// EDX: pDo_sound
void Checkpoint(int pCheckpoint_index, int pDo_sound);

// Offset: 624
// Size: 305
void IncrementCheckpoint();

// Offset: 932
// Size: 70
void IncrementLap();

// Offset: 1004
// Size: 152
// EAX: pV0
// EDX: pV1
// EBX: pV2
// ECX: pNormal
int RayHitFace(br_vector3* pV0, br_vector3* pV1, br_vector3* pV2, br_vector3* pNormal, br_vector3* pStart, br_vector3* pDir);

// Offset: 1156
// Size: 241
// EAX: pCheckpoint_index
void WrongCheckpoint(int pCheckpoint_index);

// Offset: 1400
// Size: 870
void CheckCheckpoints();

// Offset: 2272
// Size: 72
void TotalRepair();

// Offset: 2344
// Size: 64
void DoLogos();

// Offset: 2408
// Size: 54
void DoProgOpeningAnimation();

// Offset: 2464
// Size: 59
void DoProgramDemo();

// Offset: 2524
// Size: 308
// EAX: pNastiness
// EDX: pHad_scum
int ChooseOpponent(int pNastiness, int* pHad_scum);

// Offset: 2832
// Size: 294
// EAX: pRace_info
void SelectOpponents(tRace_info* pRace_info);

// Offset: 3128
// Size: 269
// EAX: pCurrent_race
// EDX: pNet_race_sequence
int PickNetRace(int pCurrent_race, tNet_sequence_type pNet_race_sequence);

// Offset: 3400
// Size: 288
void SwapNetCarsLoad();

// Offset: 3688
// Size: 165
void SwapNetCarsDispose();

// Offset: 3856
// Size: 1161
void DoGame();

// Offset: 5020
// Size: 242
void InitialiseProgramState();

// Offset: 5264
// Size: 171
void DoProgram();

// Offset: 5436
// Size: 270
void JumpTheStart();

// Offset: 5708
// Size: 81
void GoingToInterfaceFromRace();

// Offset: 5792
// Size: 81
void GoingBackToRaceFromInterface();

#endif
