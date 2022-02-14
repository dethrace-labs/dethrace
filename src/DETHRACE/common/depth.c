#include "depth.h"

#include "brender/brender.h"
#include "displays.h"
#include "errors.h"
#include "globvars.h"
#include "globvrkm.h"
#include "harness/trace.h"
#include "spark.h"
#include "utility.h"
#include <math.h>
#include <stdlib.h>

tDepth_effect gDistance_depth_effects[4];
int gSky_on;
int gDepth_cueing_on;
tDepth_effect_type gSwap_depth_effect_type;
br_scalar gSky_height;
br_scalar gSky_x_multiplier;
br_scalar gSky_width;
br_scalar gSky_y_multiplier;
tU32 gLast_depth_change;
br_scalar gOld_yon;
br_pixelmap* gWater_shade_table;
br_material* gHorizon_material;
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

    bits[0] = 1;
    bits[1] = 2;
    bits[2] = 4;
    bits[3] = 8;
    bits[4] = 16;
    bits[5] = 32;
    bits[6] = 64;
    bits[7] = 128;
    bits[8] = 256;
    bits[9] = 512;
    bits[10] = 1024;
    bits[11] = 2048;
    bits[12] = 4096;
    bits[13] = 0x2000;
    bits[14] = 0x4000;
    bits[15] = 0x8000;
    for (i = 15; i >= 0; --i) {
        if ((bits[i] & pNumber) != 0) {
            return i;
        }
    }
    return 0;
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

    if (pType == -1) {
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

// IDA: br_scalar __cdecl Tan(br_scalar pAngle)
br_scalar Tan(br_scalar pAngle) {
    LOG_TRACE("(%f)", pAngle);
    pAngle = sin(pAngle * 0.00009587379924285257);
    return pAngle / cos(pAngle);
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
    STUB();
    return NULL;
}

// IDA: void __usercall LoadDepthTable(char *pName@<EAX>, br_pixelmap **pTable@<EDX>, int *pPower@<EBX>)
void LoadDepthTable(char* pName, br_pixelmap** pTable, int* pPower) {
    tPath_name the_path;
    int i;
    int j;
    tU8 temp;
    LOG_TRACE("(\"%s\", %p, %p)", pName, pTable, pPower);

#define pTABLE_PIXELS ((tU8*)(*pTable)->pixels)

    PathCat(the_path, gApplication_path, "SHADETAB");
    PathCat(the_path, the_path, pName);
    *pTable = DRPixelmapLoad(the_path);
    if (!*pTable) {
        FatalError(87);
    }
    *pPower = Log2((*pTable)->height);
    for (i = 0; i < (*pTable)->width; i++) {
        for (j = 0; j < (*pTable)->height / 2; j++) {
            temp = pTABLE_PIXELS[j * (*pTable)->row_bytes + i];
            pTABLE_PIXELS[j * (*pTable)->row_bytes + i] = pTABLE_PIXELS[(*pTable)->row_bytes * ((*pTable)->height - j - 1) + i];
            pTABLE_PIXELS[(*pTable)->row_bytes * ((*pTable)->height - j - 1) + i] = temp;
        }
    }
}

// IDA: void __cdecl InitDepthEffects()
void InitDepthEffects() {
    tPath_name the_path;
    int i;
    int j;

    LoadDepthTable("DEPTHCUE.TAB", &gDepth_shade_table, &gDepth_shade_table_power);
    LoadDepthTable("FOG.TAB", &gFog_shade_table, &gFog_shade_table_power);
    LoadDepthTable("ACIDFOG.TAB", &gAcid_shade_table, &gAcid_shade_table_power);
    LoadDepthTable("BLUEGIT.TAB", &gWater_shade_table, &gWater_shade_table_power);
    GenerateSmokeShades();
    PathCat(the_path, gApplication_path, "MATERIAL");
    PathCat(the_path, the_path, "HORIZON.MAT");
    gHorizon_material = BrMaterialLoad(the_path);
    if (!gHorizon_material) {
        FatalError(89);
    }
    gHorizon_material->index_blend = BrPixelmapAllocate(3u, 256, 256, 0, 0);
    BrTableAdd(gHorizon_material->index_blend);
    for (i = 0; i < 256; i++) {
        for (j = 0; j < 256; j++) {
            *((tU8*)gHorizon_material->index_blend->pixels + 256 * i + j) = j;
        }
    }
    gHorizon_material->flags |= 0x20u;
    BrMaterialAdd(gHorizon_material);
    gForward_sky_model = CreateHorizonModel(gCamera);
    gRearview_sky_model = CreateHorizonModel(gRearview_camera);
    LOG_WARN("InitDepthEffects not fully implemented. This will break cockpit views");
    // BrModelAdd(gForward_sky_model);
    // BrModelAdd(gRearview_sky_model);
    // gForward_sky_actor = BrActorAllocate(BR_ACTOR_MODEL, 0);
    // gForward_sky_actor->model = gForward_sky_model;
    // gForward_sky_actor->material = gHorizon_material;
    // gForward_sky_actor->render_style = 1;
    // BrActorAdd(gUniverse_actor, gForward_sky_actor);
    // gRearview_sky_actor = BrActorAllocate(BR_ACTOR_MODEL, 0);
    // gRearview_sky_actor->model = gRearview_sky_model;
    // gRearview_sky_actor->material = gHorizon_material;
    // gRearview_sky_actor->render_style = 1;
    // BrActorAdd(gUniverse_actor, gRearview_sky_actor);
    gLast_camera_special_volume = 0;
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
    return;
    dx = 0;
    col_map = gHorizon_material->colour_map;
    camera = (br_camera*)pCamera->type_data;
    // LOG_DEBUG("camera fov %d", camera->field_of_view);
    tan_half_fov = Tan(camera->field_of_view / 2);
    tan_half_hori_fov = tan_half_fov * camera->aspect;
    tan_pitch = 0;
    // LOG_DEBUG("tan_half_fov %f, tan_half_hori_fov %f, tan_pitch %f", tan_half_fov, tan_half_hori_fov, tan_pitch);

    // LOG_DEBUG("pCamera->m[2][0] %f, pCamera->m[2][2] %f", pCamera->t.t.mat.m[2][0], pCamera->t.t.mat.m[2][2]);
    // LOG_DEBUG("pCamera_to_world->m[2][0] %f, pCamera_to_world->m[2][2] %f", pCamera_to_world->m[2][0], pCamera_to_world->m[2][2]);

    pitch = BrRadianToAngle(atan2(pCamera_to_world->m[2][0], pCamera_to_world->m[2][2]));
    yaw = BrRadianToAngle(atan2(col_map->width * tan_half_hori_fov / (double)pRender_buffer->width, 1));
    tan_pitch = -((double)pitch
        * 0.0000152587890625
        / ((double)(uint16_t)(65520
               / (int)(1.0
                       / ((double)(uint16_t)(2 * yaw) * 0.0000152587890625)
                   + 0.5))
            * 0.0000152587890625));

    // LOG_DEBUG("tan_half_fov %f, tan_half_hori_fov %f, tan_pitch %f", tan_half_fov, tan_half_hori_fov, tan_pitch);
    // LOG_DEBUG("rep1 %d", (int)((double)col_map->width * tan_pitch));
    for (repetitions = (int)((double)col_map->width * tan_pitch); repetitions < 0; repetitions += col_map->width) {
        ;
    }
    // LOG_DEBUG("rep2 %d", repetitions);
    while (col_map->width < repetitions) {
        repetitions -= col_map->width;
    }
    // LOG_DEBUG("rep3 %d", repetitions);

    top_y = -(pCamera_to_world->m[2][1] / sqrt(pCamera_to_world->m[2][1]) / tan_half_fov * (double)pRender_buffer->height / 2.0)
        - (col_map->height - gSky_image_underground * col_map->height / gSky_image_height);
    // LOG_DEBUG("gSky_image_underground %d, gSky_image_height %d", gSky_image_underground, gSky_image_height);
    // LOG_DEBUG("top_y %d, r_o_x %d, repetitions %d", top_y, pRender_buffer->origin_x, repetitions);
    while (pRender_buffer->width > dx) {
        hori_pixels = col_map->width - repetitions;
        if (hori_pixels >= pRender_buffer->width - dx) {
            hori_pixels = pRender_buffer->width - dx;
        }
        DRPixelmapRectangleCopy(
            pRender_buffer,
            dx - pRender_buffer->origin_x,
            top_y,
            col_map,
            repetitions - col_map->origin_x,
            -col_map->origin_y,
            hori_pixels,
            col_map->height);
        repetitions = 0;
        dx += hori_pixels;
    }
    if (top_y + pRender_buffer->origin_y > 0) {
        DRPixelmapRectangleFill(
            pRender_buffer,
            -pRender_buffer->origin_x,
            -pRender_buffer->origin_y,
            pRender_buffer->width,
            top_y + pRender_buffer->origin_y,
            1);
    }
    bot_height = pRender_buffer->height - pRender_buffer->origin_y - top_y - col_map->height;
    if (bot_height > 0) {
        DRPixelmapRectangleFill(
            pRender_buffer,
            -pRender_buffer->origin_x,
            top_y + col_map->height,
            pRender_buffer->width,
            bot_height,
            *((tU8*)col_map->pixels + col_map->row_bytes * (col_map->height - 1) + 3));
    }
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
    STUB_ONCE();
}

// IDA: void __usercall DepthEffectSky(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>)
void DepthEffectSky(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world) {
    LOG_TRACE("(%p, %p, %p, %p)", pRender_buffer, pDepth_buffer, pCamera, pCamera_to_world);
    STUB_ONCE();
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
    STUB_ONCE();
}

// IDA: void __cdecl LessDepthFactor()
void LessDepthFactor() {
    char s[256];
    LOG_TRACE("()");

    if (gProgram_state.current_depth_effect.start > 3) {
        gProgram_state.current_depth_effect.start--;
    }
    sprintf(s, "Depth start reduced to %d", gProgram_state.current_depth_effect.start);
    NewTextHeadupSlot(4, 0, 500, -1, s);
    gProgram_state.default_depth_effect.start = gProgram_state.current_depth_effect.start;
}

// IDA: void __cdecl MoreDepthFactor()
void MoreDepthFactor() {
    char s[256];
    LOG_TRACE("()");

    if (gProgram_state.current_depth_effect.start < 14) {
        gProgram_state.current_depth_effect.start++;
    }
    sprintf(s, "Depth start increased to %d", gProgram_state.current_depth_effect.start);
    NewTextHeadupSlot(4, 0, 500, -1, s);
    gProgram_state.default_depth_effect.start = gProgram_state.current_depth_effect.start;
}

// IDA: void __cdecl LessDepthFactor2()
void LessDepthFactor2() {
    char s[256];
    LOG_TRACE("()");

    if (gProgram_state.current_depth_effect.end < 14) {
        gProgram_state.current_depth_effect.end++;
    }
    sprintf(s, "Depth end reduced to %d", gProgram_state.current_depth_effect.end);
    NewTextHeadupSlot(4, 0, 500, -1, s);
    gProgram_state.default_depth_effect.end = gProgram_state.current_depth_effect.end;
}

// IDA: void __cdecl MoreDepthFactor2()
void MoreDepthFactor2() {
    char s[256];
    LOG_TRACE("()");

    if (gProgram_state.current_depth_effect.end > 0) {
        gProgram_state.current_depth_effect.end--;
    }
    sprintf(s, "Depth end increased to %d", gProgram_state.current_depth_effect.end);
    NewTextHeadupSlot(4, 0, 500, -1, s);
    gProgram_state.default_depth_effect.end = gProgram_state.current_depth_effect.end;
}

// IDA: void __cdecl AssertYons()
void AssertYons() {
    br_camera* camera_ptr;
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gCamera_list); ++i) {
        camera_ptr = gCamera_list[i]->type_data;
        camera_ptr->yon_z = gYon_multiplier * gCamera_yon;
    }
}

