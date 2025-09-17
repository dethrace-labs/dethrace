#include "utility.h"
#include <stdlib.h>

#include "brender.h"
#include "constants.h"
#include "errors.h"
#include "globvars.h"
#include "globvrbm.h"
#include "globvrpb.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include "input.h"
#include "loading.h"
#include "loadsave.h"
#include "main.h"
#include "mainmenu.h"
#include "network.h"
#include "pd/sys.h"
#include "sound.h"
#include "world.h"

#include <ctype.h>
#include <float.h>
#include <stdio.h>
#include <string.h>

// Added >>
#define MIN_SERVICE_INTERVAL 200
// <<

// GLOBAL: CARM95 0x00521488
int gIn_check_quit = 0;

// GLOBAL: CARM95 0x0052148c
tU32 gLost_time = 0;

// GLOBAL: CARM95 0x00521490
// gLong_key

// GLOBAL: CARM95 0x005214a0
// gOther_long_key

#if BR_ENDIAN_BIG
tU32 gLong_key[4] = { 0x6c1b995f, 0xb9cd5f13, 0xcb04200e, 0x5e1ca10e };
tU32 gOther_long_key[4] = { 0x67a8d626, 0xb6dd451b, 0x327e2213, 0x15c29437 };
#else
tU32 gLong_key[4] = { 0x5f991b6c, 0x135fcdb9, 0x0e2004cb, 0x0ea11c5e };
tU32 gOther_long_key[4] = { 0x26d6a867, 0x1b45ddb6, 0x13227e32, 0x3794c215 };
#endif

// GLOBAL: CARM95 0x005214b0
int gEncryption_method = 0;

// GLOBAL: CARM95 0x00544ef0
char* gMisc_strings[250];

br_pixelmap* g16bit_palette;
br_pixelmap* gSource_for_16bit_palette;

// IDA: int __cdecl CheckQuit()
// FUNCTION: CARM95 0x004c1590
int CheckQuit(void) {
    int got_as_far_as_verify;

    got_as_far_as_verify = 0;
    if (!gIn_check_quit && KeyIsDown(KEYMAP_CTRL_QUIT) && KeyIsDown(KEYMAP_CONTROL_ANY)) {
        gIn_check_quit = 1;

        do {
            ;
        } while (AnyKeyDown());

        got_as_far_as_verify = 1;
        if (DoVerifyQuit(1)) {
            QuitGame();
        }
        gIn_check_quit = 0;
    }
    return got_as_far_as_verify;
}

// IDA: double __cdecl sqr(double pN)
// FUNCTION: CARM95 0x004c161d
double sqr(double pN) {

    return pN * pN;
}

