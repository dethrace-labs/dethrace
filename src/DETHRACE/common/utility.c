#include "utility.h"
#include <stdlib.h>

#include "brender.h"
#include "dossys.h"
#include "errors.h"
#include "globvars.h"
#include "loading.h"
#include "network.h"
#include "sound.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Added >>
#define MIN_SERVICE_INTERVAL 200
// <<

tU32 gLong_key[] = { 0x5F991B6C, 0x135FCDB9, 0x0E2004CB, 0x0EA11C5E };
tU32 gOther_long_key[] = { 0x26D6A867, 0x1B45DDB6, 0x13227E32, 0x3794C215 };

char* gMisc_strings[250];
int gIn_check_quit;
tU32 gLost_time;
int gEncryption_method = 0;
br_pixelmap* g16bit_palette;
br_pixelmap* gSource_for_16bit_palette;

// Offset: 0
// Size: 144
int CheckQuit() {
    int got_as_far_as_verify;
    NOT_IMPLEMENTED();
}

// Offset: 196
// Size: 542
// EAX: pS
void EncodeLine(char* pS) {
    int len;
    int seed;
    int i;
    char* key;
    unsigned char c;
    FILE* test;

    tPath_name test_path;
    char temp[256];

    len = strlen(pS);
    key = (char*)gLong_key;
    if (!gEncryption_method) {
        strcpy(test_path, gApplication_path);
        strcat(test_path, gDir_separator);
        strcat(test_path, "GENERAL.TXT");

        test = fopen(test_path, "rt");
        if (test) {
            fgets(temp, 256, test);
            if (temp[0] != '@') {
                gEncryption_method = 2;
            } else {
                gEncryption_method = 1;
                EncodeLine(&temp[1]);
                temp[7] = '\0';
                if (strcmp(&temp[1], "0.01\t\t") != 0) {
                    gEncryption_method = 2;
                }
            }
            fclose(test);
        } else {
            gEncryption_method = 2;
        }
    }
    while (len > 0 && (pS[len - 1] == '\r' || pS[len - 1] == '\n')) {
        --len;
        pS[len] = 0;
    }

    seed = len % 16;
    for (i = 0; i < len; i++) {
        c = pS[i];
        if (i >= 2) {
            if (pS[i - 1] == '/' && pS[i - 2] == '/') {
                key = (char*)gOther_long_key;
            }
        }
        if (gEncryption_method == 1) {
            if (c == '\t') {
                c = 0x80;
            }
            c -= 0x20;
            if (!(c & 0x80)) {
                c = (c ^ key[seed]) & 0x7f;
                c += 0x20;
            }
            seed += 7;
            seed = seed % 16;

            if (c == 0x80) {
                c = '\t';
            }
        } else {
            if (c == '\t') {
                c = 0x9f;
            }
            c -= 0x20;
            c = (c ^ key[seed]) & 0x7f;
            c += 0x20;

            seed += 7;
            seed = seed % 16;

            if (c == 0x9f) {
                c = '\t';
            }
        }
        pS[i] = c;
    }
}

// Offset: 740
// Size: 73
// EAX: pA
// EDX: pB
int IRandomBetween(int pA, int pB) {
    int num;
    char s[32];
    NOT_IMPLEMENTED();
}

// Offset: 816
// Size: 79
// EAX: pC
int PercentageChance(int pC) {
    NOT_IMPLEMENTED();
}

// Offset: 896
// Size: 56
// EAX: pN
int IRandomPosNeg(int pN) {
    NOT_IMPLEMENTED();
}

// Offset: 952
// Size: 72
float FRandomBetween(float pA, float pB) {
    NOT_IMPLEMENTED();
}

// Offset: 1024
// Size: 69
float FRandomPosNeg(float pN) {
    NOT_IMPLEMENTED();
}

// Offset: 1096
// Size: 67
br_scalar SRandomBetween(br_scalar pA, br_scalar pB) {
    NOT_IMPLEMENTED();
}

// Offset: 1164
// Size: 69
br_scalar SRandomPosNeg(br_scalar pN) {
    NOT_IMPLEMENTED();
}

