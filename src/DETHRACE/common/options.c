#include "options.h"
#include "dr_types.h"
#include "brucetrk.h"
#include "controls.h"
#include "depth.h"
#include "displays.h"
#include "drmem.h"
#include "errors.h"
#include "flicplay.h"
#include "globvars.h"
#include "grafdata.h"
#include "graphics.h"
#include "intrface.h"
#include "input.h"
#include "loading.h"
#include "network.h"
#include "sound.h"
#include "spark.h"
#include "utility.h"
#include "world.h"
#include <brender/brender.h>
#include "harness/trace.h"
#include "pd/sys.h"
#include <stdlib.h>
#include <string.h>

int gKey_defns[18] = { 48, 49, 46, 47, 53, 44, 59, 57, 55, 45, 50, 51, 52, 56, 62, 63, 64, 66 };
tRadio_bastards gRadio_bastards__options[13] = { // suffix added to avoid duplicate symbol
    { 4,  36, 0, {132, 175, 221, 253, 0 } }, 
    { 3,  45, 0, {132, 157, 217,   0, 0 } }, 
    { 3,  54, 0, {132, 183, 236,   0, 0 } }, 
    { 3,  67, 0, {132, 194, 234,   0, 0 } }, 
    { 3,  76, 0, {132, 194, 234,   0, 0 } }, 
    { 4,  89, 0, {132, 176, 204, 246, 0 } }, 
    { 4,  98, 0, {132, 176, 204, 246, 0 } }, 
    { 2, 111, 0, {132, 158,   0,   0, 0 } }, 
    { 2, 120, 0, {132, 158,   0,   0, 0 } },
    { 2, 129, 0, {132, 158,   0,   0, 0 } }, 
    { 2, 138, 0, {132, 158,   0,   0, 0 } }, 
    { 3, 150, 0, {132, 164, 207,   0, 0 } },
    { 4, 153, 0, {177, 199, 220, 242, 0 } },
};
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

    value = GetCarSimplificationLevel();
    if (value > 1) {
        value--;
    }
    gRadio_bastards__options[0].current_value = value;

    switch (GetCarTexturingLevel()) {
    case eCTL_none:
        value = 2;
        break;
    case eCTL_transparent:
        value = 1;
        break;
    case eCTL_full:
        value = 0;
        break;
    case eCTL_count:
        TELL_ME_IF_WE_PASS_THIS_WAY();
        break;
    }
    gRadio_bastards__options[1].current_value = value;

    switch (GetShadowLevel()) {
    case eShadow_none:
        value = 2;
        break;
    case eShadow_us_only:
        value = 1;
        break;
    default:
        value = 0;
        break;
    }
    gRadio_bastards__options[2].current_value = value;

    switch (GetWallTexturingLevel()) {
    case eWTL_none:
        value = 2;
        break;
    case eWTL_linear:
        value = 1;
        break;
    case eWTL_full:
        value = 0;
        break;
    case eWTL_count:
        TELL_ME_IF_WE_PASS_THIS_WAY();
        break;
    }
    gRadio_bastards__options[3].current_value = value;

    switch (GetRoadTexturingLevel()) {
    case eRTL_none:
        value = 2;
        break;
    case eRTL_full:
        value = 0;
        break;
    case eRTL_count:
        TELL_ME_IF_WE_PASS_THIS_WAY();
        break;
    }
    gRadio_bastards__options[4].current_value = value;

    value = (int)((35.f - GetYon()) / 5.f);
    gRadio_bastards__options[5].current_value = value;

    br_value = GetYonFactor();
    if (br_value < .5f) {
        if (br_value < .25f) {
            value = 3;
        } else {
            value = 2;
        }
    } else {
        if (br_value < 1.f) {
            value = 1;
        } else {
            value = 0;
        }
    }
    gRadio_bastards__options[6].current_value = value;

    value = !GetSkyTextureOn();
    gRadio_bastards__options[7].current_value = value;

    value = !GetDepthCueingOn();
    gRadio_bastards__options[8].current_value = value;

    value = !GetAccessoryRendering();
    gRadio_bastards__options[9].current_value = value;

    value = !GetSmokeOn();
    gRadio_bastards__options[10].current_value = value;

    value = 2 - GetSoundDetailLevel();
    gRadio_bastards__options[11].current_value = value;
}

// IDA: void __cdecl SetGraphicsOptions()
void SetGraphicsOptions() {
    LOG_TRACE("()");

    if (gRadio_bastards__options[0].current_value < 2) {
        SetCarSimplificationLevel(gRadio_bastards__options[0].current_value);
    } else {
        SetCarSimplificationLevel(gRadio_bastards__options[0].current_value + 1);
    }

    switch (gRadio_bastards__options[1].current_value) {
    case 0:
        SetCarTexturingLevel(eCTL_full);
        break;
    case 1:
        SetCarTexturingLevel(eCTL_transparent);
        break;
    case 2:
        SetCarTexturingLevel(eCTL_none);
        break;
    }

    switch (gRadio_bastards__options[2].current_value) {
    case 0:
        SetShadowLevel(eShadow_everyone);
        break;
    case 1:
        SetShadowLevel(eShadow_us_only);
        break;
    case 2:
        SetShadowLevel(eShadow_none);
        break;
    }

    switch (gRadio_bastards__options[3].current_value) {
    case 0:
        SetWallTexturingLevel(eWTL_full);
        break;
    case 1:
        SetWallTexturingLevel(eWTL_linear);
        break;
    case 2:
        SetWallTexturingLevel(eWTL_none);
        break;
    }

    switch (gRadio_bastards__options[4].current_value) {
    case 0:
        SetRoadTexturingLevel(eRTL_full);
        break;
    case 2:
        SetRoadTexturingLevel(eRTL_none);
        break;
    }

    SetYon(35.f - gRadio_bastards__options[5].current_value * 5.f);

    switch (gRadio_bastards__options[6].current_value) {
    case 0:
        SetYonFactor(1.f);
        break;
    case 1:
        SetYonFactor(.5f);
        break;
    case 2:
        SetYonFactor(.25f);
        break;
    case 3:
        SetYonFactor(.125f);
        break;
    }

    SetSkyTextureOn(1 - gRadio_bastards__options[7].current_value);
    SetDepthCueingOn(1 - gRadio_bastards__options[8].current_value);
    SetAccessoryRendering(1 - gRadio_bastards__options[9].current_value);
    SetSmokeOn(1 - gRadio_bastards__options[10].current_value);
    SetSoundDetailLevel(2 - gRadio_bastards__options[11].current_value);
    SaveOptions();
}

