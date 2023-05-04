#include "depth.h"

#include "brender/brender.h"
#include "displays.h"
#include "errors.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include "pd/sys.h"
#include "replay.h"
#include "spark.h"
#include "trig.h"
#include "utility.h"
#include "world.h"
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
    for (i = COUNT_OF(bits) - 1; i >= 0; --i) {
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

    k = pYon * 1.3f * TAU / pValue;
    int_k = (int)k;
    if (k - int_k <= .5f) {
        trunc_k = int_k;
    } else {
        trunc_k = int_k + 1.f;
    }
    return trunc_k / TAU * pValue;
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
void FrobFog(void) {
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

    return sinf(BrAngleToRadian(pAngle)) / cosf(BrAngleToRadian(pAngle));
}

// IDA: br_scalar __usercall EdgeU@<ST0>(br_angle pSky@<EAX>, br_angle pView@<EDX>, br_angle pPerfect@<EBX>)
br_scalar EdgeU(br_angle pSky, br_angle pView, br_angle pPerfect) {
    br_scalar a;
    br_scalar b;
    br_scalar c;
    LOG_TRACE("(%d, %d, %d)", pSky, pView, pPerfect);

    a = cosf(BrAngleToRadian(pPerfect));
    b = sinf(BrAngleToRadian(pView));
    c = cosf(BrAngleToRadian(pView));
    return b * a * a / (BrAngleToRadian(pSky) * (1.f + c));
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

    camera_data = pCamera->type_data;
    tan_half_fov = Tan(camera_data->field_of_view / 2);
    sky_distance = camera_data->yon_z - 1.f;
    horizon_half_width = sky_distance * tan_half_fov;
    horizon_half_height = horizon_half_width * camera_data->aspect;
    horizon_half_diag = sqrtf(horizon_half_height * horizon_half_height + horizon_half_width * horizon_half_width);
    half_diag_fov = BrRadianToAngle(atan2f(horizon_half_diag, sky_distance));
    edge_u = EdgeU(gSky_image_width, 2 * half_diag_fov, BR_ANGLE_DEG(10));
    narrow_u = edge_u / 2.f;
    gSky_width = horizon_half_width * 2.f;
    gSky_height = horizon_half_height * 2.f;
    gSky_x_multiplier = CalculateWrappingMultiplier(gSky_width, camera_data->yon_z);
    gSky_y_multiplier = CalculateWrappingMultiplier(gSky_height, camera_data->yon_z);

    for (vertex = 0; vertex < 88; vertex += 4) {
        pModel->vertices[vertex].map.v[0] = -edge_u;
    }
    for (vertex = 1; vertex < 88; vertex += 4) {
        pModel->vertices[vertex].map.v[0] = -narrow_u;
    }
    for (vertex = 2; vertex < 88; vertex += 4) {
        pModel->vertices[vertex].map.v[0] = narrow_u;
    }
    for (vertex = 3; vertex < 88; vertex += 4) {
        pModel->vertices[vertex].map.v[0] = edge_u;
    }
    for (vertex = 0; vertex < 88; vertex += 4) {
        pModel->vertices[vertex].p.v[0] = -horizon_half_diag;
    }
    for (vertex = 1; vertex < 88; vertex += 4) {
        pModel->vertices[vertex].p.v[0] = -(horizon_half_diag / 2.f);
    }
    for (vertex = 2; vertex < 88; vertex += 4) {
        pModel->vertices[vertex].p.v[0] = horizon_half_diag / 2.f;
    }
    for (vertex = 3; vertex < 88; vertex += 4) {
        pModel->vertices[vertex].p.v[0] = horizon_half_diag;
    }
    PossibleService();
    angle_range = -gSky_image_underground - (-BR_ANGLE_DEG(90) - half_diag_fov);
    for (band = 0; band < 2u; band++) {
        vertex = 4 * band;
        angle = -BR_ANGLE_DEG(90) - half_diag_fov + angle_range * band / 2;
        pModel->vertices[vertex].p.v[1] = sinf(BrAngleToRadian(angle)) * sky_distance;
        pModel->vertices[vertex].p.v[2] = -cosf(BrAngleToRadian(angle)) * sky_distance;
    }
    min_angle = -gSky_image_underground;
    angle_range = gSky_image_height;
    nbands = 18;
    for (band = 0; band < nbands; band++) {
        vertex = 4 * band + 8;
        pModel->vertices[vertex].p.v[1] = sinf(BrAngleToRadian(min_angle + angle_range * band / nbands)) * sky_distance;
        pModel->vertices[vertex].p.v[2] = -cosf(BrAngleToRadian(min_angle + angle_range * band / nbands)) * sky_distance;
    }
    min_angle = gSky_image_height - gSky_image_underground;
    angle_range = half_diag_fov + BR_ANGLE_DEG(90) - (gSky_image_height - gSky_image_underground);
    for (band = 0; band <= 1u; band++) {
        vertex = 4 * band + 80;
        angle = min_angle + angle_range * band;
        pModel->vertices[vertex].p.v[1] = sinf(BrAngleToRadian(angle)) * sky_distance;
        pModel->vertices[vertex].p.v[2] = -cosf(BrAngleToRadian(angle)) * sky_distance;
    }
    PossibleService();
    for (band = 0; band <= 21u; ++band) {
        vertex = 4 * band;
        for (stripe = 1; stripe < 4u; ++stripe) {
            pModel->vertices[vertex + stripe].p.v[1] = pModel->vertices[vertex].p.v[1];
            pModel->vertices[vertex + stripe].p.v[2] = pModel->vertices[vertex].p.v[2];
        }
    }

    BrModelUpdate(pModel, BR_MODU_ALL & ~BR_MODU_VERTEX_NORMALS);
}

// IDA: br_model* __usercall CreateHorizonModel@<EAX>(br_actor *pCamera@<EAX>)
br_model* CreateHorizonModel(br_actor* pCamera) {
    tU8 nbands;
    tU8 band;
    tU8 vertex;
    tU8 stripe;
    br_model* model;
    LOG_TRACE("(%p)", pCamera);

    model = BrModelAllocate(NULL, 88, 126);
    model->flags |= BR_MODF_KEEP_ORIGINAL;
    for (band = 0; band < 21; band++) {
        for (stripe = 0; stripe < 3; stripe++) {
            model->faces[6 * band + 2 * stripe].vertices[0] = stripe + 4 * band + 0;
            model->faces[6 * band + 2 * stripe].vertices[1] = stripe + 4 * band + 1;
            model->faces[6 * band + 2 * stripe].vertices[2] = stripe + 4 * band + 5;
            model->faces[6 * band + 2 * stripe + 1].vertices[0] = stripe + 4 * band + 0;
            model->faces[6 * band + 2 * stripe + 1].vertices[1] = stripe + 4 * band + 5;
            model->faces[6 * band + 2 * stripe + 1].vertices[2] = stripe + 4 * band + 4;
            model->faces[6 * band + 2 * stripe + 0].smoothing = 1;
            model->faces[6 * band + 2 * stripe + 1].smoothing = 1;
            model->faces[6 * band + 2 * stripe + 0].material = NULL;
            model->faces[6 * band + 2 * stripe + 1].material = NULL;
        }
    }
    for (vertex = 0; vertex < 12; vertex++) {
        model->vertices[vertex].map.v[1] = 0.9999999f;
    }
    for (vertex = 80; vertex < 88; vertex++) {
        model->vertices[vertex].map.v[1] = 0.f;
    }
    for (band = 1; band < 18; band++) {
        model->vertices[4 * band + 8].map.v[1] = (float)(18 - band) / 18.f;
        for (stripe = 1; stripe < 4; stripe++) {
            model->vertices[4 * band + 8 + stripe].map.v[1] = model->vertices[4 * band + 8].map.v[1];
        }
    }
    return model;
}

// IDA: void __usercall LoadDepthTable(char *pName@<EAX>, br_pixelmap **pTable@<EDX>, int *pPower@<EBX>)
void LoadDepthTable(char* pName, br_pixelmap** pTable, int* pPower) {
    tPath_name the_path;
    int i;
    int j;
    tU8 temp;
    LOG_TRACE("(\"%s\", %p, %p)", pName, pTable, pPower);

#define PTABLE_PIXEL_AT(X, Y) ((tU8*)((*pTable)->pixels))[(X) + (Y) * (*pTable)->row_bytes]

    PathCat(the_path, gApplication_path, "SHADETAB");
    PathCat(the_path, the_path, pName);
    *pTable = DRPixelmapLoad(the_path);
    if (*pTable == NULL) {
        FatalError(kFatalError_LoadDepthCueShadeTable);
    }
    *pPower = Log2((*pTable)->height);
    for (i = 0; i < (*pTable)->width; i++) {
        for (j = 0; j < (*pTable)->height / 2; j++) {
            temp = PTABLE_PIXEL_AT(i, j);
            PTABLE_PIXEL_AT(i, j) = PTABLE_PIXEL_AT(i, ((*pTable)->height - j - 1));
            PTABLE_PIXEL_AT(i, ((*pTable)->height - j - 1)) = temp;
        }
    }

#undef PTABLE_PIXEL_AT
}

// IDA: void __cdecl InitDepthEffects()
void InitDepthEffects(void) {
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
    if (gHorizon_material == NULL) {
        FatalError(kFatalError_FindSkyMaterial_S, "HORIZON.MAT"); // 2nd argument added
    }
    gHorizon_material->index_blend = BrPixelmapAllocate(BR_PMT_INDEX_8, 256, 256, NULL, 0);

    // HACK: moved below loop
    // BrTableAdd(gHorizon_material->index_blend);
    for (i = 0; i < 256; i++) {
        for (j = 0; j < 256; j++) {
            *((tU8*)gHorizon_material->index_blend->pixels + 256 * i + j) = j;
        }
    }
    // HACK: this should be above the for loop. Haven't been able to figure out how this is working in OG, as changes made to the pixelmap
    // don't update the stored copy without calling `BrTableUpdate`.
    BrTableAdd(gHorizon_material->index_blend);

    gHorizon_material->flags |= BR_MATF_PERSPECTIVE;
    BrMaterialAdd(gHorizon_material);
    gForward_sky_model = CreateHorizonModel(gCamera);
    gRearview_sky_model = CreateHorizonModel(gRearview_camera);
    BrModelAdd(gForward_sky_model);
    BrModelAdd(gRearview_sky_model);
    gForward_sky_actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
    gForward_sky_actor->model = gForward_sky_model;
    gForward_sky_actor->material = gHorizon_material;
    gForward_sky_actor->render_style = BR_RSTYLE_NONE;
    BrActorAdd(gUniverse_actor, gForward_sky_actor);
    gRearview_sky_actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
    gRearview_sky_actor->model = gRearview_sky_model;
    gRearview_sky_actor->material = gHorizon_material;
    gRearview_sky_actor->render_style = BR_RSTYLE_NONE;
    BrActorAdd(gUniverse_actor, gRearview_sky_actor);
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

    too_near = 0xffff - (1 << pStart);
    shade_table_pixels = pShade_table->pixels;
    depth_shift_amount = pShade_table_power + 8 - pStart - pEnd;
    render_ptr = (tU8*)pRender_buffer->pixels + pRender_buffer->base_x + pRender_buffer->base_y * pRender_buffer->row_bytes;
    depth_ptr = pDepth_buffer->pixels;
    render_line_skip = pRender_buffer->row_bytes - pRender_buffer->width;
    depth_line_skip = pDepth_buffer->row_bytes / 2 - pRender_buffer->width;

    if (depth_shift_amount <= 0) {
        if (depth_shift_amount >= 0) {
            for (y = 0; y < pRender_buffer->height; ++y) {
                for (x = 0; x < pRender_buffer->width; ++x) {
                    if (*depth_ptr != 0xFFFF) {
                        depth_value = *depth_ptr - too_near;
                        if (depth_value < -(int16_t)too_near) {
                            *render_ptr = shade_table_pixels[(depth_value & 0xFF00) + *render_ptr];
                        }
                    }
                    ++render_ptr;
                    ++depth_ptr;
                }
                render_ptr += render_line_skip;
                depth_ptr += depth_line_skip;
            }
        } else {
            for (y = 0; pRender_buffer->height > y; ++y) {
                for (x = 0; pRender_buffer->width > x; ++x) {
                    if (*depth_ptr != 0xFFFF) {
                        depth_value = *depth_ptr - too_near;
                        if (depth_value < -(int16_t)too_near) {
                            *render_ptr = shade_table_pixels[*render_ptr + ((depth_value >> (pEnd - (pShade_table_power + 8 - pStart))) & 0xFF00)];
                        }
                    }
                    ++render_ptr;
                    ++depth_ptr;
                }
                render_ptr += render_line_skip;
                depth_ptr += depth_line_skip;
            }
        }
    } else {
        for (y = 0; pRender_buffer->height > y; ++y) {
            for (x = 0; pRender_buffer->width > x; ++x) {
                if (*depth_ptr != 0xFFFF) {
                    depth_value = *depth_ptr - too_near;
                    if (depth_value < -(int16_t)too_near) {
                        *render_ptr = shade_table_pixels[*render_ptr + ((depth_value << depth_shift_amount) & 0xFF00)];
                    }
                }
                ++render_ptr;
                ++depth_ptr;
            }
            render_ptr += render_line_skip;
            depth_ptr += depth_line_skip;
        }
    }
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

    dst_x = 0;
    col_map = gHorizon_material->colour_map;
    camera = (br_camera*)pCamera->type_data;
    tan_half_fov = Tan(camera->field_of_view / 2);
    tan_half_hori_fov = tan_half_fov * camera->aspect;

    vert_sky = BrRadianToAngle(atan2(pCamera_to_world->m[2][0], pCamera_to_world->m[2][2]));
    hori_sky = BrRadianToAngle(atan2(col_map->width * tan_half_hori_fov / (double)pRender_buffer->width, 1));

    tan_half_hori_sky = -BrFixedToFloat(vert_sky) / BrFixedToFloat(BR_ANGLE_DEG(360) / (int)(1.0f / BrFixedToFloat(2 * hori_sky) + 0.5f));

    dx = col_map->width * tan_half_hori_sky;
    while (dx < 0) {
        dx += col_map->width;
    }
    while (dx > col_map->width) {
        dx -= col_map->width;
    }

    hshift = col_map->height - gSky_image_underground * col_map->height / gSky_image_height;
    tan_pitch = sqrtf(pCamera_to_world->m[2][0] * pCamera_to_world->m[2][0] + pCamera_to_world->m[2][2] * pCamera_to_world->m[2][2]);
    hori_y = -(pCamera_to_world->m[2][1]
                 / tan_pitch
                 / tan_half_fov * pRender_buffer->height / 2.0f)
        - hshift;

    while (dst_x < pRender_buffer->width) {
        hori_pixels = col_map->width - dx;
        if (hori_pixels > pRender_buffer->width - dst_x) {
            hori_pixels = pRender_buffer->width - dst_x;
        }
        src_x = dx - col_map->origin_x;
        DRPixelmapRectangleCopy(pRender_buffer, dst_x - pRender_buffer->origin_x, hori_y, col_map, src_x, -col_map->origin_y, hori_pixels, col_map->height);
        dx = 0;
        dst_x += hori_pixels;
    }

    top_y = hori_y + pRender_buffer->origin_y;
    if (top_y > 0) {
        top_col = ((tU8*)col_map->pixels)[0];
        DRPixelmapRectangleFill(pRender_buffer, -pRender_buffer->origin_x, -pRender_buffer->origin_y, pRender_buffer->width, top_y, top_col);
    }
    bot_height = pRender_buffer->height - pRender_buffer->origin_y - hori_y - col_map->height;
    if (bot_height > 0) {
        bot_col = ((tU8*)col_map->pixels)[col_map->row_bytes * (col_map->height - 1)];
        DRPixelmapRectangleFill(pRender_buffer, -pRender_buffer->origin_x, hori_y + col_map->height, pRender_buffer->width, bot_height, bot_col);
    }
}

#define ACTOR_CAMERA(ACTOR) ((br_camera*)((ACTOR)->type_data))

// IDA: void __usercall DoHorizon(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>)
void DoHorizon(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world) {
    br_angle yaw;
    br_actor* actor;
    LOG_TRACE("(%p, %p, %p, %p)", pRender_buffer, pDepth_buffer, pCamera, pCamera_to_world);

    yaw = BrRadianToAngle(atan2f(pCamera_to_world->m[2][0], pCamera_to_world->m[2][2]));
    if (!gProgram_state.cockpit_on && !(gAction_replay_mode && gAction_replay_camera_mode)) {
        return;
    }
    if (gRendering_mirror) {
        actor = gRearview_sky_actor;
    } else {
        actor = gForward_sky_actor;
        if (ACTOR_CAMERA(gCamera)->field_of_view != gOld_fov || ACTOR_CAMERA(gCamera)->yon_z != gOld_yon) {
            gOld_fov = ACTOR_CAMERA(gCamera)->field_of_view;
            gOld_yon = ACTOR_CAMERA(gCamera)->yon_z;
            MungeSkyModel(gCamera, gForward_sky_model);
        }
    }
    BrMatrix34RotateY(&actor->t.t.mat, yaw);
    BrVector3Copy(&actor->t.t.translate.t, (br_vector3*)pCamera_to_world->m[3]);
    gHorizon_material->map_transform.m[0][0] = 1.f;
    gHorizon_material->map_transform.m[0][1] = 0.f;
    gHorizon_material->map_transform.m[1][0] = 0.f;
    gHorizon_material->map_transform.m[1][1] = 1.f;
    gHorizon_material->map_transform.m[2][0] = -BrFixedToFloat(yaw) / BrFixedToFloat(gSky_image_width);
    gHorizon_material->map_transform.m[2][1] = 0.f;
    BrMaterialUpdate(gHorizon_material, BR_MATU_ALL);
    actor->render_style = BR_RSTYLE_FACES;
    BrZbSceneRenderAdd(actor);
    actor->render_style = BR_RSTYLE_NONE;
}

// IDA: void __usercall DoDepthCue(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>)
void DoDepthCue(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer) {
    LOG_TRACE("(%p, %p)", pRender_buffer, pDepth_buffer);

    DoDepthByShadeTable(
        pRender_buffer,
        pDepth_buffer,
        gDepth_shade_table,
        gDepth_shade_table_power,
        gProgram_state.current_depth_effect.start,
        gProgram_state.current_depth_effect.end);
}

// IDA: void __usercall DoFog(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>)
void DoFog(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer) {
    LOG_TRACE("(%p, %p)", pRender_buffer, pDepth_buffer);

    DoDepthByShadeTable(
        pRender_buffer,
        pDepth_buffer,
        gFog_shade_table,
        gFog_shade_table_power,
        gProgram_state.current_depth_effect.start,
        gProgram_state.current_depth_effect.end);
}

// IDA: void __usercall DepthEffect(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>)
void DepthEffect(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world) {
    LOG_TRACE("(%p, %p, %p, %p)", pRender_buffer, pDepth_buffer, pCamera, pCamera_to_world);

    if (gProgram_state.current_depth_effect.type == eDepth_effect_darkness) {
        DoDepthCue(pRender_buffer, pDepth_buffer);
    }
    if (gProgram_state.current_depth_effect.type == eDepth_effect_fog) {
        DoFog(pRender_buffer, pDepth_buffer);
    }
}

// IDA: void __usercall DepthEffectSky(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>)
void DepthEffectSky(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world) {
    LOG_TRACE("(%p, %p, %p, %p)", pRender_buffer, pDepth_buffer, pCamera, pCamera_to_world);

    if (gProgram_state.current_depth_effect.sky_texture != NULL
        && (gLast_camera_special_volume == NULL || gLast_camera_special_volume->sky_col < 0)) {
        DoHorizon(pRender_buffer, pDepth_buffer, pCamera, pCamera_to_world);
    }
}

// IDA: void __usercall DoWobbleCamera(br_actor *pCamera@<EAX>)
void DoWobbleCamera(br_actor* pCamera) {
    float f_time;
    static br_scalar mag00 = 0.02f;
    static br_scalar mag01 = 0.02f;
    static br_scalar mag02 = 0.02f;
    static br_scalar mag10 = 0.15f;
    static br_scalar mag11 = 0.05f;
    static br_scalar mag12 = 0.02f;
    static br_scalar mag20 = 0.f;
    static br_scalar mag21 = 0.f;
    static br_scalar mag22 = 0.f;
    static float period00 = 3000.f;
    static float period01 = 3000.f;
    static float period02 = 4000.f;
    static float period10 = 2200.f;
    static float period11 = 3300.f;
    static float period12 = 3100.f;
    static float period20 = 2800.f;
    static float period21 = 2500.f;
    static float period22 = 3900.f;
    LOG_TRACE("(%p)", pCamera);

    f_time = (float)PDGetTotalTime();
    pCamera->t.t.mat.m[0][0] += FastScalarSin(fmod(f_time / period00 * 360.f, 360.f)) * mag00;
    pCamera->t.t.mat.m[0][1] += FastScalarSin(fmod(f_time / period01 * 360.f, 360.f)) * mag01;
    pCamera->t.t.mat.m[0][2] += FastScalarSin(fmod(f_time / period02 * 360.f, 360.f)) * mag02;
    pCamera->t.t.mat.m[1][0] += FastScalarSin(fmod(f_time / period10 * 360.f, 360.f)) * mag10;
    pCamera->t.t.mat.m[1][1] += FastScalarSin(fmod(f_time / period11 * 360.f, 360.f)) * mag11;
    pCamera->t.t.mat.m[1][2] += FastScalarSin(fmod(f_time / period12 * 360.f, 360.f)) * mag12;
    pCamera->t.t.mat.m[2][0] += FastScalarSin(fmod(f_time / period20 * 360.f, 360.f)) * mag20;
    pCamera->t.t.mat.m[2][1] += FastScalarSin(fmod(f_time / period21 * 360.f, 360.f)) * mag21;
    pCamera->t.t.mat.m[2][2] += FastScalarSin(fmod(f_time / period22 * 360.f, 360.f)) * mag22;
}

// IDA: void __usercall DoDrugWobbleCamera(br_actor *pCamera@<EAX>)
void DoDrugWobbleCamera(br_actor* pCamera) {
    float f_time;
    static br_scalar mag00 = 0.03f;
    static br_scalar mag01 = 0.03f;
    static br_scalar mag02 = 0.03f;
    static br_scalar mag10 = 0.2f;
    static br_scalar mag11 = 0.07;
    static br_scalar mag12 = 0.03f;
    static br_scalar mag20 = 0.02f;
    static br_scalar mag21 = 0.03f;
    static br_scalar mag22 = 0.01f;
    static float period00 = 550.f;
    static float period01 = 700.f;
    static float period02 = 200.f;
    static float period10 = 100.f;
    static float period11 = 1300.f;
    static float period12 = 500.f;
    static float period20 = 800.f;
    static float period21 = 1500.f;
    static float period22 = 300.f;
    LOG_TRACE("(%p)", pCamera);

    f_time = (float)PDGetTotalTime();
    pCamera->t.t.mat.m[0][0] += FastScalarSin(fmod(f_time / period00 * 360.f, 360.f)) * mag00;
    pCamera->t.t.mat.m[0][1] += FastScalarSin(fmod(f_time / period01 * 360.f, 360.f)) * mag01;
    pCamera->t.t.mat.m[0][2] += FastScalarSin(fmod(f_time / period02 * 360.f, 360.f)) * mag02;
    pCamera->t.t.mat.m[1][0] += FastScalarSin(fmod(f_time / period10 * 360.f, 360.f)) * mag10;
    pCamera->t.t.mat.m[1][1] += FastScalarSin(fmod(f_time / period11 * 360.f, 360.f)) * mag11;
    pCamera->t.t.mat.m[1][2] += FastScalarSin(fmod(f_time / period12 * 360.f, 360.f)) * mag12;
    pCamera->t.t.mat.m[2][0] += FastScalarSin(fmod(f_time / period20 * 360.f, 360.f)) * mag20;
    pCamera->t.t.mat.m[2][1] += FastScalarSin(fmod(f_time / period21 * 360.f, 360.f)) * mag21;
    pCamera->t.t.mat.m[2][2] += FastScalarSin(fmod(f_time / period22 * 360.f, 360.f)) * mag22;
}

// IDA: void __usercall DoSpecialCameraEffect(br_actor *pCamera@<EAX>, br_matrix34 *pCamera_to_world@<EDX>)
void DoSpecialCameraEffect(br_actor* pCamera, br_matrix34* pCamera_to_world) {
    LOG_TRACE("(%p, %p)", pCamera, pCamera_to_world);

    if (gOn_drugs) {
        DoDrugWobbleCamera(pCamera);
    } else {
        gLast_camera_special_volume = FindSpecialVolume((br_vector3*)pCamera_to_world->m[3], gLast_camera_special_volume);
        if (gLast_camera_special_volume != NULL) {
            if (gLast_camera_special_volume->camera_special_effect_index == 0) {
                DoWobbleCamera(pCamera);
            }
        }
    }
}

// IDA: void __cdecl LessDepthFactor()
void LessDepthFactor(void) {
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
void MoreDepthFactor(void) {
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
void LessDepthFactor2(void) {
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
void MoreDepthFactor2(void) {
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
void AssertYons(void) {
    br_camera* camera_ptr;
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gCamera_list); ++i) {
        camera_ptr = gCamera_list[i]->type_data;
        camera_ptr->yon_z = gYon_multiplier * gCamera_yon;
    }
}

// IDA: void __cdecl IncreaseYon()
void IncreaseYon(void) {
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
void DecreaseYon(void) {
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
br_scalar GetYon(void) {
    LOG_TRACE("()");

    return gCamera_yon;
}

// IDA: void __cdecl IncreaseAngle()
void IncreaseAngle(void) {
    br_camera* camera_ptr;
    int i;
    char s[256];
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gCamera_list); i++) {
        camera_ptr = gCamera_list[i]->type_data;
        camera_ptr->field_of_view += 0x1c7;       // 2.4993896484375 degrees
        if (camera_ptr->field_of_view > 0x78e3) { // 169.9969482421875 degrees
            camera_ptr->field_of_view = 0x78e3;
        }
#ifdef DETHRACE_FIX_BUGS
        sprintf(s, "Camera angle increased to %f", (float)BrAngleToDegrees(camera_ptr->field_of_view));
#else
        sprintf(s, "Camera angle increased to %d", gProgram_state.current_depth_effect.end);
#endif
        NewTextHeadupSlot(4, 0, 500, -1, s);
    }
}

// IDA: void __cdecl DecreaseAngle()
void DecreaseAngle(void) {
    br_camera* camera_ptr;
    int i;
    char s[256];
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gCamera_list); i++) {
        camera_ptr = gCamera_list[i]->type_data;
        camera_ptr->field_of_view -= 0x1c7;      // 2.4993896484375 degrees
        if (camera_ptr->field_of_view < 0x71c) { // 9.99755859375 degrees
            camera_ptr->field_of_view = 0x71c;
        }
#ifdef DETHRACE_FIX_BUGS
        sprintf(s, "Camera angle decreased to %f", (float)BrAngleToDegrees(camera_ptr->field_of_view));
#else
        sprintf(s, "Camera angle decreased to %d", gProgram_state.current_depth_effect.end);
#endif
        NewTextHeadupSlot(4, 0, 500, -1, s);
    }
}

// IDA: void __cdecl ToggleDepthMode()
void ToggleDepthMode(void) {
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
int GetSkyTextureOn(void) {
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
void ToggleSkyQuietly(void) {
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
void ToggleSky(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl GetDepthCueingOn()
int GetDepthCueingOn(void) {
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
void ToggleDepthCueingQuietly(void) {
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
void ToggleDepthCueing(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ChangeDepthEffect()
void ChangeDepthEffect(void) {
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
void MungeForwardSky(void) {
    LOG_TRACE("()");
}

// IDA: void __cdecl MungeRearviewSky()
void MungeRearviewSky(void) {
    LOG_TRACE("()");

    if (gSky_image_width != 0) {
        MungeSkyModel(gRearview_camera, gRearview_sky_model);
    }
}
