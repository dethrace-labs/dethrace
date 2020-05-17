#include "flicplay.h"
#include <stdlib.h>

tFlic_bunch gFlic_bunch[9];
int gFlic_bunch8[16];
int gFlic_bunch4[22];
int gFlic_bunch2[8];
tFlic_spec gMain_flic_list[372];
tU32 gPanel_flic_data_length[2];
tU32 gLast_panel_frame_time[2];
tU8* gPanel_flic_data[2];
int gPanel_flic_top[2];
tFlic_descriptor gPanel_flic[2];
br_pixelmap* gPanel_buffer[2];
int gPanel_flic_left[2];
int gFlic_bunch6[51];
int gFlic_bunch7[7];
int gFlic_bunch5[5];
int gFlic_bunch3[13];
int gFlic_bunch0[29];
int gFlic_bunch1[31];
tU32 gSound_time;
int gTrans_enabled;
int gPanel_flic_disable;
int gDark_mode;
int gPending_pending_flic;
int gTransparency_on;
int gSound_ID;
int gPlay_from_disk;
int gTranslation_count;
int gPending_flic;
tDR_font* gTrans_fonts[15];
int gPalette_fuck_prevention;
tTranslation_record* gTranslations;
br_pixelmap* gPalette;
void* gPalette_pixels;
int gPalette_allocate_count;
tFlic_descriptor* gFirst_flic;
char gLast_flic_name[14];

// Offset: 0
// Size: 44
void EnableTranslationText() {
    NOT_IMPLEMENTED();
}

// Offset: 44
// Size: 44
void DisableTranslationText() {
    NOT_IMPLEMENTED();
}

// Offset: 88
// Size: 54
// EAX: pSound_ID
// EDX: pSound_time
void SetFlicSound(int pSound_ID, tU32 pSound_time) {
    NOT_IMPLEMENTED();
}

// Offset: 144
// Size: 45
int TranslationMode() {
    NOT_IMPLEMENTED();
}

// Offset: 192
// Size: 44
void DontLetFlicFuckWithPalettes() {
    NOT_IMPLEMENTED();
}

// Offset: 236
// Size: 44
void LetFlicFuckWithPalettes() {
    NOT_IMPLEMENTED();
}

// Offset: 280
// Size: 44
void PlayFlicsInDarkness() {
    NOT_IMPLEMENTED();
}

// Offset: 324
// Size: 49
void ReilluminateFlics() {
    NOT_IMPLEMENTED();
}

// Offset: 376
// Size: 44
void TurnFlicTransparencyOn() {
    NOT_IMPLEMENTED();
}

// Offset: 420
// Size: 44
void TurnFlicTransparencyOff() {
    NOT_IMPLEMENTED();
}

// Offset: 464
// Size: 44
void PlayFlicsFromDisk() {
    NOT_IMPLEMENTED();
}

// Offset: 508
// Size: 44
void PlayFlicsFromMemory() {
    NOT_IMPLEMENTED();
}

// Offset: 552
// Size: 45
int FlicsPlayedFromDisk() {
    NOT_IMPLEMENTED();
}

// Offset: 600
// Size: 44
void TurnOffPanelFlics() {
    NOT_IMPLEMENTED();
}

// Offset: 644
// Size: 44
void TurnOnPanelFlics() {
    NOT_IMPLEMENTED();
}

