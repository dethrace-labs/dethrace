#ifndef _FLICPLAY_H_
#define _FLICPLAY_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern int gPalette_allocate_count;
extern int gPalette_fuck_prevention;
extern int gDark_mode;
extern int gTransparency_on;
extern int gPanel_flic_disable;
extern int gPending_flic;
extern int gPlay_from_disk;
extern int gTrans_enabled;
extern br_pixelmap* gPanel_buffer[2];
extern tU32 gSound_time;
extern tFlic_spec gMain_flic_list[372];
extern int gFlic_bunch0[29];
extern int gFlic_bunch1[31];
extern int gFlic_bunch2[8];
extern int gFlic_bunch3[13];
extern int gFlic_bunch4[22];
extern int gFlic_bunch5[5];
extern int gFlic_bunch6[51];
extern int gFlic_bunch7[7];
extern int gFlic_bunch8[16];
extern tFlic_bunch gFlic_bunch[9];
extern char gLast_flic_name[14];
extern tU32 gPanel_flic_data_length[2];
extern tU32 gLast_panel_frame_time[2];
extern tU8* gPanel_flic_data[2];
extern int gPanel_flic_top[2];
extern tFlic_descriptor gPanel_flic[2];
extern int gPanel_flic_left[2];
extern int gPending_pending_flic;
extern int gSound_ID;
extern int gTranslation_count;
extern tDR_font* gTrans_fonts[15];
extern tTranslation_record* gTranslations;
extern br_pixelmap* gPalette;
extern void* gPalette_pixels;
extern tFlic_descriptor* gFirst_flic;

void EnableTranslationText();

void DisableTranslationText();

void SetFlicSound(int pSound_ID, tU32 pSound_time);

int TranslationMode();

void DontLetFlicFuckWithPalettes();

void LetFlicFuckWithPalettes();

void PlayFlicsInDarkness();

void ReilluminateFlics();

void TurnFlicTransparencyOn();

void TurnFlicTransparencyOff();

void PlayFlicsFromDisk();

void PlayFlicsFromMemory();

int FlicsPlayedFromDisk();

void TurnOffPanelFlics();

void TurnOnPanelFlics();

int GetPanelFlicFrameIndex(int pIndex);

void FlicPaletteAllocate();

void AssertFlicPixelmap(tFlic_descriptor_ptr pFlic_info, br_pixelmap* pDest_pixelmap);

int StartFlic(char* pFile_name, int pIndex, tFlic_descriptor_ptr pFlic_info, tU32 pSize, tS8* pData_ptr, br_pixelmap* pDest_pixelmap, int pX_offset, int pY_offset, int pFrame_rate);

void FreeFlicPaletteAllocate();

int EndFlic(tFlic_descriptor_ptr pFlic_info);

void DoColourMap(tFlic_descriptor_ptr pFlic_info, tU32 chunk_length);

void DoDifferenceX(tFlic_descriptor* pFlic_info, tU32 chunk_length);

void DoDifferenceTrans(tFlic_descriptor* pFlic_info, tU32 chunk_length);

void DoColour256(tFlic_descriptor* pFlic_info, tU32 chunk_length);

void DoDeltaTrans(tFlic_descriptor* pFlic_info, tU32 chunk_length);

void DoDeltaX(tFlic_descriptor* pFlic_info, tU32 chunk_length);

void DoBlack(tFlic_descriptor* pFlic_info, tU32 chunk_length);

void DoRunLengthX(tFlic_descriptor* pFlic_info, tU32 chunk_length);

void DoRunLengthTrans(tFlic_descriptor* pFlic_info, tU32 chunk_length);

void DoUncompressed(tFlic_descriptor* pFlic_info, tU32 chunk_length);

void DoUncompressedTrans(tFlic_descriptor* pFlic_info, tU32 chunk_length);

void DoMini(tFlic_descriptor* pFlic_info, tU32 chunk_length);

void DrawTranslations(tFlic_descriptor* pFlic_info, int pLast_frame);

int PlayNextFlicFrame2(tFlic_descriptor* pFlic_info, int pPanel_flic);

int PlayNextFlicFrame(tFlic_descriptor* pFlic_info);

int PlayFlic(int pIndex, tU32 pSize, tS8* pData_ptr, br_pixelmap* pDest_pixelmap, int pX_offset, int pY_offset, void (*DoPerFrame)(), int pInterruptable, int pFrame_rate);

void SwapScreen();

void ShowFlic(int pIndex);

void InitFlics();

int LoadFlic(int pIndex);

void UnlockFlic(int pIndex);

int LoadFlicData(char* pName, tU8** pData, tU32* pData_length);

void FreeFlic(int pIndex);

void ForceRunFlic(int pIndex);

void RunFlicAt(int pIndex, int pX, int pY);

void RunFlic(int pIndex);

void PreloadBunchOfFlics(int pBunch_index);

void UnlockBunchOfFlics(int pBunch_index);

void FlushAllFlics(int pBunch_index);

void InitFlicQueue();

int FlicQueueFinished();

void ProcessFlicQueue(tU32 pInterval);

void FlushFlicQueue();

void AddToFlicQueue(int pIndex, int pX, int pY, int pMust_finish);

void InitialiseFlicPanel(int pIndex, int pLeft, int pTop, int pWidth, int pHeight);

void DisposeFlicPanel(int pIndex);

void ServicePanelFlics(int pCopy_to_buffer);

void ChangePanelFlic(int pIndex, tU8* pData, tU32 pData_length);

br_pixelmap* GetPanelPixelmap(int pIndex);

void LoadInterfaceStrings();

void FlushInterfaceFonts();

void SuspendPendingFlic();

void ResumePendingFlic();

#endif
