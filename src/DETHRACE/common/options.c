#include "options.h"
#include <stdlib.h>

int gKey_defns[18];
tRadio_bastards gRadio_bastards[13];
int gKey_count;
int gLast_graph_sel;
char* gKey_names[125];
int gPending_entry;
tInterface_spec* gThe_interface_spec;
int gOrig_key_mapping[67];
br_pixelmap* gDials_pix;
int gCurrent_key;

// Offset: 0
// Size: 161
void DrawDial(int pWhich_one, int pWhich_stage) {
    NOT_IMPLEMENTED();
}

// Offset: 164
// Size: 386
void MoveDialFromTo(int pWhich_one, int pOld_stage, int pNew_stage) {
    tS32 time_diff;
    tU32 start_time;
    NOT_IMPLEMENTED();
}

// Offset: 552
// Size: 94
void SoundOptionsStart() {
    NOT_IMPLEMENTED();
}

// Offset: 648
// Size: 90
int SoundOptionsDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

// Offset: 740
// Size: 154
int SoundOptionsLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int old_value;
    int* the_value;
    NOT_IMPLEMENTED();
}

// Offset: 896
// Size: 154
int SoundOptionsRight(int* pCurrent_choice, int* pCurrent_mode) {
    int old_value;
    int* the_value;
    NOT_IMPLEMENTED();
}

// Offset: 1052
// Size: 384
int SoundClick(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    float x_delta;
    float y_delta;
    float angle;
    int old_value;
    int* the_value;
    NOT_IMPLEMENTED();
}

// Offset: 1436
// Size: 77
void DoSoundOptions() {
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[3];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 1516
// Size: 586
void GetGraphicsOptions() {
    br_scalar br_value;
    NOT_IMPLEMENTED();
}

// Offset: 2104
// Size: 513
void SetGraphicsOptions() {
    NOT_IMPLEMENTED();
}

// Offset: 2620
// Size: 80
void PlayRadioOn2(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

// Offset: 2700
// Size: 80
void PlayRadioOff2(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

// Offset: 2780
// Size: 79
void PlayRadioOn(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

// Offset: 2860
// Size: 79
void PlayRadioOff(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

// Offset: 2940
// Size: 109
void DrawInitialRadios() {
    NOT_IMPLEMENTED();
}

// Offset: 3052
// Size: 84
void RadioChanged(int pIndex, int pNew_value) {
    NOT_IMPLEMENTED();
}

// Offset: 3136
// Size: 121
int GraphOptLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    NOT_IMPLEMENTED();
}

// Offset: 3260
// Size: 125
int GraphOptRight(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    NOT_IMPLEMENTED();
}

// Offset: 3388
// Size: 148
int GraphOptUp(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 3536
// Size: 148
int GraphOptDown(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 3684
// Size: 175
int RadioClick(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 3860
// Size: 59
int GraphOptGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 4216
// Size: 80
void DrawGraphBox(int pCurrent_choice, int pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 4296
// Size: 128
void DoGraphicsOptions() {
    static tFlicette flicker_off[14];
    static tFlicette push[14];
    static tMouse_area mouse_areas[14];
    static tInterface_spec interface_spec;
    NOT_IMPLEMENTED();
}

// Offset: 4424
// Size: 1526
void CalibrateJoysticks() {
    int escaped;
    int joy_value_x;
    int joy_value_y;
    int key;
    int top_left[2];
    int bot_right[2];
    int centre[2];
    int range[2];
    int min[2];
    int i;
    int saved_1x;
    int saved_1y;
    int saved_2x;
    int saved_2y;
    int max;
    char s[256];
    NOT_IMPLEMENTED();
}

// Offset: 5952
// Size: 118
void StripControls(unsigned char* pStr) {
    int i;
    int len;
    NOT_IMPLEMENTED();
}

// Offset: 6072
// Size: 215
void LoadKeyNames() {
    FILE* f;
    tPath_name the_path;
    unsigned char s[256];
    NOT_IMPLEMENTED();
}

// Offset: 6288
// Size: 79
void DisposeKeyNames() {
    NOT_IMPLEMENTED();
}

// Offset: 6368
// Size: 54
void SaveOrigKeyMapping() {
    NOT_IMPLEMENTED();
}

// Offset: 6424
// Size: 384
void GetKeyCoords(int pIndex, int* pY, int* pName_x, int* pKey_x, int* pEnd_box) {
    int col;
    NOT_IMPLEMENTED();
}

// Offset: 6808
// Size: 182
void SetKeysToDefault() {
    tPath_name the_path;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 6992
// Size: 217
void SaveKeyMapping() {
    tPath_name the_path;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 7212
// Size: 60
void ChangeKeyMapIndex(int pNew_one) {
    NOT_IMPLEMENTED();
}

// Offset: 7272
// Size: 1178
void DrawKeyAssignments(int pCurrent_choice, int pCurrent_mode) {
    int i;
    int y;
    int x_coord;
    int y_coord;
    int name_x;
    int key_x;
    int new_key;
    int end_box;
    tDR_font* font_n;
    tDR_font* font_k;
    static int on_radios_last_time;
    NOT_IMPLEMENTED();
}

// Offset: 8452
// Size: 252
int KeyAssignLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int new_index;
    NOT_IMPLEMENTED();
}

// Offset: 8704
// Size: 252
int KeyAssignRight(int* pCurrent_choice, int* pCurrent_mode) {
    int new_index;
    NOT_IMPLEMENTED();
}

// Offset: 8956
// Size: 299
int KeyAssignUp(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 9256
// Size: 265
int KeyAssignDown(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 9524
// Size: 1009
int KeyAssignGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    int key;
    int i;
    int j;
    int y;
    int new_index;
    int disallowed;
    int name_x;
    int key_x;
    int end_box;
    tDR_font* font;
    NOT_IMPLEMENTED();
}

// Offset: 10536
// Size: 168
int MouseyClickBastard(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int i;
    int x_coord;
    int y_coord;
    NOT_IMPLEMENTED();
}

// Offset: 10704
// Size: 90
void DrawInitialKMRadios() {
    NOT_IMPLEMENTED();
}

// Offset: 10796
// Size: 366
void DoControlOptions() {
    static tFlicette flicker_off[4];
    static tFlicette push[4];
    static tMouse_area mouse_areas[5];
    static tInterface_spec interface_spec;
    int result;
    int swap_font_1;
    int swap_font_2;
    int swap_font_3;
    int second_time_around;
    int orig_key_map_index;
    NOT_IMPLEMENTED();
}

// Offset: 11164
// Size: 68
void LoadSoundOptionsData() {
    NOT_IMPLEMENTED();
}

// Offset: 11232
// Size: 48
void FreeSoundOptionsData() {
    NOT_IMPLEMENTED();
}

// Offset: 11280
// Size: 165
void DrawDisabledOptions() {
    NOT_IMPLEMENTED();
}

// Offset: 11448
// Size: 146
void DoOptions() {
    static tFlicette flicker_off[4];
    static tFlicette push[4];
    static tMouse_area mouse_areas[4];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}
