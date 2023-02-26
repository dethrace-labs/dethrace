#include "spark.h"
#include "brender/brender.h"
#include "car.h"
#include "depth.h"
#include "errors.h"
#include "globvars.h"
#include "globvrkm.h"
#include "graphics.h"
#include "harness/trace.h"
#include "loading.h"
#include "opponent.h"
#include "piping.h"
#include "replay.h"
#include "trig.h"
#include "utility.h"
#include "world.h"

#include <stdlib.h>

int gNext_spark;
int gSpark_flags;
int gNext_shrapnel;
int gShrapnel_flags;
br_model* gShrapnel_model[2];
int gSmoke_flags;
int gSmoke_num;
int gOffset = 0;
int gColumn_flags;
int gNext_column;
br_pixelmap* gBlack_smoke_shade_table;
br_pixelmap* gDark_smoke_shade_table;
br_pixelmap* gGrey_smoke_shade_table;
int gSmoke_on = 1;
int gNum_splash_types;
int gIt_type;
br_pixelmap* gIt_shade_table;
br_pixelmap** gDust_table = &gShade_list[8];
br_pixelmap* gFlame_map[20];
tBRender_smoke* gBR_smoke_pointers[30];
tSplash gSplash[32];
br_material* gSplash_material[20];
tBRender_smoke gBR_smoke_structs[30];
tSmoke_column gSmoke_column[25];
br_matrix4 gCameraToScreen;
tSpark gSparks[32];
br_pixelmap* gShade_list[16];
int gN_BR_smoke_structs;
tSmoke gSmoke[25];
tU32 gSplash_flags;
tU32 gNext_splash;
br_model* gLollipop_model;
int gNum_dust_tables;
br_model* gSplash_model;
int gDust_rotate;
br_camera* gSpark_cam;
br_material* gBlack_material;
tShrapnel gShrapnel[15];

// gSmoke_column has 25 elements but all the code just checks the first 5 elements
#define MAX_SMOKE_COLUMNS 5

// Bugfix: At higher FPS, flame animation runs too quickly, so introduce a new frame timer
#define FLAME_ANIMATION_FRAME_INTERVAL 40

// Bugfix: At higher FPS, `CreatePuffOfSmoke` is called too often and causes smoke cirlces to be recycled too quickly so assume around 25fps
#define SMOKE_COLUMN_NEW_PUFF_INTERVAL 30

// IDA: void __cdecl DrawDot(br_scalar z, tU8 *scr_ptr, tU16 *depth_ptr, tU8 *shade_ptr)
void DrawDot(br_scalar z, tU8* scr_ptr, tU16* depth_ptr, tU8* shade_ptr) {
    LOG_TRACE("(%f, %p, %p, %p)", z, scr_ptr, depth_ptr, shade_ptr);

    if (*depth_ptr > (1.0 - z) * 32768.0f) {
        *depth_ptr = (1.0 - z) * 32768.0f;
        *scr_ptr = shade_ptr[*scr_ptr];
    }
}

// IDA: void __usercall SetWorldToScreen(br_pixelmap *pScreen@<EAX>)
void SetWorldToScreen(br_pixelmap* pScreen) {
    br_matrix4 mat;
    br_matrix4 mat2;
    LOG_TRACE("(%p)", pScreen);

    BrMatrix4Perspective(&mat, gSpark_cam->field_of_view, gSpark_cam->aspect, -gSpark_cam->hither_z, -gSpark_cam->yon_z);
    BrMatrix4Scale(&mat2, pScreen->width / 2, pScreen->height / 2, 1.0f);
    BrMatrix4Mul(&gCameraToScreen, &mat, &mat2);
}