// Offset: 1236
// Size: 506
// EAX: pF
// EDX: pS
char* GetALineWithNoPossibleService(FILE* pF, /*unsigned*/ char* pS) {
    // JeffH removed "signed' to avoid compiler warnings..
    /*signed*/ char* result;
    /*signed*/ char s[256];
    int ch;
    int len;
    int i;

    do {
        result = fgets(s, 256, pF);
        if (!result) {
            break;
        }
        if (s[0] == '@') {
            EncodeLine(&s[1]);
            goto LABEL_5;
        }
        while (1) {
            if (s[0] != ' ' && s[0] != '\t') {
                break;
            }
        LABEL_5:
            len = strlen(s);
            memmove(s, &s[1], len);
        }

        while (1) {
            ch = fgetc(pF);
            if (ch != '\r' && ch != '\n') {
                break;
            }
        }
        if (ch != -1) {
            ungetc(ch, pF);
        }
    } while (!isalnum(s[0])
        && s[0] != '-'
        && s[0] != '.'
        && s[0] != '!'
        && s[0] != '&'
        && s[0] != '('
        && s[0] != '\''
        && s[0] != '\"'
        && s[0] >= 0);

    if (result) {
        len = strlen(result);
        if (len != 0 && (result[len - 1] == '\r' || result[len - 1] == '\n')) {
            result[len - 1] = 0;
        }
        if (len != 1 && (result[len - 2] == '\r' || result[len - 2] == '\n')) {
            result[len - 2] = 0;
        }
    }
    strcpy((char*)pS, s);
    len = strlen(s);
    for (i = 0; i < len; i++) {
        if (pS[i] >= 0xE0u) {
            pS[i] -= 32;
        }
    }
    return result;
}

// Offset: 1744
// Size: 60
// EAX: pF
// EDX: pS
char* GetALineAndDontArgue(FILE* pF, char* pS) {
    PossibleService();
    return GetALineWithNoPossibleService(pF, pS);
    NOT_IMPLEMENTED();
}

// Offset: 1804
// Size: 86
// EAX: pDestn_str
// EDX: pStr_1
// EBX: pStr_2
void PathCat(char* pDestn_str, char* pStr_1, char* pStr_2) {
    if (pDestn_str != pStr_1) { // Added to avoid strcpy overlap checks
        strcpy(pDestn_str, pStr_1);
    }
    if (strlen(pStr_2) != 0) {
        strcat(pDestn_str, gDir_separator);
        strcat(pDestn_str, pStr_2);
    }
}

// Offset: 1892
// Size: 96
int Chance(float pChance_per_second, int pPeriod) {
    NOT_IMPLEMENTED();
}

// Offset: 1988
// Size: 96
float tandeg(float pAngle) {
    NOT_IMPLEMENTED();
}

// Offset: 2084
// Size: 80
// EAX: pF
tU32 GetFileLength(FILE* pF) {
    tU32 the_size;
    NOT_IMPLEMENTED();
}

// Offset: 2164
// Size: 68
// EAX: pB
int BooleanTo1Or0(int pB) {
    NOT_IMPLEMENTED();
}

// Offset: 2232
// Size: 114
// EAX: pType
// EDX: pW
// EBX: pH
// ECX: pPixels
br_pixelmap* DRPixelmapAllocate(br_uint_8 pType, br_uint_16 pW, br_uint_16 pH, void* pPixels, int pFlags) {
    br_pixelmap* the_map;
    the_map = BrPixelmapAllocate(pType, pW, pH, pPixels, pFlags);
    if (the_map) {
        the_map->origin_y = 0;
        the_map->origin_x = 0;
    }
    return the_map;
}

// Offset: 2348
// Size: 118
// EAX: pPm
// EDX: pX
// EBX: pY
// ECX: pW
br_pixelmap* DRPixelmapAllocateSub(br_pixelmap* pPm, br_uint_16 pX, br_uint_16 pY, br_uint_16 pW, br_uint_16 pH) {
    br_pixelmap* the_map;
    the_map = BrPixelmapAllocateSub(pPm, pX, pY, pW, pH);
    if (the_map) {
        the_map->origin_y = 0;
        the_map->origin_x = 0;
    }
    return the_map;
    NOT_IMPLEMENTED();
}

