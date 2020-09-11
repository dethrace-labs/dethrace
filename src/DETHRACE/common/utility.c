#include "utility.h"
#include <stdlib.h>

#include "brender.h"
#include "constants.h"
#include "dossys.h"
#include "errors.h"
#include "globvars.h"
#include "input.h"
#include "loading.h"
#include "loadsave.h"
#include "mainmenu.h"
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
int gIn_check_quit = 0;
tU32 gLost_time;
int gEncryption_method = 0;
br_pixelmap* g16bit_palette;
br_pixelmap* gSource_for_16bit_palette;

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

int IRandomBetween(int pA, int pB) {
    int num;
    char s[32];
    num = (rand() % (pB - pA + 1)) + pA;
    return num;
}

int PercentageChance(int pC) {
    NOT_IMPLEMENTED();
}

int IRandomPosNeg(int pN) {
    NOT_IMPLEMENTED();
}

float FRandomBetween(float pA, float pB) {
    NOT_IMPLEMENTED();
}

float FRandomPosNeg(float pN) {
    NOT_IMPLEMENTED();
}

br_scalar SRandomBetween(br_scalar pA, br_scalar pB) {
    NOT_IMPLEMENTED();
}

br_scalar SRandomPosNeg(br_scalar pN) {
    NOT_IMPLEMENTED();
}

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

char* GetALineAndDontArgue(FILE* pF, char* pS) {
    PossibleService();
    return GetALineWithNoPossibleService(pF, pS);
    NOT_IMPLEMENTED();
}

void PathCat(char* pDestn_str, char* pStr_1, char* pStr_2) {
    if (pDestn_str != pStr_1) { // Added to avoid strcpy overlap checks
        strcpy(pDestn_str, pStr_1);
    }
    if (strlen(pStr_2) != 0) {
        strcat(pDestn_str, gDir_separator);
        strcat(pDestn_str, pStr_2);
    }
}

int Chance(float pChance_per_second, int pPeriod) {
    NOT_IMPLEMENTED();
}

float tandeg(float pAngle) {
    NOT_IMPLEMENTED();
}

tU32 GetFileLength(FILE* pF) {
    tU32 the_size;
    fseek(pF, 0, SEEK_END);
    the_size = ftell(pF);
    rewind(pF);
    return the_size;
}

int BooleanTo1Or0(int pB) {
    NOT_IMPLEMENTED();
}

br_pixelmap* DRPixelmapAllocate(br_uint_8 pType, br_uint_16 pW, br_uint_16 pH, void* pPixels, int pFlags) {
    br_pixelmap* the_map;
    the_map = BrPixelmapAllocate(pType, pW, pH, pPixels, pFlags);
    if (the_map) {
        the_map->origin_y = 0;
        the_map->origin_x = 0;
    }
    return the_map;
}

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

br_pixelmap* DRPixelmapMatchSized(br_pixelmap* pSrc, tU8 pMatch_type, tS32 pWidth, tS32 pHeight) {
    br_pixelmap* result;
    NOT_IMPLEMENTED();
}

void CopyDoubled8BitTo16BitRectangle(br_pixelmap* pDst, br_pixelmap* pSrc, int pSrc_width, int pSrc_height, int pDst_x, int pDst_y, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start0;
    tU16* dst_start1;
    tU16* palette_entry;
    NOT_IMPLEMENTED();
}

br_pixelmap* Scale8BitPixelmap(br_pixelmap* pSrc, int pWidth, int pHeight) {
    br_pixelmap* result;
    int x;
    int y;
    tU8* src_pixels;
    tU8* dst_pixels;
    NOT_IMPLEMENTED();
}

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

tException_list FindExceptionInList(char* pName, tException_list pList) {
    NOT_IMPLEMENTED();
}

br_pixelmap* PurifiedPixelmap(br_pixelmap* pSrc) {
    br_pixelmap* intermediate;
    br_pixelmap* result;
    int new_width;
    int new_height;
    tException_list e;
    NOT_IMPLEMENTED();
}

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

