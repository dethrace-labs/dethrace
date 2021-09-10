#include "options.h"
#include <stdlib.h>

int gKey_defns[18];
tRadio_bastards gRadio_bastards__options[13]; // suffix added to avoid duplicate symbol
int gKey_count;
int gLast_graph_sel__options; // suffix added to avoid duplicate symbol
char* gKey_names[125];
int gPending_entry;
tInterface_spec* gThe_interface_spec__options; // suffix added to avoid duplicate symbol
int gOrig_key_mapping[67];
br_pixelmap* gDials_pix;
int gCurrent_key;

// IDA: void __usercall DrawDial(int pWhich_one@<EAX>, int pWhich_stage@<EDX>)
void DrawDial(int pWhich_one, int pWhich_stage) {
    LOG_TRACE("(%d, %d)", pWhich_one, pWhich_stage);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MoveDialFromTo(int pWhich_one@<EAX>, int pOld_stage@<EDX>, int pNew_stage@<EBX>)
void MoveDialFromTo(int pWhich_one, int pOld_stage, int pNew_stage) {
    tS32 time_diff;
    tU32 start_time;
    LOG_TRACE("(%d, %d, %d)", pWhich_one, pOld_stage, pNew_stage);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SoundOptionsStart()
void SoundOptionsStart() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SoundOptionsDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int SoundOptionsDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SoundOptionsLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int SoundOptionsLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int old_value;
    int* the_value;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SoundOptionsRight@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int SoundOptionsRight(int* pCurrent_choice, int* pCurrent_mode) {
    int old_value;
    int* the_value;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SoundClick@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int SoundClick(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    float x_delta;
    float y_delta;
    float angle;
    int old_value;
    int* the_value;
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoSoundOptions()
void DoSoundOptions() {
    static tFlicette flicker_on[3];
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[3];
    static tInterface_spec interface_spec;
    int result;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GetGraphicsOptions()
void GetGraphicsOptions() {
    int value;
    br_scalar br_value;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SetGraphicsOptions()
void SetGraphicsOptions() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PlayRadioOn2(int pIndex@<EAX>, int pValue@<EDX>)
void PlayRadioOn2(int pIndex, int pValue) {
    LOG_TRACE("(%d, %d)", pIndex, pValue);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PlayRadioOff2(int pIndex@<EAX>, int pValue@<EDX>)
void PlayRadioOff2(int pIndex, int pValue) {
    LOG_TRACE("(%d, %d)", pIndex, pValue);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PlayRadioOn(int pIndex@<EAX>, int pValue@<EDX>)
void PlayRadioOn__options(int pIndex, int pValue) {
    LOG_TRACE("(%d, %d)", pIndex, pValue);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PlayRadioOff(int pIndex@<EAX>, int pValue@<EDX>)
void PlayRadioOff__options(int pIndex, int pValue) {
    LOG_TRACE("(%d, %d)", pIndex, pValue);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DrawInitialRadios()
void DrawInitialRadios() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RadioChanged(int pIndex@<EAX>, int pNew_value@<EDX>)
void RadioChanged(int pIndex, int pNew_value) {
    LOG_TRACE("(%d, %d)", pIndex, pNew_value);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GraphOptLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GraphOptLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GraphOptRight@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GraphOptRight(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GraphOptUp@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GraphOptUp(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GraphOptDown@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GraphOptDown(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall RadioClick@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int RadioClick(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int i;
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GraphOptGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GraphOptGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

//IDA: void __usercall PlotAGraphBox(int pIndex@<EAX>, int pColour_value@<EDX>)
// Suffix added to avoid duplicate symbol
void PlotAGraphBox__options(int pIndex, int pColour_value) {
    LOG_TRACE("(%d, %d)", pIndex, pColour_value);
    NOT_IMPLEMENTED();
}

//IDA: void __usercall DrawAGraphBox(int pIndex@<EAX>)
// Suffix added to avoid duplicate symbol
void DrawAGraphBox__options(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

//IDA: void __usercall EraseAGraphBox(int pIndex@<EAX>)
// Suffix added to avoid duplicate symbol
void EraseAGraphBox__options(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawGraphBox(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void DrawGraphBox(int pCurrent_choice, int pCurrent_mode) {
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoGraphicsOptions()
void DoGraphicsOptions() {
    static tFlicette flicker_on[14];
    static tFlicette flicker_off[14];
    static tFlicette push[14];
    static tMouse_area mouse_areas[14];
    static tInterface_spec interface_spec;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CalibrateJoysticks()
void CalibrateJoysticks() {
    tJoy_calib_stage stage;
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
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall StripControls(unsigned char *pStr@<EAX>)
void StripControls(unsigned char* pStr) {
    int i;
    int len;
    LOG_TRACE("(%p)", pStr);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LoadKeyNames()
void LoadKeyNames() {
    int i;
    FILE* f;
    tPath_name the_path;
    unsigned char s[256];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposeKeyNames()
void DisposeKeyNames() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SaveOrigKeyMapping()
void SaveOrigKeyMapping() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetKeyCoords(int pIndex@<EAX>, int *pY@<EDX>, int *pName_x@<EBX>, int *pKey_x@<ECX>, int *pEnd_box)
void GetKeyCoords(int pIndex, int* pY, int* pName_x, int* pKey_x, int* pEnd_box) {
    int col;
    LOG_TRACE("(%d, %p, %p, %p, %p)", pIndex, pY, pName_x, pKey_x, pEnd_box);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SetKeysToDefault()
void SetKeysToDefault() {
    FILE* f;
    tPath_name the_path;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SaveKeyMapping()
void SaveKeyMapping() {
    FILE* f;
    tPath_name the_path;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ChangeKeyMapIndex(int pNew_one@<EAX>)
void ChangeKeyMapIndex(int pNew_one) {
    LOG_TRACE("(%d)", pNew_one);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawKeyAssignments(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
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
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall KeyAssignLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int KeyAssignLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int new_index;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall KeyAssignRight@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int KeyAssignRight(int* pCurrent_choice, int* pCurrent_mode) {
    int new_index;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall KeyAssignUp@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int KeyAssignUp(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall KeyAssignDown@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int KeyAssignDown(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall KeyAssignGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
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
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall MouseyClickBastard@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int MouseyClickBastard(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int i;
    int x_coord;
    int y_coord;
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DrawInitialKMRadios()
void DrawInitialKMRadios() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoControlOptions()
void DoControlOptions() {
    static tFlicette flicker_on[4];
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
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LoadSoundOptionsData()
void LoadSoundOptionsData() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl FreeSoundOptionsData()
void FreeSoundOptionsData() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DrawDisabledOptions()
void DrawDisabledOptions() {
    br_pixelmap* image;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoOptions()
void DoOptions() {
    static tFlicette flicker_on[4];
    static tFlicette flicker_off[4];
    static tFlicette push[4];
    static tMouse_area mouse_areas[4];
    static tInterface_spec interface_spec;
    int result;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
