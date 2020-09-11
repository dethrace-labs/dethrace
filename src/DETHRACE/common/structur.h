#ifndef _STRUCTUR_H_
#define _STRUCTUR_H_

#include "br_types.h"
#include "dr_types.h"

int NumberOfOpponentsLeft();

void RaceCompleted(tRace_over_reason pReason);

void Checkpoint(int pCheckpoint_index, int pDo_sound);

void IncrementCheckpoint();

void IncrementLap();

int RayHitFace(br_vector3* pV0, br_vector3* pV1, br_vector3* pV2, br_vector3* pNormal, br_vector3* pStart, br_vector3* pDir);

void WrongCheckpoint(int pCheckpoint_index);

void CheckCheckpoints();

void TotalRepair();

void DoLogos();

void DoProgOpeningAnimation();

void DoProgramDemo();

int ChooseOpponent(int pNastiness, int* pHad_scum);

void SelectOpponents(tRace_info* pRace_info);

int PickNetRace(int pCurrent_race, tNet_sequence_type pNet_race_sequence);

void SwapNetCarsLoad();

void SwapNetCarsDispose();

void DoGame();

void InitialiseProgramState();

void DoProgram();

void JumpTheStart();

void GoingToInterfaceFromRace();

void GoingBackToRaceFromInterface();

#endif