// IDA: void __usercall DrawLine3DThroughBRender(br_vector3 *pStart@<EAX>, br_vector3 *pEnd@<EDX>)
void DrawLine3DThroughBRender(br_vector3* pStart, br_vector3* pEnd) {
    LOG_TRACE("(%p, %p)", pStart, pEnd);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DrawLine3D@<EAX>(br_vector3 *start@<EAX>, br_vector3 *end@<EDX>, br_pixelmap *pScreen@<EBX>, br_pixelmap *pDepth_buffer@<ECX>, br_pixelmap *shade_table)
int DrawLine3D(br_vector3* start, br_vector3* end, br_pixelmap* pScreen, br_pixelmap* pDepth_buffer, br_pixelmap* shade_table) {
    br_vector3 o;
    br_vector3 p;
    br_vector3 tv;
    br_vector4 o2;
    br_vector4 p2;
    br_scalar ts;
    LOG_TRACE("(%p, %p, %p, %p, %p)", start, end, pScreen, pDepth_buffer, shade_table);

    o = *start;
    p = *end;
    if (-gSpark_cam->hither_z < o.v[2] || -gSpark_cam->hither_z < p.v[2]) {
        if (-gSpark_cam->hither_z < o.v[2] && -gSpark_cam->hither_z < p.v[2]) {
            return 0;
        }
        ts = (p.v[2] + gSpark_cam->hither_z) / (p.v[2] - o.v[2]);
        if (-gSpark_cam->hither_z < o.v[2]) {
            o.v[0] = p.v[0] - (p.v[0] - o.v[0]) * ts;
            o.v[1] = p.v[1] - (p.v[1] - o.v[1]) * ts;
            o.v[2] = -gSpark_cam->hither_z;
        }
        if (-gSpark_cam->hither_z < p.v[2]) {
            p.v[0] = p.v[0] - (p.v[0] - o.v[0]) * ts;
            p.v[1] = p.v[1] - (p.v[1] - o.v[1]) * ts;
            p.v[2] = -gSpark_cam->hither_z;
        }
    }
    BrMatrix4ApplyP(&o2, &o, &gCameraToScreen);
    BrMatrix4ApplyP(&p2, &p, &gCameraToScreen);
    o.v[0] = o2.v[0] / o2.v[3];
    o.v[1] = o2.v[1] / o2.v[3];
    o.v[2] = o2.v[2] / o2.v[3];
    p.v[0] = p2.v[0] / p2.v[3];
    p.v[1] = p2.v[1] / p2.v[3];
    p.v[2] = p2.v[2] / p2.v[3];
    return DrawLine2D(&o, &p, pScreen, pDepth_buffer, 1.0, shade_table);
}

// IDA: int __usercall DrawLine2D@<EAX>(br_vector3 *o@<EAX>, br_vector3 *p@<EDX>, br_pixelmap *pScreen@<EBX>, br_pixelmap *pDepth_buffer@<ECX>, br_scalar brightness, br_pixelmap *shade_table)
int DrawLine2D(br_vector3* o, br_vector3* p, br_pixelmap* pScreen, br_pixelmap* pDepth_buffer, br_scalar brightness, br_pixelmap* shade_table) {
    tU8* scr_ptr;
    tU16* depth_ptr;
    tU8* shade_ptr;
    int x1;
    int x2;
    int y1;
    int y2;
    int d;
    int dx;
    int dy;
    int ax;
    int sx;
    int ay;
    int sy;
    int x;
    int y;
    br_scalar zbuff;
    br_scalar zbuff_inc;
    int darken_count;
    int darken_init;
    LOG_TRACE("(%p, %p, %p, %p, %f, %p)", o, p, pScreen, pDepth_buffer, brightness, shade_table);

    scr_ptr = (tU8*)pScreen->pixels + pScreen->base_x + pScreen->base_y * pScreen->row_bytes;
    depth_ptr = (tU16*)pDepth_buffer->pixels;
    shade_ptr = (tU8*)shade_table->pixels + shade_table->base_y * shade_table->row_bytes;
    x1 = pScreen->width / 2 + o->v[0];
    x2 = pScreen->width / 2 + p->v[0];
    y1 = pScreen->height / 2 - o->v[1];
    y2 = pScreen->height / 2 - p->v[1];
    if (brightness < 0.001 || brightness > 1.0) {
        return 0;
    }
    if (x1 < 0 || x2 < 0) {
        if (x1 < 0 && x2 < 0) {
            return 0;
        }
        if (x1 >= 0) {
            y2 = y1 - x1 * (y1 - y2) / (x1 - x2);
            p->v[2] = o->v[2] - (o->v[2] - p->v[2]) * (float)x1 / (float)(x1 - x2);
            x2 = 0;
        } else {
            y1 = y2 - x2 * (y2 - y1) / (x2 - x1);
            o->v[2] = p->v[2] - (p->v[2] - o->v[2]) * (float)x2 / (float)(x2 - x1);
            x1 = 0;
        }
    }
    if (pScreen->width <= x1 || pScreen->width <= x2) {
        if (pScreen->width <= x1 && pScreen->width <= x2) {
            return 0;
        }
        if (pScreen->width > x1) {
            y2 = y1 - (y1 - y2) * (x1 - (pScreen->width - 1)) / (x1 - x2);
            p->v[2] = o->v[2] - (o->v[2] - p->v[2]) * (x1 - (float)(pScreen->width - 1)) / (float)(x1 - x2);
            x2 = pScreen->width - 1;
        } else {
            y1 = y2 - (y2 - y1) * (x2 - (pScreen->width - 1)) / (x2 - x1);
            o->v[2] = p->v[2] - (p->v[2] - o->v[2]) * (x2 - (float)(pScreen->width - 1)) / (float)(x2 - x1);
            x1 = pScreen->width - 1;
        }
    }
    if (y1 < 0 || y2 < 0) {
        if (y1 < 0 && y2 < 0) {
            return 0;
        }
        if (y1 >= 0) {
            x2 = x1 - y1 * (x1 - x2) / (y1 - y2);
            p->v[2] = o->v[2] - (o->v[2] - p->v[2]) * (float)y1 / (float)(y1 - y2);
            y2 = 0;
        } else {
            x1 = x2 - y2 * (x2 - x1) / (y2 - y1);
            o->v[2] = p->v[2] - (p->v[2] - o->v[2]) * (float)y2 / (float)(y2 - y1);
            y1 = 0;
        }
    }
    if (pScreen->height <= y1 || pScreen->height <= y2) {
        if (pScreen->height <= y1 && pScreen->height <= y2) {
            return 0;
        }
        if (pScreen->height > y1) {
            x2 = x1 - (x1 - x2) * (y1 - (pScreen->height - 1)) / (y1 - y2);
            p->v[2] = o->v[2] - (o->v[2] - p->v[2]) * (float)(y1 - (pScreen->height - 1)) / (float)(y1 - y2);
            y2 = pScreen->height - 1;
        } else {
            x1 = x2 - (x2 - x1) * (y2 - (pScreen->height - 1)) / (y2 - y1);
            o->v[2] = p->v[2] - (p->v[2] - o->v[2]) * (float)(y2 - (pScreen->height - 1)) / (float)(y2 - y1);
            y1 = pScreen->height - 1;
        }
    }
    zbuff = o->v[2];
    dx = x2 - x1;
    dy = y2 - y1;
    ax = 2 * abs(dx);
    if (x2 - x1 < 0) {
        sx = -1;
    } else {
        sx = 1;
    }
    ay = 2 * abs(dy);
    if (dy < 0) {
        sy = -1;
    } else {
        sy = 1;
    }
    x = x1;
    y = y1;
    scr_ptr += x1 + y1 * pScreen->row_bytes;
    depth_ptr += x1 + y1 * (pDepth_buffer->row_bytes / 2);
    darken_init = (brightness - 0.001) * (float)shade_table->height;
    if (ay >= ax) {
        d = ax - ay / 2;
        darken_init = 500 * ay / darken_init;
        darken_count = darken_init;
        zbuff_inc = (p->v[2] - o->v[2]) * 2.0 / (float)ay;
        while (1) {
            DrawDot(zbuff, scr_ptr, depth_ptr, shade_ptr);
            if (y == y2) {
                break;
            }
            if (d >= 0) {
                scr_ptr += sx;
                depth_ptr += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
            scr_ptr += sy * pScreen->row_bytes;
            depth_ptr += sy * (pDepth_buffer->row_bytes / 2);
            zbuff = zbuff_inc + zbuff;
            darken_count -= 1000;
            while (darken_count <= 0) {
                darken_count += darken_init;
                shade_ptr += shade_table->row_bytes;
            }
        }
    } else {
        d = ay - ax / 2;
        darken_init = 500 * ax / darken_init;
        darken_count = darken_init;
        zbuff_inc = (p->v[2] - o->v[2]) * 2.0 / (float)ax;
        while (1) {
            DrawDot(zbuff, scr_ptr, depth_ptr, shade_ptr);
            if (x == x2) {
                break;
            }
            if (d >= 0) {
                scr_ptr += sy * pScreen->row_bytes;
                depth_ptr += sy * (pDepth_buffer->row_bytes / 2);
                d -= ax;
            }
            x += sx;
            scr_ptr += sx;
            depth_ptr += sx;
            d += ay;
            zbuff = zbuff_inc + zbuff;
            darken_count -= 1000;
            while (darken_count <= 0) {
                darken_count += darken_init;
                shade_ptr += shade_table->row_bytes;
            }
        }
    }
    return 1;
}

// IDA: void __usercall SetLineModelCols(tU8 pCol@<EAX>)
void SetLineModelCols(tU8 pCol) {
    LOG_TRACE("(%d)", pCol);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReplaySparks(br_pixelmap *pRender_screen@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, tU32 pTime@<ECX>)
void ReplaySparks(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera, tU32 pTime) {
    int i;
    br_vector3 pos;
    br_vector3 o;
    br_vector3 p;
    br_vector3 tv;
    br_vector3 new_pos;
    LOG_TRACE("(%p, %p, %p, %d)", pRender_screen, pDepth_buffer, pCamera, pTime);

    for (i = 0; i < COUNT_OF(gSparks); i++) {
        if (gSpark_flags & (1 << i)) {
            if (gSparks[i].car == NULL) {
                BrVector3Copy(&pos, &gSparks[i].pos);
            } else {
                BrMatrix34ApplyP(&tv, &o, &gSparks[i].car->car_master_actor->t.t.mat);
                BrVector3Copy(&o, &tv);
                BrMatrix34ApplyP(&pos, &gSparks[i].pos, &gSparks[i].car->car_master_actor->t.t.mat);
            }
            BrVector3Add(&o, &pos, &gSparks[i].length);
            BrVector3Sub(&tv, &pos, (br_vector3*)gCamera_to_world.m[3]);
            BrMatrix34TApplyV(&new_pos, &tv, &gCamera_to_world);
            BrVector3Sub(&tv, &o, (br_vector3*)gCamera_to_world.m[3]);
            BrMatrix34TApplyV(&p, &tv, &gCamera_to_world);
            if (gSparks[i].colour) {
                DrawLine3D(&p, &new_pos, pRender_screen, pDepth_buffer, gFog_shade_table);
            } else {
                DrawLine3D(&p, &new_pos, pRender_screen, pDepth_buffer, gAcid_shade_table);
            }
        }
    }
}

// IDA: void __usercall RenderSparks(br_pixelmap *pRender_screen@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>, tU32 pTime)
void RenderSparks(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world, tU32 pTime) {
    int i;
    int time;
    br_vector3 tv;
    br_vector3 o;
    br_vector3 p;
    br_vector3 pos;
    br_vector3 new_pos;
    br_scalar ts;
    LOG_TRACE("(%p, %p, %p, %p, %d)", pRender_screen, pDepth_buffer, pCamera, pCamera_to_world, pTime);

    gSpark_cam = pCamera->type_data;
    SetWorldToScreen(pRender_screen);

    if (!gSpark_flags) {
        return;
    }

    if (gAction_replay_mode) {
        ReplaySparks(pRender_screen, pDepth_buffer, pCamera, pTime);
        return;
    }
    StartPipingSession(ePipe_chunk_spark);
    for (i = 0; i < COUNT_OF(gSparks); i++) {
        if (((1u << i) & gSpark_flags) == 0) {
            continue;
        }
        if (gSparks[i].count <= 0) {
            gSparks[i].count = 0;
            gSpark_flags &= ~(1u << i);
        }
        ts = BrVector3Dot(&gSparks[i].normal, &gSparks[i].v);
        BrVector3Scale(&tv, &gSparks[i].normal, ts);
        BrVector3Sub(&gSparks[i].v, &gSparks[i].v, &tv);
        if (gSparks[i].time_sync) {
            BrVector3Scale(&o, &gSparks[i].v, gSparks[i].time_sync / 1000.0);
            gSparks[i].count = gSparks[i].time_sync + gSparks[i].count - pTime;
            gSparks[i].time_sync = 0;
        } else {
            BrVector3Scale(&o, &gSparks[i].v, pTime / 1000.0);
            gSparks[i].count -= pTime;
        }
        BrVector3Accumulate(&gSparks[i].pos, &o);
        time = 1000 - gSparks[i].count;
        if (time > 150) {
            time = 150;
        }
        ts = -time / 1000.0;
        if (gSparks[i].colour) {
            ts = ts / 2.0;
        }
        BrVector3Scale(&gSparks[i].length, &gSparks[i].v, ts);
        ts = pTime * 10.0 / 6900.0;
        if (gSparks[i].car) {
            BrMatrix34ApplyV(&tv, &gSparks[i].length, &gSparks[i].car->car_master_actor->t.t.mat);
            BrVector3Copy(&gSparks[i].length, &tv);
            BrMatrix34ApplyP(&pos, &gSparks[i].pos, &gSparks[i].car->car_master_actor->t.t.mat);
            o = tv;
            gSparks[i].v.v[0] = gSparks[i].v.v[0] - gSparks[i].car->car_master_actor->t.t.mat.m[0][1] * ts;
            gSparks[i].v.v[1] = gSparks[i].v.v[1] - gSparks[i].car->car_master_actor->t.t.mat.m[1][1] * ts;
            gSparks[i].v.v[2] = gSparks[i].v.v[2] - gSparks[i].car->car_master_actor->t.t.mat.m[2][1] * ts;
        } else {
            BrVector3Copy(&pos, &gSparks[i].pos);
            gSparks[i].v.v[1] = gSparks[i].v.v[1] - ts;
        }
        AddSparkToPipingSession(i + (gSparks[i].colour << 8), &pos, &gSparks[i].length);
        BrVector3Add(&o, &gSparks[i].length, &pos);
        BrVector3Sub(&tv, &pos, (br_vector3*)gCamera_to_world.m[3]);
        BrMatrix34TApplyV(&new_pos, &tv, &gCamera_to_world);
        BrVector3Sub(&tv, &o, (br_vector3*)gCamera_to_world.m[3]);
        BrMatrix34TApplyV(&p, &tv, &gCamera_to_world);
        BrVector3SetFloat(&tv, FRandomBetween(-0.1f, 0.1f), FRandomBetween(-0.1f, 0.1f), FRandomBetween(-0.1f, 0.1f));
        BrVector3Accumulate(&gSparks[i].v, &tv);
        ts = 1.0f - BrVector3Length(&gSparks[i].v) / 1.4f * pTime / 1000.0f;
        if (ts < 0.1f) {
            ts = 0.1f;
        }
        BrVector3Scale(&gSparks[i].v, &gSparks[i].v, ts);
        if (gSparks[i].colour) {
            DrawLine3D(&p, &new_pos, pRender_screen, pDepth_buffer, gFog_shade_table);
        } else {
            DrawLine3D(&p, &new_pos, pRender_screen, pDepth_buffer, gAcid_shade_table);
        }
    }
    EndPipingSession();
}

// IDA: void __usercall CreateSingleSpark(tCar_spec *pCar@<EAX>, br_vector3 *pPos@<EDX>, br_vector3 *pVel@<EBX>)
void CreateSingleSpark(tCar_spec* pCar, br_vector3* pPos, br_vector3* pVel) {
    LOG_TRACE("(%p, %p, %p)", pCar, pPos, pVel);

    BrVector3Copy(&gSparks[gNext_spark].pos, pPos);
    BrVector3SetFloat(&gSparks[gNext_spark].normal, 0.0f, 0.0f, 0.0f);
    BrVector3Copy(&gSparks[gNext_spark].v, pVel);
    gSparks[gNext_spark].count = 500;
    gSparks[gNext_spark].car = pCar;
    gSpark_flags |= 1u << gNext_spark;
    gSparks[gNext_spark].time_sync = 1;
    gSparks[gNext_spark].colour = 1;
    gNext_spark++;
    if (gNext_spark >= COUNT_OF(gSparks)) {
        gNext_spark = 0;
    }
}

// IDA: void __usercall CreateSparks(br_vector3 *pos@<EAX>, br_vector3 *v@<EDX>, br_vector3 *pForce@<EBX>, br_scalar sparkiness, tCar_spec *pCar)
void CreateSparks(br_vector3* pos, br_vector3* v, br_vector3* pForce, br_scalar sparkiness, tCar_spec* pCar) {
    br_vector3 norm;
    br_vector3 normal;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 pos2;
    br_scalar ts;
    br_scalar ts2;
    int num;
    int i;
    LOG_TRACE("(%p, %p, %p, %f, %p)", pos, v, pForce, sparkiness, pCar);

    ts = BrVector3Length(pForce);
    BrVector3InvScale(&normal, pForce, ts);
    ts2 = BrVector3Dot(pForce, v);
    if (ts2 >= 0) {
        ts2 = 1.f / (10.f * ts);
    } else {
        ts2 = 1.f / (10.f * ts) - ts2 / (ts * ts);
    }

    BrVector3Scale(&norm, pForce, ts2);
    BrVector3Accumulate(v, &norm);
    num = FRandomBetween(0.f, BrVector3Length(v) / 2.f + 0.7f) * sparkiness;

    if (num > 10) {
        num = 10;
    }
    for (i = 0; i < num; i++) {
        BrVector3Copy(&gSparks[gNext_spark].pos, pos);
        BrVector3Copy(&gSparks[gNext_spark].normal, &normal);
        BrVector3Copy(&gSparks[gNext_spark].v, v);
        gSparks[gNext_spark].v.v[0] *= FRandomBetween(.5f, .9f);
        gSparks[gNext_spark].v.v[1] *= FRandomBetween(.5f, .9f);
        gSparks[gNext_spark].v.v[2] *= FRandomBetween(.5f, .9f);
        gSparks[gNext_spark].count = 1000;
        gSparks[gNext_spark].car = NULL;
        gSpark_flags |= 1u << gNext_spark;
        gSparks[gNext_spark].time_sync = gMechanics_time_sync;
        gSparks[gNext_spark].colour = 0;
        gNext_spark++;
        if (gNext_spark >= COUNT_OF(gSparks)) {
            gNext_spark = 0;
        }
    }
    if ((ts * sparkiness) >= 10.f) {
        tv.v[0] = pos->v[0] - pCar->car_master_actor->t.t.translate.t.v[0] / WORLD_SCALE;
        tv.v[1] = pos->v[1] - pCar->car_master_actor->t.t.translate.t.v[1] / WORLD_SCALE;
        tv.v[2] = pos->v[2] - pCar->car_master_actor->t.t.translate.t.v[2] / WORLD_SCALE;
        BrMatrix34TApplyV(&pos2, &tv, &pCar->car_master_actor->t.t.mat);
        BrMatrix34TApplyV(&norm, &normal, &pCar->car_master_actor->t.t.mat);
        BrVector3Scale(&tv, &norm, .1f);
        BrVector3Accumulate(&pos2, &tv);
        num = (ts * sparkiness / 10.f) + 3;
        if (num > 10) {
            num = 10;
        }
        for (i = 0; i < num; i++) {
            BrVector3Copy(&gSparks[gNext_spark].pos, &pos2);
            BrVector3Copy(&gSparks[gNext_spark].normal, &norm);
            BrVector3SetFloat(&tv, FRandomBetween(-1.f, 1.f), FRandomBetween(-.2f, 1.f), FRandomBetween(-1.f, 1.f));
            ts2 = BrVector3Dot(&norm, &tv);
            BrVector3Scale(&tv2, &norm, ts2);
            BrVector3Sub(&gSparks[gNext_spark].v, &tv, &tv2);
            gSparks[gNext_spark].count = 1000;
            gSparks[gNext_spark].car = pCar;
            gSpark_flags |= 1u << gNext_spark;
            gSparks[gNext_spark].time_sync = gMechanics_time_sync;
            gSparks[gNext_spark].colour = 0;
            gNext_spark++;
            if (gNext_spark >= COUNT_OF(gSparks)) {
                gNext_spark = 0;
            }
        }
        CreateShrapnelShower(pos, v, &normal, ts, pCar, pCar);
    }
}

// IDA: void __usercall CreateSparkShower(br_vector3 *pos@<EAX>, br_vector3 *v@<EDX>, br_vector3 *pForce@<EBX>, tCar_spec *pCar1@<ECX>, tCar_spec *pCar2)
void CreateSparkShower(br_vector3* pos, br_vector3* v, br_vector3* pForce, tCar_spec* pCar1, tCar_spec* pCar2) {
    br_scalar ts;
    br_scalar ts2;
    int num;
    int i;
    tCar_spec* c;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 normal;
    LOG_TRACE("(%p, %p, %p, %p, %p)", pos, v, pForce, pCar1, pCar2);

    ts = BrVector3Length(pForce);
    if (pCar1->driver == eDriver_local_human) {
        c = pCar1;
    } else {
        c = pCar2;
    }
    BrVector3InvScale(&tv, pForce, ts);
    if (ts < 10.f) {
        return;
    }
    CreateShrapnelShower(pos, v, &tv, ts, pCar1, pCar2);
    ts2 = BrVector3Dot(pForce, v) / (ts * ts);
    BrVector3Scale(v, pForce, ts2);
    normal.v[0] = pos->v[0] - c->car_master_actor->t.t.translate.t.v[0] / WORLD_SCALE;
    normal.v[1] = pos->v[1] - c->car_master_actor->t.t.translate.t.v[1] / WORLD_SCALE;
    normal.v[2] = pos->v[2] - c->car_master_actor->t.t.translate.t.v[2] / WORLD_SCALE;
    BrMatrix34TApplyV(pos, &normal, &c->car_master_actor->t.t.mat);
    BrMatrix34TApplyV(&normal, pForce, &c->car_master_actor->t.t.mat);
    num = (ts / 10.f) + 3;
    for (i = 0; i < num; i++) {
        BrVector3Copy(&gSparks[gNext_spark].pos, pos);
        BrVector3SetFloat(&gSparks[gNext_spark].normal, 0.f, 0.f, 0.f);
        BrVector3SetFloat(&normal, FRandomBetween(-1.f, 1.f), FRandomBetween(-.2f, 1.f), FRandomBetween(-1.f, 1.f));
        ts2 = BrVector3LengthSquared(&normal) / (ts * ts);
        BrVector3Scale(&tv, &normal, ts2);
        BrVector3Sub(&gSparks[gNext_spark].v, &normal, &tv);
        BrVector3Accumulate(&gSparks[gNext_spark].v, v);
        gSparks[gNext_spark].count = 1000;
        gSparks[gNext_spark].car = c;
        gSpark_flags |= 1u << gNext_spark;
        gSparks[gNext_spark].time_sync = gMechanics_time_sync;
        gSparks[gNext_spark].colour = 0;
        gNext_spark++;
        if (gNext_spark >= COUNT_OF(gSparks)) {
            gNext_spark = 0;
        }
    }
}

// IDA: void __usercall AdjustSpark(int pSpark_num@<EAX>, br_vector3 *pos@<EDX>, br_vector3 *length@<EBX>)
void AdjustSpark(int pSpark_num, br_vector3* pos, br_vector3* length) {
    br_vector3 tv;
    br_matrix34* mat;
    int i;
    LOG_TRACE("(%d, %p, %p)", pSpark_num, pos, length);

    i = pSpark_num & 0xff;
    gSpark_flags |= 1u << pSpark_num;
    if (gSparks[i].car != NULL) {
        mat = &gSparks[i].car->car_master_actor->t.t.mat;
        tv.v[0] = pos->v[0] - mat->m[3][0];
        tv.v[1] = pos->v[0] - mat->m[3][1];
        tv.v[2] = pos->v[0] - mat->m[3][2];
        BrMatrix34TApplyV(&gSparks[i].pos, &tv, mat);
    } else {
        gSparks[i].pos.v[0] = pos->v[0];
        gSparks[i].pos.v[1] = pos->v[1];
        gSparks[i].pos.v[2] = pos->v[2];
    }
    gSparks[i].length.v[0] = length->v[0];
    gSparks[i].length.v[1] = length->v[1];
    gSparks[i].length.v[2] = length->v[2];
    gSparks[i].colour = pSpark_num >> 8;
}

// IDA: void __usercall AdjustShrapnel(int pShrapnel_num@<EAX>, br_vector3 *pos@<EDX>, tU16 pAge@<EBX>, br_material *pMaterial@<ECX>)
void AdjustShrapnel(int pShrapnel_num, br_vector3* pos, tU16 pAge, br_material* pMaterial) {
    int i;
    LOG_TRACE("(%d, %p, %d, %p)", pShrapnel_num, pos, pAge, pMaterial);

    i = pShrapnel_num & 0x7fff;
    if (!(gShrapnel_flags & (1u << i))) {
        BrActorAdd(gNon_track_actor, gShrapnel[i].actor);
    }
    gShrapnel_flags |= 1u << i;
    gShrapnel[i].actor->t.t.translate.t.v[0] = pos->v[0];
    gShrapnel[i].actor->t.t.translate.t.v[1] = pos->v[1];
    gShrapnel[i].actor->t.t.translate.t.v[2] = pos->v[2];
    if (pShrapnel_num & 0x8000) {
        gShrapnel[i].age = pAge;
        gShrapnel[i].actor->material = pMaterial;
    }
}

// IDA: void __cdecl ResetSparks()
void ResetSparks() {
    LOG_TRACE("()");

    gSpark_flags = 0;
}

// IDA: void __cdecl ResetShrapnel()
void ResetShrapnel() {
    int i;
    LOG_TRACE("()");

    if (gShrapnel_flags == 0) {
        return;
    }
    for (i = 0; i < COUNT_OF(gShrapnel); i++) {
        if (gShrapnel_flags & (1u << i)) {
            BrActorRemove(gShrapnel[i].actor);
        }
    }
    gShrapnel_flags = 0;
}

// IDA: void __usercall CreateShrapnelShower(br_vector3 *pos@<EAX>, br_vector3 *v@<EDX>, br_vector3 *pNormal@<EBX>, br_scalar pForce, tCar_spec *c1, tCar_spec *c2)
void CreateShrapnelShower(br_vector3* pos, br_vector3* v, br_vector3* pNormal, br_scalar pForce, tCar_spec* c1, tCar_spec* c2) {
    br_scalar ts;
    br_scalar ts2;
    br_scalar rnd;
    int num;
    int i;
    tCar_spec* c;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 vel;
    LOG_TRACE("(%p, %p, %p, %f, %p, %p)", pos, v, pNormal, pForce, c1, c2);

    if (pForce < 10.f) {
        return;
    }
    ts = .3f;
    if (v->v[1] < 0.f) {
        ts = .3f - v->v[1];
    }
    ts2 = pNormal->v[1] * ts;

    tv.v[0] = v->v[0] - ts2 * pNormal->v[0];
    tv.v[1] = v->v[1] + ts - pNormal->v[1] * ts2;
    tv.v[2] = v->v[2] - pNormal->v[2] * ts2;

    num = (pForce / 10.f) * 3;
    rnd = ((pForce + 20.f) * 3.f) / 200.f;
    for (i = 0; i < num; i++) {
        if ((gShrapnel_flags & (1u << gNext_shrapnel)) == 0) {
            BrActorAdd(gNon_track_actor, gShrapnel[gNext_shrapnel].actor);
        }
        gShrapnel_flags |= 1u << gNext_shrapnel;
        BrVector3Copy(&gShrapnel[gNext_shrapnel].actor->t.t.translate.t, pos);
        BrVector3SetFloat(&vel, FRandomBetween(-rnd, rnd), FRandomBetween(0.3f - tv.v[1], rnd), FRandomBetween(-rnd, rnd));
        ts2 = BrVector3Dot(pNormal, &vel);
        BrVector3Scale(&tv2, pNormal, ts2);
        BrVector3Sub(&gShrapnel[gNext_shrapnel].v, &vel, &tv2);
        BrVector3Accumulate(&gShrapnel[gNext_shrapnel].v, &tv);
        gShrapnel[gNext_shrapnel].time_sync = gMechanics_time_sync;
        gShrapnel[gNext_shrapnel].age = 0;
        if (IRandomBetween(0, 2) != 0) {
            c = (IRandomBetween(0, 1) != 0) ? c1 : c2;
            gShrapnel[gNext_shrapnel].actor->material = c->shrapnel_material[IRandomBetween(0, c->max_shrapnel_material - 1)];
        } else {
            gShrapnel[gNext_shrapnel].actor->material = gBlack_material;
        }
        gNext_shrapnel++;
        if (gNext_shrapnel >= COUNT_OF(gShrapnel)) {
            gNext_shrapnel = 0;
        }
    }
}

// IDA: void __cdecl InitShrapnel()
void InitShrapnel() {
    int i;
    int j;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gShrapnel); i++) {
        gShrapnel[i].actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        gShrapnel[i].actor->parent = NULL;
        gShrapnel[i].actor->model = gShrapnel_model[1];
        gShrapnel[i].actor->render_style = BR_RSTYLE_DEFAULT;
        gShrapnel[i].actor->t.type = BR_TRANSFORM_MATRIX34;
        gShrapnel[i].actor->material = BrMaterialFind("DEBRIS.MAT");
        gShrapnel[i].age = 0;
        gShrapnel[i].shear1 = FRandomBetween(-2.f, 2.f);
        gShrapnel[i].shear2 = FRandomBetween(-2.f, 2.f);
        BrVector3SetFloat(&gShrapnel[i].axis,
            FRandomBetween(-1.f, 1.f), FRandomBetween(-1.f, 1.f), FRandomBetween(-1.f, 1.f));
        BrVector3Normalise(&gShrapnel[i].axis, &gShrapnel[i].axis);
    }
}

// IDA: void __cdecl LoadInShrapnel()
void LoadInShrapnel() {
    LOG_TRACE("()");

    gShrapnel_model[0] = LoadModel("FRAG4.DAT");
    gShrapnel_model[1] = LoadModel("FRAG5.DAT");
    BrModelAdd(gShrapnel_model[0]);
    BrModelAdd(gShrapnel_model[1]);
    gBlack_material = BrMaterialFind("M14.MAT");
}

// IDA: void __usercall KillShrapnel(int i@<EAX>)
void KillShrapnel(int i) {
    LOG_TRACE("(%d)", i);

    BrActorRemove(gShrapnel[i].actor);
    gShrapnel_flags &= ~(1u << i);
}

// IDA: void __cdecl DisposeShrapnel()
void DisposeShrapnel() {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gShrapnel); i++) {
        if (gShrapnel_flags & (1u << i)) {
            BrActorRemove(gShrapnel[i].actor);
        }
        BrActorFree(gShrapnel[i].actor);
    }
    gShrapnel_flags = 0;
    for (i = 0; i < COUNT_OF(gShrapnel_model); i++) {
        BrModelRemove(gShrapnel_model[i]);
        BrModelFree(gShrapnel_model[i]);
    }
}