// IDA: void __cdecl IncreaseYon()
void IncreaseYon() {
    br_camera* camera_ptr;
    int i;
    char s[256];
    LOG_TRACE("()");

    gCamera_yon = gCamera_yon + 5.f;
    AssertYons();
    camera_ptr = gCamera_list[1]->type_data;
    i = (int)camera_ptr->yon_z;
    sprintf(s, GetMiscString(114), i);
    NewTextHeadupSlot(4, 0, 2000, -4, s);
}

// IDA: void __cdecl DecreaseYon()
void DecreaseYon() {
    br_camera* camera_ptr;
    int i;
    char s[256];
    LOG_TRACE("()");

    gCamera_yon = gCamera_yon - 5.f;
    if (gCamera_yon < 5.f) {
        gCamera_yon = 5.f;
    }
    AssertYons();
    camera_ptr = gCamera_list[1]->type_data;
    i = (int)camera_ptr->yon_z;
    sprintf(s, GetMiscString(115), i);
    NewTextHeadupSlot(4, 0, 2000, -4, s);
}

// IDA: void __cdecl SetYon(br_scalar pYon)
void SetYon(br_scalar pYon) {
    int i;
    br_camera* camera_ptr;
    LOG_TRACE("(%d)", pYon);

    if (pYon < 5.0f) {
        pYon = 5.0f;
    }

    for (i = 0; i < COUNT_OF(gCamera_list); i++) {
        if (gCamera_list[i]) {
            camera_ptr = gCamera_list[i]->type_data;
            camera_ptr->yon_z = pYon;
        }
    }
    gCamera_yon = pYon;
}