// Offset: 2468
// Size: 195
// EAX: pSrc
// EDX: pMatch_type
// EBX: pWidth
// ECX: pHeight
br_pixelmap* DRPixelmapMatchSized(br_pixelmap* pSrc, tU8 pMatch_type, tS32 pWidth, tS32 pHeight) {
    br_pixelmap* result;
    NOT_IMPLEMENTED();
}

// Offset: 2664
// Size: 357
// EAX: pDst
// EDX: pSrc
// EBX: pSrc_width
// ECX: pSrc_height
void CopyDoubled8BitTo16BitRectangle(br_pixelmap* pDst, br_pixelmap* pSrc, int pSrc_width, int pSrc_height, int pDst_x, int pDst_y, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start0;
    tU16* dst_start1;
    tU16* palette_entry;
    NOT_IMPLEMENTED();
}

// Offset: 3024
// Size: 262
// EAX: pSrc
// EDX: pWidth
// EBX: pHeight
br_pixelmap* Scale8BitPixelmap(br_pixelmap* pSrc, int pWidth, int pHeight) {
    br_pixelmap* result;
    int x;
    int y;
    tU8* src_pixels;
    tU8* dst_pixels;
    NOT_IMPLEMENTED();
}

// Offset: 3288
// Size: 390
// EAX: pSrc
// EDX: pN
br_pixelmap* Tile8BitPixelmap(br_pixelmap* pSrc, int pN) {
    br_pixelmap* result;
    int new_width;
    int new_height;
    int x;
    int y2;
    int y;
    tU8* src_pixels;
    tU8* dst_pixels;
    NOT_IMPLEMENTED();
}

// Offset: 3680
// Size: 83
// EAX: pName
// EDX: pList
tException_list FindExceptionInList(char* pName, tException_list pList) {
    NOT_IMPLEMENTED();
}

// Offset: 3764
// Size: 492
// EAX: pSrc
br_pixelmap* PurifiedPixelmap(br_pixelmap* pSrc) {
    br_pixelmap* intermediate;
    br_pixelmap* result;
    int new_width;
    int new_height;
    tException_list e;
    NOT_IMPLEMENTED();
}

// Offset: 4256
// Size: 105
// EAX: pFile_name
br_pixelmap* DRPixelmapLoad(char* pFile_name) {
    br_pixelmap* the_map;
    LOG_TRACE("(\"%s\")", pFile_name);
    br_int_8 lobyte;

    the_map = BrPixelmapLoad(pFile_name);
    if (the_map) {
        the_map->origin_x = 0;
        the_map->origin_y = 0;

        lobyte = the_map->row_bytes & 0xff;
        lobyte += 3;
        lobyte &= 0xfc;
        the_map->row_bytes = (the_map->row_bytes & 0xff00) | lobyte;
    }
    return the_map;
}

// Offset: 4364
// Size: 174
// EAX: pFile_name
// EDX: pPixelmaps
// EBX: pNum
br_uint_32 DRPixelmapLoadMany(char* pFile_name, br_pixelmap** pPixelmaps, br_uint_16 pNum) {
    br_pixelmap* the_map;
    int number_loaded;
    int i;
    LOG_TRACE("(\"%s\", %p, %d)", pFile_name, pPixelmaps, pNum);
    br_uint_8 lobyte;

    number_loaded = BrPixelmapLoadMany(pFile_name, pPixelmaps, pNum);
    for (i = 0; i < number_loaded; i++) {
        the_map = pPixelmaps[i];
        lobyte = the_map->row_bytes & 0xff;
        lobyte += 3;
        lobyte &= 0xfc;
        the_map->row_bytes = (the_map->row_bytes & 0xff00) | lobyte;
        the_map->base_x = 0;
        the_map->base_y = 0;
    }
    return number_loaded;
}

// Offset: 4540
// Size: 67
// EAX: pDelay
void WaitFor(tU32 pDelay) {
    tU32 start_time;
    NOT_IMPLEMENTED();
}

