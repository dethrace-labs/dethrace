#ifndef _RACESUMM_H_
#define _RACESUMM_H_

#include "br_types.h"
#include "dr_types.h"

void MungeRankEtc(tProgram_state* pThe_state);

void CalcRankIncrease();

int RaceSummaryDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

void DrawInBox(int pBox_left, int pText_left, int pTop, int pRight, int pBottom, int pColour, int pAmount);

void DrawChromeNumber(int pLeft_1, int pLeft_2, int pPitch, int pTop, int pAmount);

void DrawSummaryItems();

void RampUpRate(float* pRate, tU32 pTime);

void DrawSummary(int pCurrent_choice, int pCurrent_mode);

void StartSummary();

void SetUpTemps();

int Summ1GoAhead(int* pCurrent_choice, int* pCurrent_mode);

int SummCheckGameOver(int* pCurrent_choice, int* pCurrent_mode);

tSO_result DoEndRaceSummary1();

void BuildWrecks();

void DisposeWrecks();

int MatrixIsIdentity(br_matrix34* pMat);

void SpinWrecks(tU32 pFrame_period);

void ZoomInTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode);

void ZoomOutTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode);

int WreckPick(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pNear, br_scalar pFar, void* pArg);

int CastSelectionRay(int* pCurrent_choice, int* pCurrent_mode);

int DamageScrnExit(int* pCurrent_choice, int* pCurrent_mode);

void DamageScrnDraw(int pCurrent_choice, int pCurrent_mode);

int DamageScrnLeft(int* pCurrent_choice, int* pCurrent_mode);

int DamageScrnRight(int* pCurrent_choice, int* pCurrent_mode);

int DamageScrnUp(int* pCurrent_choice, int* pCurrent_mode);

int DamageScrnDown(int* pCurrent_choice, int* pCurrent_mode);

int DamageScrnGoHead(int* pCurrent_choice, int* pCurrent_mode);

int ClickDamage(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

int DamageScrnDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

tSO_result DoEndRaceSummary2();

void DrawColumnHeading(int pStr_index, int pX);

int SortScores(void* pFirst_one, void* pSecond_one);

void SortGameScores();

void NetSumDraw(int pCurrent_choice, int pCurrent_mode);

void DoNetRaceSummary();

tSO_result DoEndRaceSummary(int* pFirst_summary_done, tRace_result pRace_result);

#endif
