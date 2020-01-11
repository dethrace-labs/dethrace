#ifndef _RACESUMM_H_
#define _RACESUMM_H_

#include "br_types.h"
#include "dr_types.h"

// Offset: 0
// Size: 302
// EAX: pThe_state
void MungeRankEtc(tProgram_state* pThe_state);

// Offset: 304
// Size: 96
void CalcRankIncrease();

// Offset: 400
// Size: 81
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int RaceSummaryDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

// Offset: 484
// Size: 166
// EAX: pBox_left
// EDX: pText_left
// EBX: pTop
// ECX: pRight
void DrawInBox(int pBox_left, int pText_left, int pTop, int pRight, int pBottom, int pColour, int pAmount);

// Offset: 652
// Size: 220
// EAX: pLeft_1
// EDX: pLeft_2
// EBX: pPitch
// ECX: pTop
void DrawChromeNumber(int pLeft_1, int pLeft_2, int pPitch, int pTop, int pAmount);

// Offset: 872
// Size: 604
void DrawSummaryItems();

// Offset: 1476
// Size: 127
// EAX: pRate
// EDX: pTime
void RampUpRate(float* pRate, tU32 pTime);

// Offset: 1604
// Size: 747
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void DrawSummary(int pCurrent_choice, int pCurrent_mode);

// Offset: 2352
// Size: 49
void StartSummary();

// Offset: 2404
// Size: 86
void SetUpTemps();

// Offset: 2492
// Size: 78
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int Summ1GoAhead(int* pCurrent_choice, int* pCurrent_mode);

// Offset: 2572
// Size: 324
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int SummCheckGameOver(int* pCurrent_choice, int* pCurrent_mode);

// Offset: 2896
// Size: 288
tSO_result DoEndRaceSummary1();

// Offset: 3352
// Size: 1003
void BuildWrecks();

// Offset: 4356
// Size: 385
void DisposeWrecks();

// Offset: 4744
// Size: 185
// EAX: pMat
int MatrixIsIdentity(br_matrix34* pMat);

// Offset: 4932
// Size: 403
// EAX: pFrame_period
void SpinWrecks(tU32 pFrame_period);

// Offset: 5336
// Size: 143
// EAX: pIndex
// EDX: pCurrent_choice
// EBX: pCurrent_mode
void ZoomInTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode);

// Offset: 5480
// Size: 189
// EAX: pIndex
// EDX: pCurrent_choice
// EBX: pCurrent_mode
void ZoomOutTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode);

// Offset: 5672
// Size: 102
int WreckPick(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pNear, br_scalar pFar, void* pArg);

// Offset: 5776
// Size: 404
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int CastSelectionRay(int* pCurrent_choice, int* pCurrent_mode);

// Offset: 6180
// Size: 167
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DamageScrnExit(int* pCurrent_choice, int* pCurrent_mode);

// Offset: 6348
// Size: 2481
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void DamageScrnDraw(int pCurrent_choice, int pCurrent_mode);

// Offset: 8832
// Size: 268
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DamageScrnLeft(int* pCurrent_choice, int* pCurrent_mode);

// Offset: 9100
// Size: 275
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DamageScrnRight(int* pCurrent_choice, int* pCurrent_mode);

// Offset: 9376
// Size: 428
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DamageScrnUp(int* pCurrent_choice, int* pCurrent_mode);

// Offset: 9804
// Size: 470
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DamageScrnDown(int* pCurrent_choice, int* pCurrent_mode);

// Offset: 10276
// Size: 176
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DamageScrnGoHead(int* pCurrent_choice, int* pCurrent_mode);

// Offset: 10452
// Size: 258
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int ClickDamage(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

// Offset: 10712
// Size: 66
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int DamageScrnDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

// Offset: 10780
// Size: 286
tSO_result DoEndRaceSummary2();

// Offset: 11160
// Size: 96
// EAX: pStr_index
// EDX: pX
void DrawColumnHeading(int pStr_index, int pX);

// Offset: 11256
// Size: 74
// EAX: pFirst_one
// EDX: pSecond_one
int SortScores(void* pFirst_one, void* pSecond_one);

// Offset: 11332
// Size: 60
void SortGameScores();

// Offset: 11392
// Size: 709
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void NetSumDraw(int pCurrent_choice, int pCurrent_mode);

// Offset: 12104
// Size: 188
void DoNetRaceSummary();

// Offset: 12292
// Size: 227
// EAX: pFirst_summary_done
// EDX: pRace_result
tSO_result DoEndRaceSummary(int* pFirst_summary_done, tRace_result pRace_result);

#endif
