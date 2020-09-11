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

void DrawDial(int pWhich_one, int pWhich_stage) {
    NOT_IMPLEMENTED();
}

void MoveDialFromTo(int pWhich_one, int pOld_stage, int pNew_stage) {
    tS32 time_diff;
    tU32 start_time;
    NOT_IMPLEMENTED();
}

void SoundOptionsStart() {
    NOT_IMPLEMENTED();
}

int SoundOptionsDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

int SoundOptionsLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int old_value;
    int* the_value;
    NOT_IMPLEMENTED();
}

int SoundOptionsRight(int* pCurrent_choice, int* pCurrent_mode) {
    int old_value;
    int* the_value;
    NOT_IMPLEMENTED();
}

int SoundClick(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    float x_delta;
    float y_delta;
    float angle;
    int old_value;
    int* the_value;
    NOT_IMPLEMENTED();
}

void DoSoundOptions() {
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[3];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

void GetGraphicsOptions() {
    br_scalar br_value;
    NOT_IMPLEMENTED();
}

void SetGraphicsOptions() {
    NOT_IMPLEMENTED();
}

void PlayRadioOn2(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

void PlayRadioOff2(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

void PlayRadioOn(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

void PlayRadioOff(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

void DrawInitialRadios() {
    NOT_IMPLEMENTED();
}

void RadioChanged(int pIndex, int pNew_value) {
    NOT_IMPLEMENTED();
}

int GraphOptLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    NOT_IMPLEMENTED();
}

int GraphOptRight(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    NOT_IMPLEMENTED();
}

int GraphOptUp(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int GraphOptDown(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int RadioClick(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int i;
    NOT_IMPLEMENTED();
}

int GraphOptGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

void DrawGraphBox(int pCurrent_choice, int pCurrent_mode) {
    NOT_IMPLEMENTED();
}

void DoGraphicsOptions() {
    static tFlicette flicker_off[14];
    static tFlicette push[14];
    static tMouse_area mouse_areas[14];
    static tInterface_spec interface_spec;
    NOT_IMPLEMENTED();
}

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

void StripControls(unsigned char* pStr) {
    int i;
    int len;
    NOT_IMPLEMENTED();
}

void LoadKeyNames() {
    FILE* f;
    tPath_name the_path;
    unsigned char s[256];
    NOT_IMPLEMENTED();
}

void DisposeKeyNames() {
    NOT_IMPLEMENTED();
}

void SaveOrigKeyMapping() {
    NOT_IMPLEMENTED();
}

void GetKeyCoords(int pIndex, int* pY, int* pName_x, int* pKey_x, int* pEnd_box) {
    int col;
    NOT_IMPLEMENTED();
}

void SetKeysToDefault() {
    tPath_name the_path;
    int i;
    NOT_IMPLEMENTED();
}

void SaveKeyMapping() {
    tPath_name the_path;
    int i;
    NOT_IMPLEMENTED();
}

void ChangeKeyMapIndex(int pNew_one) {
    NOT_IMPLEMENTED();
}

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

int KeyAssignLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int new_index;
    NOT_IMPLEMENTED();
}

int KeyAssignRight(int* pCurrent_choice, int* pCurrent_mode) {
    int new_index;
    NOT_IMPLEMENTED();
}

int KeyAssignUp(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int KeyAssignDown(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

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

int MouseyClickBastard(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int i;
    int x_coord;
    int y_coord;
    NOT_IMPLEMENTED();
}

void DrawInitialKMRadios() {
    NOT_IMPLEMENTED();
}

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

void LoadSoundOptionsData() {
    NOT_IMPLEMENTED();
}

void FreeSoundOptionsData() {
    NOT_IMPLEMENTED();
}

void DrawDisabledOptions() {
    NOT_IMPLEMENTED();
}

void DoOptions() {
    static tFlicette flicker_off[4];
    static tFlicette push[4];
    static tMouse_area mouse_areas[4];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}
