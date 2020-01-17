#include "depth.h"

#include "brender.h"
#include "globvars.h"
#include "globvrkm.h"

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

// Offset: 0
// Size: 110
// EAX: pNumber
int Log2(int pNumber) {
    int i;
    int bits[16];
}

// Offset: 112
// Size: 130
br_scalar CalculateWrappingMultiplier(br_scalar pValue, br_scalar pYon) {
    br_scalar k;
    br_scalar trunc_k;
    int int_k;
}

// Offset: 244
// Size: 78
// EAX: pShift
br_scalar DepthCueingShiftToDistance(int pShift) {
}

// Offset: 324
// Size: 210
// EAX: pMaterial
void FogAccordingToGPSCDE(br_material* pMaterial) {
    int start;
    int end;
}

// Offset: 536
// Size: 206
void FrobFog() {
    br_material* mat;
}

// Offset: 744
// Size: 170
// EAX: pType
// EDX: pSky_texture
// EBX: pStart
// ECX: pEnd
void InstantDepthChange(tDepth_effect_type pType, br_pixelmap* pSky_texture, int pStart, int pEnd) {
    // TODO: not sure if this line is right..
    if ((int)pType == -1) {
        pStart = 3;
        pEnd = 3;
    }
    gProgram_state.current_depth_effect.sky_texture = pSky_texture;
    gHorizon_material->colour_map = pSky_texture;
    BrMaterialUpdate(gHorizon_material, 0x7FFFu);
    gProgram_state.current_depth_effect.type = pType;
    gProgram_state.current_depth_effect.start = pStart;
    gProgram_state.current_depth_effect.end = pEnd;
    gProgram_state.default_depth_effect.type = pType;
    gProgram_state.default_depth_effect.start = pStart;
    gProgram_state.default_depth_effect.end = pEnd;
}

// Offset: 916
// Size: 90
br_scalar Tan(br_scalar pAngle) {
}

// Offset: 1008
// Size: 213
// EAX: pSky
// EDX: pView
// EBX: pPerfect
br_scalar EdgeU(br_angle pSky, br_angle pView, br_angle pPerfect) {
    br_scalar a;
    br_scalar b;
    br_scalar c;
}

// Offset: 1224
// Size: 1602
// EAX: pCamera
// EDX: pModel
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
}

// Offset: 2828
// Size: 882
// EAX: pCamera
br_model* CreateHorizonModel(br_actor* pCamera) {
    tU8 nbands;
    tU8 band;
    tU8 vertex;
    tU8 stripe;
    br_model* model;
}

// Offset: 3712
// Size: 408
// EAX: pName
// EDX: pTable
// EBX: pPower
void LoadDepthTable(char* pName, br_pixelmap** pTable, int* pPower) {
    tPath_name the_path;
    int i;
    int j;
    tU8 temp;
}

// Offset: 4120
// Size: 611
void InitDepthEffects() {
    int i;
    int j;
}

// Offset: 4732
// Size: 769
// EAX: pRender_buffer
// EDX: pDepth_buffer
// EBX: pShade_table
// ECX: pShade_table_power
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
}

// Offset: 5504
// Size: 1009
// EAX: pRender_buffer
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
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
}

// Offset: 6516
// Size: 463
// EAX: pRender_buffer
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void DoHorizon(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world) {
    br_angle yaw;
    br_actor* actor;
}

// Offset: 6980
// Size: 73
// EAX: pRender_buffer
// EDX: pDepth_buffer
void DoDepthCue(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer) {
}

// Offset: 7056
// Size: 73
// EAX: pRender_buffer
// EDX: pDepth_buffer
void DoFog(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer) {
}

// Offset: 7132
// Size: 91
// EAX: pRender_buffer
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void DepthEffect(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world) {
}

// Offset: 7224
// Size: 93
// EAX: pRender_buffer
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void DepthEffectSky(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world) {
}

// Offset: 7320
// Size: 627
// EAX: pCamera
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
}

// Offset: 7948
// Size: 627
// EAX: pCamera
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
}

// Offset: 8576
// Size: 116
// EAX: pCamera
// EDX: pCamera_to_world
void DoSpecialCameraEffect(br_actor* pCamera, br_matrix34* pCamera_to_world) {
}

// Offset: 8692
// Size: 115
void LessDepthFactor() {
}

// Offset: 8808
// Size: 115
void MoreDepthFactor() {
}

// Offset: 8924
// Size: 115
void LessDepthFactor2() {
}

// Offset: 9040
// Size: 115
void MoreDepthFactor2() {
}

// Offset: 9156
// Size: 95
void AssertYons() {
    int i;
}

// Offset: 9252
// Size: 140
void IncreaseYon() {
    int i;
    char s[256];
}

// Offset: 9392
// Size: 162
void DecreaseYon() {
    int i;
    char s[256];
}

// Offset: 9556
// Size: 127
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

// Offset: 9684
// Size: 47
br_scalar GetYon() {
}

// Offset: 9732
// Size: 162
void IncreaseAngle() {
    int i;
    char s[256];
}

// Offset: 9896
// Size: 162
void DecreaseAngle() {
    int i;
    char s[256];
}

// Offset: 10060
// Size: 261
void ToggleDepthMode() {
}

// Offset: 10324
// Size: 45
int GetSkyTextureOn() {
}

// Offset: 10372
// Size: 60
// EAX: pOn
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

// Offset: 10432
// Size: 124
void ToggleSkyQuietly() {
}

// Offset: 10556
// Size: 188
void ToggleSky() {
}

// Offset: 10744
// Size: 45
int GetDepthCueingOn() {
}

// Offset: 10792
// Size: 71
// EAX: pOn
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

// Offset: 10864
// Size: 110
void ToggleDepthCueingQuietly() {
    int temp_start;
    int temp_end;
}

// Offset: 10976
// Size: 188
void ToggleDepthCueing() {
}

// Offset: 11164
// Size: 74
void ChangeDepthEffect() {
    br_scalar x2;
    br_scalar y1;
    br_scalar y2;
    br_scalar z1;
    br_scalar z2;
    br_scalar distance;
    tSpecial_volume* special_volume;
}

// Offset: 11240
// Size: 34
void MungeForwardSky() {
}

// Offset: 11276
// Size: 60
void MungeRearviewSky() {
}