// IDA: void __usercall ReplayShrapnel(tU32 pTime@<EAX>)
void ReplayShrapnel(tU32 pTime) {
    int i;
    br_matrix34* mat;
    LOG_TRACE("(%d)", pTime);

    for (i = 0; i < COUNT_OF(gShrapnel); i++) {
        mat = &gShrapnel[i].actor->t.t.mat;
        if (gShrapnel_flags & (1 << i)) {
            gShrapnel[i].age += GetReplayRate() * pTime;
            DrMatrix34Rotate(mat, gShrapnel[i].age * BrDegreeToAngle(1), &gShrapnel[i].axis);
            BrMatrix34PreShearX(mat, gShrapnel[i].shear1, gShrapnel[i].shear2);
        }
    }
}

// IDA: void __usercall MungeShrapnel(tU32 pTime@<EAX>)
void MungeShrapnel(tU32 pTime) {
    br_vector3 disp;
    int i;
    br_matrix34* mat;
    br_scalar ts;
    LOG_TRACE("(%d)", pTime);

    MungeSmokeColumn(pTime);
    MungeSplash(pTime);

    if (gAction_replay_mode) {
        ReplayShrapnel(pTime);
        return;
    }

    StartPipingSession(ePipe_chunk_shrapnel);
    for (i = 0; i < COUNT_OF(gShrapnel); i++) {
        mat = &gShrapnel[i].actor->t.t.mat;
        if (((1u << i) & gShrapnel_flags) == 0) {
            continue;
        }
        if (gShrapnel[i].age == -1) {
            KillShrapnel(i);
        } else {
            if (gShrapnel[i].time_sync) {
                BrVector3Scale(&disp, &gShrapnel[i].v, gShrapnel[i].time_sync / 1000.0f);
                gShrapnel[i].time_sync = 0;
            } else {
                BrVector3Scale(&disp, &gShrapnel[i].v, pTime / 1000.0f);
                gShrapnel[i].age += pTime;
            }
            mat->m[3][0] = mat->m[3][0] + disp.v[0];
            mat->m[3][1] = mat->m[3][1] + disp.v[1];
            mat->m[3][2] = mat->m[3][2] + disp.v[2];
            gShrapnel[i].v.v[1] -= (10 * pTime) * 0.00014492753f;
            DrMatrix34Rotate(mat, 182 * gShrapnel[i].age, &gShrapnel[i].axis);
            BrMatrix34PreShearX(mat, gShrapnel[i].shear1, gShrapnel[i].shear2);
            // bug: should this be using "&gShrapnel[i].v"??
            ts = 1.0 - BrVector3Length(&gSparks[i].v) / 1.4 * pTime / 1000.0;
            if (ts < 0.1) {
                ts = 0.1;
            }
            BrVector3Scale(&gShrapnel[i].v, &gShrapnel[i].v, ts);
            AddShrapnelToPipingSession(i + ((gShrapnel[i].age > 1000 || gShrapnel[i].age < pTime) << 15), (br_vector3*)mat->m[3], gShrapnel[i].age - pTime, gShrapnel[i].actor->material);
            if (gShrapnel[i].age > 1000) {
                gShrapnel[i].age = -1;
            }
        }
    }
    EndPipingSession();
}

// IDA: void __usercall DrMatrix34Rotate(br_matrix34 *mat@<EAX>, br_angle r@<EDX>, br_vector3 *a@<EBX>)
void DrMatrix34Rotate(br_matrix34* mat, br_angle r, br_vector3* a) {
    br_scalar t;
    br_scalar s;
    br_scalar c;
    br_scalar txy;
    br_scalar txz;
    br_scalar tyz;
    br_scalar sx;
    br_scalar sy;
    br_scalar sz;
    LOG_TRACE("(%p, %d, %p)", mat, r, a);

    s = FastScalarSinAngle(r);
    c = FastScalarCosAngle(r);
    t = 1.0f - c;
    txy = t * a->v[0] * a->v[1];
    txz = t * a->v[0] * a->v[2];
    tyz = t * a->v[1] * a->v[2];
    sx = a->v[0] * s;
    sy = a->v[1] * s;
    sz = a->v[2] * s;
    mat->m[0][0] = a->v[0] * a->v[0] * t + c;
    mat->m[0][1] = sz + txy;
    mat->m[0][2] = txz - sy;
    mat->m[1][0] = txy - sz;
    mat->m[1][1] = a->v[1] * a->v[1] * t + c;
    mat->m[1][2] = sx + tyz;
    mat->m[2][0] = sy + txz;
    mat->m[2][1] = tyz - sx;
    mat->m[2][2] = a->v[2] * a->v[2] * t + c;
}

// IDA: void __usercall SmokeLine(int l@<EAX>, int x@<EDX>, br_scalar zbuff, int r_squared, tU8 *scr_ptr, tU16 *depth_ptr, tU8 *shade_ptr, br_scalar r_multiplier, br_scalar z_multiplier, br_scalar shade_offset)
void SmokeLine(int l, int x, br_scalar zbuff, int r_squared, tU8* scr_ptr, tU16* depth_ptr, tU8* shade_ptr, br_scalar r_multiplier, br_scalar z_multiplier, br_scalar shade_offset) {
    int i;
    int offset; /* Added by dethrace. */
    int r_multiplier_int;
    int shade_offset_int;
    tU16 z;
    LOG_TRACE("(%d, %d, %f, %d, %p, %p, %p, %f, %f, %f)", l, x, zbuff, r_squared, scr_ptr, depth_ptr, shade_ptr, r_multiplier, z_multiplier, shade_offset);

    scr_ptr += gOffset;
    if (gProgram_state.cockpit_on) {
        depth_ptr += gOffset;
    }
    z = (1.f - zbuff) * 32768.0f;
    r_multiplier_int = r_multiplier * 65536.0f;
    shade_offset_int = shade_offset * 65536.0f;

    for (i = 0; i < l; i++) {
        if (*depth_ptr > z) {
            offset = ((shade_offset_int - r_squared * r_multiplier_int) >> 8) &
                     0xffffff00;
#if defined(DETHRACE_FIX_BUGS)
            /* Prevent buffer underflows by capping negative offsets. */
            offset = MAX(0, offset);
#endif
            *scr_ptr = shade_ptr[*scr_ptr + offset];
        }
        r_multiplier = x + r_squared;
        scr_ptr++;
        x++;
        depth_ptr++;
        r_squared = x + r_multiplier;
    }
}

// IDA: void __usercall SmokeCircle(br_vector3 *o@<EAX>, br_scalar r, br_scalar extra_z, br_scalar strength, br_scalar pAspect, br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_pixelmap *pShade_table)
void SmokeCircle(br_vector3* o, br_scalar r, br_scalar extra_z, br_scalar strength, br_scalar pAspect, br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_pixelmap* pShade_table) {
    tU8* scr_ptr;
    tU16* depth_ptr;
    tU8* shade_ptr;
    tU8* osp;
    tU16* odp;
    int ox;
    int oy;
    int i;
    int r_squared;
    int max_r_squared;
    int l;
    int l2;
    int x;
    int x2;
    int sx;
    int y;
    int inc;
    int y_limit;
    int max_x;
    int min_x;
    br_scalar shade_offset;
    br_scalar r_multiplier;
    br_scalar ry;
    br_scalar z_multiplier;
    br_scalar zbuff;
    br_scalar aspect_squared;
    void (*line)(int, int, br_scalar, int, tU8*, tU16*, tU8*, br_scalar, br_scalar, br_scalar);
    LOG_TRACE("(%p, %f, %f, %f, %f, %p, %p, %p)", o, r, extra_z, strength, pAspect, pRender_screen, pDepth_buffer, pShade_table);

    line = SmokeLine;
    ox = pRender_screen->width / 2 + o->v[0];
    oy = pRender_screen->height / 2 + o->v[1];
    max_r_squared = r * r;
    zbuff = o->v[2];
    aspect_squared = pAspect * pAspect;
    if (pRender_screen->width / 4 <= r
        || pRender_screen->width <= ox - r
        || ox + r < 0.0f) {
        return;
    }
    shade_ptr = (tU8*)pShade_table->pixels + pShade_table->row_bytes * (pShade_table->base_y + 1);
    shade_offset = strength * 14.99f;
    r_multiplier = shade_offset / (double)max_r_squared;
    z_multiplier = extra_z / (double)max_r_squared;
    max_x = pRender_screen->width - ox - 1;
    min_x = -ox;
    ry = r / pAspect;
    l = pRender_screen->height - oy - 1;
    scr_ptr = pRender_screen->pixels;
    scr_ptr += pRender_screen->base_x + pRender_screen->base_y * pRender_screen->row_bytes + ox + l * pRender_screen->row_bytes;
    depth_ptr = (tU16*)pDepth_buffer->pixels + ox + l * (pDepth_buffer->row_bytes / 2);
    osp = scr_ptr;
    odp = depth_ptr;
    if (pRender_screen->height > oy && oy + ry >= 0.0) {
        r_squared = r * r;
        inc = -r;
        y = 0;
        y_limit = ry;
        if (oy < 0) {
            y = -oy;
            r_squared = (y * y) * aspect_squared;
            scr_ptr += oy * pRender_screen->row_bytes;
            depth_ptr += oy * (pDepth_buffer->row_bytes / 2);
            inc = -sqrtf(max_r_squared - r_squared);
            r_squared += inc * inc;
        }
        if (pRender_screen->height < oy + ry) {
            y_limit = pRender_screen->height - oy - 1;
        }
        l = -2 * inc;
        scr_ptr += inc;
        depth_ptr += inc;
        gOffset = 0;
        while (1) {

            x = inc + gOffset;
            if (min_x <= inc + gOffset && l + x - 1 <= max_x) {
                line(l, inc, zbuff, r_squared, scr_ptr, depth_ptr, shade_ptr, r_multiplier, z_multiplier, shade_offset);
            } else {
                if (max_x < x || l + x < min_x) {
                    break;
                }
                x2 = l;
                if (l + x - 1 - max_x > 0) {
                    x2 = max_x - (x - 1);
                }
                if (min_x - x <= 0) {
                    line(x2, inc, zbuff, r_squared, scr_ptr, depth_ptr, shade_ptr, r_multiplier, z_multiplier, shade_offset);
                } else {
                    line(x2 - (min_x - x), min_x - x + inc, zbuff, y * y + (min_x - x + inc) * (min_x - x + inc), &scr_ptr[min_x - x], &depth_ptr[min_x - x], shade_ptr, r_multiplier, z_multiplier, shade_offset);
                }
            }
            if (y_limit <= y) {
                break;
            }
            y++;
            scr_ptr -= pRender_screen->row_bytes;
            depth_ptr -= pDepth_buffer->row_bytes / 2;
            for (r_squared += (2 * y - 1) * aspect_squared; max_r_squared < r_squared && inc < 0; r_squared += 2 * inc - 1) {
                inc++;
                scr_ptr++;
                depth_ptr++;
                l -= 2;
            }
            gOffset += IRandomBetween(-1, 1);
            if (gOffset > r / 5.f) {
                gOffset = r / 5.f;
            }
            if (gOffset < -(r / 5.f)) {
                gOffset = -(r / 5.f);
            }
        }
    }
    if (pAspect < 1.0) {
        aspect_squared = 9.f;
        ry = r / 3.f;
    }
    if (oy > 0 && oy <= pRender_screen->height + ry - 2.f) {
        r_squared = (r * r);
        inc = -r;
        y = 0;
        scr_ptr = osp;
        depth_ptr = odp;
        y_limit = ry;
        if (pRender_screen->height < oy) {
            l2 = oy - pRender_screen->height;
            y = pRender_screen->height - oy;
            r_squared = y * y * aspect_squared;
            scr_ptr = &osp[l2 * pRender_screen->row_bytes];
            depth_ptr = &odp[l2 * (pDepth_buffer->row_bytes / 2)];
            inc = -sqrtf(max_r_squared - r_squared);
            r_squared += inc * inc;
        }
        if (oy - ry < 0.f) {
            y_limit = oy;
        }
        l = -2 * inc;
        scr_ptr += inc;
        depth_ptr += inc;
        gOffset = 0;
        do {
            y--;
            scr_ptr += pRender_screen->row_bytes;
            depth_ptr += pDepth_buffer->row_bytes / 2;
            for (r_squared -= (2 * y - 1) * aspect_squared; max_r_squared < r_squared && inc < 0; r_squared += 2 * inc - 1) {
                inc++;
                scr_ptr++;
                depth_ptr++;
                l -= 2;
            }
            x = inc + gOffset;
            if (min_x <= inc + gOffset && l + x - 1 <= max_x) {
                line(l, inc, zbuff, r_squared, scr_ptr, depth_ptr, shade_ptr, r_multiplier, z_multiplier, shade_offset);
            } else {
                if (max_x < x || l + x < min_x) {
                    return;
                }
                x2 = l;
                if (l + x - 1 - max_x > 0) {
                    x2 = max_x - (x - 1);
                }
                if (min_x - x <= 0) {
                    line(x2, inc, zbuff, r_squared, scr_ptr, depth_ptr, shade_ptr, r_multiplier, z_multiplier, shade_offset);
                } else {
                    line(x2 - (min_x - x), min_x - x + inc, zbuff, y * y + (min_x - x + inc) * (min_x - x + inc), &scr_ptr[min_x - x], &depth_ptr[min_x - x], shade_ptr, r_multiplier, z_multiplier, shade_offset);
                }
            }
            gOffset += IRandomBetween(-1, 1);
            if (gOffset > r / 5.0) {
                gOffset = r / 5.0;
            }
            if (gOffset < -(r / 5.0)) {
                gOffset = -r / 5.0;
            }
        } while (-y < y_limit);
    }
}