// IDA: void __usercall PlayRadioOn2(int pIndex@<EAX>, int pValue@<EDX>)
void PlayRadioOn2(int pIndex, int pValue) {
    LOG_TRACE("(%d, %d)", pIndex, pValue);

    RunFlicAt(288,
            gRadio_bastards__options[pIndex].left[pValue],
            gRadio_bastards__options[pIndex].top);
}

// IDA: void __usercall PlayRadioOff2(int pIndex@<EAX>, int pValue@<EDX>)
void PlayRadioOff2(int pIndex, int pValue) {
    LOG_TRACE("(%d, %d)", pIndex, pValue);

    RunFlicAt(287,
            gRadio_bastards__options[pIndex].left[pValue],
            gRadio_bastards__options[pIndex].top);
}

// IDA: void __usercall PlayRadioOn(int pIndex@<EAX>, int pValue@<EDX>)
void PlayRadioOn__options(int pIndex, int pValue) {
    LOG_TRACE("(%d, %d)", pIndex, pValue);

    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    PlayRadioOn2(pIndex, pValue);
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
}

// IDA: void __usercall PlayRadioOff(int pIndex@<EAX>, int pValue@<EDX>)
void PlayRadioOff__options(int pIndex, int pValue) {
    LOG_TRACE("(%d, %d)", pIndex, pValue);

    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    PlayRadioOff2(pIndex, pValue);
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
}

// IDA: void __cdecl DrawInitialRadios()
void DrawInitialRadios() {
    int i;
    LOG_TRACE("()");

    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    // FIXME: count of radio buttons does not agree with windows version
    for (i = 0; i < COUNT_OF(gRadio_bastards__options) - 1; i++) {
        PlayRadioOn2(i, gRadio_bastards__options[i].current_value);
    }
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
}

// IDA: void __usercall RadioChanged(int pIndex@<EAX>, int pNew_value@<EDX>)
void RadioChanged(int pIndex, int pNew_value) {
    LOG_TRACE("(%d, %d)", pIndex, pNew_value);

    PlayRadioOff__options(pIndex, gRadio_bastards__options[pIndex].current_value);
    PlayRadioOn__options(pIndex, pNew_value);
    gRadio_bastards__options[pIndex].current_value = pNew_value;
}

// IDA: int __usercall GraphOptLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GraphOptLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    DRS3StartSound(gIndexed_outlets[0], 3000);
    new_value = gRadio_bastards__options[*pCurrent_choice - 2].current_value - 1;
    if (new_value < 0) {
        new_value = gRadio_bastards__options[*pCurrent_choice - 2].count - 1;
    }
    RadioChanged(*pCurrent_choice - 2, new_value);
    return 1;
}

// IDA: int __usercall GraphOptRight@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GraphOptRight(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    DRS3StartSound(gIndexed_outlets[0], 3000);
    new_value = gRadio_bastards__options[*pCurrent_choice + -2].current_value + 1;
    if (new_value == gRadio_bastards__options[*pCurrent_choice - 2].count) {
        new_value = 0;
    }
    RadioChanged(*pCurrent_choice - 2, new_value);
    return 1;
}

// IDA: int __usercall GraphOptUp@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GraphOptUp(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (*pCurrent_mode == 0) {
        *pCurrent_mode = 1;
        *pCurrent_choice = 13;
        DRS3StartSound(gIndexed_outlets[0], 3000);
        return 1;
    } else if (*pCurrent_choice == 1) {
        *pCurrent_mode = 0;
        *pCurrent_choice = 0;
        DRS3StartSound(gIndexed_outlets[0], 3000);
        return 1;
    } else {
        return 0;
    }
}

// IDA: int __usercall GraphOptDown@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GraphOptDown(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (*pCurrent_mode == 0) {
        *pCurrent_mode = 1;
        *pCurrent_choice = 2;
        DRS3StartSound(gIndexed_outlets[0], 3000);
        return 1;
    } else if (*pCurrent_choice == 14) {
        *pCurrent_mode = 0;
        *pCurrent_choice = 0;
        DRS3StartSound(gIndexed_outlets[0], 3000);
        return 1;
    } else {
        return 0;
    }
}

// IDA: int __usercall RadioClick@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int RadioClick(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int i;
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);

    for (i = gRadio_bastards__options[*pCurrent_choice - 2].count - 1; i >= 0; i--) {
        if (gThe_interface_spec__options->mouse_areas[2].left[gGraf_data_index] + pX_offset + 3 >= gRadio_bastards__options[*pCurrent_choice - 2].left[i]) {
            DRS3StartSound(gIndexed_outlets[0], 3000);
            RadioChanged(*pCurrent_choice - 2, i);
            return 0;
        }
    }
    return 0;
}

// IDA: int __usercall GraphOptGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GraphOptGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    GraphOptRight(pCurrent_choice, pCurrent_mode);
    return 0;
}