// IDA: br_scalar __cdecl GetYon()
br_scalar GetYon() {
    LOG_TRACE("()");

    return gCamera_yon;
}

// IDA: void __cdecl IncreaseAngle()
void IncreaseAngle() {
    br_camera* camera_ptr;
    int i;
    char s[256];
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gCamera_list); i++) {
        camera_ptr = gCamera_list[i]->type_data;
        camera_ptr->field_of_view += 0x1c7;  // 2.4993896484375 degrees
        if (camera_ptr->field_of_view > 0x78e3) {  // 169.9969482421875 degrees
            camera_ptr->field_of_view = 0x78e3;
        }
#ifdef DETHRACE_FIX_BUGS
        sprintf(s, "Camera angle increased to %f", BrAngleToDegrees(camera_ptr->field_of_view));
#else
        sprintf(s, "Camera angle increased to %d", gProgram_state.current_depth_effect.end);
#endif
        NewTextHeadupSlot(4, 0, 500, -1, s);
    }
}

// IDA: void __cdecl DecreaseAngle()
void DecreaseAngle() {
    br_camera* camera_ptr;
    int i;
    char s[256];
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gCamera_list); i++) {
        camera_ptr = gCamera_list[i]->type_data;
        camera_ptr->field_of_view -= 0x1c7;  // 2.4993896484375 degrees
        if (camera_ptr->field_of_view < 0x71c) {  // 9.99755859375 degrees
            camera_ptr->field_of_view = 0x71c;
        }
#ifdef DETHRACE_FIX_BUGS
        sprintf(s, "Camera angle decreased to %f", BrAngleToDegrees(camera_ptr->field_of_view));
#else
        sprintf(s, "Camera angle decreased to %d", gProgram_state.current_depth_effect.end);
#endif
        NewTextHeadupSlot(4, 0, 500, -1, s);
    }
}

