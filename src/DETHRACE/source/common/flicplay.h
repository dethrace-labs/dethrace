#include "dr_types.h"
#include "br_types.h"
// Offset: 0
// Size: 44
void EnableTranslationText();

// Offset: 44
// Size: 44
void DisableTranslationText();

// Offset: 88
// Size: 54
// EAX: pSound_ID
// EDX: pSound_time
void SetFlicSound(int pSound_ID, tU32 pSound_time, signed char __unk2__);

// Offset: 144
// Size: 45
int TranslationMode();

// Offset: 192
// Size: 44
void DontLetFlicFuckWithPalettes();

// Offset: 236
// Size: 44
void LetFlicFuckWithPalettes();

// Offset: 280
// Size: 44
void PlayFlicsInDarkness();

// Offset: 324
// Size: 49
void ReilluminateFlics();

// Offset: 376
// Size: 44
void TurnFlicTransparencyOn();

// Offset: 420
// Size: 44
void TurnFlicTransparencyOff();

// Offset: 464
// Size: 44
void PlayFlicsFromDisk();

// Offset: 508
// Size: 44
void PlayFlicsFromMemory();

// Offset: 552
// Size: 45
int FlicsPlayedFromDisk();

// Offset: 600
// Size: 44
void TurnOffPanelFlics();

// Offset: 644
// Size: 44
void TurnOnPanelFlics();

// Offset: 688
// Size: 53
// EAX: pIndex
int GetPanelFlicFrameIndex(int pIndex);

// Offset: 744
// Size: 91
void FlicPaletteAllocate();

// Offset: 836
// Size: 92
// EAX: pFlic_info
// EDX: pDest_pixelmap
void AssertFlicPixelmap(tFlic_descriptor_ptr pFlic_info, br_pixelmap *pDest_pixelmap);

// Offset: 928
// Size: 791
// EAX: pFile_name
// EDX: pIndex
// EBX: pFlic_info
// ECX: pSize
int StartFlic(char *pFile_name, int pIndex, tFlic_descriptor_ptr pFlic_info, tU32 pSize, tS8 *pData_ptr, br_pixelmap *pDest_pixelmap, int pX_offset, int pY_offset, int pFrame_rate, unsigned char claimed_speed, br_memory_classes magic_number, int the_path, struct total_size);

// Offset: 1720
// Size: 96
void FreeFlicPaletteAllocate();

// Offset: 1816
// Size: 118
// EAX: pFlic_info
int EndFlic(tFlic_descriptor_ptr pFlic_info);

// Offset: 1936
// Size: 312
// EAX: pFlic_info
// EDX: chunk_length
void DoColourMap(tFlic_descriptor_ptr pFlic_info, tU32 chunk_length);

// Offset: 2248
// Size: 343
// EAX: pFlic_info
// EDX: chunk_length
void DoDifferenceX(tFlic_descriptor *pFlic_info, tU32 chunk_length);

// Offset: 2592
// Size: 377
// EAX: pFlic_info
// EDX: chunk_length
void DoDifferenceTrans(tFlic_descriptor *pFlic_info, tU32 chunk_length);

// Offset: 2972
// Size: 303
// EAX: pFlic_info
// EDX: chunk_length
void DoColour256(tFlic_descriptor *pFlic_info, tU32 chunk_length);

// Offset: 3276
// Size: 537
// EAX: pFlic_info
// EDX: chunk_length
void DoDeltaTrans(tFlic_descriptor *pFlic_info, tU32 chunk_length);

// Offset: 3816
// Size: 354
// EAX: pFlic_info
// EDX: chunk_length
void DoDeltaX(tFlic_descriptor *pFlic_info, tU32 chunk_length);

// Offset: 4172
// Size: 156
// EAX: pFlic_info
// EDX: chunk_length
void DoBlack(tFlic_descriptor *pFlic_info, tU32 chunk_length);

// Offset: 4328
// Size: 272
// EAX: pFlic_info
// EDX: chunk_length
void DoRunLengthX(tFlic_descriptor *pFlic_info, tU32 chunk_length);

// Offset: 4600
// Size: 309
// EAX: pFlic_info
// EDX: chunk_length
void DoRunLengthTrans(tFlic_descriptor *pFlic_info, tU32 chunk_length);

// Offset: 4912
// Size: 160
// EAX: pFlic_info
// EDX: chunk_length
void DoUncompressed(tFlic_descriptor *pFlic_info, tU32 chunk_length);