// IDA: int __cdecl CmpSmokeZ(void *p1, void *p2)
int CmpSmokeZ(void* p1, void* p2) {
    tBRender_smoke** a;
    tBRender_smoke** b;
    LOG_TRACE("(%p, %p)", p1, p2);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RenderRecordedSmokeCircles()
void RenderRecordedSmokeCircles() {
    int i;
    tBRender_smoke* smoke;
    tU8 red;
    tU8 grn;
    tU8 blu;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RecordSmokeCircle(br_vector3 *pCent@<EAX>, br_scalar pR, br_scalar pStrength, br_pixelmap *pShade, br_scalar pAspect)
void RecordSmokeCircle(br_vector3* pCent, br_scalar pR, br_scalar pStrength, br_pixelmap* pShade, br_scalar pAspect) {
    tU8 shade_index;
    br_colour shade_rgb;
    LOG_TRACE("(%p, %f, %f, %p, %f)", pCent, pR, pStrength, pShade, pAspect);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SmokeCircle3D(br_vector3 *o@<EAX>, br_scalar r, br_scalar strength, br_scalar pAspect, br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_pixelmap *pShade_table, br_actor *pCam)
void SmokeCircle3D(br_vector3* o, br_scalar r, br_scalar strength, br_scalar pAspect, br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_pixelmap* pShade_table, br_actor* pCam) {
    br_vector3 tv;
    br_vector3 p;
    br_vector4 o2;
    br_camera* cam;
    int scaled_r;
    br_scalar extra_z;
    LOG_TRACE("(%p, %f, %f, %f, %p, %p, %p, %p)", o, r, strength, pAspect, pRender_screen, pDepth_buffer, pShade_table, pCam);

    cam = pCam->type_data;
    srand(o->v[2] * 16777216.0f + o->v[1] * 65536.0f + o->v[0] * 256.0f + r);
    BrVector3Sub(&tv, o, (br_vector3*)gCamera_to_world.m[3]);
    BrMatrix34TApplyV(&p, &tv, &gCamera_to_world);

    if (-p.v[2] >= cam->hither_z && -p.v[2] <= cam->yon_z) {
        scaled_r = gCameraToScreen.m[0][0] * r / -p.v[2];
        extra_z = gCameraToScreen.m[3][2] * r / (p.v[2] * p.v[2]);
        BrMatrix4ApplyP(&o2, &p, &gCameraToScreen);
        BrVector3InvScale(&p, &o2, o2.v[3]);
        SmokeCircle(&p, (br_scalar)scaled_r, extra_z, strength, pAspect, pRender_screen, pDepth_buffer, pShade_table);
    }
}

// IDA: void __usercall ReplaySmoke(br_pixelmap *pRender_screen@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>)
void ReplaySmoke(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera) {
    br_scalar aspect;
    int i;
    LOG_TRACE("(%p, %p, %p)", pRender_screen, pDepth_buffer, pCamera);

    for (i = 0; i < COUNT_OF(gSmoke_column); i++) {
        if (gSmoke_flags & (1 << i)) {
            aspect = 1.f + (gSmoke[i].radius - .05f) / .25f * .5f;
            if (gSmoke[i].type & 0x10) {
                SmokeCircle3D(&gSmoke[i].pos, gSmoke[i].radius / aspect, gSmoke[i].strength, 1.f,
                    pRender_screen, pDepth_buffer, gShade_list[gSmoke[i].type & 0xf], pCamera);
            } else {
                SmokeCircle3D(&gSmoke[i].pos, gSmoke[i].radius, gSmoke[i].strength, aspect,
                    pRender_screen, pDepth_buffer, gShade_list[gSmoke[i].type & 0xf], pCamera);
            }
        }
    }
}

// IDA: void __usercall GenerateContinuousSmoke(tCar_spec *pCar@<EAX>, int wheel@<EDX>, tU32 pTime@<EBX>)
void GenerateContinuousSmoke(tCar_spec* pCar, int wheel, tU32 pTime) {
    br_vector3 pos;
    br_vector3 v;
    br_vector3 vcs;
    br_vector3 tv;
    br_scalar decay_factor;
    br_scalar ts;
    br_scalar alpha;
    br_scalar beta;
    int colour;
    LOG_TRACE("(%p, %d, %d)", pCar, wheel, pTime);

    if (pCar->dust_time[wheel] > pTime) {
        pCar->dust_time[wheel] -= pTime;
        return;
    }
    pCar->dust_time[wheel] += IRandomBetween(200, 400) - pTime;
    if (pCar->dust_time[wheel] < 0) {
        pCar->dust_time[wheel] = 0;
    }
    BrVector3Cross(&tv, &pCar->omega, &pCar->wpos[wheel]);
    BrVector3Scale(&vcs, &pCar->velocity_car_space, WORLD_SCALE * 1000.0f);
    BrVector3Accumulate(&vcs, &tv);
    ts = BrVector3LengthSquared(&vcs);
    if (ts < 25.0f) {
        return;
    }
    decay_factor = sqrtf(ts) / 25.0f;
    if (decay_factor > 1.0f) {
        decay_factor = 1.0f;
    }
    BrVector3InvScale(&tv, &pCar->wpos[wheel], WORLD_SCALE);
    tv.v[1] -= pCar->oldd[wheel] / WORLD_SCALE;

    alpha = -1000.0f;
    if (vcs.v[2] > 0.0f) {
        alpha = (pCar->bounds[0].min.v[2] - tv.v[2]) / vcs.v[2];
    } else if (vcs.v[2] < 0.0f) {
        alpha = (pCar->bounds[0].max.v[2] - tv.v[2]) / vcs.v[2];
    }

    beta = -1000.0f;
    if (vcs.v[0] > 0.0f) {
        beta = (pCar->bounds[0].min.v[0] - tv.v[0]) / vcs.v[0];
    } else if (vcs.v[0] < 0.0f) {
        beta = (pCar->bounds[0].max.v[0] - tv.v[0]) / vcs.v[0];
    }

    ts = MAX(alpha, beta);
    BrVector3Scale(&pos, &vcs, ts);
    BrVector3Accumulate(&tv, &pos);
    BrMatrix34ApplyP(&pos, &tv, &pCar->car_master_actor->t.t.mat);
    BrMatrix34ApplyV(&v, &vcs, &pCar->car_master_actor->t.t.mat);

    colour = gDust_rotate + gCurrent_race.material_modifiers[pCar->material_index[wheel]].smoke_type - 2;
    while (colour >= gNum_dust_tables) {
        colour -= gNum_dust_tables;
    }
    CreatePuffOfSmoke(&pos, &v, decay_factor, decay_factor * 2, colour + 8, pCar);
}

// IDA: void __cdecl DustRotate()
void DustRotate() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RenderSmoke(br_pixelmap *pRender_screen@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>, tU32 pTime)
void RenderSmoke(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world, tU32 pTime) {
    int i;
    int j;
    br_vector3 tv;
    br_scalar aspect;
    br_scalar ts;
    tU32 seed;
    tU32 not_lonely;
    LOG_TRACE("(%p, %p, %p, %p, %d)", pRender_screen, pDepth_buffer, pCamera, pCamera_to_world, pTime);

    not_lonely = 0;
    DrawTheGlow(pRender_screen, pDepth_buffer, pCamera);

    if (gSmoke_flags != 0) {
        seed = rand();
        if (gAction_replay_mode) {
            ReplaySmoke(pRender_screen, pDepth_buffer, pCamera);
            srand(seed);
        } else {
            StartPipingSession(ePipe_chunk_smoke);
            for (i = 0; i < COUNT_OF(gSmoke); i++) {
                if ((gSmoke_flags & (1u << i)) != 0) {
                    if (gSmoke[i].strength > 0.0) {
                        if (gSmoke[i].time_sync) {
                            BrVector3Scale(&tv, &gSmoke[i].v, gSmoke[i].time_sync / 1000.0);
                            gSmoke[i].time_sync = 0;
                        } else {
                            BrVector3Scale(&tv, &gSmoke[i].v, pTime / 1000.0);
                        }
                        BrVector3Accumulate(&gSmoke[i].pos, &tv);
                    } else {
                        gSmoke_flags &= ~(1u << i);
                    }
                }
            }
            for (i = 0; i < COUNT_OF(gSmoke); i++) {
                if ((gSmoke_flags & (1u << i)) != 0) {
                    if ((gSmoke[i].type & 0xf) == 7) {
                        not_lonely |= 1u << i;
                    } else if ((not_lonely & (1u << i)) == 0) {
                        for (j = i + 1; j < COUNT_OF(gSmoke); j++) {
                            if ((gSmoke_flags & (1u << j)) != 0) {
                                BrVector3Sub(&tv, &gSmoke[i].pos, &gSmoke[i].pos);
                                ts = BrVector3LengthSquared(&tv);
                                if ((gSmoke[i].radius + gSmoke[j].radius) * (gSmoke[i].radius + gSmoke[j].radius) > ts) {
                                    not_lonely |= (1u << j) | (1u << i);
                                    break;
                                }
                            }
                        }
                    }
                    if (((1u << i) & not_lonely) == 0) {
                        gSmoke[i].strength = gSmoke[i].strength / 2.0;
                    }
                    aspect = (gSmoke[i].radius - 0.05f) / 0.25f * 0.5f + 1.0f;
                    if ((gSmoke[i].type & 0x10) != 0) {
                        SmokeCircle3D(&gSmoke[i].pos, gSmoke[i].radius / aspect, gSmoke[i].strength, 1.0, pRender_screen, pDepth_buffer, gShade_list[gSmoke[i].type & 0xf], pCamera);
                    } else {
                        SmokeCircle3D(&gSmoke[i].pos, gSmoke[i].radius, gSmoke[i].strength, aspect, pRender_screen, pDepth_buffer, gShade_list[gSmoke[i].type & 0xf], pCamera);
                    }
                    if (gSmoke[i].pipe_me) {
                        AddSmokeToPipingSession(i, gSmoke[i].type, &gSmoke[i].pos, gSmoke[i].radius, gSmoke[i].strength);
                    }
                    gSmoke[i].radius = (double)pTime / 1000.0 * gSmoke[i].strength * 0.5 + gSmoke[i].radius;
                    gSmoke[i].strength = gSmoke[i].strength - (double)pTime * gSmoke[i].decay_factor / 1000.0;
                    if (gSmoke[i].radius > 0.3f) {
                        gSmoke[i].radius = 0.3f;
                    }
                    if (gSmoke[i].strength > 0.0) {
                        ts = 1.0f - (double)pTime * 0.002f;
                        if (ts < 0.5f) {
                            ts = 0.5f;
                        }
                        BrVector3Scale(&gSmoke[i].v, &gSmoke[i].v, ts);
                        if (fabs(gSmoke[i].v.v[1]) < 0.43478259f && (gSmoke[i].type & 0xFu) < 7) {
                            if (gSmoke[i].v.v[1] >= 0.0) {
                                gSmoke[i].v.v[1] = 0.43478259f;
                            } else {
                                gSmoke[i].v.v[1] += 0.43478259f;
                            }
                        }
                    } else {
                        gSmoke_flags &= ~(1u << i);
                    }
                }
            }
            EndPipingSession();
            srand(seed);
        }
    }
}

// IDA: void __usercall CreatePuffOfSmoke(br_vector3 *pos@<EAX>, br_vector3 *v@<EDX>, br_scalar strength, br_scalar pDecay_factor, int pType, tCar_spec *pC)
void CreatePuffOfSmoke(br_vector3* pos, br_vector3* v, br_scalar strength, br_scalar pDecay_factor, int pType, tCar_spec* pC) {
    br_vector3 tv;
    int pipe_me;
    LOG_TRACE("(%p, %p, %f, %f, %d, %p)", pos, v, strength, pDecay_factor, pType, pC);

    if (!gSmoke_on) {
        return;
    }
    // if we are too far away from the current car...
    BrVector3Sub(&tv, pos, &gProgram_state.current_car.pos);
    if (BrVector3LengthSquared(&tv) > 625.0) {
        // check the distance from the car we are viewing and if it is too far away also, just return
        BrVector3Sub(&tv, pos, &gCar_to_view->pos);
        if (&gProgram_state.current_car != gCar_to_view && BrVector3LengthSquared(&tv) > 625.0) {
            return;
        }
    }

    BrVector3InvScale(&gSmoke[gSmoke_num].v, v, WORLD_SCALE);
    gSmoke[gSmoke_num].v.v[1] += (1.0f / WORLD_SCALE);
    BrVector3Copy(&gSmoke[gSmoke_num].pos, pos);
    gSmoke[gSmoke_num].radius = 0.05f;
    if ((pType & 0xF) == 7) {
        gSmoke[gSmoke_num].radius *= 2.0f;
    } else {
        gSmoke[gSmoke_num].pos.v[1] += 0.04;
    }
    gSmoke[gSmoke_num].pos.v[1] += 0.04;
    if (strength > 1.0) {
        strength = 1.0;
    }
    gSmoke[gSmoke_num].strength = strength;
    gSmoke_flags |= 1u << gSmoke_num;
    gSmoke[gSmoke_num].time_sync = gMechanics_time_sync;
    gSmoke[gSmoke_num].type = pType;
    gSmoke[gSmoke_num].decay_factor = pDecay_factor;
    gSmoke[gSmoke_num].pipe_me = 1;
    gSmoke_num++;
    if (gSmoke_num >= COUNT_OF(gSmoke)) {
        gSmoke_num = 0;
    }
}

// IDA: void __cdecl ResetSmoke()
void ResetSmoke() {
    LOG_TRACE("()");

    gSmoke_flags = 0;
    ;
}

// IDA: void __usercall AdjustSmoke(int pIndex@<EAX>, tU8 pType@<EDX>, br_vector3 *pPos@<EBX>, br_scalar pRadius, br_scalar pStrength)
void AdjustSmoke(int pIndex, tU8 pType, br_vector3* pPos, br_scalar pRadius, br_scalar pStrength) {
    LOG_TRACE("(%d, %d, %p, %f, %f)", pIndex, pType, pPos, pRadius, pStrength);

    gSmoke[pIndex].type = pType;
    gSmoke[pIndex].radius = pRadius;
    gSmoke[pIndex].strength = pStrength;
    BrVector3Copy(&gSmoke[pIndex].pos, pPos);
    gSmoke_flags |= 1 << pIndex;
}

// IDA: void __cdecl ActorError()
void ActorError() {
    LOG_TRACE("()");
}

// IDA: void __usercall AdjustSmokeColumn(int pIndex@<EAX>, tCar_spec *pCar@<EDX>, int pVertex@<EBX>, int pColour@<ECX>)
void AdjustSmokeColumn(int pIndex, tCar_spec* pCar, int pVertex, int pColour) {
    int i;
    br_actor* actor;
    LOG_TRACE("(%d, %p, %d, %d)", pIndex, pCar, pVertex, pColour);

    gColumn_flags ^= 1 << pIndex;
    gSmoke_column[pIndex].car = pCar;
    gSmoke_column[pIndex].vertex_index = pVertex;
    gSmoke_column[pIndex].colour = pColour;
    for (i = 0; i < COUNT_OF(gSmoke_column->frame_count); i++) {
        gSmoke_column[pIndex].frame_count[i] = 100;
    }
    if (pColour == 0) {
        if ((gColumn_flags & (1 << pIndex)) != 0) {
            if (gSmoke_column[pIndex].flame_actor->depth != 0) {
                ActorError();
            }
            BrActorAdd(gNon_track_actor, gSmoke_column[pIndex].flame_actor);
        } else {
            if (gSmoke_column[pIndex].flame_actor->depth == 0) {
                ActorError();
            }
            BrActorRemove(gSmoke_column[pIndex].flame_actor);
        }
    }
}

// IDA: void __usercall CreateSmokeColumn(tCar_spec *pCar@<EAX>, int pColour@<EDX>, int pVertex_index@<EBX>, tU32 pLifetime@<ECX>)
void CreateSmokeColumn(tCar_spec* pCar, int pColour, int pVertex_index, tU32 pLifetime) {
    int i;
    br_actor* actor;
    tSmoke_column* col;
    LOG_TRACE("(%p, %d, %d, %d)", pCar, pColour, pVertex_index, pLifetime);

    col = &gSmoke_column[gNext_column];
    if (pCar->last_special_volume != NULL && pCar->last_special_volume->gravity_multiplier < 1.0f) {
        return;
    }
    SmudgeCar(pCar, pVertex_index);
    if (pCar->knackered) {
        SmudgeCar(pCar, pCar->fire_vertex[IRandomBetween(0, 11)]);
    }
    if (!gSmoke_on) {
        return;
    }
    if (((1u << gNext_column) & gColumn_flags) != 0) {
        if (gSmoke_column[gNext_column].car != NULL) {
            gSmoke_column[gNext_column].car->num_smoke_columns--;
        }
        StartPipingSession(ePipe_chunk_smoke_column);
        AddSmokeColumnToPipingSession(gNext_column, gSmoke_column[gNext_column].car, gSmoke_column[gNext_column].vertex_index, gSmoke_column[gNext_column].colour);
        EndPipingSession();
        if (gSmoke_column[gNext_column].colour == 0) {
            StartPipingSession(ePipe_chunk_flame);
            actor = col->flame_actor->children;
            for (i = 0; i < 3; i++) {
                if (actor->type == BR_ACTOR_MODEL) {
                    AddFlameToPipingSession(i + 16 * gNext_column, col->frame_count[i], col->scale_x[i], col->scale_y[i], col->offset_x[i], col->offset_z[i]);
                }
                actor->type = BR_ACTOR_NONE;
                actor = actor->next;
            }
            EndPipingSession();
        }
    }
    if (pColour == 0 && (((1u << gNext_column) & gColumn_flags) == 0 || gSmoke_column[gNext_column].colour != 0)) {
        BrActorAdd(gNon_track_actor, gSmoke_column[gNext_column].flame_actor);
    }
    if (pColour != 0 && ((1u << gNext_column) & gColumn_flags) != 0 && gSmoke_column[gNext_column].colour == 0) {
        BrActorRemove(gSmoke_column[gNext_column].flame_actor);
    }
    StartPipingSession(ePipe_chunk_smoke_column);
    AddSmokeColumnToPipingSession(gNext_column, pCar, pVertex_index, pColour);
    EndPipingSession();
    gSmoke_column[gNext_column].car = pCar;
    gSmoke_column[gNext_column].colour = pColour;
    gSmoke_column[gNext_column].lifetime = pLifetime;
    gSmoke_column[gNext_column].time = 0;
    gSmoke_column[gNext_column].smudge_timer = 1000;
    gSmoke_column[gNext_column].vertex_index = pVertex_index;
    gSmoke_column[gNext_column].upright = 1;
    gColumn_flags |= 1u << gNext_column;
    pCar->num_smoke_columns++;
    for (i = 0; i < COUNT_OF(gSmoke_column[gNext_column].frame_count); i++) {
        gSmoke_column[gNext_column].frame_count[i] = 100;
    }
    gNext_column++;
    if (gNext_column >= MAX_SMOKE_COLUMNS) {
        gNext_column = 0;
    }
}

// IDA: void __cdecl GenerateSmokeShades()
void GenerateSmokeShades() {
    static int rb = 0x00;
    static int gb = 0x00;
    static int bb = 0x00;
    static int rd = 0x40;
    static int gd = 0x40;
    static int bd = 0x40;
    static int rg = 0x80;
    static int gg = 0x80;
    static int bg = 0x80;
    LOG_TRACE("()");

    gBlack_smoke_shade_table = GenerateShadeTable(16, gRender_palette, rb, gb, bb, .25f, .6f, .9f);
    gDark_smoke_shade_table = GenerateShadeTable(16, gRender_palette, rd, gd, bd, .25f, .6f, .9f);
    gGrey_smoke_shade_table = GenerateShadeTable(16, gRender_palette, rg, gg, bg, .25f, .6f, .9f);
    gIt_shade_table = GenerateDarkenedShadeTable(16, gRender_palette, 0, 255, 254, .25f, .5f, .75f, .6f);

    gShade_list[0] = gBlack_smoke_shade_table;
    gShade_list[1] = gDark_smoke_shade_table;
    gShade_list[2] = gGrey_smoke_shade_table;
    gShade_list[3] = gFog_shade_table;
    gShade_list[4] = gFog_shade_table;
    gShade_list[7] = gAcid_shade_table;
}

// IDA: void __cdecl GenerateItFoxShadeTable()
void GenerateItFoxShadeTable() {
    LOG_TRACE("()");

    if (gIt_shade_table == NULL) {
        gIt_shade_table = GenerateDarkenedShadeTable(16, gRender_palette, 0, 255, 254, .25f, .5f, .75f, .6f);
    }
}

// IDA: void __usercall AdjustFlame(int pIndex@<EAX>, int pFrame_count@<EDX>, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z)
void AdjustFlame(int pIndex, int pFrame_count, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z) {
    int i;
    int j;
    tSmoke_column* col;
    br_actor* actor;
    LOG_TRACE("(%d, %d, %f, %f, %f, %f)", pIndex, pFrame_count, pScale_x, pScale_y, pOffset_x, pOffset_z);

    i = pIndex >> 4;
    j = pIndex & 0xf;
    col = &gSmoke_column[i];
    col->frame_count[j] = pFrame_count;
    col->scale_x[j] = pScale_x;
    col->scale_y[j] = pScale_y;
    col->offset_x[j] = pOffset_x;
    col->offset_z[j] = pOffset_z;
}

// IDA: void __usercall ReplayFlame(tSmoke_column *col@<EAX>, br_actor *actor@<EDX>)
void ReplayFlame(tSmoke_column* col, br_actor* actor) {
    int i;
    LOG_TRACE("(%p, %p)", col, actor);

    for (i = 0; i < COUNT_OF(col->frame_count); i++, actor = actor->next) {
        col->frame_count[i] += GetReplayRate();
        if (col->frame_count[i] < 0 || col->frame_count[i] >= COUNT_OF(gFlame_map)) {
            actor->type = BR_ACTOR_NONE;
        } else {
            actor->type = BR_ACTOR_MODEL;
            actor->material->colour_map = gFlame_map[col->frame_count[i]];
            BrMaterialUpdate(actor->material, BR_MATU_ALL);
            BrMatrix34Scale(&actor->t.t.mat,
                col->scale_x[i] * gFlame_map[col->frame_count[i]]->width,
                col->scale_y[i] * gFlame_map[col->frame_count[i]]->height,
                1.f);
            actor->t.t.translate.t.v[0] = col->offset_x[i];
            actor->t.t.translate.t.v[2] = col->offset_z[i];
        }
    }
}

// IDA: void __usercall FlameAnimate(int c@<EAX>, br_vector3 *pPos@<EDX>, tU32 pTime@<EBX>)
void FlameAnimate(int c, br_vector3* pPos, tU32 pTime) {
    tSmoke_column* col;
    br_actor* actor;
    int i;
    LOG_TRACE("(%d, %p, %d)", c, pPos, pTime);

    col = &gSmoke_column[c];
    actor = col->flame_actor;
    DRMatrix34RotateY(&actor->t.t.mat, FastScalarArcTan2Angle(gCamera_to_world.m[2][0], gCamera_to_world.m[2][2]));
    actor->t.t.translate.t = *pPos;
    actor = actor->children;

    if (gAction_replay_mode) {
        ReplayFlame(col, actor);
        return;
    }
    for (i = 0; i < COUNT_OF(col->frame_count); i++) {

#ifdef DETHRACE_FIX_BUGS
        col->frame_time[i] += pTime;
        if (col->frame_time[i] > FLAME_ANIMATION_FRAME_INTERVAL) {
            col->frame_time[i] = 0;
            col->frame_count[i]++;
        }
#else
        col->frame_count[i]++;
#endif

        if (col->frame_count[i] >= COUNT_OF(gFlame_map)) {
            StartPipingSession(ePipe_chunk_flame);
            AddFlameToPipingSession(i + 16 * c, col->frame_count[i] + 1, col->scale_x[i], col->scale_y[i], col->offset_x[i], col->offset_z[i]);
            EndPipingSession();
            col->frame_count[i] = IRandomBetween(-5, -1);
            col->scale_x[i] = (2 * IRandomBetween(0, 1) - 1) * SRandomBetween(1.0f, 1.5f) * 0.003f;
            col->scale_y[i] = SRandomBetween(0.5f, 1.0f) * 0.003f;
            col->offset_x[i] = SRandomPosNeg(0.03f);
            col->offset_z[i] = SRandomBetween(-0.03f, 0.0);
            actor->type = BR_ACTOR_NONE;
        }
        if (col->frame_count[i] == 0) {
            if (BrVector3LengthSquared(&col->car->v) >= 80.0f || col->lifetime <= 30 * pTime) {
                col->frame_count[i] = -5;
            } else {
                actor->type = BR_ACTOR_MODEL;
                StartPipingSession(ePipe_chunk_flame);
                AddFlameToPipingSession(i + 16 * c, col->frame_count[i] - 1, col->scale_x[i], col->scale_y[i], col->offset_x[i], col->offset_z[i]);
                EndPipingSession();
            }
        }
        if (col->frame_count[i] >= 0) {
            actor->material->colour_map = gFlame_map[col->frame_count[i]];
            BrMaterialUpdate(actor->material, BR_MATU_ALL);
            BrMatrix34Scale(&actor->t.t.mat, gFlame_map[col->frame_count[i]]->width * col->scale_x[i], gFlame_map[col->frame_count[i]]->height * col->scale_y[i], 1.0);
            actor->t.t.mat.m[3][0] = col->offset_x[i];
            actor->t.t.mat.m[3][2] = col->offset_z[i];
        }
        actor = actor->next;
    }
}

// IDA: void __usercall DoSmokeColumn(int i@<EAX>, tU32 pTime@<EDX>, br_vector3 *pRet_car_pos@<EBX>)
void DoSmokeColumn(int i, tU32 pTime, br_vector3* pRet_car_pos) {
    tCar_spec* c;
    br_actor* actor;
    br_actor* bonny;
    int group;
    LOG_TRACE("(%d, %d, %p)", i, pTime, pRet_car_pos);

    c = gSmoke_column[i].car;
    if (c->car_master_actor->t.t.mat.m[1][1] > 0.1f) {
        gSmoke_column[i].upright = 1;
    }
    if (c->car_master_actor->t.t.mat.m[1][1] < -0.1f) {
        gSmoke_column[i].upright = 0;
    }
    actor = c->car_model_actors[c->principal_car_actor].actor;
    bonny = c->car_model_actors[c->car_actor_count - 1].actor;

    BrVector3Add(pRet_car_pos, &V11MODEL(actor->model)->groups->vertices[gSmoke_column[i].vertex_index].p, &actor->t.t.translate.t);
    if (gProgram_state.cockpit_on && c->driver == eDriver_local_human) {
        if (c->driver_z_offset + 0.2f <= pRet_car_pos->v[2]) {
            pRet_car_pos->v[1] -= -0.07f;
        } else {
            BrMatrix34ApplyP(pRet_car_pos, &V11MODEL(actor->model)->groups->vertices[gSmoke_column[i].vertex_index].p, &bonny->t.t.mat);
        }
    }
    if (!gSmoke_column[i].upright) {
        pRet_car_pos->v[1] = c->bounds[1].min.v[1] / WORLD_SCALE;
    }
    BrMatrix34ApplyP(&gSmoke_column[i].pos, pRet_car_pos, &c->car_master_actor->t.t.mat);
    gSmoke_column[i].pos.v[1] -= 0.03f;
}

// IDA: void __usercall ReplaySmokeColumn(tU32 pTime@<EAX>)
void ReplaySmokeColumn(tU32 pTime) {
    int i;
    br_vector3 dummy;
    LOG_TRACE("(%d)", pTime);

    for (i = 0; i < MAX_SMOKE_COLUMNS; i++) {
        if ((gColumn_flags & (1 << i)) != 0) {
            DoSmokeColumn(i, pTime, &dummy);
            if (gSmoke_column[i].colour == 0) {
                FlameAnimate(i, &gSmoke_column[i].pos, pTime);
            }
        }
    }
}

// IDA: void __usercall MungeSmokeColumn(tU32 pTime@<EAX>)
void MungeSmokeColumn(tU32 pTime) {
    int i;
    int plane;
    br_actor* actor;
    br_actor* bonny;
    br_vector3 car_pos;
    br_vector3 pos;
    br_vector3 v;
    br_vector3 up;
    br_vector3 start;
    br_vector3 end;
    br_scalar ts;
    br_scalar decay_factor;
    tCar_spec* c;
    LOG_TRACE("(%d)", pTime);

    if (gColumn_flags == 0) {
        return;
    }
    if (gAction_replay_mode) {
        ReplaySmokeColumn(pTime);
        return;
    }

    gMechanics_time_sync = 1;
    for (i = 0; i < MAX_SMOKE_COLUMNS; i++) {
        if (((1u << i) & gColumn_flags) == 0) {
            continue;
        }
        if (gSmoke_column[i].lifetime >= pTime) {
            gSmoke_column[i].lifetime -= pTime;
            c = gSmoke_column[i].car;
            DoSmokeColumn(i, pTime, &car_pos);
            if (gSmoke_column[i].colour == 0) {
                FlameAnimate(i, &gSmoke_column[i].pos, pTime);
                if (gSmoke_column[i].smudge_timer >= pTime) {
                    gSmoke_column[i].smudge_timer -= pTime;
                } else {
                    gSmoke_column[i].smudge_timer += 2000;
                    SmudgeCar(gSmoke_column[i].car, gSmoke_column[i].vertex_index);
                    if (gSmoke_column[i].car->knackered) {
                        plane = IRandomBetween(0, COUNT_OF(gSmoke_column[i].car->fire_vertex) - 1);
                        SmudgeCar(gSmoke_column[i].car, gSmoke_column[i].car->fire_vertex[plane]);
                    }
                }
            }
            gSmoke_column[i].time += pTime;
            if (gSmoke_column[i].time > 200) {
#ifdef DETHRACE_FIX_BUGS
                gSmoke_column[i].time -= fmaxf(SMOKE_COLUMN_NEW_PUFF_INTERVAL, pTime);
#else
                gSmoke_column[i].time -= pTime;
#endif
                gSmoke_column[i].count++;
                BrVector3Cross(&v, &c->omega, &car_pos);
                BrMatrix34ApplyV(&car_pos, &v, &c->car_master_actor->t.t.mat);
                BrVector3Add(&v, &c->v, &car_pos);
                v.v[1] = v.v[1] + 2.898550724637681f; // 100 / 34.5 ?
                pos.v[0] = SRandomBetween(-0.03f, 0.03f) + gSmoke_column[i].pos.v[0];
                pos.v[1] = (gSmoke_column[i].colour == 0) * 0.05f + gSmoke_column[i].pos.v[1];
                pos.v[2] = SRandomBetween(-0.03f, 0.03f) + gSmoke_column[i].pos.v[2];
                if ((gSmoke_column[i].whiter & 2) == 0 || IRandomBetween(0, 3)) {
                    if (gSmoke_column[i].whiter < 1) {
                        gSmoke_column[i].whiter = -1;
                    } else {
                        gSmoke_column[i].whiter = 2;
                    }
                } else {
                    gSmoke_column[i].whiter &= 1;
                }
                decay_factor = ((gSmoke_column[i].whiter > 0) + 1.0f) / 2.0f;
                if (gSmoke_column[i].lifetime < 4000) {
                    decay_factor = gSmoke_column[i].lifetime * decay_factor / 4000.0f;
                }
                CreatePuffOfSmoke(&pos, &v, decay_factor, decay_factor, gSmoke_column[i].colour + 16, c);
            }
        } else {
            if (gSmoke_column[i].car != NULL) {
                StartPipingSession(ePipe_chunk_smoke_column);
                AddSmokeColumnToPipingSession(i, gSmoke_column[i].car, gSmoke_column[i].vertex_index, gSmoke_column[i].colour);
                EndPipingSession();
            }
            gColumn_flags &= ~(1u << i);
            if (gSmoke_column[i].colour == 0) {
                BrActorRemove(gSmoke_column[i].flame_actor);
            }
            if (gSmoke_column[i].car != NULL) {
                if (gSmoke_column[i].car->num_smoke_columns != 0) {
                    gSmoke_column[i].car->num_smoke_columns--;
                }
            }
        }
    }
}

// IDA: void __cdecl DisposeFlame()
void DisposeFlame() {
    int i;
    int j;
    br_actor* actor;
    br_material* material;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gFlame_map); i++) {
        BrMapRemove(gFlame_map[i]);
        BrPixelmapFree(gFlame_map[i]);
    }

    for (i = 0; i < MAX_SMOKE_COLUMNS; i++) {
        if ((gColumn_flags & (1u << i)) && (gSmoke_column[i].colour == 0)) {
            BrActorRemove(gSmoke_column[i].flame_actor);
        }
        actor = gSmoke_column[i].flame_actor->children;
        for (j = 0; j < COUNT_OF(gSmoke_column[0].frame_count); j++) {
            BrMaterialRemove(actor->material);
            BrMaterialFree(actor->material);
            actor = actor->next;
        }
        BrActorFree(gSmoke_column[i].flame_actor);
    }
    BrModelRemove(gLollipop_model);
    BrModelFree(gLollipop_model);
}

// IDA: void __cdecl InitFlame()
void InitFlame() {
    int i;
    int j;
    int num;
    char the_path[256];
    br_actor* actor;
    br_material* material;
    LOG_TRACE("()");

    gColumn_flags = 0;
    gLollipop_model = BrModelAllocate("Lollipop", 4, 2);
    PathCat(the_path, gApplication_path, "PIXELMAP");
    PathCat(the_path, the_path, "FLAMES.PIX");
    num = DRPixelmapLoadMany(the_path, gFlame_map, COUNT_OF(gFlame_map));
    if (num != COUNT_OF(gFlame_map)) {
        FatalError(kFatalError_LoadPixelmapFile_S, the_path);
    }
    BrMapAddMany(gFlame_map, COUNT_OF(gFlame_map));
    for (i = 0; i < MAX_SMOKE_COLUMNS; i++) {
        gSmoke_column[i].flame_actor = BrActorAllocate(BR_ACTOR_NONE, NULL);
        for (j = 0; j < COUNT_OF(gSmoke_column[0].frame_count); j++) {
            actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
            material = BrMaterialAllocate(NULL);
            BrActorAdd(gSmoke_column[i].flame_actor, actor);
            actor->model = gLollipop_model;
            actor->material = material;
            material->flags &= ~BR_MATF_LIGHT;
            material->flags |= BR_MATF_ALWAYS_VISIBLE;
            material->colour_map = gFlame_map[0];
            BrMaterialAdd(material);
            gSmoke_column[i].frame_count[j] = 100;
        }
    }
    gLollipop_model->nvertices = 4;
    BrVector3SetFloat(&gLollipop_model->vertices[0].p, -.5f, 0.f, .0f);
    BrVector3SetFloat(&gLollipop_model->vertices[1].p, .5f, 0.f, .0f);
    BrVector3SetFloat(&gLollipop_model->vertices[2].p, .5f, 1.f, .0f);
    BrVector3SetFloat(&gLollipop_model->vertices[3].p, -.5f, 1.f, .0f);
    gLollipop_model->vertices[0].map.v[0] = 0.f;
    gLollipop_model->vertices[0].map.v[1] = 1.f;
    gLollipop_model->vertices[1].map.v[0] = 1.f;
    gLollipop_model->vertices[1].map.v[1] = 1.f;
    gLollipop_model->vertices[2].map.v[0] = 1.f;
    gLollipop_model->vertices[2].map.v[1] = 0.f;
    gLollipop_model->vertices[3].map.v[0] = 0.f;
    gLollipop_model->vertices[3].map.v[1] = 0.f;

    gLollipop_model->nfaces = 2;
    gLollipop_model->faces[0].vertices[0] = 0;
    gLollipop_model->faces[0].vertices[1] = 1;
    gLollipop_model->faces[0].vertices[2] = 2;
    gLollipop_model->faces[1].vertices[0] = 0;
    gLollipop_model->faces[1].vertices[1] = 2;
    gLollipop_model->faces[1].vertices[2] = 3;
    gLollipop_model->faces[0].smoothing = 1;
    gLollipop_model->faces[1].smoothing = 1;
    BrModelAdd(gLollipop_model);
}

// IDA: void __usercall InitSplash(FILE *pF@<EAX>)
void InitSplash(FILE* pF) {
    int i;
    int num_files;
    int num;
    br_actor* actor;
    char the_path[256];
    char s[256];
    br_pixelmap* splash_maps[20];
    LOG_TRACE("(%p)", pF);

    gSplash_flags = 0;
    gSplash_model = BrModelAllocate("Splash", 4, 2);
    if (pF != NULL) {
        num = GetAnInt(pF);
        gNum_splash_types = 0;
        for (i = 0; num > i; ++i) {
            GetAString(pF, s);
            PathCat(the_path, gApplication_path, "PIXELMAP");
            PathCat(the_path, the_path, s);
            num_files = DRPixelmapLoadMany(the_path, &splash_maps[gNum_splash_types], 20 - gNum_splash_types);
            if (num_files == 0) {
                FatalError(kFatalError_LoadPixelmapFile_S, the_path);
            }
            gNum_splash_types += num_files;
        }
    } else {
        PathCat(the_path, gApplication_path, "PIXELMAP");
        PathCat(the_path, the_path, "SPLSHBLU.PIX");
        gNum_splash_types = DRPixelmapLoadMany(the_path, splash_maps, 0x14u);
    }
    BrMapAddMany(splash_maps, gNum_splash_types);
    for (i = 0; i < gNum_splash_types; ++i) {
        gSplash_material[i] = BrMaterialAllocate(0);
        gSplash_material[i]->flags &= ~(BR_MATF_LIGHT | BR_MATF_PRELIT);
        gSplash_material[i]->flags |= BR_MATF_ALWAYS_VISIBLE | BR_MATF_PERSPECTIVE;
        gSplash_material[i]->index_blend = LoadSingleShadeTable(&gTrack_storage_space, "BLEND50.TAB");
        gSplash_material[i]->colour_map = splash_maps[i];
        BrMaterialAdd(gSplash_material[i]);
    }
    gSplash_model->nvertices = 4;
    BrVector3SetFloat(&gSplash_model->vertices[0].p, -0.5f, 0.0f, 0.0f);
    BrVector3SetFloat(&gSplash_model->vertices[1].p, 0.5f, 0.0f, 0.0f);
    BrVector3SetFloat(&gSplash_model->vertices[2].p, 0.5f, 1.0f, 0.0f);
    BrVector3SetFloat(&gSplash_model->vertices[3].p, -0.5f, 1.0f, 0.0f);
    gSplash_model->vertices[0].map.v[0] = 0.0f;
    gSplash_model->vertices[0].map.v[1] = 1.0f;
    gSplash_model->vertices[1].map.v[0] = 1.0f;
    gSplash_model->vertices[1].map.v[1] = 1.0f;
    gSplash_model->vertices[2].map.v[0] = 1.0f;
    gSplash_model->vertices[2].map.v[1] = 0.0f;
    gSplash_model->vertices[3].map.v[0] = 0.0f;
    gSplash_model->vertices[3].map.v[1] = 0.0f;
    gSplash_model->nfaces = 2;
    gSplash_model->faces[0].vertices[0] = 0;
    gSplash_model->faces[0].vertices[1] = 1;
    gSplash_model->faces[0].vertices[2] = 2;
    gSplash_model->faces[1].vertices[0] = 0;
    gSplash_model->faces[1].vertices[1] = 2;
    gSplash_model->faces[1].vertices[2] = 3;
    gSplash_model->faces[0].smoothing = 1;
    gSplash_model->faces[1].smoothing = 1;
    BrModelAdd(gSplash_model);
    for (i = 0; i < COUNT_OF(gSplash); i++) {
        gSplash[i].actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        actor = gSplash[i].actor;
        actor->model = gSplash_model;
        if (gNum_splash_types != 0) {
            actor->material = gSplash_material[IRandomBetween(0, gNum_splash_types - 1)];
        } else {
            actor->material = NULL;
        }
        gSplash[i].scale_x = SRandomBetween(0.9f, 1.1f) * (2 * IRandomBetween(0, 1) - 1);
    }
}

// IDA: void __cdecl DisposeSplash()
void DisposeSplash() {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < gNum_splash_types; i++) {
        BrMapRemove(gSplash_material[i]->colour_map);
        BrPixelmapFree(gSplash_material[i]->colour_map);
        BrMaterialRemove(gSplash_material[i]);
        BrMaterialFree(gSplash_material[i]);
    }
    for (i = 0; i < COUNT_OF(gSplash); i++) {
        if (gSplash_flags & (1u << i)) {
            BrActorRemove(gSplash[i].actor);
        }
        BrActorFree(gSplash[i].actor);
    }
    BrModelRemove(gSplash_model);
    BrModelFree(gSplash_model);
}