//IDA: void __usercall PlotAGraphBox(int pIndex@<EAX>, int pColour_value@<EDX>)
// Suffix added to avoid duplicate symbol
void PlotAGraphBox__options(int pIndex, int pColour_value) {
    LOG_TRACE("(%d, %d)", pIndex, pColour_value);

    if (pIndex < 0) {
        return;
    }
    DrawRRectangle(gBack_screen,
            gThe_interface_spec__options->mouse_areas[2].left[gGraf_data_index] - 6,
            gRadio_bastards__options[pIndex].top - 3,
            gThe_interface_spec__options->mouse_areas[2].right[gGraf_data_index] + 3,
            gRadio_bastards__options[pIndex].top + gFonts[12].height + (TranslationMode() ? 2 : 0), pColour_value);
}

//IDA: void __usercall DrawAGraphBox(int pIndex@<EAX>)
// Suffix added to avoid duplicate symbol
void DrawAGraphBox__options(int pIndex) {
    LOG_TRACE("(%d)", pIndex);

    PlotAGraphBox__options(pIndex, 45);
}

//IDA: void __usercall EraseAGraphBox(int pIndex@<EAX>)
// Suffix added to avoid duplicate symbol
void EraseAGraphBox__options(int pIndex) {
    LOG_TRACE("(%d)", pIndex);

    PlotAGraphBox__options(pIndex, 0);
}

// IDA: void __usercall DrawGraphBox(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void DrawGraphBox(int pCurrent_choice, int pCurrent_mode) {
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);

    if (pCurrent_choice != gCurrent_key) {
        EraseAGraphBox__options(gCurrent_key - 2);
        DrawAGraphBox__options(pCurrent_choice - 2);
        gCurrent_key = pCurrent_choice;
    }
}

// IDA: void __cdecl DoGraphicsOptions()
void DoGraphicsOptions() {
    static tFlicette flicker_on[14] = {
        {  43, {  45,  90 }, { 166, 398 } },
        {  43, { 220, 440 }, { 166, 398 } },
        { 238, {  49,  98 }, {  36,  86 } },
        { 239, {  49,  98 }, {  45, 108 } }, 
        { 240, {  49,  98 }, {  54, 130 } }, 
        { 241, {  49,  98 }, {  67, 161 } }, 
        { 242, {  49,  98 }, {  76, 182 } }, 
        { 243, {  49,  98 }, {  89, 214 } }, 
        { 244, {  49,  98 }, {  98, 235 } }, 
        { 245, {  49,  98 }, { 111, 266 } }, 
        { 246, {  49,  98 }, { 120, 288 } }, 
        { 247, {  49,  98 }, { 129, 310 } }, 
        { 248, {  49,  98 }, { 138, 331 } }, 
        { 249, {  49,  98 }, { 150, 360 } },
    };
    static tFlicette flicker_off[14] = {
        { 42,  {  45,  90 }, { 166, 398 } }, 
        { 42,  { 220, 440 }, { 166, 398 } }, 
        { 265, {  49,  98 }, {  36,  86 } }, 
        { 266, {  49,  98 }, {  45, 108 } }, 
        { 267, {  49,  98 }, {  54, 130 } }, 
        { 268, {  49,  98 }, {  67, 161 } }, 
        { 269, {  49,  98 }, {  76, 182 } }, 
        { 270, {  49,  98 }, {  89, 214 } }, 
        { 271, {  49,  98 }, {  98, 235 } },
        { 272, {  49,  98 }, { 111, 266 } },
        { 273, {  49,  98 }, { 120, 288 } },
        { 274, {  49,  98 }, { 129, 310 } },
        { 275, {  49,  98 }, { 138, 331 } },
        { 276, {  49,  98 }, { 150, 360 } },
    };
    static tFlicette push[14] = {
        { 154, {  45,  90 }, { 166, 398 } }, 
        {  45, { 220, 440 }, { 166, 398 } }, 
        {  45, { 210, 440 }, { 170, 408 } },
        {  45, { 210, 440 }, { 170, 408 } },
        {  45, { 210, 440 }, { 170, 408 } },
        {  45, { 210, 440 }, { 170, 408 } },
        {  45, { 210, 440 }, { 170, 408 } },
        {  45, { 210, 440 }, { 170, 408 } }, 
        {  45, { 210, 440 }, { 170, 408 } },
        {  45, { 210, 440 }, { 170, 408 } },
        {  45, { 210, 440 }, { 170, 408 } }, 
        {  45, { 210, 440 }, { 170, 408 } },
        {  45, { 210, 440 }, { 170, 408 } }, 
        {  45, { 210, 440 }, { 170, 408 } },
    };
    static tMouse_area mouse_areas[14] = {
        { {  45,  90 }, { 165, 396 }, { 104, 214 }, { 185, 444 },  0, 0, 0, NULL },
        { { 220, 440 }, { 165, 396 }, { 276, 552 }, { 185, 444 },  1, 0, 0, NULL },
        { {  49,  98 }, {  35,  84 }, { 284, 568 }, {  43, 103 },  2, 1, 0, RadioClick },
        { {  49,  98 }, {  44, 106 }, { 284, 568 }, {  52, 125 },  3, 1, 0, RadioClick },
        { {  49,  98 }, {  53, 127 }, { 284, 568 }, {  61, 146 },  4, 1, 0, RadioClick },
        { {  49,  98 }, {  66, 158 }, { 284, 568 }, {  74, 178 },  5, 1, 0, RadioClick },
        { {  49,  98 }, {  75, 180 }, { 284, 568 }, {  83, 199 },  6, 1, 0, RadioClick },
        { {  49,  98 }, {  88, 192 }, { 284, 568 }, {  96, 230 },  7, 1, 0, RadioClick },
        { {  49,  98 }, {  97, 233 }, { 284, 568 }, { 105, 252 },  8, 1, 0, RadioClick },
        { {  49,  98 }, { 110, 264 }, { 284, 568 }, { 118, 283 },  9, 1, 0, RadioClick },
        { {  49,  98 }, { 119, 286 }, { 284, 568 }, { 127, 305 }, 10, 1, 0, RadioClick },
        { {  49,  98 }, { 128, 307 }, { 284, 322 }, { 136, 326 }, 11, 1, 0, RadioClick },
        { {  49,  98 }, { 137, 329 }, { 284, 322 }, { 145, 348 }, 12, 1, 0, RadioClick },
        { {  49,  98 }, { 149, 358 }, { 284, 322 }, { 157, 377 }, 13, 1, 0, RadioClick },
    };
    static tInterface_spec interface_spec = {
        0, 160, 0, 0, 0, 0, 1,
        { -1,  0 }, { -1,  0 }, { 0, 2 }, { 1,  2 }, { NULL,         GraphOptLeft  },
        { -1,  0 }, {  1,  0 }, { 0, 2 }, { 1, 13 }, { NULL,         GraphOptRight },
        { -1, -1 }, {  0, -1 }, { 0, 1 }, { 0, 13 }, { GraphOptUp,   GraphOptUp    },
        { -1, -1 }, {  1,  1 }, { 0, 2 }, { 0, 14 }, { GraphOptDown, GraphOptDown  },
        { 1, 1 }, { NULL, GraphOptGoAhead }, { 1, 1 }, { NULL, NULL },
        NULL, DrawGraphBox, 0, NULL, DrawInitialRadios, NULL, 0, { 0, 0 },
        NULL, 1, 1, 
        COUNT_OF(flicker_on), flicker_on, flicker_off, push,
        COUNT_OF(mouse_areas), mouse_areas, 0, NULL,
    };
    LOG_TRACE("()");

    gThe_interface_spec__options = &interface_spec;
    gCurrent_key = -1;
    LoadFont(12);
    GetGraphicsOptions();
    if (DoInterfaceScreen(&interface_spec, 0, 0) == 0) {
        SetGraphicsOptions();
    }
    if (gProgram_state.racing) {
        FadePaletteDown();
    } else {
        RunFlic(161);
    }
    DisposeFont(12);
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

#ifdef __DOS__
    NOT_IMPLEMENTED();
#else
    NetFullScreenMessage(229, 0);
#endif
}