// Offset: 5072
// Size: 176
// EAX: pFlic_info
// EDX: chunk_length
void DoUncompressedTrans(tFlic_descriptor *pFlic_info, tU32 chunk_length);

// Offset: 5248
// Size: 52
// EAX: pFlic_info
// EDX: chunk_length
void DoMini(tFlic_descriptor *pFlic_info, tU32 chunk_length);

// Offset: 5300
// Size: 377
// EAX: pFlic_info
// EDX: pLast_frame
void DrawTranslations(tFlic_descriptor *pFlic_info);

// Offset: 5680
// Size: 783
// EAX: pFlic_info
// EDX: pPanel_flic
int PlayNextFlicFrame2(tFlic_descriptor *pFlic_info);

// Offset: 6464
// Size: 53
// EAX: pFlic_info
int PlayNextFlicFrame(tFlic_descriptor *pFlic_info);

// Offset: 6520
// Size: 295
// EAX: pIndex
// EDX: pSize
// EBX: pData_ptr
// ECX: pDest_pixelmap
int PlayFlic(int pIndex, tU32 pSize, tS8 *pData_ptr, br_pixelmap *pDest_pixelmap, int pX_offset, int pY_offset, void (*DoPerFrame)(), int pInterruptable, int pFrame_rate, signed char finished_playing, br_memory_classes the_flic, struct new_time);

// Offset: 6816
// Size: 41
void SwapScreen();

// Offset: 6860
// Size: 154
// EAX: pIndex
void ShowFlic(int pIndex);

// Offset: 7016
// Size: 76
void InitFlics();

// Offset: 7092
// Size: 395
// EAX: pIndex
int LoadFlic(int pIndex);

// Offset: 7488
// Size: 75
// EAX: pIndex
void UnlockFlic(int pIndex);

// Offset: 7564
// Size: 257
// EAX: pName
// EDX: pData
// EBX: pData_length
int LoadFlicData(char *pName, tU8 **pData, tU32 *pData_length, short f, br_memory_classes the_path);

// Offset: 7824
// Size: 87
// EAX: pIndex
void FreeFlic(int pIndex);

// Offset: 7912
// Size: 61
// EAX: pIndex
void ForceRunFlic(int pIndex);

// Offset: 7976
// Size: 107
// EAX: pIndex
// EDX: pX
// EBX: pY
void RunFlicAt(int pIndex, int pX, int pY);

// Offset: 8084
// Size: 117
// EAX: pIndex
void RunFlic(int pIndex);

// Offset: 8204
// Size: 105
// EAX: pBunch_index
void PreloadBunchOfFlics(int pBunch_index);

// Offset: 8312
// Size: 105
// EAX: pBunch_index
void UnlockBunchOfFlics(int pBunch_index);

// Offset: 8420
// Size: 73
// EAX: pBunch_index
void FlushAllFlics(int pBunch_index);

// Offset: 8496
// Size: 44
void InitFlicQueue();

// Offset: 8540
// Size: 87
int FlicQueueFinished();

// Offset: 8628
// Size: 230
// EAX: pInterval
void ProcessFlicQueue(tU32 pInterval);

// Offset: 8860
// Size: 138
void FlushFlicQueue();

// Offset: 9000
// Size: 424
// EAX: pIndex
// EDX: pX
// EBX: pY
// ECX: pMust_finish
void AddToFlicQueue(int pIndex, int pX, int pY, int pMust_finish);

// Offset: 9424
// Size: 205
// EAX: pIndex
// EDX: pLeft
// EBX: pTop
// ECX: pWidth
void InitialiseFlicPanel(int pIndex, int pLeft, int pTop, int pWidth, int pHeight);

// Offset: 9632
// Size: 112
// EAX: pIndex
void DisposeFlicPanel(int pIndex);

// Offset: 9744
// Size: 502
// EAX: pCopy_to_buffer
void ServicePanelFlics(int pCopy_to_buffer);

// Offset: 10248
// Size: 205
// EAX: pIndex
// EDX: pData
// EBX: pData_length
void ChangePanelFlic(int pIndex, tU8 *pData, tU32 pData_length, unsigned char __unk3__, br_memory_classes __unk4__);

// Offset: 10456
// Size: 55
// EAX: pIndex
br_pixelmap* GetPanelPixelmap(int pIndex);

// Offset: 10512
// Size: 1433
void LoadInterfaceStrings();

// Offset: 11948
// Size: 114
void FlushInterfaceFonts();

// Offset: 12064
// Size: 54
void SuspendPendingFlic();

// Offset: 12120
// Size: 44
void ResumePendingFlic();

