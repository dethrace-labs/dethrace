#include "utility.h"
#include <stdlib.h>

#include "brender.h"
#include "constants.h"
#include "errors.h"
#include "globvars.h"
#include "globvrpb.h"
#include "input.h"
#include "loading.h"
#include "loadsave.h"
#include "mainmenu.h"
#include "network.h"
#include "pd/sys.h"
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
int gIn_check_quit = 0;
tU32 gLost_time;
int gEncryption_method = 0;
br_pixelmap* g16bit_palette;
br_pixelmap* gSource_for_16bit_palette;

// IDA: int __cdecl CheckQuit()
int CheckQuit() {
    LOG_TRACE8("()");

    if (!gIn_check_quit && KeyIsDown(KEYMAP_Q) && KeyIsDown(KEYMAP_LCTRL)) {
        gIn_check_quit = 1;
        while (AnyKeyDown()) {
            ;
        }

        if (DoVerifyQuit(1)) {
            DoSaveGame(1);
        }
        gIn_check_quit = 0;
    }
    return 0;
}

// IDA: void __usercall EncodeLine(char *pS@<EAX>)
void EncodeLine(char* pS) {
    int len;
    int seed;
    int i;
    char* key;
    unsigned char c;
    FILE* test;
    tPath_name the_path;
    char s[256];

    len = strlen(pS);
    key = (char*)gLong_key;
    if (!gEncryption_method) {
        strcpy(the_path, gApplication_path);
        strcat(the_path, gDir_separator);
        strcat(the_path, "GENERAL.TXT");

        test = fopen(the_path, "rt");
        if (test) {
            fgets(s, 256, test);
            if (s[0] != '@') {
                gEncryption_method = 2;
            } else {
                gEncryption_method = 1;
                EncodeLine(&s[1]);
                s[7] = '\0';
                if (strcmp(&s[1], "0.01\t\t") != 0) {
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

// IDA: int __usercall IRandomBetween@<EAX>(int pA@<EAX>, int pB@<EDX>)
int IRandomBetween(int pA, int pB) {
    int num;
    char s[32];
    num = (rand() % (pB - pA + 1)) + pA;
    return num;
}

// IDA: int __usercall PercentageChance@<EAX>(int pC@<EAX>)
int PercentageChance(int pC) {
    LOG_TRACE("(%d)", pC);
    return (rand() % 100) < pC;
}

// IDA: int __usercall IRandomPosNeg@<EAX>(int pN@<EAX>)
int IRandomPosNeg(int pN) {
    LOG_TRACE("(%d)", pN);
    NOT_IMPLEMENTED();
}

// IDA: float __cdecl FRandomBetween(float pA, float pB)
float FRandomBetween(float pA, float pB) {
    LOG_TRACE8("(%f, %f)", pA, pB);

    return (double)rand() * (pB - pA) / 32768.0 + pA;
}

// IDA: float __cdecl FRandomPosNeg(float pN)
float FRandomPosNeg(float pN) {
    LOG_TRACE("(%f)", pN);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl SRandomBetween(br_scalar pA, br_scalar pB)
br_scalar SRandomBetween(br_scalar pA, br_scalar pB) {
    LOG_TRACE8("(%f, %f)", pA, pB);
    return FRandomBetween(pA, pB);
}

// IDA: br_scalar __cdecl SRandomPosNeg(br_scalar pN)
br_scalar SRandomPosNeg(br_scalar pN) {
    LOG_TRACE("(%f)", pN);
    NOT_IMPLEMENTED();
}

// IDA: char* __usercall GetALineWithNoPossibleService@<EAX>(FILE *pF@<EAX>, unsigned char *pS@<EDX>)
char* GetALineWithNoPossibleService(FILE* pF, /*unsigned*/ char* pS) {
    // Jeff removed "signed' to avoid compiler warnings..
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
    //LOG_DEBUG("%s", result);
    return result;
}

// IDA: char* __usercall GetALineAndDontArgue@<EAX>(FILE *pF@<EAX>, char *pS@<EDX>)
char* GetALineAndDontArgue(FILE* pF, char* pS) {
    //LOG_TRACE10("(%p, \"%s\")", pF, pS);
    PossibleService();
    return GetALineWithNoPossibleService(pF, pS);
}

// IDA: void __usercall PathCat(char *pDestn_str@<EAX>, char *pStr_1@<EDX>, char *pStr_2@<EBX>)
void PathCat(char* pDestn_str, char* pStr_1, char* pStr_2) {
    if (pDestn_str != pStr_1) { // Added to avoid strcpy overlap checks
        strcpy(pDestn_str, pStr_1);
    }
    if (strlen(pStr_2) != 0) {
        strcat(pDestn_str, gDir_separator);
        strcat(pDestn_str, pStr_2);
    }
}

// IDA: int __cdecl Chance(float pChance_per_second, int pPeriod)
int Chance(float pChance_per_second, int pPeriod) {
    LOG_TRACE("(%f, %d)", pChance_per_second, pPeriod);
    NOT_IMPLEMENTED();
}

// IDA: float __cdecl tandeg(float pAngle)
float tandeg(float pAngle) {
    LOG_TRACE("(%f)", pAngle);
    pAngle = sin(pAngle * 3.141592653589793 / 180.0);
    return pAngle / cos(pAngle);
}

// IDA: tU32 __usercall GetFileLength@<EAX>(FILE *pF@<EAX>)
tU32 GetFileLength(FILE* pF) {
    tU32 the_size;
    fseek(pF, 0, SEEK_END);
    the_size = ftell(pF);
    rewind(pF);
    return the_size;
}

// IDA: int __usercall BooleanTo1Or0@<EAX>(int pB@<EAX>)
int BooleanTo1Or0(int pB) {
    LOG_TRACE("(%d)", pB);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __usercall DRPixelmapAllocate@<EAX>(br_uint_8 pType@<EAX>, br_uint_16 pW@<EDX>, br_uint_16 pH@<EBX>, void *pPixels@<ECX>, int pFlags)
br_pixelmap* DRPixelmapAllocate(br_uint_8 pType, br_uint_16 pW, br_uint_16 pH, void* pPixels, int pFlags) {
    br_pixelmap* the_map;
    the_map = BrPixelmapAllocate(pType, pW, pH, pPixels, pFlags);
    if (the_map) {
        the_map->origin_y = 0;
        the_map->origin_x = 0;
    }
    return the_map;
}

// IDA: br_pixelmap* __usercall DRPixelmapAllocateSub@<EAX>(br_pixelmap *pPm@<EAX>, br_uint_16 pX@<EDX>, br_uint_16 pY@<EBX>, br_uint_16 pW@<ECX>, br_uint_16 pH)
br_pixelmap* DRPixelmapAllocateSub(br_pixelmap* pPm, br_uint_16 pX, br_uint_16 pY, br_uint_16 pW, br_uint_16 pH) {
    br_pixelmap* the_map;
    LOG_TRACE("(%p, %d, %d, %d, %d)", pPm, pX, pY, pW, pH);
    the_map = BrPixelmapAllocateSub(pPm, pX, pY, pW, pH);
    if (the_map) {
        the_map->origin_y = 0;
        the_map->origin_x = 0;
    }
    return the_map;
}

// IDA: br_pixelmap* __usercall DRPixelmapMatchSized@<EAX>(br_pixelmap *pSrc@<EAX>, tU8 pMatch_type@<EDX>, tS32 pWidth@<EBX>, tS32 pHeight@<ECX>)
br_pixelmap* DRPixelmapMatchSized(br_pixelmap* pSrc, tU8 pMatch_type, tS32 pWidth, tS32 pHeight) {
    br_pixelmap* result;
    LOG_TRACE("(%p, %d, %d, %d)", pSrc, pMatch_type, pWidth, pHeight);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CopyDoubled8BitTo16BitRectangle(br_pixelmap *pDst@<EAX>, br_pixelmap *pSrc@<EDX>, int pSrc_width@<EBX>, int pSrc_height@<ECX>, int pDst_x, int pDst_y, br_pixelmap *pPalette)
void CopyDoubled8BitTo16BitRectangle(br_pixelmap* pDst, br_pixelmap* pSrc, int pSrc_width, int pSrc_height, int pDst_x, int pDst_y, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start0;
    tU16* dst_start1;
    tU16* palette_entry;
    LOG_TRACE("(%p, %p, %d, %d, %d, %d, %p)", pDst, pSrc, pSrc_width, pSrc_height, pDst_x, pDst_y, pPalette);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __usercall Scale8BitPixelmap@<EAX>(br_pixelmap *pSrc@<EAX>, int pWidth@<EDX>, int pHeight@<EBX>)
br_pixelmap* Scale8BitPixelmap(br_pixelmap* pSrc, int pWidth, int pHeight) {
    br_pixelmap* result;
    int x;
    int y;
    tU8* src_pixels;
    tU8* dst_pixels;
    LOG_TRACE("(%p, %d, %d)", pSrc, pWidth, pHeight);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __usercall Tile8BitPixelmap@<EAX>(br_pixelmap *pSrc@<EAX>, int pN@<EDX>)
br_pixelmap* Tile8BitPixelmap(br_pixelmap* pSrc, int pN) {
    br_pixelmap* result;
    int new_width;
    int new_height;
    int x;
    int y2;
    int y;
    tU8* src_pixels;
    tU8* dst_pixels;
    LOG_TRACE("(%p, %d)", pSrc, pN);
    NOT_IMPLEMENTED();
}

// IDA: tException_list __usercall FindExceptionInList@<EAX>(char *pName@<EAX>, tException_list pList@<EDX>)
tException_list FindExceptionInList(char* pName, tException_list pList) {
    LOG_TRACE("(\"%s\", %d)", pName, pList);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __usercall PurifiedPixelmap@<EAX>(br_pixelmap *pSrc@<EAX>)
br_pixelmap* PurifiedPixelmap(br_pixelmap* pSrc) {
    br_pixelmap* intermediate;
    br_pixelmap* result;
    int new_width;
    int new_height;
    tException_list e;
    LOG_TRACE("(%p)", pSrc);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __usercall DRPixelmapLoad@<EAX>(char *pFile_name@<EAX>)
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

// IDA: br_uint_32 __usercall DRPixelmapLoadMany@<EAX>(char *pFile_name@<EAX>, br_pixelmap **pPixelmaps@<EDX>, br_uint_16 pNum@<EBX>)
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

// IDA: void __usercall WaitFor(tU32 pDelay@<EAX>)
void WaitFor(tU32 pDelay) {
    tU32 start_time;
    LOG_TRACE("(%d)", pDelay);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall DRActorEnumRecurse@<EAX>(br_actor *pActor@<EAX>, br_actor_enum_cbfn *callback@<EDX>, void *arg@<EBX>)
intptr_t DRActorEnumRecurse(br_actor* pActor, br_actor_enum_cbfn* callback, void* arg) {
    intptr_t result;

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

// IDA: br_uint_32 __cdecl CompareActorID(br_actor *pActor, void *pArg)
intptr_t CompareActorID(br_actor* pActor, void* pArg) {
    LOG_TRACE("(%p, %p)", pActor, pArg);

    if (pActor->identifier && !strcmp(pActor->identifier, (const char*)pArg)) {
        return (intptr_t)pActor;
    } else {
        return 0;
    }
}

// IDA: br_actor* __usercall DRActorFindRecurse@<EAX>(br_actor *pSearch_root@<EAX>, char *pName@<EDX>)
br_actor* DRActorFindRecurse(br_actor* pSearch_root, char* pName) {
    LOG_TRACE("(%p, \"%s\")", pSearch_root, pName);
    return (br_actor*)DRActorEnumRecurse(pSearch_root, CompareActorID, pName);
}

// IDA: br_uint_32 __usercall DRActorEnumRecurseWithMat@<EAX>(br_actor *pActor@<EAX>, br_material *pMat@<EDX>, br_uint_32 (*pCall_back)(br_actor*, br_material*, void*)@<EBX>, void *pArg@<ECX>)
br_uint_32 DRActorEnumRecurseWithMat(br_actor* pActor, br_material* pMat, br_uint_32 (*pCall_back)(br_actor*, br_material*, void*), void* pArg) {
    br_uint_32 result;
    LOG_TRACE("(%p, %p, %p, %p)", pActor, pMat, pCall_back, pArg);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall DRActorEnumRecurseWithTrans@<EAX>(br_actor *pActor@<EAX>, br_matrix34 *pMatrix@<EDX>, br_uint_32 (*pCall_back)(br_actor*, br_matrix34*, void*)@<EBX>, void *pArg@<ECX>)
br_uint_32 DRActorEnumRecurseWithTrans(br_actor* pActor, br_matrix34* pMatrix, br_uint_32 (*pCall_back)(br_actor*, br_matrix34*, void*), void* pArg) {
    br_uint_32 result;
    br_matrix34 combined_transform;
    LOG_TRACE("(%p, %p, %p, %p)", pActor, pMatrix, pCall_back, pArg);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall sign@<EAX>(int pNumber@<EAX>)
int sign(int pNumber) {
    LOG_TRACE("(%d)", pNumber);
    NOT_IMPLEMENTED();
}

// IDA: float __cdecl fsign(float pNumber)
float fsign(float pNumber) {
    LOG_TRACE("(%f)", pNumber);
    NOT_IMPLEMENTED();
}

// IDA: FILE* __usercall OpenUniqueFileB@<EAX>(char *pPrefix@<EAX>, char *pExtension@<EDX>)
FILE* OpenUniqueFileB(char* pPrefix, char* pExtension) {
    int index;
    FILE* f;
    tPath_name the_path;
    LOG_TRACE("(\"%s\", \"%s\")", pPrefix, pExtension);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PrintScreenFile(FILE *pF@<EAX>)
void PrintScreenFile(FILE* pF) {
    int i;
    int j;
    int bit_map_size;
    int offset;
    tU8* pixel_ptr;
    LOG_TRACE("(%p)", pF);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PrintScreenFile16(FILE *pF@<EAX>)
void PrintScreenFile16(FILE* pF) {
    int i;
    int j;
    int bit_map_size;
    int offset;
    tU8* pixel_ptr;
    tU16 pixel;
    LOG_TRACE("(%p)", pF);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PrintScreen()
void PrintScreen() {
    FILE* f;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tU32 __cdecl GetTotalTime()
tU32 GetTotalTime() {
    LOG_TRACE9("()");
    if (gAction_replay_mode) {
        return gLast_replay_frame_time;
    }
    if (gNet_mode) {
        return PDGetTotalTime();
    }
    return PDGetTotalTime() - gLost_time;
}

// IDA: tU32 __cdecl GetRaceTime()
tU32 GetRaceTime() {
    LOG_TRACE("()");
    return GetTotalTime() - gRace_start;
}

// IDA: void __usercall AddLostTime(tU32 pLost_time@<EAX>)
void AddLostTime(tU32 pLost_time) {
    gLost_time += pLost_time;
}

// IDA: void __usercall TimerString(tU32 pTime@<EAX>, char *pStr@<EDX>, int pFudge_colon@<EBX>, int pForce_colon@<ECX>)
void TimerString(tU32 pTime, char* pStr, int pFudge_colon, int pForce_colon) {
    int seconds;
    LOG_TRACE("(%d, \"%s\", %d, %d)", pTime, pStr, pFudge_colon, pForce_colon);

    seconds = (pTime + 500) / 1000;
    if (pForce_colon || seconds > 59) {
        if (pFudge_colon) {
            sprintf(pStr, "%d/%02d", seconds / 60, seconds % 60);
        } else {
            sprintf(pStr, "%d:%02d", seconds / 60, seconds % 60);
        }
    } else {
        sprintf(pStr, "%d", seconds);
    }
}

// IDA: char* __usercall GetMiscString@<EAX>(int pIndex@<EAX>)
char* GetMiscString(int pIndex) {
    return gMisc_strings[pIndex];
}

// IDA: void __usercall GetCopyOfMiscString(int pIndex@<EAX>, char *pStr@<EDX>)
void GetCopyOfMiscString(int pIndex, char* pStr) {
    LOG_TRACE("(%d, \"%s\")", pIndex, pStr);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall Flash@<EAX>(tU32 pPeriod@<EAX>, tU32 *pLast_change@<EDX>, int *pCurrent_state@<EBX>)
int Flash(tU32 pPeriod, tU32* pLast_change, int* pCurrent_state) {
    tU32 the_time;
    LOG_TRACE("(%d, %p, %p)", pPeriod, pLast_change, pCurrent_state);

    the_time = PDGetTotalTime();
    if (the_time - *pLast_change > pPeriod) {
        if (*pCurrent_state) {
            *pCurrent_state = 0;
        } else {
            *pCurrent_state = 1;
        }
        *pLast_change = the_time;
    }
    return *pCurrent_state;
}

// IDA: void __usercall MaterialCopy(br_material *pDst@<EAX>, br_material *pSrc@<EDX>)
void MaterialCopy(br_material* pDst, br_material* pSrc) {
    LOG_TRACE("(%p, %p)", pDst, pSrc);
    NOT_IMPLEMENTED();
}

// IDA: double __usercall RGBDifferenceSqr@<ST0>(tRGB_colour *pColour_1@<EAX>, tRGB_colour *pColour_2@<EDX>)
double RGBDifferenceSqr(tRGB_colour* pColour_1, tRGB_colour* pColour_2) {
    LOG_TRACE("(%p, %p)", pColour_1, pColour_2);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FindBestMatch@<EAX>(tRGB_colour *pRGB_colour@<EAX>, br_pixelmap *pPalette@<EDX>)
int FindBestMatch(tRGB_colour* pRGB_colour, br_pixelmap* pPalette) {
    int n;
    int near_c;
    double min_d;
    double d;
    tRGB_colour trial_RGB;
    br_colour* dp;
    LOG_TRACE("(%p, %p)", pRGB_colour, pPalette);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BuildShadeTablePath(char *pThe_path@<EAX>, int pR@<EDX>, int pG@<EBX>, int pB@<ECX>)
void BuildShadeTablePath(char* pThe_path, int pR, int pG, int pB) {
    char s[32];
    LOG_TRACE("(\"%s\", %d, %d, %d)", pThe_path, pR, pG, pB);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __usercall LoadGeneratedShadeTable@<EAX>(int pR@<EAX>, int pG@<EDX>, int pB@<EBX>)
br_pixelmap* LoadGeneratedShadeTable(int pR, int pG, int pB) {
    char the_path[256];
    LOG_TRACE("(%d, %d, %d)", pR, pG, pB);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SaveGeneratedShadeTable(br_pixelmap *pThe_table@<EAX>, int pR@<EDX>, int pG@<EBX>, int pB@<ECX>)
void SaveGeneratedShadeTable(br_pixelmap* pThe_table, int pR, int pG, int pB) {
    char the_path[256];
    LOG_TRACE("(%p, %d, %d, %d)", pThe_table, pR, pG, pB);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __usercall GenerateShadeTable@<EAX>(int pHeight@<EAX>, br_pixelmap *pPalette@<EDX>, int pRed_mix@<EBX>, int pGreen_mix@<ECX>, int pBlue_mix, float pQuarter, float pHalf, float pThree_quarter)
br_pixelmap* GenerateShadeTable(int pHeight, br_pixelmap* pPalette, int pRed_mix, int pGreen_mix, int pBlue_mix, float pQuarter, float pHalf, float pThree_quarter) {
    LOG_TRACE("(%d, %p, %d, %d, %d, %f, %f, %f)", pHeight, pPalette, pRed_mix, pGreen_mix, pBlue_mix, pQuarter, pHalf, pThree_quarter);

    PossibleService();
    return GenerateDarkenedShadeTable(
        pHeight,
        pPalette,
        pRed_mix,
        pGreen_mix,
        pBlue_mix,
        pQuarter,
        pHalf,
        pThree_quarter,
        1.0);
}

// IDA: br_pixelmap* __usercall GenerateDarkenedShadeTable@<EAX>(int pHeight@<EAX>, br_pixelmap *pPalette@<EDX>, int pRed_mix@<EBX>, int pGreen_mix@<ECX>, int pBlue_mix, float pQuarter, float pHalf, float pThree_quarter, br_scalar pDarken)
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
    LOG_TRACE("(%d, %p, %d, %d, %d, %f, %f, %f, %f)", pHeight, pPalette, pRed_mix, pGreen_mix, pBlue_mix, pQuarter, pHalf, pThree_quarter, pDarken);

    STUB();
    return NULL;
}

// IDA: void __cdecl PossibleService()
void PossibleService() {
    tU32 time;
    static tU32 last_service = 0;

    time = PDGetTotalTime();
    if (time - last_service > MIN_SERVICE_INTERVAL && !gProgram_state.racing) {
        SoundService();
        NetService(gProgram_state.racing);
        last_service = time;
    }
}

// IDA: void __usercall DRMatrix34TApplyP(br_vector3 *pA@<EAX>, br_vector3 *pB@<EDX>, br_matrix34 *pC@<EBX>)
void DRMatrix34TApplyP(br_vector3* pA, br_vector3* pB, br_matrix34* pC) {
    br_scalar t1;
    br_scalar t2;
    br_scalar t3;
    LOG_TRACE("(%p, %p, %p)", pA, pB, pC);
    NOT_IMPLEMENTED();
}

// IDA: tU16 __usercall PaletteEntry16Bit@<AX>(br_pixelmap *pPal@<EAX>, int pEntry@<EDX>)
tU16 PaletteEntry16Bit(br_pixelmap* pPal, int pEntry) {
    tU32* src_entry;
    int red;
    int green;
    int blue;
    LOG_TRACE("(%p, %d)", pPal, pEntry);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __usercall PaletteOf16Bits@<EAX>(br_pixelmap *pSrc@<EAX>)
br_pixelmap* PaletteOf16Bits(br_pixelmap* pSrc) {
    tU16* dst_entry;
    int value;
    LOG_TRACE("(%p)", pSrc);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall Copy8BitTo16Bit(br_pixelmap *pDst@<EAX>, br_pixelmap *pSrc@<EDX>, br_pixelmap *pPalette@<EBX>)
void Copy8BitTo16Bit(br_pixelmap* pDst, br_pixelmap* pSrc, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    LOG_TRACE("(%p, %p, %p)", pDst, pSrc, pPalette);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall Copy8BitTo16BitRectangle(br_pixelmap *pDst@<EAX>, tS16 pDst_x@<EDX>, tS16 pDst_y@<EBX>, br_pixelmap *pSrc@<ECX>, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap *pPalette)
void Copy8BitTo16BitRectangle(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d, %p)", pDst, pDst_x, pDst_y, pSrc, pSrc_x, pSrc_y, pWidth, pHeight, pPalette);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall Copy8BitTo16BitRectangleWithTransparency(br_pixelmap *pDst@<EAX>, tS16 pDst_x@<EDX>, tS16 pDst_y@<EBX>, br_pixelmap *pSrc@<ECX>, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap *pPalette)
void Copy8BitTo16BitRectangleWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d, %p)", pDst, pDst_x, pDst_y, pSrc, pSrc_x, pSrc_y, pWidth, pHeight, pPalette);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall Copy8BitToOnscreen16BitRectangleWithTransparency(br_pixelmap *pDst@<EAX>, tS16 pDst_x@<EDX>, tS16 pDst_y@<EBX>, br_pixelmap *pSrc@<ECX>, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap *pPalette)
void Copy8BitToOnscreen16BitRectangleWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d, %p)", pDst, pDst_x, pDst_y, pSrc, pSrc_x, pSrc_y, pWidth, pHeight, pPalette);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall Copy8BitRectangleTo16BitRhombusWithTransparency(br_pixelmap *pDst@<EAX>, tS16 pDst_x@<EDX>, tS16 pDst_y@<EBX>, br_pixelmap *pSrc@<ECX>, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, tX1616 pShear, br_pixelmap *pPalette)
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
    LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d, %d, %p)", pDst, pDst_x, pDst_y, pSrc, pSrc_x, pSrc_y, pWidth, pHeight, pShear, pPalette);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRPixelmapRectangleCopy(br_pixelmap *dst@<EAX>, br_int_16 dx@<EDX>, br_int_16 dy@<EBX>, br_pixelmap *src@<ECX>, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h)
void DRPixelmapRectangleCopy(br_pixelmap* dst, br_int_16 dx, br_int_16 dy, br_pixelmap* src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h) {
    LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d)", dst, dx, dy, src, sx, sy, w, h);
    BrPixelmapRectangleCopy(dst, dx, dy, src, sx, sy, w, h);
}

// IDA: void __usercall DRPixelmapCopy(br_pixelmap *dst@<EAX>, br_pixelmap *src@<EDX>)
void DRPixelmapCopy(br_pixelmap* dst, br_pixelmap* src) {
    LOG_TRACE("(%p, %p)", dst, src);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRPixelmapRectangleFill(br_pixelmap *dst@<EAX>, br_int_16 x@<EDX>, br_int_16 y@<EBX>, br_uint_16 w@<ECX>, br_uint_16 h, br_uint_32 colour)
void DRPixelmapRectangleFill(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_16 w, br_uint_16 h, br_uint_32 colour) {
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", dst, x, y, w, h, colour);
    BrPixelmapRectangleFill(dst, x, y, w, h, colour);
}

// IDA: int __usercall NormalSideOfPlane@<EAX>(br_vector3 *pPoint@<EAX>, br_vector3 *pNormal@<EDX>, br_scalar pD)
int NormalSideOfPlane(br_vector3* pPoint, br_vector3* pNormal, br_scalar pD) {
    br_scalar numer;
    br_scalar denom;
    LOG_TRACE("(%p, %p, %f)", pPoint, pNormal, pD);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall DRMaterialClone@<EAX>(br_material *pMaterial@<EAX>)
br_material* DRMaterialClone(br_material* pMaterial) {
    br_material* the_material;
    char s[256];
    static int name_suffix = 0;
    LOG_TRACE("(%p)", pMaterial);

    the_material = BrMaterialAllocate(NULL);
    the_material->flags = pMaterial->flags;
    the_material->ka = pMaterial->ka;
    the_material->kd = pMaterial->kd;
    the_material->ks = pMaterial->ks;
    the_material->power = pMaterial->power;
    the_material->colour = pMaterial->colour;
    the_material->index_base = pMaterial->index_base;
    the_material->index_range = pMaterial->index_range;
    the_material->index_shade = pMaterial->index_shade;
    the_material->index_blend = pMaterial->index_blend;
    the_material->colour_map = pMaterial->colour_map;
    memcpy(&the_material->map_transform, &pMaterial->map_transform, sizeof(the_material->map_transform));
    sprintf(s, "%s(%d)", pMaterial->identifier, name_suffix);
    name_suffix++;
    the_material->identifier = (char*)BrResAllocate(the_material, strlen(s) + 1, BR_MEMORY_STRING);
    strcpy(the_material->identifier, s);
    BrMaterialAdd(the_material);
    return the_material;
}

// IDA: void __usercall StripCR(char *s@<EAX>)
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

// IDA: void __cdecl SubsStringJob(char *pStr, ...)
void SubsStringJob(char* pStr, ...) {
    char* sub_str;
    char temp_str[256];
    char* sub_pt;
    va_list ap;
    LOG_TRACE("(\"%s\")", pStr);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DecodeLine2(char *pS@<EAX>)
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

// IDA: void __usercall EncodeLine2(char *pS@<EAX>)
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

// IDA: void __usercall EncodeFile(char *pThe_path@<EAX>)
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
    LOG_TRACE("(\"%s\")", pThe_path);

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

// IDA: void __usercall EncodeFileWrapper(char *pThe_path@<EAX>)
void EncodeFileWrapper(char* pThe_path) {
    int len;
    LOG_TRACE("(\"%s\")", pThe_path);

    len = strlen(pThe_path);

    // if file doesn't end in .txt, bail out
    if (STR_ENDS_WITH(pThe_path, ".TXT") != 0) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, "DKEYMAP0.TXT") == 0) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, "DKEYMAP1.TXT") == 0) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, "DKEYMAP2.TXT") == 0) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, "DKEYMAP3.TXT") == 0) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, "KEYMAP_0.TXT") == 0) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, "KEYMAP_1.TXT") == 0) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, "KEYMAP_2.TXT") == 0) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, "KEYMAP_3.TXT") == 0) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, "OPTIONS.TXT") == 0) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, "KEYNAMES.TXT") == 0) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, "KEYMAP.TXT") == 0) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, "PATHS.TXT") == 0) {
        return;
    }

    EncodeFile(pThe_path);
}

// IDA: void __usercall EncodeAllFilesInDirectory(char *pThe_path@<EAX>)
void EncodeAllFilesInDirectory(char* pThe_path) {
    char s[256];
    LOG_TRACE("(\"%s\")", pThe_path);

    PathCat(s, gApplication_path, pThe_path);
    PDForEveryFile(s, EncodeFileWrapper);
}

// IDA: void __usercall SkipNLines(FILE *pF@<EAX>)
void SkipNLines(FILE* pF) {
    int i;
    int count;
    char s[256];
    LOG_TRACE("(%p)", pF);

    count = GetAnInt(pF);
    for (i = 0; i < count; i++) {
        GetALineAndDontArgue(pF, s);
    }
}

// IDA: int __usercall DRStricmp@<EAX>(char *p1@<EAX>, char *p2@<EDX>)
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

// IDA: void __usercall GlorifyMaterial(br_material **pArray@<EAX>, int pCount@<EDX>)
void GlorifyMaterial(br_material** pArray, int pCount) {
    int i;
    int c;
    br_pixelmap* big_tile;
    tException_list e;
    LOG_TRACE("(%p, %d)", pArray, pCount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall WhitenVertexRGB(br_model **pArray@<EAX>, int pN@<EDX>)
void WhitenVertexRGB(br_model** pArray, int pN) {
    int m;
    int v;
    br_vertex* vertex;
    LOG_TRACE("(%p, %d)", pArray, pN);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NobbleNonzeroBlacks(br_pixelmap *pPalette@<EAX>)
void NobbleNonzeroBlacks(br_pixelmap* pPalette) {
    tU32 red;
    tU32 green;
    tU32 blue;
    tU32 value;
    tU32* palette_entry;
    tU32 frobbed;
    LOG_TRACE("(%p)", pPalette);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDCheckDriveExists@<EAX>(char *pThe_path@<EAX>)
int PDCheckDriveExists(char* pThe_path) {
    LOG_TRACE9("(\"%s\")", pThe_path);
    return PDCheckDriveExists2(pThe_path, NULL, 0);
}

// IDA: int __usercall OpacityInPrims@<EAX>(br_token_value *pPrims@<EAX>)
int OpacityInPrims(br_token_value* pPrims) {
    LOG_TRACE("(%p)", pPrims);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall AlreadyBlended@<EAX>(br_material *pMaterial@<EAX>)
int AlreadyBlended(br_material* pMaterial) {
    LOG_TRACE("(%p)", pMaterial);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BlendifyMaterialTablishly(br_material *pMaterial@<EAX>, int pPercent@<EDX>)
void BlendifyMaterialTablishly(br_material* pMaterial, int pPercent) {
    char* s;
    LOG_TRACE("(%p, %d)", pMaterial, pPercent);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BlendifyMaterialPrimitively(br_material *pMaterial@<EAX>, int pPercent@<EDX>)
void BlendifyMaterialPrimitively(br_material* pMaterial, int pPercent) {
    static br_token_value alpha25[3];
    static br_token_value alpha50[3];
    static br_token_value alpha75[3];
    LOG_TRACE("(%p, %d)", pMaterial, pPercent);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BlendifyMaterial(br_material *pMaterial@<EAX>, int pPercent@<EDX>)
void BlendifyMaterial(br_material* pMaterial, int pPercent) {
    LOG_TRACE("(%p, %d)", pMaterial, pPercent);
    NOT_IMPLEMENTED();
}