// IDA: void __usercall StripControls(unsigned char *pStr@<EAX>)
void StripControls(unsigned char* pStr) {
    int i;
    int len;
    LOG_TRACE("(%p)", pStr);

    len = strlen((char*)pStr);
    for (i = 0; i < len; i++) {
        if (pStr[i] < ' ') {
            memmove(&pStr[i], &pStr[i + 1], (len - i) * sizeof(char));
            len--;
        }
    }
}

// IDA: void __cdecl LoadKeyNames()
void LoadKeyNames() {
    int i;
    FILE* f;
    tPath_name the_path;
    unsigned char s[256];
    LOG_TRACE("()");

    PathCat(the_path, gApplication_path, "KEYNAMES.TXT");
    f = DRfopen(the_path, "rt");
    if (f == NULL) {
        FatalError(106);
    }
    for (i = 0; i < COUNT_OF(gKey_names); i++) {
        fgets((char*)s, sizeof(s), f);
        StripControls(s);
        gKey_names[i] = BrMemAllocate(strlen((char*)s) + 1, kMem_key_names);
        strcpy(gKey_names[i], (char*)s);
    }
    fclose(f);
}

// IDA: void __cdecl DisposeKeyNames()
void DisposeKeyNames() {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gKey_names); i++) {
        BrMemFree(gKey_names[i]);
    }
}

// IDA: void __cdecl SaveOrigKeyMapping()
void SaveOrigKeyMapping() {
    LOG_TRACE("()");

    memcpy(gOrig_key_mapping, gKey_mapping, sizeof(gKey_mapping));
}

// IDA: void __usercall GetKeyCoords(int pIndex@<EAX>, int *pY@<EDX>, int *pName_x@<EBX>, int *pKey_x@<ECX>, int *pEnd_box)
void GetKeyCoords(int pIndex, int* pY, int* pName_x, int* pKey_x, int* pEnd_box) {
    int col;
    LOG_TRACE("(%d, %p, %p, %p, %p)", pIndex, pY, pName_x, pKey_x, pEnd_box);

    if (pIndex >= 0) {
        col = gKey_count + 1;
        *pY = (pIndex % ((gKey_count + 1) / 2)) * gCurrent_graf_data->key_assign_y_pitch + gCurrent_graf_data->key_assign_y;
        if (pIndex < col / 2) {
            *pName_x = gCurrent_graf_data->key_assign_col_1;
            *pKey_x = gCurrent_graf_data->key_assign_col_1_a;
            *pEnd_box = gCurrent_graf_data->key_assign_col_2 - 7;
        } else {
            *pName_x = gCurrent_graf_data->key_assign_col_2;
            *pKey_x = gCurrent_graf_data->key_assign_col_2_a;
            *pEnd_box = gCurrent_graf_data->key_assign_col_2 + gCurrent_graf_data->key_assign_col_2 - gCurrent_graf_data->key_assign_col_1 - 7;
        }
    } else {
        *pName_x = gCurrent_graf_data->key_assign_col_1;
        *pKey_x = 0;
        *pEnd_box = gCurrent_graf_data->key_assign_col_2 + gCurrent_graf_data->key_assign_col_2 - gCurrent_graf_data->key_assign_col_1 - 7;
        *pY = gCurrent_graf_data->key_assign_key_map_y;
    }
}

