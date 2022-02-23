#include "spark.h"
#include "brender/brender.h"
#include "errors.h"
#include "globvars.h"
#include "graphics.h"
#include "harness/trace.h"
#include "loading.h"
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
int gOffset;
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

// IDA: void __cdecl DrawDot(br_scalar z, tU8 *scr_ptr, tU16 *depth_ptr, tU8 *shade_ptr)
void DrawDot(br_scalar z, tU8* scr_ptr, tU16* depth_ptr, tU8* shade_ptr) {
    LOG_TRACE("(%f, %p, %p, %p)", z, scr_ptr, depth_ptr, shade_ptr);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetWorldToScreen(br_pixelmap *pScreen@<EAX>)
void SetWorldToScreen(br_pixelmap* pScreen) {
    br_matrix4 mat;
    br_matrix4 mat2;
    LOG_TRACE("(%p)", pScreen);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    STUB_ONCE();
}

// IDA: void __usercall CreateSingleSpark(tCar_spec *pCar@<EAX>, br_vector3 *pPos@<EDX>, br_vector3 *pVel@<EBX>)
void CreateSingleSpark(tCar_spec* pCar, br_vector3* pPos, br_vector3* pVel) {
    LOG_TRACE("(%p, %p, %p)", pCar, pPos, pVel);
    NOT_IMPLEMENTED();
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
    STUB();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AdjustSpark(int pSpark_num@<EAX>, br_vector3 *pos@<EDX>, br_vector3 *length@<EBX>)
void AdjustSpark(int pSpark_num, br_vector3* pos, br_vector3* length) {
    br_vector3 tv;
    br_matrix34* mat;
    int i;
    LOG_TRACE("(%d, %p, %p)", pSpark_num, pos, length);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AdjustShrapnel(int pShrapnel_num@<EAX>, br_vector3 *pos@<EDX>, tU16 pAge@<EBX>, br_material *pMaterial@<ECX>)
void AdjustShrapnel(int pShrapnel_num, br_vector3* pos, tU16 pAge, br_material* pMaterial) {
    int i;
    LOG_TRACE("(%d, %p, %d, %p)", pShrapnel_num, pos, pAge, pMaterial);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResetSparks()
void ResetSparks() {
    LOG_TRACE("()");
    STUB();
}

// IDA: void __cdecl ResetShrapnel()
void ResetShrapnel() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitShrapnel()
void InitShrapnel() {
    int i;
    int j;
    LOG_TRACE("()");

    STUB();
}

// IDA: void __cdecl LoadInShrapnel()
void LoadInShrapnel() {
    LOG_TRACE("()");
    STUB();
}

// IDA: void __usercall KillShrapnel(int i@<EAX>)
void KillShrapnel(int i) {
    LOG_TRACE("(%d)", i);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposeShrapnel()
void DisposeShrapnel() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReplayShrapnel(tU32 pTime@<EAX>)
void ReplayShrapnel(tU32 pTime) {
    int i;
    br_matrix34* mat;
    LOG_TRACE("(%d)", pTime);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MungeShrapnel(tU32 pTime@<EAX>)
void MungeShrapnel(tU32 pTime) {
    br_vector3 disp;
    int i;
    br_matrix34* mat;
    br_scalar ts;
    LOG_TRACE("(%d)", pTime);
    STUB_ONCE();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SmokeLine(int l@<EAX>, int x@<EDX>, br_scalar zbuff, int r_squared, tU8 *scr_ptr, tU16 *depth_ptr, tU8 *shade_ptr, br_scalar r_multiplier, br_scalar z_multiplier, br_scalar shade_offset)
void SmokeLine(int l, int x, br_scalar zbuff, int r_squared, tU8* scr_ptr, tU16* depth_ptr, tU8* shade_ptr, br_scalar r_multiplier, br_scalar z_multiplier, br_scalar shade_offset) {
    int i;
    int r_multiplier_int;
    int shade_offset_int;
    tU16 z;
    LOG_TRACE("(%d, %d, %f, %d, %p, %p, %p, %f, %f, %f)", l, x, zbuff, r_squared, scr_ptr, depth_ptr, shade_ptr, r_multiplier, z_multiplier, shade_offset);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReplaySmoke(br_pixelmap *pRender_screen@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>)
void ReplaySmoke(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera) {
    br_scalar aspect;
    int i;
    LOG_TRACE("(%p, %p, %p)", pRender_screen, pDepth_buffer, pCamera);
    NOT_IMPLEMENTED();
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

    STUB_ONCE();
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
    STUB_ONCE();
}

// IDA: void __usercall CreatePuffOfSmoke(br_vector3 *pos@<EAX>, br_vector3 *v@<EDX>, br_scalar strength, br_scalar pDecay_factor, int pType, tCar_spec *pC)
void CreatePuffOfSmoke(br_vector3* pos, br_vector3* v, br_scalar strength, br_scalar pDecay_factor, int pType, tCar_spec* pC) {
    br_vector3 tv;
    int pipe_me;
    LOG_TRACE("(%p, %p, %f, %f, %d, %p)", pos, v, strength, pDecay_factor, pType, pC);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResetSmoke()
void ResetSmoke() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AdjustSmoke(int pIndex@<EAX>, tU8 pType@<EDX>, br_vector3 *pPos@<EBX>, br_scalar pRadius, br_scalar pStrength)
void AdjustSmoke(int pIndex, tU8 pType, br_vector3* pPos, br_scalar pRadius, br_scalar pStrength) {
    LOG_TRACE("(%d, %d, %p, %f, %f)", pIndex, pType, pPos, pRadius, pStrength);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ActorError()
void ActorError() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AdjustSmokeColumn(int pIndex@<EAX>, tCar_spec *pCar@<EDX>, int pVertex@<EBX>, int pColour@<ECX>)
void AdjustSmokeColumn(int pIndex, tCar_spec* pCar, int pVertex, int pColour) {
    int i;
    br_actor* actor;
    LOG_TRACE("(%d, %p, %d, %d)", pIndex, pCar, pVertex, pColour);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CreateSmokeColumn(tCar_spec *pCar@<EAX>, int pColour@<EDX>, int pVertex_index@<EBX>, tU32 pLifetime@<ECX>)
void CreateSmokeColumn(tCar_spec* pCar, int pColour, int pVertex_index, tU32 pLifetime) {
    int i;
    br_actor* actor;
    tSmoke_column* col;
    LOG_TRACE("(%p, %d, %d, %d)", pCar, pColour, pVertex_index, pLifetime);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GenerateSmokeShades()
void GenerateSmokeShades() {
    static int rb;
    static int gb;
    static int bb;
    static int rd;
    static int gd;
    static int bd;
    static int rg;
    static int gg;
    static int bg;
    LOG_TRACE("()");
    STUB();
}

// IDA: void __cdecl GenerateItFoxShadeTable()
void GenerateItFoxShadeTable() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AdjustFlame(int pIndex@<EAX>, int pFrame_count@<EDX>, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z)
void AdjustFlame(int pIndex, int pFrame_count, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z) {
    int i;
    int j;
    tSmoke_column* col;
    br_actor* actor;
    LOG_TRACE("(%d, %d, %f, %f, %f, %f)", pIndex, pFrame_count, pScale_x, pScale_y, pOffset_x, pOffset_z);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReplayFlame(tSmoke_column *col@<EAX>, br_actor *actor@<EDX>)
void ReplayFlame(tSmoke_column* col, br_actor* actor) {
    int i;
    LOG_TRACE("(%p, %p)", col, actor);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall FlameAnimate(int c@<EAX>, br_vector3 *pPos@<EDX>, tU32 pTime@<EBX>)
void FlameAnimate(int c, br_vector3* pPos, tU32 pTime) {
    tSmoke_column* col;
    br_actor* actor;
    int i;
    LOG_TRACE("(%d, %p, %d)", c, pPos, pTime);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoSmokeColumn(int i@<EAX>, tU32 pTime@<EDX>, br_vector3 *pRet_car_pos@<EBX>)
void DoSmokeColumn(int i, tU32 pTime, br_vector3* pRet_car_pos) {
    tCar_spec* c;
    br_actor* actor;
    br_actor* bonny;
    int group;
    LOG_TRACE("(%d, %d, %p)", i, pTime, pRet_car_pos);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReplaySmokeColumn(tU32 pTime@<EAX>)
void ReplaySmokeColumn(tU32 pTime) {
    int i;
    br_vector3 dummy;
    LOG_TRACE("(%d)", pTime);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposeFlame()
void DisposeFlame() {
    int i;
    int j;
    br_actor* actor;
    br_material* material;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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

    STUB();
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
    if (pF) {
        num = GetAnInt(pF);
        gNum_splash_types = 0;
        for (i = 0; num > i; ++i) {
            GetAString(pF, s);
            PathCat(the_path, gApplication_path, "PIXELMAP");
            PathCat(the_path, the_path, s);
            num_files = DRPixelmapLoadMany(the_path, &splash_maps[gNum_splash_types], 20 - gNum_splash_types);
            if (!num_files) {
                FatalError(79, the_path);
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
        gSplash_material[i]->flags &= 0xFFFFFFFC;
        gSplash_material[i]->flags |= 0x820u;
        gSplash_material[i]->index_blend = LoadSingleShadeTable(&gTrack_storage_space, "BLEND50.TAB");
        gSplash_material[i]->colour_map = splash_maps[i];
        BrMaterialAdd(gSplash_material[i]);
    }
    gSplash_model->nvertices = 4;
    BrVector3SetFloat(&gSplash_model->vertices->p, -0.5, 0.0, 0.0);
    BrVector3SetFloat(&gSplash_model->vertices[1].p, 0.5, 0.0, 0.0);
    BrVector3SetFloat(&gSplash_model->vertices[2].p, 0.5, 1.0, 0.0);
    BrVector3SetFloat(&gSplash_model->vertices[3].p, -0.5, 1.0, 0.0);
    gSplash_model->vertices->map.v[0] = 0.0;
    gSplash_model->vertices->map.v[1] = 1.0;
    gSplash_model->vertices[1].map.v[0] = 1.0;
    gSplash_model->vertices[1].map.v[1] = 1.0;
    gSplash_model->vertices[2].map.v[0] = 1.0;
    gSplash_model->vertices[2].map.v[1] = 0.0;
    gSplash_model->vertices[3].map.v[0] = 0.0;
    gSplash_model->vertices[3].map.v[1] = 0.0;
    gSplash_model->nfaces = 2;
    gSplash_model->faces->vertices[0] = 0;
    gSplash_model->faces->vertices[1] = 1;
    gSplash_model->faces->vertices[2] = 2;
    gSplash_model->faces[1].vertices[0] = 0;
    gSplash_model->faces[1].vertices[1] = 2;
    gSplash_model->faces[1].vertices[2] = 3;
    gSplash_model->faces->smoothing = 1;
    gSplash_model->faces[1].smoothing = 1;
    BrModelAdd(gSplash_model);
    for (i = 0; i < 32; ++i) {
        gSplash[i].actor = BrActorAllocate(BR_ACTOR_MODEL, 0);
        actor = gSplash[i].actor;
        actor->model = gSplash_model;
        if (gNum_splash_types) {
            actor->material = gSplash_material[IRandomBetween(0, gNum_splash_types - 1)];
        } else {
            actor->material = 0;
        }
        gSplash[i].scale_x = SRandomBetween(0.89999998, 1.1) * (float)(2 * IRandomBetween(0, 1) - 1);
    }
}

// IDA: void __cdecl DisposeSplash()
void DisposeSplash() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawTheGlow(br_pixelmap *pRender_screen@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>)
void DrawTheGlow(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera) {
    int i;
    br_scalar strength;
    br_vector3 tv;
    tU32 seed;
    LOG_TRACE("(%p, %p, %p)", pRender_screen, pDepth_buffer, pCamera);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall StopCarSmokingInstantly(tCar_spec *pCar@<EAX>)
void StopCarSmokingInstantly(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ConditionalSmokeColumn(tCar_spec *pCar@<EAX>, int pDamage_index@<EDX>, int pColour@<EBX>)
void ConditionalSmokeColumn(tCar_spec* pCar, int pDamage_index, int pColour) {
    int i;
    LOG_TRACE("(%p, %d, %d)", pCar, pDamage_index, pColour);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RenderSplashes()
void RenderSplashes() {
    int i;
    LOG_TRACE("()");
    STUB_ONCE();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeKevStuffCar(tCar_spec *pCar@<EAX>)
void DisposeKevStuffCar(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MakeCarIt(tCar_spec *pCar@<EAX>)
void MakeCarIt(tCar_spec* pCar) {
    br_actor* actor;
    br_actor* bonny;
    br_pixelmap* shade[6];
    static int shade_num;
    int i;
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
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