// Offset: 4608
// Size: 136
// EAX: pActor
// EDX: callback
// EBX: arg
br_uint_32 DRActorEnumRecurse(br_actor* pActor, br_actor_enum_cbfn* callback, void* arg) {
    br_uint_32 result;

    result = callback(pActor, arg);
    if (result == 0) {
        pActor = pActor->children;
        if (pActor) {
            while (1) {
                result = DRActorEnumRecurse(pActor, callback, arg);
                if (result != 0) {
                    break;
                }
                pActor = pActor->next;
                if (!pActor) {
                    return 0;
                }
            }
        }
    }
    return result;
}

// Offset: 4744
// Size: 77
br_uint_32 CompareActorID(br_actor* pActor, void* pArg) {
    NOT_IMPLEMENTED();
}

// Offset: 4824
// Size: 60
// EAX: pSearch_root
// EDX: pName
br_actor* DRActorFindRecurse(br_actor* pSearch_root, char* pName) {
    NOT_IMPLEMENTED();
}

// Offset: 4884
// Size: 156
// EAX: pActor
// EDX: pMat
// EBX: pCall_back
// ECX: pArg
br_uint_32 DRActorEnumRecurseWithMat(br_actor* pActor, br_material* pMat, br_uint_32 (*pCall_back)(br_actor*, br_material*, void*), void* pArg) {
    br_uint_32 result;
    NOT_IMPLEMENTED();
}

// Offset: 5040
// Size: 188
// EAX: pActor
// EDX: pMatrix
// EBX: pCall_back
// ECX: pArg
br_uint_32 DRActorEnumRecurseWithTrans(br_actor* pActor, br_matrix34* pMatrix, br_uint_32 (*pCall_back)(br_actor*, br_matrix34*, void*), void* pArg) {
    br_uint_32 result;
    br_matrix34 combined_transform;
    NOT_IMPLEMENTED();
}

// Offset: 5228
// Size: 89
// EAX: pNumber
int sign(int pNumber) {
    NOT_IMPLEMENTED();
}

// Offset: 5320
// Size: 98
float fsign(float pNumber) {
    NOT_IMPLEMENTED();
}