// IDA: void __cdecl ToggleDepthMode()
void ToggleDepthMode() {
    LOG_TRACE("()");

    switch (gProgram_state.current_depth_effect.type) {
    case eDepth_effect_none:
        InstantDepthChange(eDepth_effect_darkness, gProgram_state.current_depth_effect.sky_texture, 8, 0);
        NewTextHeadupSlot(4, 0, 500, -1, "Darkness mode");
        break;
    case eDepth_effect_darkness:
        InstantDepthChange(eDepth_effect_none, gProgram_state.current_depth_effect.sky_texture, 0, 0);
        InstantDepthChange(eDepth_effect_fog, gProgram_state.current_depth_effect.sky_texture, 10, 0);
        NewTextHeadupSlot(4, 0, 500, -1, "Fog mode");
        break;
    case eDepth_effect_fog:
        InstantDepthChange(eDepth_effect_none, gProgram_state.current_depth_effect.sky_texture, 0, 0);
        NewTextHeadupSlot(4, 0, 500, -1, "Depth effects disabled");
        break;
    }
    gProgram_state.default_depth_effect.type = gProgram_state.current_depth_effect.type;
}

// IDA: int __cdecl GetSkyTextureOn()
int GetSkyTextureOn() {
    LOG_TRACE("()");

    return gSky_on;
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

    temp = gProgram_state.current_depth_effect.sky_texture;
    gProgram_state.current_depth_effect.sky_texture = gSwap_sky_texture;
    gSwap_sky_texture = temp;
    gProgram_state.default_depth_effect.sky_texture = gProgram_state.current_depth_effect.sky_texture;
    if (gHorizon_material) {
        if (gProgram_state.current_depth_effect.sky_texture) {
            gHorizon_material->colour_map = gProgram_state.current_depth_effect.sky_texture;
            BrMaterialUpdate(gHorizon_material, 0x7FFFu);
        }
    }
}

// IDA: void __cdecl ToggleSky()
void ToggleSky() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl GetDepthCueingOn()
int GetDepthCueingOn() {
    LOG_TRACE("()");
    return gDepth_cueing_on;
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

    temp_type = gProgram_state.current_depth_effect.type;
    temp_start = gProgram_state.current_depth_effect.start;
    temp_end = gProgram_state.current_depth_effect.end;
    InstantDepthChange(
        gSwap_depth_effect_type,
        gProgram_state.current_depth_effect.sky_texture,
        gSwap_depth_effect_start,
        gSwap_depth_effect_end);
    gSwap_depth_effect_type = temp_type;
    gSwap_depth_effect_start = temp_start;
    gSwap_depth_effect_end = temp_end;
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
    STUB_ONCE();
}

// IDA: void __cdecl MungeForwardSky()
void MungeForwardSky() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MungeRearviewSky()
void MungeRearviewSky() {
    LOG_TRACE("()");
    STUB();
}
