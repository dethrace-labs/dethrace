#ifndef _STRUCTUR_H_
#define _STRUCTUR_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern int gLast_wrong_checkpoint;
extern int gMirror_on__structur; // suffix added to avoid duplicate symbol
extern int gPratcam_on;
extern int gCockpit_on;
extern int gOpponent_mix[10][5];
extern tU32 gLast_checkpoint_time;
extern tRace_over_reason gRace_over_reason;

int NumberOfOpponentsLeft(void);

void RaceCompleted(tRace_over_reason pReason);

void Checkpoint(int pCheckpoint_index, int pDo_sound);

void IncrementCheckpoint(void);

void IncrementLap(void);

int RayHitFace(br_vector3* pV0, br_vector3* pV1, br_vector3* pV2, br_vector3* pNormal, br_vector3* pStart, br_vector3* pDir);

void WrongCheckpoint(int pCheckpoint_index);

void CheckCheckpoints(void);

void TotalRepair(void);

void DoLogos(void);

void DoProgOpeningAnimation(void);

void DoProgramDemo(void);

int ChooseOpponent(int pNastiness, int* pHad_scum);

void SelectOpponents(tRace_info* pRace_info);

int PickNetRace(int pCurrent_race, tNet_sequence_type pNet_race_sequence);

void SwapNetCarsLoad(void);

void SwapNetCarsDispose(void);

void DoGame(void);

void InitialiseProgramState(void);

void DoProgram(void);

void JumpTheStart(void);

void GoingToInterfaceFromRace(void);

void GoingBackToRaceFromInterface(void);

#endif