// Offset: 5420
// Size: 210
// EAX: pPrefix
// EDX: pExtension
FILE* OpenUniqueFileB(char* pPrefix, char* pExtension) {
    int index;
    FILE* f;
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

// Offset: 5632
// Size: 572
// EAX: pF
void PrintScreenFile(FILE* pF) {
    int i;
    int j;
    int bit_map_size;
    int offset;
    tU8* pixel_ptr;
    NOT_IMPLEMENTED();
}

// Offset: 6204
// Size: 554
// EAX: pF
void PrintScreenFile16(FILE* pF) {
    int i;
    int j;
    int bit_map_size;
    int offset;
    tU8* pixel_ptr;
    tU16 pixel;
    NOT_IMPLEMENTED();
}

// Offset: 6760
// Size: 95
void PrintScreen() {
    NOT_IMPLEMENTED();
}

// Offset: 6856
// Size: 89
tU32 GetTotalTime() {
    NOT_IMPLEMENTED();
}

// Offset: 6948
// Size: 51
tU32 GetRaceTime() {
    NOT_IMPLEMENTED();
}

// Offset: 7000
// Size: 46
// EAX: pLost_time
void AddLostTime(tU32 pLost_time) {
    NOT_IMPLEMENTED();
}

// Offset: 7048
// Size: 208
// EAX: pTime
// EDX: pStr
// EBX: pFudge_colon
// ECX: pForce_colon
void TimerString(tU32 pTime, char* pStr, int pFudge_colon, int pForce_colon) {
    int seconds;
    NOT_IMPLEMENTED();
}

// Offset: 7256
// Size: 55
// EAX: pIndex
char* GetMiscString(int pIndex) {
    return gMisc_strings[pIndex];
}

// Offset: 7312
// Size: 58
// EAX: pIndex
// EDX: pStr
void GetCopyOfMiscString(int pIndex, char* pStr) {
    NOT_IMPLEMENTED();
}

// Offset: 7372
// Size: 113
// EAX: pPeriod
// EDX: pLast_change
// EBX: pCurrent_state
int Flash(tU32 pPeriod, tU32* pLast_change, int* pCurrent_state) {
    tU32 the_time;
    NOT_IMPLEMENTED();
}

// Offset: 7488
// Size: 225
// EAX: pDst
// EDX: pSrc
void MaterialCopy(br_material* pDst, br_material* pSrc) {
    NOT_IMPLEMENTED();
}

// Offset: 7716
// Size: 139
// EAX: pColour_1
// EDX: pColour_2
double RGBDifferenceSqr(tRGB_colour* pColour_1, tRGB_colour* pColour_2) {
    NOT_IMPLEMENTED();
}

// Offset: 7856
// Size: 200
// EAX: pRGB_colour
// EDX: pPalette
int FindBestMatch(tRGB_colour* pRGB_colour, br_pixelmap* pPalette) {
    int n;
    int near_c;
    double min_d;
    double d;
    tRGB_colour trial_RGB;
    br_colour* dp;
    NOT_IMPLEMENTED();
}

// Offset: 8056
// Size: 232
// EAX: pThe_path
// EDX: pR
// EBX: pG
// ECX: pB
void BuildShadeTablePath(char* pThe_path, int pR, int pG, int pB) {
    char s[32];
    NOT_IMPLEMENTED();
}

// Offset: 8288
// Size: 80
// EAX: pR
// EDX: pG
// EBX: pB
br_pixelmap* LoadGeneratedShadeTable(int pR, int pG, int pB) {
    char the_path[256];
    NOT_IMPLEMENTED();
}

// Offset: 8368
// Size: 79
// EAX: pThe_table
// EDX: pR
// EBX: pG
// ECX: pB
void SaveGeneratedShadeTable(br_pixelmap* pThe_table, int pR, int pG, int pB) {
    char the_path[256];
    NOT_IMPLEMENTED();
}

// Offset: 8448
// Size: 106
// EAX: pHeight
// EDX: pPalette
// EBX: pRed_mix
// ECX: pGreen_mix
br_pixelmap* GenerateShadeTable(int pHeight, br_pixelmap* pPalette, int pRed_mix, int pGreen_mix, int pBlue_mix, float pQuarter, float pHalf, float pThree_quarter) {
    NOT_IMPLEMENTED();
}

// Offset: 8556
// Size: 761
// EAX: pHeight
// EDX: pPalette
// EBX: pRed_mix
// ECX: pGreen_mix
br_pixelmap* GenerateDarkenedShadeTable(int pHeight, br_pixelmap* pPalette, int pRed_mix, int pGreen_mix, int pBlue_mix, float pQuarter, float pHalf, float pThree_quarter, br_scalar pDarken) {
    br_pixelmap* the_table;
    tRGB_colour the_RGB;
    tRGB_colour new_RGB;
    tRGB_colour ref_col;
    br_colour* cp;
    char* tab_ptr;
    char* shade_ptr;
    double f_i;
    double f_total_minus_1;
    double ratio1;
    double ratio2;
    int i;
    int c;
    NOT_IMPLEMENTED();
}

// Offset: 9320
// Size: 92
void PossibleService() {
    static tU32 last_service = 0;
    // Added >>
    tU32 current_time;
    // <<

    current_time = PDGetTotalTime();

    if (current_time - last_service > MIN_SERVICE_INTERVAL && !gProgram_state.racing) {
        SoundService();
        NetService(gProgram_state.racing);
        last_service = current_time;
    }
}

// Offset: 9412
// Size: 192
// EAX: pA
// EDX: pB
// EBX: pC
void DRMatrix34TApplyP(br_vector3* pA, br_vector3* pB, br_matrix34* pC) {
    br_scalar t1;
    br_scalar t2;
    br_scalar t3;
    NOT_IMPLEMENTED();
}

// Offset: 9604
// Size: 132
// EAX: pPal
// EDX: pEntry
tU16 PaletteEntry16Bit(br_pixelmap* pPal, int pEntry) {
    tU32* src_entry;
    int red;
    int green;
    int blue;
    NOT_IMPLEMENTED();
}

// Offset: 9736
// Size: 210
// EAX: pSrc
br_pixelmap* PaletteOf16Bits(br_pixelmap* pSrc) {
    tU16* dst_entry;
    int value;
    NOT_IMPLEMENTED();
}

// Offset: 9948
// Size: 219
// EAX: pDst
// EDX: pSrc
// EBX: pPalette
void Copy8BitTo16Bit(br_pixelmap* pDst, br_pixelmap* pSrc, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    NOT_IMPLEMENTED();
}

// Offset: 10168
// Size: 558
// EAX: pDst
// EDX: pDst_x
// EBX: pDst_y
// ECX: pSrc
void Copy8BitTo16BitRectangle(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    NOT_IMPLEMENTED();
}

// Offset: 10728
// Size: 518
// EAX: pDst
// EDX: pDst_x
// EBX: pDst_y
// ECX: pSrc
void Copy8BitTo16BitRectangleWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    NOT_IMPLEMENTED();
}