// IDA: void __usercall DrawTheGlow(br_pixelmap *pRender_screen@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>)
void DrawTheGlow(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera) {
    int i;
    br_scalar strength;
    br_vector3 tv;
    tU32 seed;
    LOG_TRACE("(%p, %p, %p)", pRender_screen, pDepth_buffer, pCamera);

    if (gColumn_flags) {
        seed = rand();
        srand(GetTotalTime());
        for (i = 0; i < MAX_SMOKE_COLUMNS; i++) {
            if (((1u << i) & gColumn_flags) != 0 && gSmoke_column[i].colour <= 1) {
                strength = 0.5f;
                if (gSmoke_column[i].lifetime < 4000) {
                    strength = gSmoke_column[i].lifetime * 0.5f / 4000.f;
                }
                BrVector3Set(&tv, gSmoke_column[i].pos.v[0], gSmoke_column[i].pos.v[1] + 0.02f, gSmoke_column[i].pos.v[2]);
                SmokeCircle3D(&tv, 0.07f, strength, SRandomBetween(0.5f, 0.99f), pRender_screen, pDepth_buffer, gAcid_shade_table, pCamera);
            }
        }
        srand(seed);
    }
}

// IDA: void __usercall PipeInstantUnSmudge(tCar_spec *pCar@<EAX>)
void PipeInstantUnSmudge(tCar_spec* pCar) {
    br_model* model;
    br_model* b_model;
    br_actor* actor;
    br_actor* bonny;
    int j;
    int n;
    int v;
    int group;
    tSmudged_vertex data[1000];
    LOG_TRACE("(%p)", pCar);

    if (!gAction_replay_mode) {
        return;
    }
    actor = pCar->car_model_actors[pCar->principal_car_actor].actor;
    model = actor->model;
    bonny = pCar->car_model_actors[pCar->car_actor_count - 1].actor;
    n = 0;
    if ((model->flags & BR_MODF_KEEP_ORIGINAL) != 0 || (model->flags & BR_MODF_UPDATEABLE) != 0) {
        StartPipingSession(ePipe_chunk_smudge);
        j = 0;
        for (group = 0; group < V11MODEL(model)->ngroups; group++) {
            for (v = 0; v < V11MODEL(model)->groups[group].nvertices; v++) {
                if ((V11MODEL(model)->groups[group].vertex_colours[v] >> 24) != 0) {
                    data[n].vertex_index = j;
                    data[n].light_index = -(V11MODEL(model)->groups[group].vertex_colours[v] >> 24);
                    n += 1;
                    V11MODEL(model)->groups[group].vertex_colours[v] = 0;
                    if ((model->flags & 0x80) != 0) {
                        model->vertices[V11MODEL(model)->groups[group].vertex_user[v]].index = 0;
                    }
                    if (n >= COUNT_OF(data)) {
                        group = V11MODEL(model)->ngroups;
                        break;
                    }
                }
                j += 1;
            }
        }
        if (n != 0) {
            AddSmudgeToPipingSession(pCar->car_ID, pCar->principal_car_actor, n, data);
        }
        if (bonny != actor) {
            b_model = bonny->model;
            n = 0;
            j = 0;
            for (group = 0; group < V11MODEL(model)->ngroups; group++) {
                for (v = 0; v < V11MODEL(model)->groups[group].nvertices; v++) {
                    if ((V11MODEL(model)->groups[group].vertex_colours[v] >> 24) != 0) {
                        data[n].vertex_index = j;
                        data[n].light_index = -V11MODEL(model)->groups[group].nvertices;
                        n += 1;
                        V11MODEL(model)->groups[group].vertex_colours[v] = 0;
                        if ((b_model->flags & BR_MODF_UPDATEABLE) != 0) {
                            b_model->vertices[V11MODEL(model)->groups[group].vertex_user[v]].index = 0;
                        }
                        if (n >= COUNT_OF(data)) {
                            group = V11MODEL(model)->groups[group].nvertices;
                            break;
                        }
                    }
                    j += 1;
                }
            }
            if (n != 0) {
                AddSmudgeToPipingSession(pCar->car_ID, pCar->car_actor_count - 1, n, data);
            }
        }
        EndPipingSession();
    }
}