// IDA: void __cdecl SetKeysToDefault()
void SetKeysToDefault() {
    FILE* f;
    tPath_name the_path;
    int i;
    LOG_TRACE("()");

    PathCat(the_path, gApplication_path, "DKEYMAPX.TXT");
    the_path[strlen(the_path) - 5] = '0' + gKey_map_index;
    f = DRfopen(the_path, "rt");
    if (f == NULL) {
        FatalError(9);
    }
    for (i = 0; i < COUNT_OF(gKey_mapping); i++) {
        fscanf(f, "%d", &gKey_mapping[i]);
    }
    fclose(f);
}

// IDA: void __cdecl SaveKeyMapping()
void SaveKeyMapping() {
    FILE* f;
    tPath_name the_path;
    int i;
    LOG_TRACE("()");

    PathCat(the_path, gApplication_path, "KEYMAP_X.TXT");
    the_path[strlen(the_path) - 5] = '0' + gKey_map_index;
    PDFileUnlock(the_path);
    f = DRfopen(the_path, "wb");
    if (f == NULL) {
        FatalError(9);
    }
    for (i = 0; i < COUNT_OF(gKey_mapping); i++) {
        fprintf(f, "%d", gKey_mapping[i]);
        fputc('\r', f);
        fputc('\n', f);
    }
    fclose(f);
}

// IDA: void __usercall ChangeKeyMapIndex(int pNew_one@<EAX>)
void ChangeKeyMapIndex(int pNew_one) {
    LOG_TRACE("(%d)", pNew_one);

    SaveKeyMapping();
    gKey_map_index = pNew_one;
    LoadKeyMapping();
    SaveOrigKeyMapping();
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

    if (gMouse_in_use && pCurrent_choice == 4) {
        GetMousePosition(&x_coord, &y_coord);
        if (y_coord >= gCurrent_graf_data->key_assign_key_map_y
                && y_coord <= gCurrent_graf_data->key_assign_key_map_y + gFonts[12].height + 5
                && x_coord > gCurrent_graf_data->key_assign_col_1
                && x_coord < gCurrent_graf_data->key_assign_col_2 + gCurrent_graf_data->key_assign_col_2 - gCurrent_graf_data->key_assign_col_1 - 7) {
            gCurrent_key = -1;
        } else {
            if (x_coord > gCurrent_graf_data->key_assign_col_2) {
                new_key = (gKey_count + 1) / 2;
            } else {
                new_key = 0;
            }
            if (y_coord >= gCurrent_graf_data->key_assign_y - 2
                    && y_coord < gCurrent_graf_data->key_assign_y + gCurrent_graf_data->key_assign_y_pitch * (gKey_count + 1) / 2) {
                new_key += (y_coord - gCurrent_graf_data->key_assign_y - 2) / gCurrent_graf_data->key_assign_y_pitch;
                if (new_key >= 0 && new_key < gKey_count) {
                    gCurrent_key = new_key;
                }
            }
        }
    }
    BrPixelmapRectangleFill(gBack_screen,
        gCurrent_graf_data->key_assign_col_1 - 3,
        gCurrent_graf_data->key_assign_y - 3,
        2 * (gCurrent_graf_data->key_assign_col_2 - gCurrent_graf_data->key_assign_col_1) - 3,
        gCurrent_graf_data->key_assign_y_pitch * gKey_count / 2 + 4,
        0);
    GetKeyCoords(-1, &y, &name_x, &key_x, &end_box);
    DrawRRectangle(gBack_screen, name_x - 3, y - 3, end_box, gFonts[12].height + y + 2 - (TranslationMode() ? 2 : 0), 0);
    for (i = 0; i < gKey_count; i++) {
        GetKeyCoords(i, &y, &name_x, &key_x, &end_box);
        if (i == gCurrent_key && pCurrent_mode != 0) {
            font_n = &gFonts[10];
            font_k = &gFonts[12];
        } else {
            font_n = &gFonts[9];
            font_k = &gFonts[11];
        }
        TransDRPixelmapText(gBack_screen, name_x, y, font_n, GetMiscString(150 + i), 640);
        TransDRPixelmapText(gBack_screen, key_x, y, font_k, gKey_names[gKey_mapping[gKey_defns[i]] + 2], 640);
        if (i == gCurrent_key && pCurrent_mode != 0) {
            DrawRRectangle(gBack_screen, name_x - 3, y - 3, end_box, font_k->height + y + 2 - (TranslationMode() ? 2 : 0), 45);
        }
    }
    if (gCurrent_key < 0 && pCurrent_mode != 0) {
        GetKeyCoords(-1, &y, &name_x, &key_x, &end_box);
        DrawRRectangle(gBack_screen, name_x - 3, y - 3, end_box, font_k->height + y + 2 - (TranslationMode() ? 2 : 0), 45);
    }
    if (on_radios_last_time && (pCurrent_mode == 0 || gCurrent_key >= 0)) {
        DontLetFlicFuckWithPalettes();
        TurnFlicTransparencyOn();
        RunFlicAt(193, gCurrent_graf_data->key_assign_flic_x, gCurrent_graf_data->key_assign_flic_y);
        TurnFlicTransparencyOff();
        DontLetFlicFuckWithPalettes();
    } else if (!on_radios_last_time && pCurrent_mode != 0 && gCurrent_key < 0) {
        DontLetFlicFuckWithPalettes();
        TurnFlicTransparencyOn();
        RunFlicAt(194, gCurrent_graf_data->key_assign_flic_x, gCurrent_graf_data->key_assign_flic_y);
        TurnFlicTransparencyOff();
        DontLetFlicFuckWithPalettes();
    }
    on_radios_last_time = pCurrent_mode != 0 && gCurrent_key < 0;
}