// Offset: 11248
// Size: 279
// EAX: pDst
// EDX: pDst_x
// EBX: pDst_y
// ECX: pSrc
void Copy8BitToOnscreen16BitRectangleWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    NOT_IMPLEMENTED();
}

// Offset: 11528
// Size: 601
// EAX: pDst
// EDX: pDst_x
// EBX: pDst_y
// ECX: pSrc
void Copy8BitRectangleTo16BitRhombusWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, tX1616 pShear, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    tX1616 total_shear;
    tS16 sheared_x;
    tS16 clipped_src_x;
    tS16 clipped_width;
    NOT_IMPLEMENTED();
}

// Offset: 12132
// Size: 173
// EAX: dst
// EDX: dx
// EBX: dy
// ECX: src
void DRPixelmapRectangleCopy(br_pixelmap* dst, br_int_16 dx, br_int_16 dy, br_pixelmap* src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h) {
    NOT_IMPLEMENTED();
}

// Offset: 12308
// Size: 107
// EAX: dst
// EDX: src
void DRPixelmapCopy(br_pixelmap* dst, br_pixelmap* src) {
    NOT_IMPLEMENTED();
}

// Offset: 12416
// Size: 82
// EAX: dst
// EDX: x
// EBX: y
// ECX: w
void DRPixelmapRectangleFill(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_16 w, br_uint_16 h, br_uint_32 colour) {
    NOT_IMPLEMENTED();
}

// Offset: 12500
// Size: 156
// EAX: pPoint
// EDX: pNormal
int NormalSideOfPlane(br_vector3* pPoint, br_vector3* pNormal, br_scalar pD) {
    br_scalar numer;
    br_scalar denom;
    NOT_IMPLEMENTED();
}

// Offset: 12656
// Size: 351
// EAX: pMaterial
br_material* DRMaterialClone(br_material* pMaterial) {
    br_material* the_material;
    char s[256];
    static int name_suffix;
    NOT_IMPLEMENTED();
}

// Offset: 13008
// Size: 93
// EAX: s
void StripCR(char* s) {
    char* pos;
    pos = s;
    while (*pos != 0) {
        if (*pos == '\r' || *pos == '\n') {
            *pos = 0;
            break;
        }
        pos++;
    }
}

// Offset: 13104
// Size: 132
void SubsStringJob(char* pStr, ...) {
    char* sub_str;
    char temp_str[256];
    char* sub_pt;
    va_list ap;
    NOT_IMPLEMENTED();
}