// IDA: void __usercall SmudgeCar(tCar_spec *pCar@<EAX>, int fire_point@<EDX>)
void SmudgeCar(tCar_spec* pCar, int fire_point) {
    int v;
    int j;
    int real_vertex_number;
    br_model* model;
    br_model* b_model;
    br_actor* actor;
    br_actor* bonny;
    br_scalar ts;
    br_vector3 tv;
    br_vector3 bonny_pos;
    int group;
    br_vector3 point;
    tSmudged_vertex data[30];
    int n;
    LOG_TRACE("(%p, %d)", pCar, fire_point);

    if (gAusterity_mode) {
        return;
    }
    actor = pCar->car_model_actors[pCar->principal_car_actor].actor;
    model = actor->model;
    bonny = pCar->car_model_actors[pCar->car_actor_count - 1].actor;
    n = 0;
    j = 0;
    if ((model->flags & BR_MODF_KEEP_ORIGINAL) != 0 || (model->flags & BR_MODF_UPDATEABLE) != 0) {
        BrVector3Copy(&bonny_pos, &V11MODEL(model)->groups->vertices[fire_point].p);
        StartPipingSession(ePipe_chunk_smudge);
        for (group = 0; group < V11MODEL(model)->ngroups; group++) {
            for (v = 0; v < V11MODEL(model)->groups[group].nvertices; v++) {
                BrVector3Sub(&tv, &V11MODEL(model)->groups[group].vertices[v].p, &bonny_pos);
                ts = (.0144f - BrVector3LengthSquared(&tv) / SRandomBetween(.5f, 1.f)) / .0144f * 127.f;
                if (ts > 0.f) {
                    ts += V11MODEL(model)->groups[group].vertex_colours[v] >> 24;
                    if (ts > 255.f) {
                        ts = 255.f;
                    }
                    real_vertex_number = ts;
                    if (V11MODEL(model)->groups[group].vertex_colours[v] >> 24 != real_vertex_number) {
                        data[n].vertex_index = j;
                        data[n].light_index = real_vertex_number - (V11MODEL(model)->groups[group].vertex_colours[v] >> 24);
                        V11MODEL(model)->groups[group].vertex_colours[v] = real_vertex_number << 24;
                        if ((model->flags & BR_MODF_UPDATEABLE) != 0) {
                            model->vertices[V11MODEL(model)->groups[group].vertex_user[v]].index = real_vertex_number;
                        }
                        n += 1;
                        if (n >= COUNT_OF(data)) {
                            break;
                        }
                    }
                }
                j = j + 1;
            }
            if (n >= COUNT_OF(data)) {
                break;
            }
        }
        if (n != 0) {
            AddSmudgeToPipingSession(pCar->car_ID, pCar->principal_car_actor, n, data);
        }
        n = 0;
        j = 0;
        if (actor != bonny) {
            b_model = bonny->model;
            BrVector3Add(&tv, &actor->t.t.translate.t, &bonny_pos);
            BrVector3Sub(&tv, &tv, &bonny->t.t.translate.t);
            BrMatrix34TApplyV(&point, &tv, &bonny->t.t.mat);
            for (group = 0; group < V11MODEL(b_model)->ngroups; group++) {
                for (v = 0; v < V11MODEL(b_model)->groups[group].nvertices; v++) {
                    BrVector3Sub(&tv, &V11MODEL(b_model)->groups[group].vertices[v].p, &point);
                    ts = (.0144f - BrVector3LengthSquared(&tv)) / SRandomBetween(.5f, 1.f) / .0144f * 127.f;
                    if (ts > 0.f) {
                        ts += V11MODEL(b_model)->groups[group].vertex_colours[v] >> 24;
                        if (ts > 255.f) {
                            ts = 255.f;
                        }
                        real_vertex_number = ts;
                        if (V11MODEL(b_model)->groups[group].vertex_colours[v] >> 24 != real_vertex_number) {
                            data[n].vertex_index = j;
                            data[n].light_index = real_vertex_number - (V11MODEL(b_model)->groups[group].vertex_colours[v] >> 24);
                            V11MODEL(b_model)->groups[group].vertex_colours[v] = real_vertex_number << 24;
                            if ((b_model->flags & BR_MODF_UPDATEABLE) != 0) {
                                b_model->vertices[V11MODEL(b_model)->groups[group].vertex_user[v]].index = real_vertex_number;
                            }
                            n += 1;
                            if (n > COUNT_OF(data)) {
                                break;
                            }
                        }
                    }
                    j += 1;
                }
                if (n >= COUNT_OF(data)) {
                    break;
                }
            }
            if (n != 0) {
                AddSmudgeToPipingSession(pCar->car_ID, pCar->car_actor_count - 1, n, data);
            }
        }
        EndPipingSession();
    }
}

