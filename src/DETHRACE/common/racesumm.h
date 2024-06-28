#ifndef _RACESUMM_H_
#define _RACESUMM_H_

#include "dr_types.h"

extern int gPlayer_lookup[6];
extern tMouse_area gOld_back_button;
extern tWreck_info gWreck_array[30];
extern br_actor* gWreck_root;
extern br_actor* gWreck_camera;
extern tU32 gWreck_start_zoom;
extern tU32 gWreck_gallery_start;
extern float gTemp_rank_increase;
extern float gRank_per_ms;
extern tU32 gLast_wreck_draw;
extern tS3_sound_tag gSumm_sound;
extern float gCredits_per_ms;
extern tMouse_area* gBack_button_ptr;
extern tU32 gSummary_start;
extern br_pixelmap* gWreck_z_buffer;
extern br_pixelmap* gWreck_render_area;
extern int gWreck_selected;
extern int gWreck_zoom_out;
extern br_pixelmap* gChrome_font;
extern int gWreck_zoom_in;
extern int gTemp_credits;
extern int gUser_interacted;
extern int gWreck_count;
extern int gRank_etc_munged;
extern int gRank_increase;
extern int gTemp_earned;
extern int gTemp_rank;
extern int gWreck_zoomed_in;
extern int gDone_initial;
extern int gTemp_lost;

void MungeRankEtc(tProgram_state* pThe_state);

void CalcRankIncrease(void);

int RaceSummaryDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

void DrawInBox(int pBox_left, int pText_left, int pTop, int pRight, int pBottom, int pColour, int pAmount);

void DrawChromeNumber(int pLeft_1, int pLeft_2, int pPitch, int pTop, int pAmount);

void DrawSummaryItems(void);

void RampUpRate(float* pRate, tU32 pTime);

void DrawSummary(int pCurrent_choice, int pCurrent_mode);

void StartSummary(void);

void SetUpTemps(void);

int Summ1GoAhead(int* pCurrent_choice, int* pCurrent_mode);

int SummCheckGameOver(int* pCurrent_choice, int* pCurrent_mode);

tSO_result DoEndRaceSummary1(void);

// Suffix added to avoid duplicate symbol
void PrepareBoundingRadius__racesumm(br_model* model);

void BuildWrecks(void);

void DisposeWrecks(void);

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

tSO_result DoEndRaceSummary2(void);

// Suffix added to avoid duplicate symbol
void DrawAnItem__racesumm(int pX, int pY_index, int pFont_index, char* pText);

// Suffix added to avoid duplicate symbol
void DrawColumnHeading__racesumm(int pStr_index, int pX);

int SortScores(const void* pFirst_one, const void* pSecond_one);

void SortGameScores(void);

void NetSumDraw(int pCurrent_choice, int pCurrent_mode);

void DoNetRaceSummary(void);

tSO_result DoEndRaceSummary(int* pFirst_summary_done, tRace_result pRace_result);

#endif