// Offset: 13236
// Size: 454
// EAX: pS
void DecodeLine2(char* pS) {
    int len;
    int seed;
    int i;
    unsigned char c;
    char* key;

    len = strlen(pS);
    key = (char*)gLong_key;
    while (len > 0 && (pS[len - 1] == 13 || pS[len - 1] == 10)) {
        --len;
        pS[len] = 0;
    }
    seed = len % 16;
    for (i = 0; i < len; i++) {
        c = pS[i];
        if (i >= 2) {
            if (pS[i - 1] == '/' && pS[i - 2] == '/') {
                key = (char*)gOther_long_key;
            }
        }
        if (gEncryption_method == 1) {
            if (c == '\t') {
                c = 0x80;
            }
            c -= 0x20;
            if (!(c & 0x80)) {
                c = (c ^ key[seed]) & 0x7f;
                c += 0x20;
            }
            seed += 7;
            seed = seed % 16;

            if (c == 0x80) {
                c = '\t';
            }
        } else {
            if (c == '\t') {
                c = 0x9f;
            }
            c -= 0x20;
            c = (c ^ key[seed]) & 0x7f;
            c += 0x20;

            seed += 7;
            seed = seed % 16;

            if (c == 0x9f) {
                c = '\t';
            }
        }
        pS[i] = c;
    }
}

// Offset: 13692
// Size: 304
// EAX: pS
void EncodeLine2(char* pS) {
    int len;
    int seed;
    int i;
    int count;
    unsigned char c;
    char* key;

    len = strlen(pS);
    count = 0;
    key = (char*)gLong_key;
    while (len > 0 && (pS[len - 1] == 13 || pS[len - 1] == 10)) {
        --len;
        pS[len] = 0;
    }

    seed = len % 16;

    for (i = 0; i < len; ++pS) {
        if (count == 2)
            key = (char*)gOther_long_key;
        if (*pS == '/') {
            ++count;
        } else {
            count = 0;
        }
        if (*pS == '\t') {
            *pS = 0x80;
        }
        c = *pS - 0x20;
        if (!(c & 0x80)) {
            c = c ^ (key[seed] & 0x7F);
            c += 0x20;
        }
        seed += 7;
        seed = seed % 16;

        if (c == 0x80) {
            c = '\t';
        }
        *pS = c;
        ++i;
    }
}

// Offset: 13996
// Size: 553
// EAX: pThe_path
void EncodeFile(char* pThe_path) {
    FILE* f;
    FILE* d;
    char line[257];
    char new_file[256];
    char* s;
    char* result;
    int ch;
    int decode;
    int len;
    int count;

    len = strlen(pThe_path);
    strcpy(new_file, pThe_path);
    strcat(new_file, "ENC");

    f = fopen(pThe_path, "rt");
    if (!f) {
        FatalError(0x6b, new_file);
    }

    ch = fgetc(f);
    ungetc(ch, f);

    if (gDecode_thing == '@' && gDecode_thing == (char)ch) {
        fclose(f);
        return;
    }

    d = fopen(new_file, "wb");
    if (!d) {
        FatalError(0x6b, new_file);
    }

    result = &line[1];

    while (!feof(f)) {
        fgets(result, 256, f);

        if (ch == '@') {
            decode = 1;
        } else {
            decode = 0;
            s = &result[1];
            while (line[0] == ' ' || line[0] == '\t') {
                memmove(result, s, strlen(result));
            }
        }

        if (decode == 0) {
            EncodeLine2(result + decode);
        } else {
            DecodeLine2(result + decode);
        }

        line[0] = '@';
        fputs(&line[decode * 2], d);
        count = -1;
        ch = fgetc(f);
        while (ch == '\r' || ch == '\n') {
            count++;
        }
        if (count >= 2) {
            fputc(0x0d, d);
            fputc(0x0a, d);
        }
        fputc(0x0d, d);
        fputc(0x0a, d);

        if (ch != -1) {
            ungetc(ch, f);
        }
    }
    fclose(f);
    fclose(d);

    PDFileUnlock(pThe_path);
    unlink(pThe_path);
    rename(new_file, pThe_path);
}