void WaitFor(tU32 pDelay) {
    tU32 start_time;
    NOT_IMPLEMENTED();
}

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

br_uint_32 CompareActorID(br_actor* pActor, void* pArg) {
    NOT_IMPLEMENTED();
}

br_actor* DRActorFindRecurse(br_actor* pSearch_root, char* pName) {
    NOT_IMPLEMENTED();
}

br_uint_32 DRActorEnumRecurseWithMat(br_actor* pActor, br_material* pMat, br_uint_32 (*pCall_back)(br_actor*, br_material*, void*), void* pArg) {
    br_uint_32 result;
    NOT_IMPLEMENTED();
}

br_uint_32 DRActorEnumRecurseWithTrans(br_actor* pActor, br_matrix34* pMatrix, br_uint_32 (*pCall_back)(br_actor*, br_matrix34*, void*), void* pArg) {
    br_uint_32 result;
    br_matrix34 combined_transform;
    NOT_IMPLEMENTED();
}

int sign(int pNumber) {
    NOT_IMPLEMENTED();
}

float fsign(float pNumber) {
    NOT_IMPLEMENTED();
}

FILE* OpenUniqueFileB(char* pPrefix, char* pExtension) {
    int index;
    FILE* f;
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

void PrintScreenFile(FILE* pF) {
    int i;
    int j;
    int bit_map_size;
    int offset;
    tU8* pixel_ptr;
    NOT_IMPLEMENTED();
}

void PrintScreenFile16(FILE* pF) {
    int i;
    int j;
    int bit_map_size;
    int offset;
    tU8* pixel_ptr;
    tU16 pixel;
    NOT_IMPLEMENTED();
}

void PrintScreen() {
    NOT_IMPLEMENTED();
}

tU32 GetTotalTime() {
    NOT_IMPLEMENTED();
}

tU32 GetRaceTime() {
    NOT_IMPLEMENTED();
}

void AddLostTime(tU32 pLost_time) {
    gLost_time += pLost_time;
}

void TimerString(tU32 pTime, char* pStr, int pFudge_colon, int pForce_colon) {
    int seconds;
    NOT_IMPLEMENTED();
}

char* GetMiscString(int pIndex) {
    return gMisc_strings[pIndex];
}

void GetCopyOfMiscString(int pIndex, char* pStr) {
    NOT_IMPLEMENTED();
}

int Flash(tU32 pPeriod, tU32* pLast_change, int* pCurrent_state) {
    tU32 the_time;
    NOT_IMPLEMENTED();
}

void MaterialCopy(br_material* pDst, br_material* pSrc) {
    NOT_IMPLEMENTED();
}

double RGBDifferenceSqr(tRGB_colour* pColour_1, tRGB_colour* pColour_2) {
    NOT_IMPLEMENTED();
}

int FindBestMatch(tRGB_colour* pRGB_colour, br_pixelmap* pPalette) {
    int n;
    int near_c;
    double min_d;
    double d;
    tRGB_colour trial_RGB;
    br_colour* dp;
    NOT_IMPLEMENTED();
}

void BuildShadeTablePath(char* pThe_path, int pR, int pG, int pB) {
    char s[32];
    NOT_IMPLEMENTED();
}

br_pixelmap* LoadGeneratedShadeTable(int pR, int pG, int pB) {
    char the_path[256];
    NOT_IMPLEMENTED();
}

void SaveGeneratedShadeTable(br_pixelmap* pThe_table, int pR, int pG, int pB) {
    char the_path[256];
    NOT_IMPLEMENTED();
}

br_pixelmap* GenerateShadeTable(int pHeight, br_pixelmap* pPalette, int pRed_mix, int pGreen_mix, int pBlue_mix, float pQuarter, float pHalf, float pThree_quarter) {
    NOT_IMPLEMENTED();
}

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

void DRMatrix34TApplyP(br_vector3* pA, br_vector3* pB, br_matrix34* pC) {
    br_scalar t1;
    br_scalar t2;
    br_scalar t3;
    NOT_IMPLEMENTED();
}

tU16 PaletteEntry16Bit(br_pixelmap* pPal, int pEntry) {
    tU32* src_entry;
    int red;
    int green;
    int blue;
    NOT_IMPLEMENTED();
}

br_pixelmap* PaletteOf16Bits(br_pixelmap* pSrc) {
    tU16* dst_entry;
    int value;
    NOT_IMPLEMENTED();
}

void Copy8BitTo16Bit(br_pixelmap* pDst, br_pixelmap* pSrc, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    NOT_IMPLEMENTED();
}

void Copy8BitTo16BitRectangle(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    NOT_IMPLEMENTED();
}

void Copy8BitTo16BitRectangleWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    NOT_IMPLEMENTED();
}