// IDA: int __usercall KeyAssignLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int KeyAssignLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int new_index;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (gCurrent_key < 0) {
        if (gKey_map_index == 0) {
            new_index = 3;
        } else {
            new_index = gKey_map_index - 1;
        }
        ChangeKeyMapIndex(new_index);
        RadioChanged(12, new_index);
        DRS3StartSound(gIndexed_outlets[0], 3000);
    } else {
        if (gCurrent_key >= (gKey_count + 1) / 2) {
            gCurrent_key -= (gKey_count + 1) / 2;
        } else {
            gCurrent_key += (gKey_count + 1) / 2;
            if (gCurrent_key >= gKey_count) {
                gCurrent_key -= (gKey_count + 1) / 2;
            }
        }
        DRS3StartSound(gIndexed_outlets[0], 3000);
    }
    return 1;
}

// IDA: int __usercall KeyAssignRight@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int KeyAssignRight(int* pCurrent_choice, int* pCurrent_mode) {
    int new_index;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (gCurrent_key < 0) {
        if (gKey_map_index < 3) {
            new_index = gKey_map_index + 1;
        } else {
            new_index = 0;
        }
        ChangeKeyMapIndex(new_index);
        RadioChanged(12, new_index);
        DRS3StartSound(gIndexed_outlets[0], 3000);
    } else {
        if (gCurrent_key >= (gKey_count + 1) / 2) {
            gCurrent_key -= (gKey_count + 1) / 2;
        } else {
            gCurrent_key += (gKey_count + 1) / 2;
            if (gCurrent_key >= gKey_count) {
                gCurrent_key -= (gKey_count + 1) / 2;
            }
        }
        DRS3StartSound(gIndexed_outlets[0], 3000);
    }
    return 1;
}

// IDA: int __usercall KeyAssignUp@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int KeyAssignUp(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (*pCurrent_mode == 0) {
        gCurrent_key = -1;
        *pCurrent_choice = 4;
        *pCurrent_mode = 1;
        DRS3StartSound(gIndexed_outlets[0], 3000);
    } else if (gCurrent_key < 0) {
        gCurrent_key = (gKey_count + 1) / 2 - 1;
        *pCurrent_choice = 4;
        *pCurrent_mode = 1;
        DRS3StartSound(gIndexed_outlets[0], 3000);
    } else if (gCurrent_key > (gKey_count + 1) / 2) {
        gCurrent_key -= 1;
    } else if (gCurrent_key == (gKey_count + 1) / 2) {
        *pCurrent_choice = 0;
        *pCurrent_mode = 0;
    } else if (gCurrent_key == 0) {
        *pCurrent_choice = 0;
        *pCurrent_mode = 0;
    } else {
        gCurrent_key -= 1;
    }
    DRS3StartSound(gIndexed_outlets[0], 3000);
    return 1;
}

// IDA: int __usercall KeyAssignDown@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int KeyAssignDown(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (*pCurrent_mode == 0) {
        if (*pCurrent_choice >= 2) {
            gCurrent_key = (gKey_count + 1) / 2;
        } else {
            gCurrent_key = 0;
        }
        *pCurrent_choice = 4;
        *pCurrent_mode = 1;
    } else if (gCurrent_key < 0) {
        *pCurrent_choice = 0;
        *pCurrent_mode = 0;
    } else if (gCurrent_key < (gKey_count + 1) / 2) {
        if (gCurrent_key < ((gKey_count + 1) / 2 - 1)) {
            gCurrent_key += 1;
        } else {
            gCurrent_key = -1;
        }
    } else if (gCurrent_key < gKey_count - 1) {
        gCurrent_key += 1;
    } else {
        gCurrent_key = -1;
    }
    DRS3StartSound(gIndexed_outlets[0], 3000);
    return 1;
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

    RemoveTransientBitmaps(1);
    if (*pCurrent_mode == 0) {
        key = -1;
        if (*pCurrent_choice == 0) {
            for (i = 0; i < COUNT_OF(gKey_defns); i++) {
                if (gKey_mapping[gKey_defns[i]] == -2) {
                    key = i;
                    break;
                }
            }
        }
        if (key < 0) {
            DontLetFlicFuckWithPalettes();
            TurnFlicTransparencyOn();
            RunFlicAt(
                gThe_interface_spec__options->pushed_flics[*pCurrent_choice].flic_index,
                gThe_interface_spec__options->pushed_flics[*pCurrent_choice].x[gGraf_data_index],
                gThe_interface_spec__options->pushed_flics[*pCurrent_choice].y[gGraf_data_index]);
            TurnFlicTransparencyOff();
            LetFlicFuckWithPalettes();
        }
        switch (*pCurrent_choice) {
        case 0:
            if (key < 0) {
                disallowed = 1;
            } else {
                *pCurrent_choice = key + 4;
                *pCurrent_mode = 1;
                gCurrent_key = key;
                gMouse_in_use = 0;
                DRS3StartSound(gIndexed_outlets[0], 3100);
                disallowed = 0;
            }
            break;
        case 1:
            *pCurrent_choice = -1;
            disallowed = 1;
            break;
        case 2:
            SetKeysToDefault();
            disallowed = 0;
            break;
        case 3:
            disallowed = 1;
            break;
        default:
            disallowed = 0;
            break;
        }
        return disallowed;
    } else {
        if (gCurrent_key < 0) {
            if (gKey_map_index < 3) {
                new_index = gKey_map_index + 1;
            } else {
                new_index = 0;
            }
            ChangeKeyMapIndex(new_index);
            RadioChanged(12, new_index);
        } else {
            PDScreenBufferSwap(0);
            CyclePollKeys();
            PollKeys();
            WaitForNoKeys();
            GetKeyCoords(gCurrent_key, &y, &name_x, &key_x, &end_box);
            font = &gFonts[12];
            gPending_entry = -1;
            while (1) {
                key = PDAnyKeyDown();
                if (key != -1 && key != 63) {
                    for (i = 27; i < 65; i++) {
                        if (gKey_mapping[i] == key && gKey_defns[gCurrent_key] != i) {
                            for (j = 0; j < COUNT_OF(gKey_defns); j++) {
                                if (gKey_defns[j] == i) {
                                    gKey_mapping[i] = -2;
                                    gPending_entry = j;
                                    break;
                                }
                            }
                            if (gPending_entry < 0) {
                                DRS3StartSound(gIndexed_outlets[0], 3100);
                                key = -1;
                                break;
                            }
                        }
                    }
                }
                CyclePollKeys();
                PollKeys();
                if ((PDGetTotalTime() / 100) & 1) {
                    TransDRPixelmapText(gBack_screen, key_x, y, font, gKey_names[gKey_mapping[gKey_defns[gCurrent_key]] + 2], 640);
                } else {
                    BrPixelmapRectangleFill(gBack_screen, key_x, y, end_box - key_x, font->height, 0);
                }
                PDScreenBufferSwap(0);
                if (key != -1 || EitherMouseButtonDown()) {
                    break;
                }
            }
            DRS3StartSound(gIndexed_outlets[0], 3004);
            WaitForNoKeys();
            if (key != 63 && key != -1) {
                gKey_mapping[gKey_defns[gCurrent_key]] = key;
            }
            if (gPending_entry >= 0) {
                *pCurrent_choice = gPending_entry + 4;
                gCurrent_key = gPending_entry;
            }
        }
        return 0;
    }
}