// Offset: 14552
// Size: 513
// EAX: pThe_path
void EncodeFileWrapper(char* pThe_path) {
    int len = strlen(pThe_path);

    if (strcmp(&pThe_path[len - 4], ".TXT") == 0) {
        return;
    }
    if (strcmp(pThe_path, "DKEYMAP0.TXT") == 0) {
        return;
    }
    if (strcmp(pThe_path, "DKEYMAP1.TXT") == 0) {
        return;
    }
    if (strcmp(pThe_path, "DKEYMAP2.TXT") == 0) {
        return;
    }
    if (strcmp(pThe_path, "DKEYMAP3.TXT") == 0) {
        return;
    }
    if (strcmp(pThe_path, "KEYMAP_0.TXT") == 0) {
        return;
    }
    if (strcmp(pThe_path, "KEYMAP_1.TXT") == 0) {
        return;
    }
    if (strcmp(pThe_path, "KEYMAP_2.TXT") == 0) {
        return;
    }
    if (strcmp(pThe_path, "KEYMAP_3.TXT") == 0) {
        return;
    }
    if (strcmp(pThe_path, "OPTIONS.TXT") == 0) {
        return;
    }
    if (strcmp(pThe_path, "KEYNAMES.TXT") == 0) {
        return;
    }
    if (strcmp(pThe_path, "KEYMAP.TXT") == 0) {
        return;
    }
    if (strcmp(pThe_path, "PATHS.TXT") == 0) {
        return;
    }

    EncodeFile(pThe_path);
}

// Offset: 15068
// Size: 72
// EAX: pThe_path
void EncodeAllFilesInDirectory(char* pThe_path) {
    char s[256];
    NOT_IMPLEMENTED();
}

// Offset: 15140
// Size: 89
// EAX: pF
void SkipNLines(FILE* pF) {
    int i;
    int count;
    char s[256];
    NOT_IMPLEMENTED();
}

// Offset: 15232
// Size: 118
// EAX: p1
// EDX: p2
int DRStricmp(char* p1, char* p2) {
    int val;
    while (p1) {
        val = tolower(*p1) - tolower(*p2);
        if (val != 0) {
            return val;
        }
        p1++;
        p2++;
    }
    return 0;
}

// Offset: 15352
// Size: 559
// EAX: pArray
// EDX: pCount
void GlorifyMaterial(br_material** pArray, int pCount) {
    int i;
    int c;
    br_pixelmap* big_tile;
    tException_list e;
    NOT_IMPLEMENTED();
}

// Offset: 15912
// Size: 178
// EAX: pArray
// EDX: pN
void WhitenVertexRGB(br_model** pArray, int pN) {
    int m;
    int v;
    br_vertex* vertex;
    NOT_IMPLEMENTED();
}

// Offset: 16092
// Size: 239
// EAX: pPalette
void NobbleNonzeroBlacks(br_pixelmap* pPalette) {
    tU32 red;
    tU32 green;
    tU32 blue;
    tU32 value;
    tU32* palette_entry;
    tU32 frobbed;
    NOT_IMPLEMENTED();
}

// Offset: 16332
// Size: 55
// EAX: pThe_path
int PDCheckDriveExists(char* pThe_path) {
    LOG_TRACE("(\"%s\")", pThe_path);
    return PDCheckDriveExists2(pThe_path, NULL, 0);
}

// Offset: 16388
// Size: 102
// EAX: pPrims
int OpacityInPrims(br_token_value* pPrims) {
    NOT_IMPLEMENTED();
}

// Offset: 16492
// Size: 97
// EAX: pMaterial
int AlreadyBlended(br_material* pMaterial) {
    NOT_IMPLEMENTED();
}

// Offset: 16592
// Size: 148
// EAX: pMaterial
// EDX: pPercent
void BlendifyMaterialTablishly(br_material* pMaterial, int pPercent) {
    char* s;
    NOT_IMPLEMENTED();
}

// Offset: 16740
// Size: 110
// EAX: pMaterial
// EDX: pPercent
void BlendifyMaterialPrimitively(br_material* pMaterial, int pPercent) {
    static br_token_value alpha25[3];
    static br_token_value alpha50[3];
    static br_token_value alpha75[3];
    NOT_IMPLEMENTED();
}

// Offset: 16852
// Size: 73
// EAX: pMaterial
// EDX: pPercent
void BlendifyMaterial(br_material* pMaterial, int pPercent) {
    NOT_IMPLEMENTED();
}