void Copy8BitToOnscreen16BitRectangleWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src_start;
    tU16* dst_start;
    tU16* palette_entry;
    NOT_IMPLEMENTED();
}

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

void DRPixelmapRectangleCopy(br_pixelmap* dst, br_int_16 dx, br_int_16 dy, br_pixelmap* src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h) {
    NOT_IMPLEMENTED();
}

void DRPixelmapCopy(br_pixelmap* dst, br_pixelmap* src) {
    NOT_IMPLEMENTED();
}

void DRPixelmapRectangleFill(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_16 w, br_uint_16 h, br_uint_32 colour) {
    NOT_IMPLEMENTED();
}

int NormalSideOfPlane(br_vector3* pPoint, br_vector3* pNormal, br_scalar pD) {
    br_scalar numer;
    br_scalar denom;
    NOT_IMPLEMENTED();
}

br_material* DRMaterialClone(br_material* pMaterial) {
    br_material* the_material;
    char s[256];
    static int name_suffix;
    NOT_IMPLEMENTED();
}

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

void SubsStringJob(char* pStr, ...) {
    char* sub_str;
    char temp_str[256];
    char* sub_pt;
    va_list ap;
    NOT_IMPLEMENTED();
}

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

void EncodeAllFilesInDirectory(char* pThe_path) {
    char s[256];
    NOT_IMPLEMENTED();
}

void SkipNLines(FILE* pF) {
    int i;
    int count;
    char s[256];
    NOT_IMPLEMENTED();
}

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

void GlorifyMaterial(br_material** pArray, int pCount) {
    int i;
    int c;
    br_pixelmap* big_tile;
    tException_list e;
    NOT_IMPLEMENTED();
}

void WhitenVertexRGB(br_model** pArray, int pN) {
    int m;
    int v;
    br_vertex* vertex;
    NOT_IMPLEMENTED();
}

void NobbleNonzeroBlacks(br_pixelmap* pPalette) {
    tU32 red;
    tU32 green;
    tU32 blue;
    tU32 value;
    tU32* palette_entry;
    tU32 frobbed;
    NOT_IMPLEMENTED();
}

int PDCheckDriveExists(char* pThe_path) {
    LOG_TRACE9("(\"%s\")", pThe_path);
    return PDCheckDriveExists2(pThe_path, NULL, 0);
}

int OpacityInPrims(br_token_value* pPrims) {
    NOT_IMPLEMENTED();
}

int AlreadyBlended(br_material* pMaterial) {
    NOT_IMPLEMENTED();
}

void BlendifyMaterialTablishly(br_material* pMaterial, int pPercent) {
    char* s;
    NOT_IMPLEMENTED();
}

void BlendifyMaterialPrimitively(br_material* pMaterial, int pPercent) {
    static br_token_value alpha25[3];
    static br_token_value alpha50[3];
    static br_token_value alpha75[3];
    NOT_IMPLEMENTED();
}

void BlendifyMaterial(br_material* pMaterial, int pPercent) {
    NOT_IMPLEMENTED();
}