// IDA: void __cdecl ResetSmokeColumns()
void ResetSmokeColumns() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetSmokeOn(int pSmoke_on@<EAX>)
void SetSmokeOn(int pSmoke_on) {
    LOG_TRACE("(%d)", pSmoke_on);

    gSmoke_on = pSmoke_on;
}

// IDA: void __usercall ReallySetSmokeOn(int pSmoke_on@<EAX>)
void ReallySetSmokeOn(int pSmoke_on) {
    LOG_TRACE("(%d)", pSmoke_on);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetSmoke(int pSmoke_on@<EAX>)
void SetSmoke(int pSmoke_on) {
    LOG_TRACE("(%d)", pSmoke_on);

    ReallySetSmokeOn(pSmoke_on);
    SetSmokeOn(pSmoke_on);
}

// IDA: int __cdecl GetSmokeOn()
int GetSmokeOn() {
    LOG_TRACE("()");

    return gSmoke_on;
}

// IDA: void __usercall StopCarSmoking(tCar_spec *pCar@<EAX>)
void StopCarSmoking(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);

    for (i = 0; i < MAX_SMOKE_COLUMNS; i++) {
        if (gSmoke_column[i].car == pCar && gSmoke_column[i].lifetime > 2000) {
            gSmoke_column[i].lifetime = 2000;
        }
    }
}

// IDA: void __usercall StopCarSmokingInstantly(tCar_spec *pCar@<EAX>)
void StopCarSmokingInstantly(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);

    for (i = 0; i < MAX_SMOKE_COLUMNS; i++) {
        if (gSmoke_column[i].car == pCar) {
            gSmoke_column[i].lifetime = 0;
        }
    }
}

// IDA: void __usercall ConditionalSmokeColumn(tCar_spec *pCar@<EAX>, int pDamage_index@<EDX>, int pColour@<EBX>)
void ConditionalSmokeColumn(tCar_spec* pCar, int pDamage_index, int pColour) {
    int i;
    LOG_TRACE("(%p, %d, %d)", pCar, pDamage_index, pColour);

    if (!pColour) {
        pColour = pCar->driver < eDriver_net_human;
    }
    if (pCar->num_smoke_columns != 0) {
        for (i = 0; i < MAX_SMOKE_COLUMNS; i++) {
            if (gSmoke_column[i].car == pCar) {
                if (((1u << i) & gColumn_flags) != 0 && gSmoke_column[i].colour <= pColour && gSmoke_column[i].lifetime) {
                    return;
                }
                gSmoke_column[i].lifetime = 2000;
            }
        }
    }
    CreateSmokeColumn(pCar, pColour, pCar->fire_vertex[pDamage_index], 10000000u);
}

// IDA: void __usercall SingleSplash(tCar_spec *pCar@<EAX>, br_vector3 *sp@<EDX>, br_vector3 *normal@<EBX>, tU32 pTime@<ECX>)
void SingleSplash(tCar_spec* pCar, br_vector3* sp, br_vector3* normal, tU32 pTime) {
    br_matrix34* mat;
    br_matrix34* c_mat;
    br_vector3 tv;
    br_vector3 vel;
    br_scalar size;
    br_scalar speed;
    br_scalar ts;
    LOG_TRACE("(%p, %p, %p, %d)", pCar, sp, normal, pTime);

    mat = &gSplash[gNext_splash].actor->t.t.mat;
    c_mat = &pCar->car_master_actor->t.t.mat;
    BrMatrix34ApplyP(&gSplash[gNext_splash].actor->t.t.euler.t, sp, c_mat);
    tv.v[0] = sp->v[2] * pCar->omega.v[1] - pCar->omega.v[2] * sp->v[1];
    tv.v[1] = pCar->omega.v[2] * sp->v[0] - sp->v[2] * pCar->omega.v[0];
    tv.v[2] = sp->v[1] * pCar->omega.v[0] - pCar->omega.v[1] * sp->v[0];
    BrMatrix34ApplyV(&vel, &tv, c_mat);
    BrVector3Accumulate(&vel, &pCar->v);
    ts = BrVector3Length(&vel);
    size = (fabs(BrVector3Dot(normal, &vel)) * 5.0 + ts) / 150.0 + 0.047826085;
    if (size > 0.5) {
        size = 0.5;
    }
    if (BrVector3Dot(&pCar->velocity_car_space, sp) < 0.0) {
        size = size / 2.0;
    }

    gSplash[gNext_splash].size = SRandomBetween(size / 2.0, size);
    if (((1u << gNext_splash) & gSplash_flags) == 0) {
        BrActorAdd(gDont_render_actor, gSplash[gNext_splash].actor);
    }
    gSplash_flags |= 1u << gNext_splash;
    gSplash[gNext_splash].just_done = 1;
    if ((double)pTime * 0.003 > SRandomBetween(0.0, 1.0) && !gAction_replay_mode) {
        BrVector3InvScale(&vel, &vel, WORLD_SCALE);
        BrVector3Scale(&tv, &vel, 0.1f);
        speed = sqrt(ts / 70.0) * 15.0;
        if (speed > 15.0f) {
            speed = 15.0f;
        }
        tv.v[1] += SRandomBetween(5.0, speed) / WORLD_SCALE;
        BrMatrix34TApplyV(&vel, &tv, &pCar->car_master_actor->t.t.mat);

        BrVector3Cross(&tv, &vel, &pCar->water_normal);
        BrVector3Scale(&tv, &tv, 0.5f);
        if (BrVector3Dot(sp, &tv) <= 0.0) {
            BrVector3Sub(&vel, &vel, &tv);
        } else {
            BrVector3Accumulate(&vel, &tv);
        }
        CreateSingleSpark(pCar, sp, &vel);
    }
    gNext_splash++;
    if (gNext_splash >= COUNT_OF(gSplash)) {
        gNext_splash = 0;
    }
}

