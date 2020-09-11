#include "depth.h"

#include "brender.h"
#include "globvars.h"
#include "globvrkm.h"
#include <stdlib.h>

tDepth_effect gDistance_depth_effects[4];
br_scalar gSky_height;
br_scalar gSky_x_multiplier;
br_scalar gSky_width;
br_scalar gSky_y_multiplier;
tU32 gLast_depth_change;
br_scalar gOld_yon;
br_pixelmap* gWater_shade_table;
br_material* gHorizon_material;
int gDepth_cueing_on;
int gSky_on;
br_model* gRearview_sky_model;
int gFog_shade_table_power;
br_actor* gRearview_sky_actor;
int gAcid_shade_table_power;
int gWater_shade_table_power;
br_model* gForward_sky_model;
br_actor* gForward_sky_actor;
int gDepth_shade_table_power;
br_pixelmap* gFog_shade_table;
int gSwap_depth_effect_start;
br_pixelmap* gDepth_shade_table;
tSpecial_volume* gLast_camera_special_volume;
tDepth_effect_type gSwap_depth_effect_type;
br_pixelmap* gAcid_shade_table;
int gSwap_depth_effect_end;
br_pixelmap* gSwap_sky_texture;
br_angle gOld_fov;
br_angle gSky_image_width;
br_angle gSky_image_height;
br_angle gSky_image_underground;