// IDA: int __usercall MouseyClickBastard@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int MouseyClickBastard(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int i;
    int x_coord;
    int y_coord;
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);

    if (gCurrent_key < 0) {
        GetMousePosition(&x_coord, &y_coord);
        for (i = gRadio_bastards__options[12].count - 1; i >= 0; i--) {
            if (x_coord + 3 >= gRadio_bastards__options[12].left[i]) { 
                DRS3StartSound(gIndexed_outlets[0], 3000);
                ChangeKeyMapIndex(i);
                RadioChanged(12, i);
                break;
            }
        }
        return 0;
    } else {
        KeyAssignGoAhead(pCurrent_choice, pCurrent_mode);
    }
}

// IDA: void __cdecl DrawInitialKMRadios()
void DrawInitialKMRadios() {
    int i;
    LOG_TRACE("()");

    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    PlayRadioOn2(12, gKey_map_index);
    gRadio_bastards__options[12].current_value = gKey_map_index;
    TurnFlicTransparencyOff();
    DontLetFlicFuckWithPalettes();
}

// IDA: void __cdecl DoControlOptions()
void DoControlOptions() {
    static tFlicette flicker_on[4] = {
        { 177, {  51, 102 }, { 166, 398 } },
        { 177, { 112, 224 }, { 166, 398 } },
        { 177, { 173, 346 }, { 166, 398 } },
        { 177, { 234, 468 }, { 166, 398 } },
    };
    static tFlicette flicker_off[4] = {
        { 176, {  51, 102 }, { 166, 398 } },
        { 176, { 112, 224 }, { 166, 398 } },
        { 176, { 173, 346 }, { 166, 398 } },
        { 176, { 234, 468 }, { 166, 398 } },
    };
    static tFlicette push[4] = {
        { 172, {  51, 102 }, { 166, 398 } },
        { 175, { 112, 224 }, { 166, 398 } },
        { 174, { 173, 346 }, { 166, 398 } },
        { 173, { 234, 468 }, { 166, 398 } },
    };
    static tMouse_area mouse_areas[5] = {
        { {  51, 102 }, { 166, 398 }, { 102, 204 }, { 187, 449 },   0,   0,   0, NULL },
        { { 112, 224 }, { 166, 398 }, { 164, 328 }, { 187, 449 },   1,   0,   0, NULL },
        { { 173, 346 }, { 166, 398 }, { 225, 450 }, { 187, 449 },   2,   0,   0, NULL },
        { { 234, 468 }, { 166, 398 }, { 286, 572 }, { 187, 449 },   3,   0,   0, NULL },
        { {  45,  90 }, {  33,  79 }, { 285, 570 }, { 159, 382 },   4,   1,   0, MouseyClickBastard },
    };
    static tInterface_spec interface_spec = {
        0, 170, 179, 0, 0, 0, 1,
        { -1, -1 }, { -1, 0 }, {  0,  4 }, {  3,  4 }, { NULL, KeyAssignLeft },
        { -1, -1 }, {  1, 0 }, {  0,  4 }, {  3,  4 }, { NULL, KeyAssignRight },
        { -1, -1 }, {  0, 0 }, {  0,  4 }, {  3,  4 }, { KeyAssignUp, KeyAssignUp },
        { -1, -1 }, {  0, 0 }, {  0,  4 }, {  3,  4 }, { KeyAssignDown, KeyAssignDown },
        {  1,  1 }, { KeyAssignGoAhead, KeyAssignGoAhead }, { 1, 1 },
        {  0,  0 }, NULL, DrawKeyAssignments, 0, NULL, DrawInitialKMRadios, NULL, 0,
        {  0,  0 }, NULL, 3, 1,
        COUNT_OF(flicker_on), flicker_on, flicker_off, push,
        COUNT_OF(mouse_areas), mouse_areas,
        0, NULL
    };
    int result;
    int swap_font_1;
    int swap_font_2;
    int swap_font_3;
    int second_time_around;
    int orig_key_map_index;
    LOG_TRACE("()");

    orig_key_map_index = gKey_map_index;
    second_time_around = 0;
    gThe_interface_spec__options = &interface_spec;
    SaveOrigKeyMapping();
    LoadKeyNames();
    LoadFont(9);
    LoadFont(11);
    LoadFont(10);
    LoadFont(12);
    gPending_entry = -1;
    gKey_count = 18;
    gCurrent_key = 8;
    while ((result = DoInterfaceScreen(&interface_spec, second_time_around, 0)) < 0) {
        FadePaletteDown();
        CalibrateJoysticks();
        FadePaletteDown();
        second_time_around = 1;
    }
    if (result == 0) {
        SaveKeyMapping();
    } else {
        gKey_map_index = orig_key_map_index;
        memcpy(gKey_mapping, gOrig_key_mapping, sizeof(gKey_mapping));
    }
    DisposeFont(9);
    DisposeFont(11);
    DisposeFont(10);
    DisposeFont(12);
    DisposeKeyNames();
    if (gProgram_state.racing) {
        FadePaletteDown();
    } else {
        RunFlic(171);
    }
}