// Offset: 688
// Size: 53
// EAX: pIndex
int GetPanelFlicFrameIndex(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 744
// Size: 91
void FlicPaletteAllocate() {
    NOT_IMPLEMENTED();
}

// Offset: 836
// Size: 92
// EAX: pFlic_info
// EDX: pDest_pixelmap
void AssertFlicPixelmap(tFlic_descriptor_ptr pFlic_info, br_pixelmap* pDest_pixelmap) {
    NOT_IMPLEMENTED();
}

// Offset: 928
// Size: 791
// EAX: pFile_name
// EDX: pIndex
// EBX: pFlic_info
// ECX: pSize
int StartFlic(char* pFile_name, int pIndex, tFlic_descriptor_ptr pFlic_info, tU32 pSize, tS8* pData_ptr, br_pixelmap* pDest_pixelmap, int pX_offset, int pY_offset, int pFrame_rate) {
    tU16 claimed_speed;
    tU16 magic_number;
    tPath_name the_path;
    int total_size;
    NOT_IMPLEMENTED();
}

// Offset: 1720
// Size: 96
void FreeFlicPaletteAllocate() {
    NOT_IMPLEMENTED();
}

// Offset: 1816
// Size: 118
// EAX: pFlic_info
int EndFlic(tFlic_descriptor_ptr pFlic_info) {
    NOT_IMPLEMENTED();
}

// Offset: 1936
// Size: 312
// EAX: pFlic_info
// EDX: chunk_length
void DoColourMap(tFlic_descriptor_ptr pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int packet_count;
    int skip_count;
    int change_count;
    int current_colour;
    tU8* palette_pixels;
    tU8 red;
    tU8 green;
    tU8 blue;
    NOT_IMPLEMENTED();
}

// Offset: 2248
// Size: 343
// EAX: pFlic_info
// EDX: chunk_length
void DoDifferenceX(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int k;
    int first_line;
    int line_count;
    int number_of_packets;
    int skip_count;
    int size_count;
    tU8* pixel_ptr;
    tU8* line_pixel_ptr;
    tU8 the_byte;
    tU32 the_row_bytes;
    NOT_IMPLEMENTED();
}

// Offset: 2592
// Size: 377
// EAX: pFlic_info
// EDX: chunk_length
void DoDifferenceTrans(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int k;
    int first_line;
    int line_count;
    int number_of_packets;
    int skip_count;
    int size_count;
    tU8* pixel_ptr;
    tU8* line_pixel_ptr;
    tU8 the_byte;
    tU32 the_row_bytes;
    NOT_IMPLEMENTED();
}

// Offset: 2972
// Size: 303
// EAX: pFlic_info
// EDX: chunk_length
void DoColour256(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int packet_count;
    int skip_count;
    int change_count;
    int current_colour;
    tU8* palette_pixels;
    tU8 red;
    tU8 green;
    tU8 blue;
    NOT_IMPLEMENTED();
}

// Offset: 3276
// Size: 537
// EAX: pFlic_info
// EDX: chunk_length
void DoDeltaTrans(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int k;
    int line_count;
    int number_of_packets;
    int skip_count;
    int size_count;
    tU8* pixel_ptr;
    tU8 the_byte;
    tU8 the_byte2;
    tU32 the_row_bytes;
    tU16* line_pixel_ptr;
    tU16 the_word;
    NOT_IMPLEMENTED();
}

// Offset: 3816
// Size: 354
// EAX: pFlic_info
// EDX: chunk_length
void DoDeltaX(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int k;
    int line_count;
    int number_of_packets;
    int skip_count;
    int size_count;
    tU8* pixel_ptr;
    tU32 the_row_bytes;
    tU16* line_pixel_ptr;
    tU16 the_word;
    NOT_IMPLEMENTED();
}

// Offset: 4172
// Size: 156
// EAX: pFlic_info
// EDX: chunk_length
void DoBlack(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int the_width;
    tU8* pixel_ptr;
    tU32 the_row_bytes;
    tU32* line_pixel_ptr;
    NOT_IMPLEMENTED();
}

// Offset: 4328
// Size: 272
// EAX: pFlic_info
// EDX: chunk_length
void DoRunLengthX(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int k;
    int number_of_packets;
    int size_count;
    tU8* pixel_ptr;
    tU8* line_pixel_ptr;
    tU8 the_byte;
    tU32 the_row_bytes;
    NOT_IMPLEMENTED();
}

// Offset: 4600
// Size: 309
// EAX: pFlic_info
// EDX: chunk_length
void DoRunLengthTrans(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int k;
    int number_of_packets;
    int size_count;
    tU8* pixel_ptr;
    tU8* line_pixel_ptr;
    tU8 the_byte;
    tU32 the_row_bytes;
    NOT_IMPLEMENTED();
}

// Offset: 4912
// Size: 160
// EAX: pFlic_info
// EDX: chunk_length
void DoUncompressed(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int the_width;
    tU8* pixel_ptr;
    tU32 the_row_bytes;
    tU32* line_pixel_ptr;
    NOT_IMPLEMENTED();
}

// Offset: 5072
// Size: 176
// EAX: pFlic_info
// EDX: chunk_length
void DoUncompressedTrans(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int the_width;
    tU8* pixel_ptr;
    tU8* line_pixel_ptr;
    tU8 the_byte;
    tU32 the_row_bytes;
    NOT_IMPLEMENTED();
}

// Offset: 5248
// Size: 52
// EAX: pFlic_info
// EDX: chunk_length
void DoMini(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    NOT_IMPLEMENTED();
}

// Offset: 5300
// Size: 377
// EAX: pFlic_info
// EDX: pLast_frame
void DrawTranslations(tFlic_descriptor* pFlic_info, int pLast_frame) {
    tTranslation_record* trans;
    int i;
    int x;
    int width;
    int right_edge;
    NOT_IMPLEMENTED();
}

// Offset: 5680
// Size: 783
// EAX: pFlic_info
// EDX: pPanel_flic
int PlayNextFlicFrame2(tFlic_descriptor* pFlic_info, int pPanel_flic) {
    tU32 frame_length;
    tU32 chunk_length;
    int chunk_count;
    int chunk_counter;
    int chunk_type;
    int magic_bytes;
    int last_frame;
    int data_knocked_off;
    int read_amount;
    NOT_IMPLEMENTED();
}

// Offset: 6464
// Size: 53
// EAX: pFlic_info
int PlayNextFlicFrame(tFlic_descriptor* pFlic_info) {
    NOT_IMPLEMENTED();
}

// Offset: 6520
// Size: 295
// EAX: pIndex
// EDX: pSize
// EBX: pData_ptr
// ECX: pDest_pixelmap
int PlayFlic(int pIndex, tU32 pSize, tS8* pData_ptr, br_pixelmap* pDest_pixelmap, int pX_offset, int pY_offset, void (*DoPerFrame)(), int pInterruptable, int pFrame_rate) {
    int finished_playing;
    tFlic_descriptor the_flic;
    tU32 last_frame;
    tU32 new_time;
    tU32 frame_period;
    NOT_IMPLEMENTED();
}

// Offset: 6816
// Size: 41
void SwapScreen() {
    NOT_IMPLEMENTED();
}

// Offset: 6860
// Size: 154
// EAX: pIndex
void ShowFlic(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 7016
// Size: 76
void InitFlics() {
    NOT_IMPLEMENTED();
}

// Offset: 7092
// Size: 395
// EAX: pIndex
int LoadFlic(int pIndex) {
    tPath_name the_path;
    FILE* f;
    char* the_buffer;
    NOT_IMPLEMENTED();
}

// Offset: 7488
// Size: 75
// EAX: pIndex
void UnlockFlic(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 7564
// Size: 257
// EAX: pName
// EDX: pData
// EBX: pData_length
int LoadFlicData(char* pName, tU8** pData, tU32* pData_length) {
    FILE* f;
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

// Offset: 7824
// Size: 87
// EAX: pIndex
void FreeFlic(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 7912
// Size: 61
// EAX: pIndex
void ForceRunFlic(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 7976
// Size: 107
// EAX: pIndex
// EDX: pX
// EBX: pY
void RunFlicAt(int pIndex, int pX, int pY) {
    NOT_IMPLEMENTED();
}

// Offset: 8084
// Size: 117
// EAX: pIndex
void RunFlic(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 8204
// Size: 105
// EAX: pBunch_index
void PreloadBunchOfFlics(int pBunch_index) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 8312
// Size: 105
// EAX: pBunch_index
void UnlockBunchOfFlics(int pBunch_index) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 8420
// Size: 73
// EAX: pBunch_index
void FlushAllFlics(int pBunch_index) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 8496
// Size: 44
void InitFlicQueue() {
    NOT_IMPLEMENTED();
}

// Offset: 8540
// Size: 87
int FlicQueueFinished() {
    NOT_IMPLEMENTED();
}

// Offset: 8628
// Size: 230
// EAX: pInterval
void ProcessFlicQueue(tU32 pInterval) {
    tFlic_descriptor* the_flic;
    tFlic_descriptor* last_flic;
    tFlic_descriptor* doomed_flic;
    tU32 new_time;
    int finished_playing;
    NOT_IMPLEMENTED();
}

// Offset: 8860
// Size: 138
void FlushFlicQueue() {
    tFlic_descriptor* old_flic;
    NOT_IMPLEMENTED();
}

// Offset: 9000
// Size: 424
// EAX: pIndex
// EDX: pX
// EBX: pY
// ECX: pMust_finish
void AddToFlicQueue(int pIndex, int pX, int pY, int pMust_finish) {
    tFlic_descriptor* the_flic;
    tFlic_descriptor* new_flic;
    tFlic_descriptor* last_flic;
    tFlic_descriptor* doomed_flic;
    NOT_IMPLEMENTED();
}

// Offset: 9424
// Size: 205
// EAX: pIndex
// EDX: pLeft
// EBX: pTop
// ECX: pWidth
void InitialiseFlicPanel(int pIndex, int pLeft, int pTop, int pWidth, int pHeight) {
    void* the_pixels;
    NOT_IMPLEMENTED();
}

// Offset: 9632
// Size: 112
// EAX: pIndex
void DisposeFlicPanel(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 9744
// Size: 502
// EAX: pCopy_to_buffer
void ServicePanelFlics(int pCopy_to_buffer) {
    tU32 time_diff;
    tU32 the_time;
    tU32 old_last_time[2];
    int i;
    int j;
    int iteration_count;
    int finished;
    NOT_IMPLEMENTED();
}

// Offset: 10248
// Size: 205
// EAX: pIndex
// EDX: pData
// EBX: pData_length
void ChangePanelFlic(int pIndex, tU8* pData, tU32 pData_length) {
    NOT_IMPLEMENTED();
}

// Offset: 10456
// Size: 55
// EAX: pIndex
br_pixelmap* GetPanelPixelmap(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 10512
// Size: 1433
void LoadInterfaceStrings() {
    char s[256];
    char s2[256];
    char* str;
    char* comment;
    char ch;
    tPath_name the_path;
    int i;
    int j;
    int len;
    NOT_IMPLEMENTED();
}

// Offset: 11948
// Size: 114
void FlushInterfaceFonts() {
    NOT_IMPLEMENTED();
}

// Offset: 12064
// Size: 54
void SuspendPendingFlic() {
    NOT_IMPLEMENTED();
}

// Offset: 12120
// Size: 44
void ResumePendingFlic() {
    NOT_IMPLEMENTED();
}