// IDA: int __usercall Log2@<EAX>(int pNumber@<EAX>)
int Log2(int pNumber) {
    int i;
    int bits[16];
    LOG_TRACE("(%d)", pNumber);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl CalculateWrappingMultiplier(br_scalar pValue, br_scalar pYon)
br_scalar CalculateWrappingMultiplier(br_scalar pValue, br_scalar pYon) {
    br_scalar k;
    br_scalar trunc_k;
    int int_k;
    LOG_TRACE("(%f, %f)", pValue, pYon);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall DepthCueingShiftToDistance@<ST0>(int pShift@<EAX>)
br_scalar DepthCueingShiftToDistance(int pShift) {
    LOG_TRACE("(%d)", pShift);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall FogAccordingToGPSCDE(br_material *pMaterial@<EAX>)
void FogAccordingToGPSCDE(br_material* pMaterial) {
    int start;
    int end;
    LOG_TRACE("(%p)", pMaterial);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl FrobFog()
void FrobFog() {
    int i;
    br_material* mat;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall InstantDepthChange(tDepth_effect_type pType@<EAX>, br_pixelmap *pSky_texture@<EDX>, int pStart@<EBX>, int pEnd@<ECX>)
void InstantDepthChange(tDepth_effect_type pType, br_pixelmap* pSky_texture, int pStart, int pEnd) {
    LOG_TRACE("(%d, %p, %d, %d)", pType, pSky_texture, pStart, pEnd);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl Tan(br_scalar pAngle)
br_scalar Tan(br_scalar pAngle) {
    LOG_TRACE("(%f)", pAngle);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall EdgeU@<ST0>(br_angle pSky@<EAX>, br_angle pView@<EDX>, br_angle pPerfect@<EBX>)
br_scalar EdgeU(br_angle pSky, br_angle pView, br_angle pPerfect) {
    br_scalar a;
    br_scalar b;
    br_scalar c;
    LOG_TRACE("(%d, %d, %d)", pSky, pView, pPerfect);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MungeSkyModel(br_actor *pCamera@<EAX>, br_model *pModel@<EDX>)
void MungeSkyModel(br_actor* pCamera, br_model* pModel) {
    br_camera* camera_data;
    br_scalar horizon_half_height;
    br_scalar horizon_half_width;
    br_scalar horizon_half_diag;
    br_scalar tan_half_fov;
    br_scalar sky_distance;
    br_angle half_hori_fov;
    br_angle half_diag_fov;
    tU8 nbands;
    tU8 band;
    tU8 vertex;
    tU8 stripe;
    br_scalar edge_u;
    br_scalar narrow_u;
    br_angle min_angle;
    br_angle angle_range;
    br_angle angle;
    LOG_TRACE("(%p, %p)", pCamera, pModel);
    NOT_IMPLEMENTED();
}

// IDA: br_model* __usercall CreateHorizonModel@<EAX>(br_actor *pCamera@<EAX>)
br_model* CreateHorizonModel(br_actor* pCamera) {
    tU8 nbands;
    tU8 band;
    tU8 vertex;
    tU8 stripe;
    br_model* model;
    LOG_TRACE("(%p)", pCamera);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LoadDepthTable(char *pName@<EAX>, br_pixelmap **pTable@<EDX>, int *pPower@<EBX>)
void LoadDepthTable(char* pName, br_pixelmap** pTable, int* pPower) {
    tPath_name the_path;
    int i;
    int j;
    tU8 temp;
    LOG_TRACE("(\"%s\", %p, %p)", pName, pTable, pPower);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitDepthEffects()
void InitDepthEffects() {
    int i;
    int j;
    STUB();
}

// IDA: void __usercall DoDepthByShadeTable(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_pixelmap *pShade_table@<EBX>, int pShade_table_power@<ECX>, int pStart, int pEnd)
void DoDepthByShadeTable(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_pixelmap* pShade_table, int pShade_table_power, int pStart, int pEnd) {
    tU8* render_ptr;
    tU8* shade_table_pixels;
    tU16* depth_ptr;
    tU16 depth_value;
    tU16 too_near;
    int depth_shift_amount;
    int depth_start;
    int y;
    int x;
    int depth_line_skip;
    int render_line_skip;
    LOG_TRACE("(%p, %p, %p, %d, %d, %d)", pRender_buffer, pDepth_buffer, pShade_table, pShade_table_power, pStart, pEnd);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ExternalSky(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>)
void ExternalSky(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world) {
    int dst_x;
    int src_x;
    int dx;
    int hori_y;
    int top_y;
    int hori_pixels;
    br_angle yaw;
    br_angle hori_sky;
    br_angle pitch;
    br_angle vert_sky;
    br_camera* camera;
    br_scalar tan_half_fov;
    br_scalar tan_half_hori_fov;
    br_scalar tan_half_hori_sky;
    br_scalar hshift;
    br_scalar tan_pitch;
    tU8 top_col;
    tU8 bot_col;
    int bot_height;
    int repetitions;
    br_pixelmap* col_map;
    LOG_TRACE("(%p, %p, %p, %p)", pRender_buffer, pDepth_buffer, pCamera, pCamera_to_world);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoHorizon(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>)
void DoHorizon(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world) {
    br_angle yaw;
    br_actor* actor;
    LOG_TRACE("(%p, %p, %p, %p)", pRender_buffer, pDepth_buffer, pCamera, pCamera_to_world);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoDepthCue(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>)
void DoDepthCue(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer) {
    LOG_TRACE("(%p, %p)", pRender_buffer, pDepth_buffer);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoFog(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>)
void DoFog(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer) {
    LOG_TRACE("(%p, %p)", pRender_buffer, pDepth_buffer);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DepthEffect(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>)
void DepthEffect(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world) {
    LOG_TRACE("(%p, %p, %p, %p)", pRender_buffer, pDepth_buffer, pCamera, pCamera_to_world);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DepthEffectSky(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>)
void DepthEffectSky(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world) {
    LOG_TRACE("(%p, %p, %p, %p)", pRender_buffer, pDepth_buffer, pCamera, pCamera_to_world);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoWobbleCamera(br_actor *pCamera@<EAX>)
void DoWobbleCamera(br_actor* pCamera) {
    float f_time;
    static br_scalar mag00;
    static br_scalar mag01;
    static br_scalar mag02;
    static br_scalar mag10;
    static br_scalar mag11;
    static br_scalar mag12;
    static br_scalar mag20;
    static br_scalar mag21;
    static br_scalar mag22;
    static float period00;
    static float period01;
    static float period02;
    static float period10;
    static float period11;
    static float period12;
    static float period20;
    static float period21;
    static float period22;
    LOG_TRACE("(%p)", pCamera);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoDrugWobbleCamera(br_actor *pCamera@<EAX>)
void DoDrugWobbleCamera(br_actor* pCamera) {
    float f_time;
    static br_scalar mag00;
    static br_scalar mag01;
    static br_scalar mag02;
    static br_scalar mag10;
    static br_scalar mag11;
    static br_scalar mag12;
    static br_scalar mag20;
    static br_scalar mag21;
    static br_scalar mag22;
    static float period00;
    static float period01;
    static float period02;
    static float period10;
    static float period11;
    static float period12;
    static float period20;
    static float period21;
    static float period22;
    LOG_TRACE("(%p)", pCamera);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoSpecialCameraEffect(br_actor *pCamera@<EAX>, br_matrix34 *pCamera_to_world@<EDX>)
void DoSpecialCameraEffect(br_actor* pCamera, br_matrix34* pCamera_to_world) {
    LOG_TRACE("(%p, %p)", pCamera, pCamera_to_world);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LessDepthFactor()
void LessDepthFactor() {
    char s[256];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoreDepthFactor()
void MoreDepthFactor() {
    char s[256];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LessDepthFactor2()
void LessDepthFactor2() {
    char s[256];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoreDepthFactor2()
void MoreDepthFactor2() {
    char s[256];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AssertYons()
void AssertYons() {
    br_camera* camera_ptr;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl IncreaseYon()
void IncreaseYon() {
    br_camera* camera_ptr;
    int i;
    char s[256];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DecreaseYon()
void DecreaseYon() {
    br_camera* camera_ptr;
    int i;
    char s[256];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SetYon(br_scalar pYon)
void SetYon(br_scalar pYon) {
    int i;
    br_camera* camera_ptr;

    LOG_TRACE("(%d)", pYon);

    if (pYon < 5.0f) {
        pYon = 5.0f;
    }

    for (i = 0; i < 2; i++) {
        if (gCamera_list[i]) {
            camera_ptr = (br_camera*)gCamera_list[i]->type_data;
            camera_ptr->yon_z = pYon;
        }
    }
    gCamera_yon = pYon;
}

// IDA: br_scalar __cdecl GetYon()
br_scalar GetYon() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl IncreaseAngle()
void IncreaseAngle() {
    br_camera* camera_ptr;
    int i;
    char s[256];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DecreaseAngle()
void DecreaseAngle() {
    br_camera* camera_ptr;
    int i;
    char s[256];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleDepthMode()
void ToggleDepthMode() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl GetSkyTextureOn()
int GetSkyTextureOn() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetSkyTextureOn(int pOn@<EAX>)
void SetSkyTextureOn(int pOn) {
    br_pixelmap* tmp;
    LOG_TRACE("(%d)", pOn);

    if (pOn != gSky_on) {
        tmp = gProgram_state.current_depth_effect.sky_texture;
        gProgram_state.current_depth_effect.sky_texture = gSwap_sky_texture;
        gProgram_state.default_depth_effect.sky_texture = gSwap_sky_texture;
        gSwap_sky_texture = tmp;

        if (gHorizon_material) {
            if (gSwap_sky_texture) {
                gHorizon_material->colour_map = gSwap_sky_texture;
                BrMaterialUpdate(gHorizon_material, -1);
            }
        }
    }
    gSky_on = pOn;
}

// IDA: void __cdecl ToggleSkyQuietly()
void ToggleSkyQuietly() {
    br_pixelmap* temp;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleSky()
void ToggleSky() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl GetDepthCueingOn()
int GetDepthCueingOn() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetDepthCueingOn(int pOn@<EAX>)
void SetDepthCueingOn(int pOn) {
    LOG_TRACE("(%d)", pOn);
    if (pOn != gDepth_cueing_on && gHorizon_material) {
        InstantDepthChange(gSwap_depth_effect_type, gProgram_state.current_depth_effect.sky_texture, gSwap_depth_effect_start, gSwap_depth_effect_end);
        gSwap_depth_effect_type = gProgram_state.current_depth_effect.type;
        gSwap_depth_effect_start = gProgram_state.current_depth_effect.start;
        gSwap_depth_effect_end = gProgram_state.current_depth_effect.end;
    }
    gDepth_cueing_on = pOn;
}

// IDA: void __cdecl ToggleDepthCueingQuietly()
void ToggleDepthCueingQuietly() {
    tDepth_effect_type temp_type;
    int temp_start;
    int temp_end;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleDepthCueing()
void ToggleDepthCueing() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ChangeDepthEffect()
void ChangeDepthEffect() {
    br_scalar x1;
    br_scalar x2;
    br_scalar y1;
    br_scalar y2;
    br_scalar z1;
    br_scalar z2;
    br_scalar distance;
    tSpecial_volume* special_volume;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MungeForwardSky()
void MungeForwardSky() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MungeRearviewSky()
void MungeRearviewSky() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