// IDA: void __cdecl LoadSoundOptionsData()
void LoadSoundOptionsData() {
    LOG_TRACE("()");

    gDials_pix = LoadPixelmap("DIALSTCK.PIX");
    if (gDials_pix == NULL) {
        FatalError(42);
    }
}

// IDA: void __cdecl FreeSoundOptionsData()
void FreeSoundOptionsData() {
    LOG_TRACE("()");

    BrPixelmapFree(gDials_pix);
}

// IDA: void __cdecl DrawDisabledOptions()
void DrawDisabledOptions() {
    br_pixelmap* image;
    LOG_TRACE("()");

    PrintMemoryDump(0, "INSIDE OPTIONS");

    // Disable sound options menu
    image = LoadPixelmap("NOSNDOPT.PIX");
    DisableChoice(0);
    if (image != NULL) {
        DRPixelmapRectangleMaskedCopy(gBack_screen, gCurrent_graf_data->sound_opt_disable_x,
                gCurrent_graf_data->sound_opt_disable_y, image, 0, 0, image->width, image->height);
        BrPixelmapFree(image);
    }

    // Disable graphics options menu when in-game
    if (gProgram_state.track_spec.the_actor != NULL) {
        image = LoadPixelmap("NODETOPT.PIX");
        DisableChoice(1);
        if (image != NULL) {
            DRPixelmapRectangleMaskedCopy(gBack_screen, gCurrent_graf_data->graph_opt_disable_x,
                    gCurrent_graf_data->graph_opt_disable_y, image, 0, 0, image->width, image->height);
            BrPixelmapFree(image);
        }
    }
}

// IDA: void __cdecl DoOptions()
void DoOptions() {
    static tFlicette flicker_on[4] = {
        { 43, { 57, 114 }, {  41,  98 } },
        { 43, { 57, 114 }, {  78, 187 } },
        { 43, { 57, 114 }, { 114, 274 } },
        { 43, { 57, 114 }, { 154, 370 } }
    };
    static tFlicette flicker_off[4] = {
        { 42, { 57, 114} , {  41,  98 } },
        { 42, { 57, 114} , {  78, 187 } },
        { 42, { 57, 114} , { 114, 274 } },
        { 42, { 57, 114} , { 154, 370 } },
    };
    static tFlicette push[4] = {
        { 144, { 57, 114 }, { 41,  98 } },
        { 146, { 57, 114 }, { 78, 187 } },
        { 145, { 57, 114 }, {114, 274 } },
        {  45, { 57, 114 }, {154, 370 } },
    };
    static tMouse_area mouse_areas[4] = {
        { { 57, 114 }, {  41,  98 }, { 123, 246 }, {  62, 149 }, 0, 0, 0, NULL }, 
        { { 57, 114 }, {  78, 187 }, { 123, 246 }, {  99, 238 }, 1, 0, 0, NULL },
        { { 57, 114 }, { 114, 274 }, { 123, 246 }, { 135, 324 }, 2, 0, 0, NULL },
        { { 57, 114 }, { 154, 370 }, { 123, 246 }, { 175, 420 }, 3, 0, 0, NULL },
    };
    static tInterface_spec interface_spec = {
        0, 140, 0, 141, 141, 141, 1,
        { -1, 0 }, {  0, 0 }, { 0, 0 }, { 0, 0 },{ NULL, NULL },
        { -1, 0 }, {  0, 0 }, { 0, 0 }, { 0, 0 },{ NULL, NULL },
        { -1, 0 }, { -1, 0 }, { 0, 0 }, { 3, 0 },{ NULL, NULL },
        { -1, 0 }, {  1, 0 }, { 0, 0 }, { 3, 0 },{ NULL, NULL },
        {  1, 1 }, { NULL, NULL},
        {  1, 1 }, { NULL, NULL},
        NULL, NULL, 0,
        NULL, DrawDisabledOptions, NULL, 0, { 0, 0 },
        NULL, 3, 1,
        COUNT_OF(flicker_on), flicker_on, flicker_off, push,
        COUNT_OF(mouse_areas), mouse_areas,
        0, NULL,
    };
    int result;
    LOG_TRACE("()");

    PrintMemoryDump(0, "BEFORE ENTERING OPTIONS");
    PreloadBunchOfFlics(1);
    result = DoInterfaceScreen(&interface_spec, 0, gProgram_state.track_spec.the_actor ? 2 : 1);
    switch (result) {
    case 0:
        DoSoundOptions();
        break;
    case 1:
        DoGraphicsOptions();
        break;
    case 2:
        DoControlOptions();
        break;
    }
    UnlockBunchOfFlics(1);
    PrintMemoryDump(0, "AFTER ENTERING OPTIONS");
}