// IDA: void __usercall CreateSplash(tCar_spec *pCar@<EAX>, tU32 pTime@<EDX>)
void CreateSplash(tCar_spec* pCar, tU32 pTime) {
    br_vector3 normal_car_space;
    br_vector3 pos2;
    br_vector3 v_plane;
    br_vector3 p;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 cm;
    int i;
    int j;
    int mask;
    int axis1;
    int axis2;
    int axis3;
    br_bounds bnds;
    br_scalar min;
    br_scalar max;
    br_scalar d;
    br_scalar d2;
    br_scalar dist;
    br_scalar dist2;
    br_scalar ts;
    br_vector3 back_point[2];
    br_scalar back_val[2];
    LOG_TRACE("(%p, %d)", pCar, pTime);

    back_val[0] = 0.0;
    back_val[1] = 0.0;
#ifdef DETHRACE_FIX_BUGS
    BrVector3Set(&p, 0.f, 0.f, 0.f);
#endif
    if (pCar->v.v[2] * pCar->v.v[2] + pCar->v.v[1] * pCar->v.v[1] + pCar->v.v[0] * pCar->v.v[0] >= 1.0) {
        BrMatrix34TApplyV(&normal_car_space, &pCar->water_normal, &pCar->car_master_actor->t.t.mat);
        BrMatrix34ApplyP(&tv, &pCar->bounds[0].min, &pCar->car_master_actor->t.t.mat);
        min = BrVector3Dot(&pCar->water_normal, &tv) - pCar->water_d;
        max = min;
        for (i = 0; i < 3; ++i) {
            if (normal_car_space.v[i] <= 0.0) {
                max = (pCar->bounds[0].max.v[i] - pCar->bounds[0].min.v[i]) * normal_car_space.v[i] + max;
            } else {
                min = (pCar->bounds[0].max.v[i] - pCar->bounds[0].min.v[i]) * normal_car_space.v[i] + min;
            }
        }
        if (min * max <= 0.0) {
            BrVector3InvScale(&back_point[0], &pCar->bounds[1].min, WORLD_SCALE);
            BrVector3InvScale(&back_point[1], &pCar->bounds[1].max, WORLD_SCALE);
            back_point[0].v[1] = 0.01;
            ts = BrVector3Dot(&pCar->velocity_car_space, &normal_car_space);
            BrVector3Scale(&tv, &normal_car_space, ts);
            BrVector3Sub(&v_plane, &pCar->velocity_car_space, &tv);
            d = pCar->water_d
                - (pCar->car_master_actor->t.t.mat.m[3][1] * pCar->water_normal.v[1]
                    + pCar->car_master_actor->t.t.mat.m[3][2] * pCar->water_normal.v[2]
                    + pCar->car_master_actor->t.t.mat.m[3][0] * pCar->water_normal.v[0]);
            mask = IRandomBetween(0, 3);
            axis2 = 2;
            for (axis1 = 0; axis1 < 3; ++axis1) {
                axis3 = 3 - axis1 - axis2;
                for (j = 0; j < 4; ++j) {
                    i = j ^ mask;
                    if (((j ^ mask) & 1) != 0) {
                        tv2.v[axis3] = back_point[0].v[axis3];
                    } else {
                        tv2.v[axis3] = back_point[1].v[axis3];
                    }
                    if (((j ^ mask) & 1) != 0) {
                        tv2.v[axis2] = back_point[0].v[axis2];
                    } else {
                        tv2.v[axis2] = back_point[1].v[axis2];
                    }

                    ts = d - tv2.v[axis3] * normal_car_space.v[axis3] - tv2.v[axis2] * normal_car_space.v[axis2];
                    ts = ts / normal_car_space.v[axis1];
                    if (ts >= back_point[0].v[axis1] && back_point[1].v[axis1] >= ts) {
                        tv2.v[axis1] = ts;
                        ts = BrVector3Dot(&pCar->velocity_car_space, &tv2);
                        if (ts >= back_val[0]) {
                            if (back_val[1] <= ts) {
                                SingleSplash(pCar, &tv2, &normal_car_space, pTime);
                            } else {
                                if (back_val[1] < 0.0) {
                                    SingleSplash(pCar, &pos2, &normal_car_space, pTime);
                                }
                                back_val[1] = ts;
                                pos2 = tv2;
                            }
                        } else {
                            if (back_val[1] < 0.0) {
                                SingleSplash(pCar, &pos2, &normal_car_space, pTime);
                            }
                            back_val[1] = back_val[0];
                            back_val[0] = ts;
                            pos2 = p;
                            p = tv2;
                        }
                    }
                }
                axis2 = axis1;
            }
            if (back_val[1] >= 0.0) {
                if (back_val[0] < 0.0) {
                    SingleSplash(pCar, &p, &normal_car_space, pTime);
                }
            } else {
                tv.v[0] = pos2.v[0] - p.v[0];
                tv.v[1] = pos2.v[1] - p.v[1];
                tv.v[2] = pos2.v[2] - p.v[2];
                BrVector3Sub(&tv, &pos2, &p);
                ts = SRandomBetween(0.4, 0.6);
                BrVector3Scale(&tv2, &tv, ts);
                BrVector3Accumulate(&tv2, &p);
                ts = SRandomBetween(0.2, 0.3);
                BrVector3Scale(&cm, &tv, ts);
                BrVector3Accumulate(&p, &cm);
                ts = -SRandomBetween(0.2, 0.3);
                BrVector3Scale(&cm, &tv, ts);
                BrVector3Accumulate(&pos2, &cm);
                ts = BrVector3Dot(&pCar->velocity_car_space, &normal_car_space);
                BrVector3Scale(&tv, &normal_car_space, -ts);
                BrVector3Add(&v_plane, &pCar->velocity_car_space, &tv);
                BrVector3Normalise(&tv, &v_plane);
                BrVector3Scale(&tv, &tv, -0.028985508f);
                BrVector3Accumulate(&tv2, &tv);
                BrVector3Scale(&tv, &tv, 0.5f);
                BrVector3Accumulate(&p, &tv);
                BrVector3Accumulate(&pos2, &tv);
                SingleSplash(pCar, &tv2, &normal_car_space, pTime);
                SingleSplash(pCar, &p, &normal_car_space, pTime);
                SingleSplash(pCar, &pos2, &normal_car_space, pTime);
            }
            d = d * WORLD_SCALE;
            dist = d - BrVector3Dot(&pCar->cmpos, &normal_car_space);
            for (i = 0; pCar->extra_point_num > i; ++i) {
                dist2 = d
                    - (pCar->extra_points[i].v[1] * normal_car_space.v[1]
                        + pCar->extra_points[i].v[2] * normal_car_space.v[2]
                        + pCar->extra_points[i].v[0] * normal_car_space.v[0]);
                if ((dist > 0.0) != (dist2 > 0.0)) {
                    ts = dist / (dist - dist2);
                    BrVector3Sub(&tv, &pCar->extra_points[i], &pCar->cmpos);
                    BrVector3Scale(&tv, &tv, ts);
                    BrVector3Accumulate(&tv, &pCar->cmpos);
                    if (pCar->bounds[1].max.v[1] - 0.028985508 > tv.v[1]
                        || pCar->bounds[1].min.v[0] > tv.v[0]
                        || pCar->bounds[1].max.v[0] < tv.v[1]
                        || pCar->bounds[1].min.v[2] > tv.v[2]
                        || pCar->bounds[1].max.v[2] < tv.v[2]) {
                        BrVector3InvScale(&tv, &tv, WORLD_SCALE);
                        SingleSplash(pCar, &tv, &normal_car_space, pTime);
                    }
                }
            }
            for (i = 0; i < 4; ++i) {
                if ((i & 1) != 0) {
                    tv.v[0] = pCar->bounds[1].max.v[0];
                } else {
                    tv.v[0] = pCar->bounds[1].min.v[0];
                }
                tv.v[1] = pCar->bounds[1].max.v[1];
                tv.v[2] = pCar->wpos[i].v[2];
                dist = d - BrVector3Dot(&tv, &normal_car_space);
                dist2 = (pCar->bounds[1].max.v[1] - 0.01) * normal_car_space.v[1] + dist;
                if ((dist > 0.0) != (dist2 > 0.0)) {
                    ts = dist / (dist - dist2);
                    tv.v[1] -= (pCar->bounds[1].max.v[1] - 0.01) * ts;
                    BrVector3InvScale(&tv, &tv, WORLD_SCALE);
                    SingleSplash(pCar, &tv, &normal_car_space, pTime);
                }
            }
        } else {
            min = min + 1.0;
        }
    }
}

// IDA: void __usercall MungeSplash(tU32 pTime@<EAX>)
void MungeSplash(tU32 pTime) {
    int i;
    br_vector3 tv;
    br_scalar dt;
    br_scalar ts;
    tCar_spec* car;
    tVehicle_type type;
    LOG_TRACE("(%d)", pTime);

    if (gNum_splash_types == 0) {
        return;
    }
    if (!gAction_replay_mode || GetReplayRate() == 0.0) {
        if (!gAction_replay_mode) {
            for (i = 0; i < gNum_cars_and_non_cars; i++) {
                if (gActive_car_list[i]->water_d != 10000.0 && gActive_car_list[i]->driver != eDriver_local_human) {
                    CreateSplash(gActive_car_list[i], pTime);
                }
            }
            if (gProgram_state.current_car.water_d != 10000.0) {
                CreateSplash(&gProgram_state.current_car, 100);
            }
        }
    } else {
        for (type = eVehicle_net_player; type <= eVehicle_rozzer; type++) {
            for (i = 0;; i++) {
                if (i >= type ? GetCarCount(type) : 1) {
                    break;
                }
                if (type) {
                    car = GetCarSpec(type, i);
                } else {
                    car = &gProgram_state.current_car;
                }
                if (car->water_d != 10000.0 && car->driver != eDriver_local_human) {
                    CreateSplash(car, pTime);
                }
            }
        }
        if (gProgram_state.current_car.water_d != 10000.0) {
            CreateSplash(&gProgram_state.current_car, 0x64u);
        }
    }
    if (!gSplash_flags) {
        return;
    }
    for (i = 0; i < COUNT_OF(gSplash); i++) {
        if (((1u << i) & gSplash_flags) == 0) {
            continue;
        }
        if (gSplash[i].just_done || (gAction_replay_mode && GetReplayRate() == 0.0f)) {
            dt = gSplash[i].size * gSplash[i].scale_x;
            gSplash[i].actor->t.t.mat.m[0][0] = gCamera_to_world.m[0][0] * dt;
            gSplash[i].actor->t.t.mat.m[0][1] = gCamera_to_world.m[0][1] * dt;
            gSplash[i].actor->t.t.mat.m[0][2] = gCamera_to_world.m[0][2] * dt;
            gSplash[i].actor->t.t.mat.m[1][0] = gSplash[i].size * gCamera_to_world.m[1][0];
            gSplash[i].actor->t.t.mat.m[1][1] = gSplash[i].size * gCamera_to_world.m[1][1];
            gSplash[i].actor->t.t.mat.m[1][2] = gSplash[i].size * gCamera_to_world.m[1][2];
            gSplash[i].actor->t.t.mat.m[2][0] = gSplash[i].size * gCamera_to_world.m[2][0];
            gSplash[i].actor->t.t.mat.m[2][1] = gSplash[i].size * gCamera_to_world.m[2][1];
            gSplash[i].actor->t.t.mat.m[2][2] = gSplash[i].size * gCamera_to_world.m[2][2];
            if (gProgram_state.cockpit_on) {
                ts = sqrt(gCamera_to_world.m[0][2] * gCamera_to_world.m[0][2] + gCamera_to_world.m[0][0] * gCamera_to_world.m[0][0]);
                DRMatrix34PreRotateZ(&gSplash[i].actor->t.t.mat, -FastScalarArcTan2Angle(gCamera_to_world.m[0][1], ts));
            }
            gSplash[i].just_done = 0;
        } else {
            gSplash_flags &= ~(1u << i);
            BrActorRemove(gSplash[i].actor);
        }
    }
}

// IDA: void __cdecl RenderSplashes()
void RenderSplashes() {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gSplash); i++) {
        if (gSplash_flags & (1u << i)) {
            BrActorRelink(gNon_track_actor, gSplash[i].actor);
            BrZbSceneRenderAdd(gSplash[i].actor);
            BrActorRelink(gDont_render_actor, gSplash[i].actor);
        }
    }
}

// IDA: void __usercall GetSmokeShadeTables(FILE *f@<EAX>)
void GetSmokeShadeTables(FILE* f) {
    int i;
    int red;
    int green;
    int blue;
    br_scalar quarter;
    br_scalar half;
    br_scalar three_quarter;
    LOG_TRACE("(%p)", f);

    gNum_dust_tables = GetAnInt(f);
    if (gNum_dust_tables > 8) {
        gNum_dust_tables = 8;
    }
    for (i = 0; gNum_dust_tables > i; ++i) {
        PossibleService();
        GetThreeInts(f, &red, &green, &blue);
        GetThreeScalars(f, &quarter, &half, &three_quarter);
        gDust_table[i] = GenerateShadeTable(16, gRender_palette, red, green, blue, quarter, half, three_quarter);
    }
}

// IDA: void __cdecl FreeSmokeShadeTables()
void FreeSmokeShadeTables() {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < gNum_dust_tables; i++) {
        PossibleService();
        BrTableRemove(gDust_table[i]);
        BrPixelmapFree(gDust_table[i]);
    }
}

// IDA: void __usercall LoadInKevStuff(FILE *pF@<EAX>)
void LoadInKevStuff(FILE* pF) {
    LOG_TRACE("(%p)", pF);

    PossibleService();
    LoadInShrapnel();
    PossibleService();
    InitShrapnel();
    PossibleService();
    InitFlame();
    PossibleService();
    InitSplash(pF);
}

// IDA: void __cdecl DisposeKevStuff()
void DisposeKevStuff() {
    LOG_TRACE("()");

    DisposeShrapnel();
    DisposeFlame();
    DisposeSplash();
}

// IDA: void __usercall DisposeKevStuffCar(tCar_spec *pCar@<EAX>)
void DisposeKevStuffCar(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);

    for (i = 0; i < MAX_SMOKE_COLUMNS; i++) {
        if (gSmoke_column[i].car == pCar) {
            gSmoke_column[i].lifetime = 0;
            gSmoke_column[i].car = NULL;
        }
    }
    for (i = 0; i < COUNT_OF(gSparks); i++) {
        if ((gSpark_flags & (1u << i)) && gSparks[i].car == pCar) {
            gSparks[i].count = 0;
            gSpark_flags &= ~(1u << i);
        }
        if (gCar_to_view == pCar) {
            gCamera_yaw = 0;
            gCar_to_view = &gProgram_state.current_car;
            InitialiseExternalCamera();
            PositionExternalCamera(gCar_to_view, 200);
            gCar_to_view = &gProgram_state.current_car;
        }
    }
}

// IDA: void __cdecl DoTrueColModelThing(br_actor *actor, br_model *pModel, br_material *material, void *render_data, br_uint_8 style, int on_screen)
void DoTrueColModelThing(br_actor* actor, br_model* pModel, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    int group;
    int j;
    int val;
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, pModel, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoModelThing(br_actor *actor, br_model *pModel, br_material *material, void *render_data, br_uint_8 style, int on_screen)
void DoModelThing(br_actor* actor, br_model* pModel, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    int j;
    int i;
    int group;
    tU32 t;
    int val;
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, pModel, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetModelShade(br_actor *pActor@<EAX>, br_pixelmap *pShade@<EDX>)
void SetModelShade(br_actor* pActor, br_pixelmap* pShade) {
    int i;
    br_material* material;
    br_model* model;
    LOG_TRACE("(%p, %p)", pActor, pShade);

    model = pActor->model;
    if (pActor->material != NULL && pActor->material->index_shade != pShade) {
        pActor->material->index_shade = pShade;
        BrMaterialUpdate(pActor->material, BR_MATU_ALL);
    }
    for (i = 0; i < model->nfaces; i++) {
        material = model->faces[i].material;
        if (material != NULL && material->index_shade != pShade) {
            material->index_shade = pShade;
            BrMaterialUpdate(material, BR_MATU_ALL);
        }
    }
}

// IDA: void __usercall MakeCarIt(tCar_spec *pCar@<EAX>)
void MakeCarIt(tCar_spec* pCar) {
    br_actor* actor;
    br_actor* bonny;
    br_pixelmap* shade[6];
    static int shade_num = 0;
    int i;
    LOG_TRACE("(%p)", pCar);

    STUB();
    return;

    shade[0] = gIt_shade_table;
    shade[1] = gFog_shade_table;
    shade[2] = gShade_list[0];
    shade[3] = gShade_list[1];
    shade[4] = gShade_list[2];
    shade[5] = NULL;

    actor = pCar->car_model_actors[pCar->principal_car_actor].actor;
    bonny = pCar->car_model_actors[pCar->car_actor_count - 1].actor;
    if (((actor->model->flags & BR_MODF_CUSTOM) == 0) || actor->model->custom != DoTrueColModelThing) {
        SetModelShade(actor, shade[shade_num]);
        actor->model->user = DoTrueColModelThing;
        actor->model->custom = DoTrueColModelThing;
        actor->model->flags |= BR_MODF_CUSTOM;
        if (bonny != actor) {
            bonny->model->user = DoTrueColModelThing;
            bonny->model->custom = DoTrueColModelThing;
            bonny->model->flags |= BR_MODF_CUSTOM;
            SetModelShade(bonny, shade[shade_num]);
        }
    }
}

// IDA: void __usercall StopCarBeingIt(tCar_spec *pCar@<EAX>)
void StopCarBeingIt(tCar_spec* pCar) {
    int i;
    int group;
    br_actor* actor;
    br_actor* bonny;
    LOG_TRACE("(%p)", pCar);
    STUB();
}