// IDA: void __usercall EncodeLine(char *pS@<EAX>)
// FUNCTION: CARM95 0x004c1ab1
void EncodeLine(char* pS) {
    int len;
    int seed;
    int i;
    char* key;
    FILE* test;
    unsigned char c;

    len = strlen(pS);
    key = (char*)gLong_key;
    if (gEncryption_method == 0) {
        tPath_name the_path;
        char s[256];
        PathCat(the_path, gApplication_path, "GENERAL.TXT");

        test = fopen(the_path, "rt");
        if (test != NULL) {
            fgets(s, 256, test);
            if (s[0] == '@') {

                gEncryption_method = 1;
                EncodeLine(&s[1]);
                s[7] = '\0';
                if (strcmp(&s[1], "0.01\t\t") != 0) {
                    gEncryption_method = 2;
                }
            } else {
                gEncryption_method = 2;
            }
            fclose(test);
        } else {
            gEncryption_method = 2;
        }
    }
#ifdef DETHRACE_FIX_BUGS
    while (len != 0 && (pS[len - 1] == '\r' || pS[len - 1] == '\n')) {
#else
    while (len != 0 && pS[len - 1] == '\r' || pS[len - 1] == '\n') {
#endif
        pS[len - 1] = '\0';
        len--;
    }

    seed = len % 16;
    for (i = 0; i < len; i++) {
#if defined(DETHRACE_FIX_BUGS)
        // When loading game data, Carmageddon does not switch the XOR cypher when the comments start.
        if (i >= 2) {
            if (pS[i - 1] == '/' && pS[i - 2] == '/') {
                key = (char*)gOther_long_key;
            }
        }
#endif
        if (gEncryption_method == 1) {
            if (pS[i] == '\t') {
                pS[i] = 0x9f;
            }

            pS[i] = ((key[seed] ^ (pS[i] - 32)) & 0x7F) + 32;
            seed = (seed + 7) % 16;

            if ((signed char)pS[i] == (signed char)0x9f) {
                pS[i] = '\t';
            }
        } else {
            if (pS[i] == '\t') {
                pS[i] = 0x80;
            }

            c = pS[i] - 32;
            if ((c & 0x80u) == 0) {
                pS[i] = (c ^ (key[seed] & 0x7f)) + 32;
            }

            seed = (seed + 7) % 16;
            if ((signed char)pS[i] == (signed char)0x80) {
                pS[i] = '\t';
            }
        }
    }
}

// IDA: int __usercall IRandomBetween@<EAX>(int pA@<EAX>, int pB@<EDX>)
// FUNCTION: CARM95 0x004c1633
int IRandomBetween(int pA, int pB) {
    int num;
    char s[32];

#if RAND_MAX == 0x7fff
    //  If RAND_MAX == 0x7fff, then `num` can be seen as a fixed point number with 15 fractional and 17 integral bits
    num = (pB + 1 - pA) * rand() / (RAND_MAX + 1) + pA;
    return num;
#else
    //  If RAND_MAX != 0x7fff, then use floating numbers (alternative is using modulo)
    return pA + (int)((pB + 1 - pA) * (rand() / (float)RAND_MAX));
#endif
}

// IDA: int __usercall PercentageChance@<EAX>(int pC@<EAX>)
// FUNCTION: CARM95 0x004c166c
int PercentageChance(int pC) {

    return IRandomBetween(0, 99) < pC;
}

// IDA: int __usercall IRandomPosNeg@<EAX>(int pN@<EAX>)
// FUNCTION: CARM95 0x004c169d
int IRandomPosNeg(int pN) {

    return IRandomBetween(-pN, pN);
}

// IDA: float __cdecl FRandomBetween(float pA, float pB)
// FUNCTION: CARM95 0x004c16bf
float FRandomBetween(float pA, float pB) {
#ifdef DETHRACE_FIX_BUGS
    return (float)rand() * (pB - pA) / (float)RAND_MAX + pA;
#else
    return (float)rand() * (pB - pA) / (RAND_MAX + 1) + pA;
#endif
}

// IDA: float __cdecl FRandomPosNeg(float pN)
// FUNCTION: CARM95 0x004c16ee
float FRandomPosNeg(float pN) {

    return FRandomBetween(-pN, pN);
}

// IDA: br_scalar __cdecl SRandomBetween(br_scalar pA, br_scalar pB)
// FUNCTION: CARM95 0x004c1715
br_scalar SRandomBetween(br_scalar pA, br_scalar pB) {

    return FRandomBetween(pA, pB);
}

// IDA: br_scalar __cdecl SRandomPosNeg(br_scalar pN)
// FUNCTION: CARM95 0x004c1735
br_scalar SRandomPosNeg(br_scalar pN) {

    return SRandomBetween(-pN, pN);
}

// IDA: char* __usercall GetALineWithNoPossibleService@<EAX>(FILE *pF@<EAX>, unsigned char *pS@<EDX>)
// FUNCTION: CARM95 0x004c175c
char* GetALineWithNoPossibleService(FILE* pF, unsigned char* pS) {
    signed char* result;
    signed char s[256];
    int i;
    int ch;
    int len;
    // name unknown, this was not in the DOS symbol dump
    // the z_ prefix helps it into the correct stack position
    int z_alnum;

    do {

        result = (signed char*)fgets((char*)s, 256, pF);
        if (result == NULL) {
            break;
        }
        if (s[0] == '@') {
            EncodeLine((char*)&s[1]);
            memmove(s, &s[1], strlen((char*)s));
        }
        while (s[0] == ' ' || s[0] == '\t') {
            memmove(s, &s[1], strlen((char*)s));
        }

        do {
            do {
                ch = fgetc(pF);
            } while (ch == 13);
        } while (ch == 10);
        if (ch != -1) {
            ungetc(ch, pF);
        }
#ifdef DETHRACE_FIX_BUGS
        z_alnum = Harness_Hook_isalnum(s[0]);
#else
        z_alnum = isalnum(s[0]);
#endif
    } while (!z_alnum
        && s[0] != '-'
        && s[0] != '.'
        && s[0] != '!'
        && s[0] != '&'
        && s[0] != '('
        && s[0] != '\''
        && s[0] != '\"'
        && s[0] >= 0);

    if (result) {
        len = strlen((char*)result);
#ifdef DETHRACE_FIX_BUGS
        if (len != 0 && (result[len - 1] == '\n' || result[len - 1] == '\r')) {
#else
        if (len != 0 && result[len - 1] == '\n' || result[len - 1] == '\r') {
#endif
            result[len - 1] = 0;
        }
        len--;
#ifdef DETHRACE_FIX_BUGS
        if (len != 0 && (result[len - 1] == '\n' || result[len - 1] == '\r')) {
#else
        if (len != 0 && result[len - 1] == '\n' || result[len - 1] == '\r') {
#endif
            result[len - 1] = 0;
        }
    }
    strcpy((char*)pS, (char*)s);
    len = strlen((char*)pS);
    for (i = 0; i < len; i++) {
        if (pS[i] >= 0xe0) {
            pS[i] -= 32;
        }
    }
    return (char*)pS;
}

// IDA: char* __usercall GetALineAndDontArgue@<EAX>(FILE *pF@<EAX>, char *pS@<EDX>)
// FUNCTION: CARM95 0x004c1d44
char* GetALineAndDontArgue(FILE* pF, char* pS) {

    PossibleService();
    return GetALineWithNoPossibleService(pF, (unsigned char*)pS);
}

// IDA: void __usercall PathCat(char *pDestn_str@<EAX>, char *pStr_1@<EDX>, char *pStr_2@<EBX>)
// FUNCTION: CARM95 0x004c1d69
void PathCat(char* pDestn_str, char* pStr_1, char* pStr_2) {
#ifdef DETHRACE_FIX_BUGS
    // Added to avoid strcpy overlap checks
    if (pDestn_str != pStr_1) {
        strcpy(pDestn_str, pStr_1);
    }
#else
    strcpy(pDestn_str, pStr_1);
#endif
    if (strlen(pStr_2) != 0) {
        strcat(pDestn_str, gDir_separator);
        strcat(pDestn_str, pStr_2);
    }
}

// IDA: int __cdecl Chance(float pChance_per_second, int pPeriod)
// FUNCTION: CARM95 0x004c1e16
int Chance(float pChance_per_second, int pPeriod) {

    return pPeriod * pChance_per_second / 1000.0 >= FRandomBetween(0.0f, 1.0f);
}

// IDA: float __cdecl tandeg(float pAngle)
// FUNCTION: CARM95 0x004c1e63
float tandeg(float pAngle) {

    pAngle = DEG_TO_RAD(pAngle);
    return sin(pAngle) / cos(pAngle);
}

// IDA: tU32 __usercall GetFileLength@<EAX>(FILE *pF@<EAX>)
// FUNCTION: CARM95 0x004c1e94
tU32 GetFileLength(FILE* pF) {
    tU32 the_size;

    fseek(pF, 0, SEEK_END);
    the_size = ftell(pF);
    rewind(pF);
    return the_size;
}

// IDA: int __usercall BooleanTo1Or0@<EAX>(int pB@<EAX>)
// FUNCTION: CARM95 0x004c1ed5
int BooleanTo1Or0(int pB) {

    return pB != 0;
}

// IDA: br_pixelmap* __usercall DRPixelmapAllocate@<EAX>(br_uint_8 pType@<EAX>, br_uint_16 pW@<EDX>, br_uint_16 pH@<EBX>, void *pPixels@<ECX>, int pFlags)
// FUNCTION: CARM95 0x004c1efb
br_pixelmap* DRPixelmapAllocate(br_uint_8 pType, br_uint_16 pW, br_uint_16 pH, void* pPixels, int pFlags) {
    br_pixelmap* the_map;

    the_map = BrPixelmapAllocate(pType, pW, pH, pPixels, pFlags);
    if (the_map != NULL) {
        the_map->origin_x = 0;
        the_map->origin_y = 0;
    }
    return the_map;
}

// IDA: br_pixelmap* __usercall DRPixelmapAllocateSub@<EAX>(br_pixelmap *pPm@<EAX>, br_uint_16 pX@<EDX>, br_uint_16 pY@<EBX>, br_uint_16 pW@<ECX>, br_uint_16 pH)
// FUNCTION: CARM95 0x004c1f56
br_pixelmap* DRPixelmapAllocateSub(br_pixelmap* pPm, br_uint_16 pX, br_uint_16 pY, br_uint_16 pW, br_uint_16 pH) {
    br_pixelmap* the_map;

    the_map = BrPixelmapAllocateSub(pPm, pX, pY, pW, pH);
    if (the_map != NULL) {
        the_map->origin_x = 0;
        the_map->origin_y = 0;
    }
    return the_map;
}

// IDA: br_pixelmap* __usercall DRPixelmapMatchSized@<EAX>(br_pixelmap *pSrc@<EAX>, tU8 pMatch_type@<EDX>, tS32 pWidth@<EBX>, tS32 pHeight@<ECX>)
br_pixelmap* DRPixelmapMatchSized(br_pixelmap* pSrc, tU8 pMatch_type, tS32 pWidth, tS32 pHeight) {
    br_pixelmap* result;
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

    palette_entry = PaletteOf16Bits(pPalette)->pixels;

    for (y = 0; y < pSrc_height; y++) {
        src_start = ((tU8*)pSrc->pixels) + pSrc->row_bytes * y;
        dst_start0 = (tU16*)(((tU8*)pDst->pixels) + pDst->row_bytes * pDst_y);
        dst_start0 += pDst_x;
        dst_start1 = (tU16*)(((tU8*)pDst->pixels) + pDst->row_bytes * (pDst_y + 1));
        dst_start1 += pDst_x;

        for (x = 0; x < pSrc_width; x++) {
            dst_start0[0] = palette_entry[*src_start];
            dst_start0[1] = palette_entry[*src_start];
            dst_start1[0] = palette_entry[*src_start];
            dst_start1[1] = palette_entry[*src_start];
            src_start++;
            dst_start0 += 2;
            dst_start1 += 2;
        }
        pDst_y += 2;
    }
}

// IDA: br_pixelmap* __usercall Scale8BitPixelmap@<EAX>(br_pixelmap *pSrc@<EAX>, int pWidth@<EDX>, int pHeight@<EBX>)
br_pixelmap* Scale8BitPixelmap(br_pixelmap* pSrc, int pWidth, int pHeight) {
    br_pixelmap* result;
    int x;
    int y;
    tU8* src_pixels;
    tU8* dst_pixels;
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
    NOT_IMPLEMENTED();
}

// IDA: tException_list __usercall FindExceptionInList@<EAX>(char *pName@<EAX>, tException_list pList@<EDX>)
tException_list FindExceptionInList(char* pName, tException_list pList) {

    while (pList) {
        if (DRStricmp(pName, pList->name) == 0) {
            return pList;
        }
        pList = pList->next;
    }
    return NULL;
}

// IDA: br_pixelmap* __usercall PurifiedPixelmap@<EAX>(br_pixelmap *pSrc@<EAX>)
br_pixelmap* PurifiedPixelmap(br_pixelmap* pSrc) {
    br_pixelmap* intermediate;
    br_pixelmap* result;
    int new_width;
    int new_height;
    tException_list e;

    // dethrace: added conditional to allow both software and 3dfx modes
    if (!harness_game_config.opengl_3dfx_mode) {
        return pSrc;
    }

    LOG_INFO("PurifiedPixelmap not implemented");
    return pSrc;
}

// IDA: br_pixelmap* __usercall DRPixelmapLoad@<EAX>(char *pFile_name@<EAX>)
// FUNCTION: CARM95 0x004c1fbb
br_pixelmap* DRPixelmapLoad(char* pFile_name) {
    br_pixelmap* the_map;

    the_map = BrPixelmapLoad(pFile_name);
    if (the_map != NULL) {
        the_map->row_bytes = (the_map->row_bytes + sizeof(tS32) - 1) & ~(sizeof(tS32) - 1);
        the_map->origin_x = 0;
        the_map->origin_y = 0;
    }
    return the_map;
}

// IDA: br_uint_32 __usercall DRPixelmapLoadMany@<EAX>(char *pFile_name@<EAX>, br_pixelmap **pPixelmaps@<EDX>, br_uint_16 pNum@<EBX>)
// FUNCTION: CARM95 0x004c2010
br_uint_32 DRPixelmapLoadMany(char* pFile_name, br_pixelmap** pPixelmaps, br_uint_16 pNum) {
    br_pixelmap* the_map;
    int number_loaded;
    int i;

    number_loaded = BrPixelmapLoadMany(pFile_name, pPixelmaps, pNum);
    for (i = 0; i < number_loaded; i++) {
        pPixelmaps[i]->row_bytes = (pPixelmaps[i]->row_bytes + sizeof(tS32) - 1) & ~(sizeof(tS32) - 1);
        pPixelmaps[i]->base_x = 0;
        pPixelmaps[i]->base_y = 0;
    }
    return number_loaded;
}

// IDA: void __usercall WaitFor(tU32 pDelay@<EAX>)
// FUNCTION: CARM95 0x004c209b
void WaitFor(tU32 pDelay) {
    tU32 start_time;

    start_time = PDGetTotalTime();
    while (start_time + pDelay > PDGetTotalTime()) {
        SoundService();
    }
}

// IDA: br_uint_32 __usercall DRActorEnumRecurse@<EAX>(br_actor *pActor@<EAX>, br_actor_enum_cbfn *callback@<EDX>, void *arg@<EBX>)
// FUNCTION: CARM95 0x004c20ce
br_uintptr_t DRActorEnumRecurse(br_actor* pActor, br_actor_enum_cbfn* callback, void* arg) {
    br_uintptr_t result;

    result = callback(pActor, arg);
    if (result != 0) {
        return result;
    }

    pActor = pActor->children;
    while (pActor) {
        result = DRActorEnumRecurse(pActor, callback, arg);
        if (result != 0) {
            return result;
        }
        pActor = pActor->next;
    }
    return 0;
}

// IDA: br_uint_32 __cdecl CompareActorID(br_actor *pActor, void *pArg)
// FUNCTION: CARM95 0x004c2174
br_uintptr_t CompareActorID(br_actor* pActor, void* pArg) {

    if (pActor->identifier && !strcmp(pActor->identifier, (const char*)pArg)) {
        return (intptr_t)pActor;
    } else {
        return 0;
    }
}

// IDA: br_actor* __usercall DRActorFindRecurse@<EAX>(br_actor *pSearch_root@<EAX>, char *pName@<EDX>)
// FUNCTION: CARM95 0x004c214f
br_actor* DRActorFindRecurse(br_actor* pSearch_root, char* pName) {

    return (br_actor*)DRActorEnumRecurse(pSearch_root, CompareActorID, pName);
}

// IDA: br_uint_32 __usercall DRActorEnumRecurseWithMat@<EAX>(br_actor *pActor@<EAX>, br_material *pMat@<EDX>, br_uint_32 (*pCall_back)(br_actor*, br_material*, void*)@<EBX>, void *pArg@<ECX>)
// FUNCTION: CARM95 0x004c21e9
br_uint_32 DRActorEnumRecurseWithMat(br_actor* pActor, br_material* pMat, recurse_with_mat_cbfn* pCall_back, void* pArg) {
    br_uint_32 result;

    if (pActor->material != NULL) {
        pMat = pActor->material;
    }
    result = pCall_back(pActor, pMat, pArg);
    if (result != 0) {
        return result;
    }
    pActor = pActor->children;
    while (pActor != NULL) {
        result = DRActorEnumRecurseWithMat(pActor, pMat, pCall_back, pArg);
        if (result != 0) {
            return result;
        }
        pActor = pActor->next;
    }
    return 0;
}

// IDA: br_uint_32 __usercall DRActorEnumRecurseWithTrans@<EAX>(br_actor *pActor@<EAX>, br_matrix34 *pMatrix@<EDX>, br_uint_32 (*pCall_back)(br_actor*, br_matrix34*, void*)@<EBX>, void *pArg@<ECX>)
// FUNCTION: CARM95 0x004c2288
br_uint_32 DRActorEnumRecurseWithTrans(br_actor* pActor, br_matrix34* pMatrix, br_uint_32 (*pCall_back)(br_actor*, br_matrix34*, void*), void* pArg) {
    br_uint_32 result;
    br_matrix34 combined_transform;

    if (pMatrix == NULL) {
        BrMatrix34Copy(&combined_transform, &pActor->t.t.mat);
    } else {
        BrMatrix34Mul(&combined_transform, pMatrix, &pActor->t.t.mat);
    }
    result = pCall_back(pActor, &combined_transform, pArg);
    if (result != 0) {
        return result;
    }
    pActor = pActor->children;
    while (pActor != NULL) {
        result = DRActorEnumRecurseWithTrans(pActor, &combined_transform, pCall_back, pArg);
        if (result != 0) {
            return result;
        }
        pActor = pActor->next;
    }
    return 0;
}

// IDA: int __usercall sign@<EAX>(int pNumber@<EAX>)
int sign(int pNumber) {

    if (pNumber > 0) {
        return 1;
    } else if (pNumber < 0) {
        return -1;
    } else {
        return 0;
    }
}

// IDA: float __cdecl fsign(float pNumber)
float fsign(float pNumber) {
    if (pNumber > 0.f) {
        return 1;
    } else if (pNumber < 0.f) {
        return -1.f;
    } else {
        return 0.f;
    }
}

// IDA: FILE* __usercall OpenUniqueFileB@<EAX>(char *pPrefix@<EAX>, char *pExtension@<EDX>)
// FUNCTION: CARM95 0x004c23e7
FILE* OpenUniqueFileB(char* pPrefix, char* pExtension) {
    int index;
    FILE* f;
    tPath_name the_path;

    index = 0;
    for (index = 0; index < 10000; index++) {
        PathCat(the_path, gApplication_path, pPrefix);
        sprintf(the_path + strlen(the_path), "%04d.%s", index, pExtension);
        f = DRfopen(the_path, "rt");
        if (f == NULL) {
            f = DRfopen(the_path, "wb");
            return f;
        }
        fclose(f);
    }
    return NULL;
}

// IDA: void __usercall PrintScreenFile(FILE *pF@<EAX>)
// FUNCTION: CARM95 0x004c24c8
void PrintScreenFile(FILE* pF) {
    int i;
    int j;
    int bit_map_size;
    int offset;
    tU8* pixel_ptr;

    bit_map_size = gBack_screen->row_bytes * gBack_screen->height;
    offset = 0x436;

    // 1. BMP Header
    //    1. 'BM' Signature
    WriteU8L(pF, 'B');
    WriteU8L(pF, 'M');
    //    2. File size in bytes (header = 0xe bytes; infoHeader = 0x28 bytes; colorTable = 0x400 bytes; pixelData = xxx)
    WriteU32L(pF, offset + bit_map_size);
    //    3. unused
    WriteU16L(pF, 0);
    //    4. unused
    WriteU16L(pF, 0);
    //    5. pixelData offset (from beginning of file)
    WriteU32L(pF, offset);

    // 2. Info Header
    //    1. InfoHeader Size
    WriteU32L(pF, 0x28);
    //    2. Width of bitmap in pixels
    WriteU32L(pF, gBack_screen->row_bytes);
    //    3. Height of bitmap in pixels
    WriteU32L(pF, gBack_screen->height);
    //    4. Number of planes
    WriteU16L(pF, 1);
    //    5. Bits per pixels / palletization (8 -> 8bit palletized ==> #colors = 256)
    WriteU16L(pF, 8);
    //    6. Compression (0 = BI_RGB -> no compression)
    WriteU32L(pF, 0);
    //    7. Image Size (0 --> no compression)
    WriteU32L(pF, 0);
    //    8. Horizontal Pixels Per Meter
    WriteU32L(pF, 0);
    //    9. Vertical Pixels Per Meter
    WriteU32L(pF, 0);
    //    10. # Actually used colors
    WriteU32L(pF, 0);
    //    11. Number of important colors
    WriteU32L(pF, 256);

    // 3. Color table (=palette)
    for (i = 0; i < 256; i++) {
        // red, green, blue, unused
#if BR_ENDIAN_BIG
        WriteU8L(pF, ((tU8*)gCurrent_palette->pixels)[4 * i + 3]);
        WriteU8L(pF, ((tU8*)gCurrent_palette->pixels)[4 * i + 2]);
        WriteU8L(pF, ((tU8*)gCurrent_palette->pixels)[4 * i + 1]);
#else
        WriteU8L(pF, ((tU8*)gCurrent_palette->pixels)[4 * i]);
        WriteU8L(pF, ((tU8*)gCurrent_palette->pixels)[4 * i + 1]);
        WriteU8L(pF, ((tU8*)gCurrent_palette->pixels)[4 * i + 2]);
#endif
        WriteU8L(pF, 0);
    }

    // 4. Pixel Data (=LUT)
    // offset = bit_map_size - gBack_screen->row_bytes;
    pixel_ptr = (tU8*)gBack_screen->pixels + bit_map_size - gBack_screen->row_bytes;
    for (i = 0; i < gBack_screen->height; i++) {
        for (j = 0; j < gBack_screen->row_bytes; j++) {
            WriteU8L(pF, *pixel_ptr++);
        }
        pixel_ptr -= 2 * gBack_screen->row_bytes;
    }
    WriteU16L(pF, 0);
}

// IDA: void __usercall PrintScreenFile16(FILE *pF@<EAX>)
void PrintScreenFile16(FILE* pF) {
    int i;
    int j;
    int bit_map_size;
    int offset;
    tU8* pixel_ptr;
    tU16 pixel;
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PrintScreen()
// FUNCTION: CARM95 0x004c272c
void PrintScreen(void) {
    FILE* f;

    f = OpenUniqueFileB("DUMP", "BMP");
    if (f != NULL) {

#ifdef DETHRACE_3DFX_PATCH
        if (gBack_screen->type == BR_PMT_RGB_565) {
            PrintScreenFile16(f);
        } else
#endif
        {
            PrintScreenFile(f);
        }
        fclose(f);
    }
}

// IDA: tU32 __cdecl GetTotalTime()
// FUNCTION: CARM95 0x004c2771
tU32 GetTotalTime(void) {

    if (gAction_replay_mode) {
        return gLast_replay_frame_time;
    } else if (gNet_mode != eNet_mode_none) {
        return PDGetTotalTime();
    }

    return PDGetTotalTime() - gLost_time;
}

// IDA: tU32 __cdecl GetRaceTime()
// FUNCTION: CARM95 0x004c27bf
tU32 GetRaceTime(void) {

    return GetTotalTime() - gRace_start;
}

// IDA: void __usercall AddLostTime(tU32 pLost_time@<EAX>)
// FUNCTION: CARM95 0x004c27da
void AddLostTime(tU32 pLost_time) {

    gLost_time += pLost_time;
}

// IDA: void __usercall TimerString(tU32 pTime@<EAX>, char *pStr@<EDX>, int pFudge_colon@<EBX>, int pForce_colon@<ECX>)
// FUNCTION: CARM95 0x004c27ee
void TimerString(tU32 pTime, char* pStr, int pFudge_colon, int pForce_colon) {
    int seconds;

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
// FUNCTION: CARM95 0x004c289f
char* GetMiscString(int pIndex) {

    return gMisc_strings[pIndex];
}

// IDA: void __usercall GetCopyOfMiscString(int pIndex@<EAX>, char *pStr@<EDX>)
void GetCopyOfMiscString(int pIndex, char* pStr) {

    strcpy(pStr, GetMiscString(pIndex));
}

// IDA: int __usercall Flash@<EAX>(tU32 pPeriod@<EAX>, tU32 *pLast_change@<EDX>, int *pCurrent_state@<EBX>)
// FUNCTION: CARM95 0x004c28f0
int Flash(tU32 pPeriod, tU32* pLast_change, int* pCurrent_state) {
    tU32 the_time;

    the_time = PDGetTotalTime();
    if (the_time - *pLast_change > pPeriod) {
        *pCurrent_state = !*pCurrent_state;
        *pLast_change = the_time;
    }
    return *pCurrent_state;
}

// IDA: void __usercall MaterialCopy(br_material *pDst@<EAX>, br_material *pSrc@<EDX>)
// FUNCTION: CARM95 0x004c294c
void MaterialCopy(br_material* pDst, br_material* pSrc) {

    pDst->flags = pSrc->flags;
    pDst->ka = pSrc->ka;
    pDst->kd = pSrc->kd;
    pDst->ks = pSrc->ks;
    pDst->power = pSrc->power;
    pDst->colour = pSrc->colour;
    pDst->index_base = pSrc->index_base;
    pDst->index_range = pSrc->index_range;
    pDst->index_shade = pSrc->index_shade;
    pDst->colour_map = pSrc->colour_map;
    pDst->map_transform = pSrc->map_transform;
    pDst->identifier = pSrc->identifier;
}

// IDA: double __usercall RGBDifferenceSqr@<ST0>(tRGB_colour *pColour_1@<EAX>, tRGB_colour *pColour_2@<EDX>)
// FUNCTION: CARM95 0x004c2f71
double RGBDifferenceSqr(tRGB_colour* pColour_1, tRGB_colour* pColour_2) {

    return ((pColour_1->red - pColour_2->red) * (pColour_1->red - pColour_2->red))
        + ((pColour_1->green - pColour_2->green) * (pColour_1->green - pColour_2->green))
        + ((pColour_1->blue - pColour_2->blue) * (pColour_1->blue - pColour_2->blue));
}

// IDA: int __usercall FindBestMatch@<EAX>(tRGB_colour *pRGB_colour@<EAX>, br_pixelmap *pPalette@<EDX>)
// FUNCTION: CARM95 0x004c2eb8
int FindBestMatch(tRGB_colour* pRGB_colour, br_pixelmap* pPalette) {
    int n;
    int near_c;
    double min_d;
    double d;
    tRGB_colour trial_RGB;
    br_colour* dp;

    near_c = 127;
    min_d = DBL_MAX;
    n = 0;
    dp = pPalette->pixels;
    for (; n < 256; n++, dp++) {
        trial_RGB.red = BR_RED(*dp);
        trial_RGB.green = BR_GRN(*dp);
        trial_RGB.blue = BR_BLU(*dp);
        d = RGBDifferenceSqr(pRGB_colour, &trial_RGB);
        if (d < min_d) {
            min_d = d;
            near_c = n;
        }
    }
    return near_c;
}

// IDA: void __usercall BuildShadeTablePath(char *pThe_path@<EAX>, int pR@<EDX>, int pG@<EBX>, int pB@<ECX>)
// FUNCTION: CARM95 0x004c2a2e
void BuildShadeTablePath(char* pThe_path, int pR, int pG, int pB) {
    char s[32];

    s[0] = 's';
    s[1] = 't';
    s[2] = pR / 16 + 'A';
    s[3] = pR % 16 + 'A';
    s[4] = pG / 16 + 'A';
    s[5] = pG % 16 + 'A';
    s[6] = pB / 16 + 'A';
    s[7] = pB % 16 + 'A';
    s[8] = '\0';
    strcat(s, ".TAB");
    PathCat(pThe_path, gApplication_path, "SHADETAB");
    PathCat(pThe_path, pThe_path, s);
}

// IDA: br_pixelmap* __usercall LoadGeneratedShadeTable@<EAX>(int pR@<EAX>, int pG@<EDX>, int pB@<EBX>)
// FUNCTION: CARM95 0x004c29ee
br_pixelmap* LoadGeneratedShadeTable(int pR, int pG, int pB) {
    char the_path[256];

    BuildShadeTablePath(the_path, pR, pG, pB);
    return BrPixelmapLoad(the_path);
}

// IDA: void __usercall SaveGeneratedShadeTable(br_pixelmap *pThe_table@<EAX>, int pR@<EDX>, int pG@<EBX>, int pB@<ECX>)
// FUNCTION: CARM95 0x004c2b03
void SaveGeneratedShadeTable(br_pixelmap* pThe_table, int pR, int pG, int pB) {
    char the_path[256];

    BuildShadeTablePath(the_path, pR, pG, pB);
    BrPixelmapSave(the_path, pThe_table);
}

// IDA: br_pixelmap* __usercall GenerateShadeTable@<EAX>(int pHeight@<EAX>, br_pixelmap *pPalette@<EDX>, int pRed_mix@<EBX>, int pGreen_mix@<ECX>, int pBlue_mix, float pQuarter, float pHalf, float pThree_quarter)
// FUNCTION: CARM95 0x004c2b42
br_pixelmap* GenerateShadeTable(int pHeight, br_pixelmap* pPalette, int pRed_mix, int pGreen_mix, int pBlue_mix, float pQuarter, float pHalf, float pThree_quarter) {

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
// FUNCTION: CARM95 0x004c2b84
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

    the_table = LoadGeneratedShadeTable(pRed_mix, pGreen_mix, pBlue_mix);
    if (the_table == NULL) {
        the_table = BrPixelmapAllocate(BR_PMT_INDEX_8, 256, pHeight, NULL, 0);
        if (the_table == NULL) {
            FatalError(kFatalError_LoadGeneratedShadeTable);
        }
        ref_col.red = pRed_mix;
        ref_col.green = pGreen_mix;
        ref_col.blue = pBlue_mix;
        ;

        for (tab_ptr = the_table->pixels, c = 0, cp = pPalette->pixels; c < 256; c++, cp++) {
            the_RGB.red = BR_RED(*cp) * pDarken;
            the_RGB.green = BR_GRN(*cp) * pDarken;
            the_RGB.blue = BR_BLU(*cp) * pDarken;
            shade_ptr = tab_ptr;
            tab_ptr++;
            if (pHeight == 1) {
                f_total_minus_1 = 1.;
            } else {
                f_total_minus_1 = pHeight - 1;
            }

            for (i = 0; i < pHeight; i++) {
                int unk;

                if (pHeight == 1) {
                    unk = 1;
                } else {
                    unk = i;
                }

                f_i = unk;
                ratio1 = f_i / f_total_minus_1;
                if (ratio1 < .5) {
                    if (ratio1 >= .25) {
                        ratio2 = (ratio1 - .25) * (pHalf - pQuarter) * 4. + pQuarter;
                    } else {
                        ratio2 = pQuarter * ratio1 * 4.;
                    }
                } else {
                    if (ratio1 >= 0.75) {
                        ratio2 = 1.0 - (1.0 - ratio1) * (1.0 - pThree_quarter) * 4.0;
                    } else {
                        ratio2 = (ratio1 - .5) * (pThree_quarter - pHalf) * 4. + pHalf;
                    }
                }
                new_RGB.red = (int)((double)((1.0 - ratio2) * (double)the_RGB.red) + ((double)ref_col.red * ratio2));
                new_RGB.green = ref_col.green * ratio2 + the_RGB.green * (1. - ratio2);
                new_RGB.blue = ref_col.blue * ratio2 + the_RGB.blue * (1. - ratio2);
                *shade_ptr = FindBestMatch(&new_RGB, pPalette);
                shade_ptr += 256;
            }
        }
        SaveGeneratedShadeTable(the_table, pRed_mix, pGreen_mix, pBlue_mix);
    }
    BrTableAdd(the_table);
    return the_table;
}

// IDA: void __cdecl PossibleService()
// FUNCTION: CARM95 0x004c2fdb
void PossibleService(void) {
    tU32 time;
    // GLOBAL: CARM95 0x5214b4
    static tU32 last_service = 0;

    time = PDGetTotalTime();
    if (time - last_service > MIN_SERVICE_INTERVAL && !gProgram_state.racing) {
        SoundService();
        NetService(gProgram_state.racing);
        last_service = time;
    }
}

// IDA: void __usercall DRMatrix34TApplyP(br_vector3 *pA@<EAX>, br_vector3 *pB@<EDX>, br_matrix34 *pC@<EBX>)
// FUNCTION: CARM95 0x004c302d
void DRMatrix34TApplyP(br_vector3* pA, br_vector3* pB, br_matrix34* pC) {
    br_scalar t1;
    br_scalar t2;
    br_scalar t3;

    t1 = BR_SUB(pB->v[0], pC->m[3][0]);
    t2 = BR_SUB(pB->v[1], pC->m[3][1]);
    t3 = BR_SUB(pB->v[2], pC->m[3][2]);

    // this avoids the +fstp in the line above, but including the "add" breaks it again. Some combination of braces etc...
    // pA->v[0] = BR_MUL(pC->m[0][2], t3);

    pA->v[0] = pC->m[0][2] * t3 + pC->m[0][1] * t2 + pC->m[0][0] * t1;
    pA->v[1] = pC->m[1][0] * t1 + pC->m[1][2] * t3 + pC->m[1][1] * t2;
    pA->v[2] = pC->m[2][0] * t1 + pC->m[2][1] * t2 + pC->m[2][2] * t3;
}

// IDA: tU16 __usercall PaletteEntry16Bit@<AX>(br_pixelmap *pPal@<EAX>, int pEntry@<EDX>)
tU16 PaletteEntry16Bit(br_pixelmap* pPal, int pEntry) {
    tU32* src_entry;
    int red;
    int green;
    int blue;

    src_entry = pPal->pixels;
    return ((tU8)src_entry[pEntry] >> 3) | (((src_entry[pEntry] >> 19) & 0x1F) << 11) | (32 * ((tU16)src_entry[pEntry] >> 10));
}

// IDA: br_pixelmap* __usercall PaletteOf16Bits@<EAX>(br_pixelmap *pSrc@<EAX>)
br_pixelmap* PaletteOf16Bits(br_pixelmap* pSrc) {
    tU16* dst_entry;
    int value;

    if (g16bit_palette == NULL) {
        g16bit_palette = BrPixelmapAllocate(BR_PMT_RGB_565, 1, 256, g16bit_palette, 0);
        if (g16bit_palette == NULL) {
            FatalError(kFatalError_OOMCarmageddon_S, "16-bit palette");
        }
    }
    if (!g16bit_palette_valid || pSrc != gSource_for_16bit_palette) {
        value = 0;
        dst_entry = g16bit_palette->pixels;
        for (value = 0; value < 256; value++) {
            *dst_entry = PaletteEntry16Bit(pSrc, value);
            dst_entry++;
        }
        gSource_for_16bit_palette = pSrc;
        g16bit_palette_valid = 1;
    }
    return g16bit_palette;
}

// IDA: void __usercall Copy8BitTo16Bit(br_pixelmap *pDst@<EAX>, br_pixelmap *pSrc@<EDX>, br_pixelmap *pPalette@<EBX>)
void Copy8BitTo16Bit(br_pixelmap* pDst, br_pixelmap* pSrc, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;

    palette_entry = PaletteOf16Bits(pPalette)->pixels;
    for (y = 0; y < pDst->height; y++) {
        src_start = (tU8*)pSrc->pixels + pSrc->row_bytes * y;
        dst_start = (tU16*)((tU8*)pDst->pixels + pDst->row_bytes * y);
        for (x = 0; x < pDst->width; x++) {
            *dst_start = palette_entry[*src_start];
            src_start++;
            dst_start++;
        }
    }
}

// IDA: void __usercall Copy8BitTo16BitRectangle(br_pixelmap *pDst@<EAX>, tS16 pDst_x@<EDX>, tS16 pDst_y@<EBX>, br_pixelmap *pSrc@<ECX>, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap *pPalette)
void Copy8BitTo16BitRectangle(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;

    if (pSrc_x < 0) {
        pWidth = pSrc_x + pWidth;
        pDst_x = pDst_x - pSrc_x;
        pSrc_x = 0;
    }
    if (pDst_x < 0) {
        pWidth += pDst_x;
        pSrc_x -= pDst_x;
        pDst_x = 0;
    }
    if (pSrc_y < 0) {
        pHeight = pSrc_y + pHeight;
        pDst_y = pDst_y - pSrc_y;
        pSrc_y = 0;
    }
    if (pDst_y < 0) {
        pHeight += pDst_y;
        pSrc_y -= pDst_y;
        pDst_y = 0;
    }

    if (pSrc_x + pWidth > pSrc->width) {
        pWidth = pSrc->width - pSrc_x;
    }
    if (pSrc_y + pHeight > pSrc->height) {
        pHeight = pSrc->height - pSrc_y;
    }

    if (pDst_x + pWidth > pDst->width) {
        pWidth = pDst->width - pDst_x;
    }
    if (pDst_y + pHeight > pDst->height) {
        pHeight = pDst->height - pDst_y;
    }

    palette_entry = PaletteOf16Bits(pPalette)->pixels;
    for (y = 0; y < pHeight; y++) {
        src_start = (tU8*)pSrc->pixels + (pSrc->row_bytes * (pSrc_y + y));
        src_start += pSrc_x;
        dst_start = (tU16*)((tU8*)pDst->pixels + (pDst->row_bytes * (pDst_y + y)));
        dst_start += pDst_x;
        for (x = 0; x < pWidth; x++) {
            // even though we have a specific `WithTransparency` version of this function, this one also handles transparency!
            if (*src_start != 0) {
                *dst_start = palette_entry[*src_start];
            }
            src_start++;
            dst_start++;
        }
    }
}

// IDA: void __usercall Copy8BitTo16BitRectangleWithTransparency(br_pixelmap *pDst@<EAX>, tS16 pDst_x@<EDX>, tS16 pDst_y@<EBX>, br_pixelmap *pSrc@<ECX>, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap *pPalette)
void Copy8BitTo16BitRectangleWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;

    if (pSrc_x < 0) {
        pWidth = pSrc_x + pWidth;
        pDst_x = pDst_x - pSrc_x;
        pSrc_x = 0;
    }
    if (pDst_x < 0) {
        pWidth += pDst_x;
        pSrc_x -= pDst_x;
        pDst_x = 0;
    }
    if (pSrc_y < 0) {
        pHeight = pSrc_y + pHeight;
        pDst_y = pDst_y - pSrc_y;
        pSrc_y = 0;
    }
    if (pDst_y < 0) {
        pHeight += pDst_y;
        pSrc_y -= pDst_y;
        pDst_y = 0;
    }

    if (pSrc_x + pWidth > pSrc->width) {
        pWidth = pSrc->width - pSrc_x;
    }
    if (pSrc_y + pHeight > pSrc->height) {
        pHeight = pSrc->height - pSrc_y;
    }

    if (pDst_x + pWidth > pDst->width) {
        pWidth = pDst->width - pDst_x;
    }
    if (pDst_y + pHeight > pDst->height) {
        pHeight = pDst->height - pDst_y;
    }

    palette_entry = PaletteOf16Bits(pPalette)->pixels;
    for (y = 0; y < pHeight; y++) {
        src_start = (tU8*)pSrc->pixels + (pSrc->row_bytes * (pSrc_y + y)) + pSrc_x;
        dst_start = (tU16*)((tU8*)pDst->pixels + (pDst->row_bytes * (pDst_y + y)));
        dst_start += pDst_x;
        for (x = 0; x < pWidth; x++) {
            if (*src_start != 0) {
                *dst_start = palette_entry[*src_start];
            }
            src_start++;
            dst_start++;
        }
    }
}

// IDA: void __usercall Copy8BitToOnscreen16BitRectangleWithTransparency(br_pixelmap *pDst@<EAX>, tS16 pDst_x@<EDX>, tS16 pDst_y@<EBX>, br_pixelmap *pSrc@<ECX>, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap *pPalette)
void Copy8BitToOnscreen16BitRectangleWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;

    palette_entry = PaletteOf16Bits(pPalette)->pixels;
    for (y = 0; y < pHeight; y++) {
        src_start = (tU8*)pSrc->pixels + (pSrc->row_bytes * (pSrc_y + y)) + pSrc_x;
        dst_start = (tU16*)((tU8*)pDst->pixels + (pDst->row_bytes * (pDst_y + y)));
        dst_start += pDst_x;
        for (x = 0; x < pWidth; x++) {
            if (*src_start != 0) {
                *dst_start = palette_entry[*src_start];
            }
            src_start++;
            dst_start++;
        }
    }
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

    palette_entry = PaletteOf16Bits(pPalette)->pixels;
    total_shear = 0;
    if (pSrc_y + pSrc->origin_y < 0) {
        pHeight += pSrc_y + pSrc->origin_y;
        pDst_y -= (pSrc_y + pSrc->origin_y);
        pSrc_y = 0;
    }
    if (pDst_y + pDst->origin_y < 0) {
        pHeight += pDst_y + pDst->origin_y;
        pSrc_y -= (pDst_y + pDst->origin_y);
        pDst_y = 0;
    }

    if (pSrc_y + pSrc->origin_y + pHeight > pSrc->height) {
        pHeight = pSrc->height - (pSrc_y + pSrc->origin_y);
    }

    if (pDst_y + pDst->origin_y + pHeight > pDst->height) {
        pHeight = pDst->height - pDst_y + pDst->origin_y;
    }
    if (pHeight > 0) {
        if (pSrc_x + pSrc->origin_x < 0) {
            pWidth += pSrc_x + pSrc->origin_x;
            pDst_x -= (pSrc_x + pSrc->origin_x);
            pSrc_x = 0;
        }

        if (pSrc_x + pSrc->origin_x + pWidth > pSrc->width) {
            pWidth = pSrc->width - (pSrc_x + pSrc->origin_x);
        }
        for (y = 0; y < pHeight; y++) {
            clipped_src_x = pSrc_x + pSrc->origin_x;
            sheared_x = pDst_x + pDst->origin_x + (total_shear >> 16);
            clipped_width = pWidth;
            if ((sheared_x & 0x8000u) != 0) {
                clipped_width = pWidth + sheared_x;
                clipped_src_x = (pSrc_x + pSrc->origin_x) - sheared_x;
                sheared_x = 0;
            }

            if (sheared_x + clipped_width > pDst->width) {
                clipped_width = pDst->width - sheared_x;
            }
            if (clipped_width > 0) {
                src_start = ((tU8*)pSrc->pixels) + (y + pSrc_y + pSrc->origin_y) * pSrc->row_bytes + clipped_src_x;
                dst_start = (tU16*)((tU8*)pDst->pixels + 2 * sheared_x + (y + pDst_y + pDst->origin_y) * pDst->row_bytes);

                for (x = clipped_width; x > 0; x--) {
                    if (*src_start) {
                        *dst_start = palette_entry[*src_start];
                    }
                    src_start++;
                    dst_start++;
                }
            }
            total_shear += pShear;
        }
    }
}

// IDA: void __usercall DRPixelmapRectangleCopy(br_pixelmap *dst@<EAX>, br_int_16 dx@<EDX>, br_int_16 dy@<EBX>, br_pixelmap *src@<ECX>, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h)
// FUNCTION: CARM95 0x004c30d1
void DRPixelmapRectangleCopy(br_pixelmap* dst, br_int_16 dx, br_int_16 dy, br_pixelmap* src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h) {

#ifdef DETHRACE_3DFX_PATCH
    if (dst->type == src->type) {
        BrPixelmapRectangleCopy(dst, dx, dy, src, sx, sy, w, h);
    } else if (dst->type == BR_PMT_RGB_565 && src->type == BR_PMT_INDEX_8) {
        Copy8BitTo16BitRectangle(dst, dx, dy, src, sx, sy, w, h, gCurrent_palette);
    }
#else
    BrPixelmapRectangleCopy(dst, dx, dy, src, sx, sy, w, h);
#endif
}

// IDA: void __usercall DRPixelmapCopy(br_pixelmap *dst@<EAX>, br_pixelmap *src@<EDX>)
// FUNCTION: CARM95 0x004ca180
void DRPixelmapCopy(br_pixelmap* dst, br_pixelmap* src) {

#ifdef DETHRACE_3DFX_PATCH
    if (dst->type == src->type) {
        BrPixelmapCopy(dst, src);
    } else if (dst->type == BR_PMT_RGB_565 && src->type == BR_PMT_INDEX_8) {
        Copy8BitTo16Bit(dst, src, gCurrent_palette);
    }
#else
    BrPixelmapCopy(dst, src);
#endif
}

// IDA: void __usercall DRPixelmapRectangleFill(br_pixelmap *dst@<EAX>, br_int_16 x@<EDX>, br_int_16 y@<EBX>, br_uint_16 w@<ECX>, br_uint_16 h, br_uint_32 colour)
// FUNCTION: CARM95 0x004c3112
void DRPixelmapRectangleFill(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_16 w, br_uint_16 h, br_uint_32 colour) {

    BrPixelmapRectangleFill(dst, x, y, w, h, colour);
}

// IDA: int __usercall NormalSideOfPlane@<EAX>(br_vector3 *pPoint@<EAX>, br_vector3 *pNormal@<EDX>, br_scalar pD)
// FUNCTION: CARM95 0x004c3149
int NormalSideOfPlane(br_vector3* pPoint, br_vector3* pNormal, br_scalar pD) {
    br_scalar numer;
    br_scalar denom;

    // numer = BR_MUL(pPoint->v[1], pNormal->v[1]) + BR_MUL(pPoint->v[2], pNormal->v[2]); // + BR_MUL(pNormal->v[0], pPoint->v[0]) - pD;
    numer = pNormal->v[1] * pPoint->v[1] + pNormal->v[2] * pPoint->v[2] + pNormal->v[0] * pPoint->v[0] - pD;
    denom = BR_SQR(pNormal->v[2]) + BR_SQR(pNormal->v[1]) + BR_SQR(pNormal->v[0]);
    return denom * numer >= 0.0f;
}

// IDA: br_material* __usercall DRMaterialClone@<EAX>(br_material *pMaterial@<EAX>)
// FUNCTION: CARM95 0x004c31d1
br_material* DRMaterialClone(br_material* pMaterial) {
    br_material* the_material;
    char s[256];
    // GLOBAL: CARM95 0x5214b8
    static int name_suffix = 0;

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
    sprintf(s, "%s(%d)", pMaterial->identifier, name_suffix++);
    // name_suffix++;
    the_material->identifier = BrResAllocate(the_material, strlen(s) + 1, BR_MEMORY_STRING);
    strcpy(the_material->identifier, s);
    BrMaterialAdd(the_material);
    return the_material;
}

// IDA: void __usercall StripCR(char *s@<EAX>)
// FUNCTION: CARM95 0x004c331e
void StripCR(char* s) {
    char* pos;

    pos = strchr(s, '\n');
    if (pos) {
        *pos = 0;
    }
    pos = strchr(s, '\r');
    if (pos) {
        *pos = 0;
    }
}

// IDA: void __cdecl SubsStringJob(char *pStr, ...)
// FUNCTION: CARM95 0x004c336e
void SubsStringJob(char* pStr, ...) {
    char* sub_str;
    char temp_str[256];
    char* sub_pt;
    va_list ap;

    va_start(ap, pStr);
    while ((sub_pt = strchr(pStr, '%')) != NULL) {
        sub_str = va_arg(ap, char*);
        StripCR(sub_str);
        strcpy(temp_str, &sub_pt[1]);
        strcpy(sub_pt, sub_str);
        strcat(pStr, temp_str);
    }
    va_end(ap);
}

// IDA: void __usercall DecodeLine2(char *pS@<EAX>)
// FUNCTION: CARM95 0x004c3468
void DecodeLine2(char* pS) {
    int len;
    int seed;
    int i;
    unsigned char c;
    char* key;

    len = strlen(pS);
    key = (char*)gLong_key;
#ifdef DETHRACE_FIX_BUGS
    while (len != 0 && (pS[len - 1] == '\r' || pS[len - 1] == '\n')) {
#else
    while (len != 0 && pS[len - 1] == '\r' || pS[len - 1] == '\n') {
#endif
        pS[len - 1] = 0;
        len--;
    }
    seed = len % 16;
    for (i = 0; i < len; i++) {
        if (gEncryption_method == 1) {
            if (i >= 2 && pS[i - 1] == '/' && pS[i - 2] == '/') {
                key = (char*)&gOther_long_key;
            }
            if (pS[i] == '\t') {
                pS[i] = 0x9f;
            }
            pS[i] = ((key[seed] ^ (pS[i] - 32)) & 0x7f) + 32;
            seed = (seed + 7) % 16;
            if ((signed char)pS[i] == (signed char)0x9f) {
                pS[i] = '\t';
            }
        } else {
            if (i >= 2 && pS[i - 1] == '/' && pS[i - 2] == '/') {
                key = (char*)&gOther_long_key;
            }
            if (pS[i] == '\t') {
                pS[i] = 0x80;
            }
            c = pS[i] - 32;
            if (((unsigned char)c & 0x80) == 0) {
                pS[i] = (c ^ (key[seed] & 0x7f)) + 32;
            }
            seed = (seed + 7) % 16;
            if ((signed char)pS[i] == (signed char)0x80) {
                pS[i] = '\t';
            }
        }
    }
}

// IDA: void __usercall EncodeLine2(char *pS@<EAX>)
// FUNCTION: CARM95 0x004c368f
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
#ifdef DETHRACE_FIX_BUGS
    while (len != 0 && (pS[len - 1] == '\r' || pS[len - 1] == '\n')) {
#else
    while (len != 0 && pS[len - 1] == '\r' || pS[len - 1] == '\n') {
#endif
        pS[len - 1] = 0;
        len--;
    }
    seed = len % 16;
    for (i = 0; i < len; ++i) {
        if (count == 2) {
            key = (char*)&gOther_long_key;
        }
        if (pS[i] == '/') {
            ++count;
        } else {
            count = 0;
        }
        if (pS[i] == '\t') {
            pS[i] = 0x80;
        }
        c = pS[i] - 32;
        if ((c & 0x80u) == 0) {
            pS[i] = (c ^ (key[seed] & 0x7f)) + 32;
        }
        seed = (seed + 7) % 16;
        if ((signed char)pS[i] == (signed char)0x80) {
            pS[i] = '\t';
        }
    }
}

// IDA: void __usercall EncodeFile(char *pThe_path@<EAX>)
// FUNCTION: CARM95 0x004c37f5
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

    s = line + 1;
    len = strlen(pThe_path);
    strcpy(new_file, pThe_path);
    strcpy(&new_file[len - 3], "ENC");
    f = fopen(pThe_path, "rt");
    if (!f) {
        FatalError(kFatalError_Open_S, pThe_path);
    }
    ch = fgetc(f);
    ungetc(ch, f);
    if (gDecode_thing == '@' && ch == gDecode_thing) {
        fclose(f);
    } else {
        d = fopen(new_file, "wb");
        if (!d) {
            FatalError(kFatalError_Open_S, new_file);
        }
        do {
            result = fgets(s, 256, f);
            if (result == NULL) {
                continue;
            }
            if (*s == '@') {
                decode = 1;
            } else {
                decode = 0;
                while (*s == ' ' || *s == '\t') {
                    memmove(s, s + 1, strlen(s));
                }
            }
            if (decode) {
                DecodeLine2(&s[decode]);
            } else {
                EncodeLine2(&s[decode]);
            }
            *line = '@';
            fputs(line + decode * 2, d);
            count = -1;
            do {
                do {
                    count++;
                    ch = fgetc(f);
                } while (ch == '\r');
            } while (ch == '\n');
            if (count > 2) {
                fputc('\r', d);
                fputc('\n', d);
            }
            fputc('\r', d);
            fputc('\n', d);
            if (ch != -1) {
                ungetc(ch, f);
            }

        } while (!feof(f));
        fclose(f);
        fclose(d);
        PDFileUnlock(pThe_path);
        remove(pThe_path);
        rename(new_file, pThe_path);
    }
}

// IDA: void __usercall EncodeFileWrapper(char *pThe_path@<EAX>)
// FUNCTION: CARM95 0x004c3b44
void EncodeFileWrapper(char* pThe_path) {
    int len;

#define STR_ENDS_WITH(haystack, haystack_len, needle) strcmp(haystack_len - sizeof(needle) + 1 + haystack, needle)

    len = strlen(pThe_path);

    if (STR_ENDS_WITH(pThe_path, len, ".TXT")) {
        return;
    }
    if (STR_ENDS_WITH(pThe_path, len, "DKEYMAP0.TXT")
        && STR_ENDS_WITH(pThe_path, len, "DKEYMAP1.TXT")
        && STR_ENDS_WITH(pThe_path, len, "DKEYMAP2.TXT")
        && STR_ENDS_WITH(pThe_path, len, "DKEYMAP3.TXT")
        && STR_ENDS_WITH(pThe_path, len, "KEYMAP_0.TXT")
        && STR_ENDS_WITH(pThe_path, len, "KEYMAP_1.TXT")
        && STR_ENDS_WITH(pThe_path, len, "KEYMAP_2.TXT")
        && STR_ENDS_WITH(pThe_path, len, "KEYMAP_3.TXT")
        && STR_ENDS_WITH(pThe_path, len, "OPTIONS.TXT")
        && STR_ENDS_WITH(pThe_path, len, "KEYNAMES.TXT")
        && STR_ENDS_WITH(pThe_path, len, "KEYMAP.TXT")) {

        if (!STR_ENDS_WITH(pThe_path, len, "PATHS.TXT")) {
            return;
        }
        EncodeFile(pThe_path);
    }
}

// IDA: void __usercall EncodeAllFilesInDirectory(char *pThe_path@<EAX>)
// FUNCTION: CARM95 0x004c3cf5
void EncodeAllFilesInDirectory(char* pThe_path) {
    char s[256];

    PathCat(s, gApplication_path, pThe_path);
    PDForEveryFile(s, EncodeFileWrapper);
}

// IDA: void __usercall SkipNLines(FILE *pF@<EAX>)
// FUNCTION: CARM95 0x004c3d32
void SkipNLines(FILE* pF) {
    int i;
    int count;
    char s[256];

    count = GetAnInt(pF);
    for (i = 0; i < count; i++) {
        GetALineAndDontArgue(pF, s);
    }
}

// IDA: int __usercall DRStricmp@<EAX>(char *p1@<EAX>, char *p2@<EDX>)
// FUNCTION: CARM95 0x004c3d94
int DRStricmp(char* p1, char* p2) {
    int val;

    do {
        val = tolower(*p1) - tolower(*p2);
        if (val) {
            break;
        }
        if (!*(p1++)) {
            break;
        }
    } while (*(p2++));
    return val;
}

// IDA: void __usercall GlorifyMaterial(br_material **pArray@<EAX>, int pCount@<EDX>)
void GlorifyMaterial(br_material** pArray, int pCount) {
    int i;
    int c;
    br_pixelmap* big_tile;
    tException_list e;

    // Added by dethrace.
    // `GlorifyMaterial` is only present in the 3dfx patch.
    // If software mode, don't glorify, otherwise it puts the software renderer into lit mode
    // See `WhitenVertexRGB` for a similar check that is present in the original code
    if (!harness_game_config.opengl_3dfx_mode) {
        return;
    }
    // <<<

    for (i = 0; i < pCount; i++) {
        if (pArray[i]->colour_map != NULL) {
            e = FindExceptionInList(pArray[i]->colour_map->identifier, gExceptions);

            if (gInterpolate_textures) {
                // use linear texture filtering unless we have a "nobilinear" flag or the texture has transparent parts
                if ((e == NULL || (e->flags & ExceptionFlag_NoBilinear) == 0) && !DRPixelmapHasZeros(pArray[i]->colour_map)) {
                    pArray[i]->flags |= BR_MATF_MAP_INTERPOLATION;
                }
            }
            if (gUse_mip_maps) {
                pArray[i]->flags |= BR_MATF_MAP_ANTIALIASING;
            }
            if (gPerspective_is_fast) {
                pArray[i]->flags |= BR_MATF_PERSPECTIVE;
            }
            if (e && (e->flags & ExceptionFlag_Double)) {
                pArray[i]->map_transform.m[0][0] = 0.5f;
                pArray[i]->map_transform.m[1][1] = 0.5f;
            } else if (e && (e->flags & ExceptionFlag_Quadruple)) {
                pArray[i]->map_transform.m[0][0] = 0.25f;
                pArray[i]->map_transform.m[1][1] = 0.25f;
            }
        } else {
            c = pArray[i]->index_base + pArray[i]->index_range / 2;
            pArray[i]->colour = ((br_colour*)gRender_palette->pixels)[c];
        }
        pArray[i]->ka = 1.0f;
        pArray[i]->kd = 0.0f;
        pArray[i]->ks = 0.0f;
        pArray[i]->flags &= ~BR_MATF_PRELIT;
        pArray[i]->flags |= BR_MATF_LIGHT;
    }
}

// IDA: void __usercall WhitenVertexRGB(br_model **pArray@<EAX>, int pN@<EDX>)
void WhitenVertexRGB(br_model** pArray, int pN) {
    int m;
    int v;
    br_vertex* vertex;

    if (gScreen && gScreen->type != BR_PMT_INDEX_8 && pN > 0) {
        for (m = 0; m < pN; m++) {
            vertex = pArray[m]->vertices;
            for (v = 0; v < pArray[m]->nvertices; v++, vertex++) {
                vertex->red = 255;
                vertex->grn = 255;
                vertex->blu = 255;
            }
        }
    }
}

// IDA: void __usercall NobbleNonzeroBlacks(br_pixelmap *pPalette@<EAX>)
void NobbleNonzeroBlacks(br_pixelmap* pPalette) {
    tU32 red;
    tU32 green;
    tU32 blue;
    tU32 value;
    tU32* palette_entry;
    tU32 frobbed;

    int i;

    palette_entry = pPalette->pixels;
    frobbed = 0;
    if (*palette_entry != 0) {
        *palette_entry = 0;
        frobbed = 1;
    }
    palette_entry++;
    for (i = 1; i < 256; i++) {
        blue = (*palette_entry >> 16) & 0xff;
        green = (*palette_entry >> 8) & 0xff;
        red = (*palette_entry) & 0xff;
        if (blue == 0 && green == 0 && red == 0) {
            frobbed = 1;
            *palette_entry = 0x010101;
        }
        palette_entry++;
    }
    if (frobbed) {
        BrMapUpdate(pPalette, BR_MAPU_ALL);
    }
}

// IDA: int __usercall PDCheckDriveExists@<EAX>(char *pThe_path@<EAX>)
// FUNCTION: CARM95 0x004c3e0c
int PDCheckDriveExists(char* pThe_path) {

    return PDCheckDriveExists2(pThe_path, NULL, 0);
}

// IDA: int __usercall OpacityInPrims@<EAX>(br_token_value *pPrims@<EAX>)
// FUNCTION: CARM95 0x004c3e79
int OpacityInPrims(br_token_value* pPrims) {

    for (; pPrims->t != 0 && pPrims->t != BRT_OPACITY_X; pPrims++) {
    }
    return pPrims->t != 0;
}

// IDA: int __usercall AlreadyBlended@<EAX>(br_material *pMaterial@<EAX>)
// FUNCTION: CARM95 0x004c3e2c
int AlreadyBlended(br_material* pMaterial) {

    return pMaterial->index_blend || (pMaterial->extra_prim && OpacityInPrims(pMaterial->extra_prim));
}

// IDA: void __usercall BlendifyMaterialTablishly(br_material *pMaterial@<EAX>, int pPercent@<EDX>)
// FUNCTION: CARM95 0x004c3f0d
void BlendifyMaterialTablishly(br_material* pMaterial, int pPercent) {
    char* s;

    switch (pPercent) {
    case 25:
        s = "BLEND75.TAB";
        break;
    case 50:
        s = "BLEND50.TAB";
        break;
    case 75:
        s = "BLEND25.TAB";
        break;
    default:
        PDFatalError("Invalid alpha");
    }
    pMaterial->index_blend = BrTableFind(s);
    if (pMaterial->index_blend == NULL) {
        pMaterial->index_blend = LoadSingleShadeTable(&gTrack_storage_space, s);
    }
}

// IDA: void __usercall BlendifyMaterialPrimitively(br_material *pMaterial@<EAX>, int pPercent@<EDX>)
// FUNCTION: CARM95 0x004c3fb5
void BlendifyMaterialPrimitively(br_material* pMaterial, int pPercent) {

    static br_token_value alpha25[3] = {
        { BRT_BLEND_B, { 1 } },          // .b = 1
        { BRT_OPACITY_X, { 0x400000 } }, // .x = 0x400000
        { 0, { 0 } },
    };

    static br_token_value alpha50[3] = {
        { BRT_BLEND_B, { 1 } },
        { BRT_OPACITY_X, { 0x800000 } },
        { 0, { 0 } },
    };

    static br_token_value alpha75[3] = {
        { BRT_BLEND_B, { 1 } },
        { BRT_OPACITY_X, { 0xc00000 } },
        { 0, { 0 } },
    };

    switch (pPercent) {
    case 25:
        pMaterial->extra_prim = alpha25;
        break;
    case 50:
        pMaterial->extra_prim = alpha50;
        break;
    case 75:
        pMaterial->extra_prim = alpha75;
        break;
    default:
        PDFatalError("Invalid alpha");
    }
}

// IDA: void __usercall BlendifyMaterial(br_material *pMaterial@<EAX>, int pPercent@<EDX>)
// FUNCTION: CARM95 0x004c3eca
void BlendifyMaterial(br_material* pMaterial, int pPercent) {

    if (gScreen->type == BR_PMT_INDEX_8) {
        BlendifyMaterialTablishly(pMaterial, pPercent);
    } else {
        BlendifyMaterialPrimitively(pMaterial, pPercent);
    }
}

// Added to handle demo-specific text file decryption behavior
void EncodeLine_DEMO(char* pS) {
    int len;
    int seed;
    int i;
    char* key;
    unsigned char c;
    FILE* test;
    tPath_name the_path;
#if BR_ENDIAN_BIG
    const tU32 gLong_key_DEMO[] = { 0x58503A76, 0xCBB68565, 0x15CD5B07, 0xB168DE3A };
#else
    const tU32 gLong_key_DEMO[] = { 0x763A5058, 0x6585B6CB, 0x75BCD15, 0x3ADE68B1 };
#endif

    len = strlen(pS);
    key = (char*)gLong_key_DEMO;

#ifdef DETHRACE_FIX_BUGS
    while (len != 0 && (pS[len - 1] == '\r' || pS[len - 1] == '\n')) {
#else
    while (len != 0 && pS[len - 1] == '\r' || pS[len - 1] == '\n') {
#endif
        len--;
        pS[len] = 0;
    }
    seed = len % 16;
    for (i = 0; i < len; i++) {
        c = pS[i];
        if (c == '\t') {
            c = 0x9F;
        }
        c = ((key[seed] ^ (c - 32)) & 0x7F) + 32;
        seed = (seed + 7) % 16;
        if (c == 0x9F) {
            c = '\t';
        }
        pS[i] = c;
    }
}
