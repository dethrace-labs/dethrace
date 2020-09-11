#include "spark.h"
#include <stdlib.h>

br_pixelmap* gFlame_map[20];
tBRender_smoke* gBR_smoke_pointers[30];
tSplash gSplash[32];
br_material* gSplash_material[20];
tBRender_smoke gBR_smoke_structs[30];
tSmoke_column gSmoke_column[25];
br_matrix4 gCameraToScreen;
tSpark gSparks[32];
br_model* gShrapnel_model[2];
br_pixelmap* gShade_list[16];
int gN_BR_smoke_structs;
br_pixelmap** gDust_table;
tSmoke gSmoke[25];
tU32 gSplash_flags;
int gSmoke_num;
int gNext_column;
int gColumn_flags;
tU32 gNext_splash;
int gOffset;
int gIt_type;
br_pixelmap* gBlack_smoke_shade_table;
br_pixelmap* gGrey_smoke_shade_table;
br_model* gLollipop_model;
int gNum_splash_types;
int gNum_dust_tables;
int gSmoke_on = 1;
int gSmoke_flags;
br_model* gSplash_model;
int gDust_rotate;
br_pixelmap* gIt_shade_table;
br_pixelmap* gDark_smoke_shade_table;
int gShrapnel_flags;
int gSpark_flags;
int gNext_shrapnel;
br_camera* gSpark_cam;
int gNext_spark;
br_material* gBlack_material;
tShrapnel gShrapnel[15];

void DrawDot(br_scalar z, tU8* scr_ptr, tU16* depth_ptr, tU8* shade_ptr) {
    NOT_IMPLEMENTED();
}

void SetWorldToScreen(br_pixelmap* pScreen) {
    br_matrix4 mat;
    br_matrix4 mat2;
    NOT_IMPLEMENTED();
}

void DrawLine3DThroughBRender(br_vector3* pStart, br_vector3* pEnd) {
    NOT_IMPLEMENTED();
}

int DrawLine3D(br_vector3* start, br_vector3* end, br_pixelmap* pScreen, br_pixelmap* pDepth_buffer, br_pixelmap* shade_table) {
    br_vector3 o;
    br_vector3 p;
    br_vector3 tv;
    br_vector4 o2;
    br_vector4 p2;
    br_scalar ts;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void SetLineModelCols(tU8 pCol) {
    NOT_IMPLEMENTED();
}

void ReplaySparks(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera, tU32 pTime) {
    int i;
    br_vector3 pos;
    br_vector3 o;
    br_vector3 p;
    br_vector3 tv;
    br_vector3 new_pos;
    NOT_IMPLEMENTED();
}

void RenderSparks(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world, tU32 pTime) {
    int i;
    int time;
    br_vector3 tv;
    br_vector3 o;
    br_vector3 p;
    br_vector3 pos;
    br_vector3 new_pos;
    br_scalar ts;
    NOT_IMPLEMENTED();
}

void CreateSingleSpark(tCar_spec* pCar, br_vector3* pPos, br_vector3* pVel) {
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void CreateSparkShower(br_vector3* pos, br_vector3* v, br_vector3* pForce, tCar_spec* pCar1, tCar_spec* pCar2) {
    br_scalar ts;
    br_scalar ts2;
    int num;
    int i;
    tCar_spec* c;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 normal;
    NOT_IMPLEMENTED();
}

void AdjustSpark(int pSpark_num, br_vector3* pos, br_vector3* length) {
    br_vector3 tv;
    br_matrix34* mat;
    int i;
    NOT_IMPLEMENTED();
}

void AdjustShrapnel(int pShrapnel_num, br_vector3* pos, tU16 pAge, br_material* pMaterial) {
    int i;
    NOT_IMPLEMENTED();
}

void ResetSparks() {
    NOT_IMPLEMENTED();
}

void ResetShrapnel() {
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void InitShrapnel() {
    int j;
    NOT_IMPLEMENTED();
}

void LoadInShrapnel() {
    NOT_IMPLEMENTED();
}

void KillShrapnel(int i) {
    NOT_IMPLEMENTED();
}

void DisposeShrapnel() {
    NOT_IMPLEMENTED();
}

void ReplayShrapnel(tU32 pTime) {
    int i;
    br_matrix34* mat;
    NOT_IMPLEMENTED();
}

void MungeShrapnel(tU32 pTime) {
    br_vector3 disp;
    int i;
    br_matrix34* mat;
    br_scalar ts;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void SmokeLine(int l, int x, br_scalar zbuff, int r_squared, tU8* scr_ptr, tU16* depth_ptr, tU8* shade_ptr, br_scalar r_multiplier, br_scalar z_multiplier, br_scalar shade_offset) {
    int i;
    int r_multiplier_int;
    int shade_offset_int;
    tU16 z;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

int CmpSmokeZ(void* p1, void* p2) {
    tBRender_smoke** a;
    tBRender_smoke** b;
    NOT_IMPLEMENTED();
}

void RenderRecordedSmokeCircles() {
    tBRender_smoke* smoke;
    tU8 red;
    tU8 grn;
    tU8 blu;
    NOT_IMPLEMENTED();
}

void RecordSmokeCircle(br_vector3* pCent, br_scalar pR, br_scalar pStrength, br_pixelmap* pShade, br_scalar pAspect) {
    tU8 shade_index;
    br_colour shade_rgb;
    NOT_IMPLEMENTED();
}

void SmokeCircle3D(br_vector3* o, br_scalar r, br_scalar strength, br_scalar pAspect, br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_pixelmap* pShade_table, br_actor* pCam) {
    br_vector3 tv;
    br_vector3 p;
    br_vector4 o2;
    br_camera* cam;
    int scaled_r;
    br_scalar extra_z;
    NOT_IMPLEMENTED();
}

void ReplaySmoke(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera) {
    br_scalar aspect;
    int i;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void DustRotate() {
    NOT_IMPLEMENTED();
}

void RenderSmoke(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world, tU32 pTime) {
    int i;
    int j;
    br_vector3 tv;
    br_scalar aspect;
    br_scalar ts;
    tU32 seed;
    tU32 not_lonely;
    NOT_IMPLEMENTED();
}

void CreatePuffOfSmoke(br_vector3* pos, br_vector3* v, br_scalar strength, br_scalar pDecay_factor, int pType, tCar_spec* pC) {
    br_vector3 tv;
    int pipe_me;
    NOT_IMPLEMENTED();
}

void ResetSmoke() {
    NOT_IMPLEMENTED();
}

void AdjustSmoke(int pIndex, tU8 pType, br_vector3* pPos, br_scalar pRadius, br_scalar pStrength) {
    NOT_IMPLEMENTED();
}

void ActorError() {
    NOT_IMPLEMENTED();
}

void AdjustSmokeColumn(int pIndex, tCar_spec* pCar, int pVertex, int pColour) {
    int i;
    br_actor* actor;
    NOT_IMPLEMENTED();
}

void CreateSmokeColumn(tCar_spec* pCar, int pColour, int pVertex_index, tU32 pLifetime) {
    int i;
    br_actor* actor;
    tSmoke_column* col;
    NOT_IMPLEMENTED();
}

void GenerateSmokeShades() {
    static int gb;
    static int bb;
    static int rd;
    static int gd;
    static int bd;
    static int rg;
    static int gg;
    static int bg;
    NOT_IMPLEMENTED();
}

void GenerateItFoxShadeTable() {
    NOT_IMPLEMENTED();
}

void AdjustFlame(int pIndex, int pFrame_count, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z) {
    int i;
    int j;
    tSmoke_column* col;
    br_actor* actor;
    NOT_IMPLEMENTED();
}

void ReplayFlame(tSmoke_column* col, br_actor* actor) {
    int i;
    NOT_IMPLEMENTED();
}

void FlameAnimate(int c, br_vector3* pPos, tU32 pTime) {
    tSmoke_column* col;
    br_actor* actor;
    int i;
    NOT_IMPLEMENTED();
}

void DoSmokeColumn(int i, tU32 pTime, br_vector3* pRet_car_pos) {
    tCar_spec* c;
    br_actor* actor;
    br_actor* bonny;
    int group;
    NOT_IMPLEMENTED();
}

void ReplaySmokeColumn(tU32 pTime) {
    int i;
    br_vector3 dummy;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void DisposeFlame() {
    int j;
    br_actor* actor;
    br_material* material;
    NOT_IMPLEMENTED();
}

void InitFlame() {
    int j;
    int num;
    char the_path[256];
    br_actor* actor;
    br_material* material;
    NOT_IMPLEMENTED();
}

void InitSplash(FILE* pF) {
    int i;
    int num_files;
    int num;
    br_actor* actor;
    char the_path[256];
    char s[256];
    br_pixelmap* splash_maps[20];
    NOT_IMPLEMENTED();
}

void DisposeSplash() {
    int i;
    NOT_IMPLEMENTED();
}

void DrawTheGlow(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera) {
    int i;
    br_scalar strength;
    br_vector3 tv;
    tU32 seed;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void ResetSmokeColumns() {
    NOT_IMPLEMENTED();
}

void SetSmokeOn(int pSmoke_on) {
    LOG_TRACE("(%d)", pSmoke_on);
    gSmoke_on = pSmoke_on;
}

void ReallySetSmokeOn(int pSmoke_on) {
    NOT_IMPLEMENTED();
}

void SetSmoke(int pSmoke_on) {
    NOT_IMPLEMENTED();
}

int GetSmokeOn() {
    NOT_IMPLEMENTED();
}

void StopCarSmoking(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

void StopCarSmokingInstantly(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

void ConditionalSmokeColumn(tCar_spec* pCar, int pDamage_index, int pColour) {
    int i;
    NOT_IMPLEMENTED();
}

void SingleSplash(tCar_spec* pCar, br_vector3* sp, br_vector3* normal, tU32 pTime) {
    br_matrix34* mat;
    br_matrix34* c_mat;
    br_vector3 tv;
    br_vector3 vel;
    br_scalar size;
    br_scalar speed;
    br_scalar ts;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void MungeSplash(tU32 pTime) {
    int i;
    br_vector3 tv;
    br_scalar dt;
    br_scalar ts;
    tCar_spec* car;
    tVehicle_type type;
    NOT_IMPLEMENTED();
}

void RenderSplashes() {
    NOT_IMPLEMENTED();
}

void GetSmokeShadeTables(FILE* f) {
    int i;
    int red;
    int green;
    int blue;
    br_scalar quarter;
    br_scalar half;
    br_scalar three_quarter;
    NOT_IMPLEMENTED();
}

void FreeSmokeShadeTables() {
    int i;
    NOT_IMPLEMENTED();
}

void LoadInKevStuff(FILE* pF) {
    NOT_IMPLEMENTED();
}

void DisposeKevStuff() {
    NOT_IMPLEMENTED();
}

void DisposeKevStuffCar(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

void DoTrueColModelThing(br_actor* actor, br_model* pModel, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    int group;
    int j;
    int val;
    NOT_IMPLEMENTED();
}

void DoModelThing(br_actor* actor, br_model* pModel, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    int j;
    int i;
    int group;
    tU32 t;
    int val;
    NOT_IMPLEMENTED();
}

void SetModelShade(br_actor* pActor, br_pixelmap* pShade) {
    int i;
    br_material* material;
    br_model* model;
    NOT_IMPLEMENTED();
}

void MakeCarIt(tCar_spec* pCar) {
    br_actor* actor;
    br_actor* bonny;
    br_pixelmap* shade[6];
    static int shade_num;
    int i;
    NOT_IMPLEMENTED();
}

void StopCarBeingIt(tCar_spec* pCar) {
    int i;
    int group;
    br_actor* actor;
    br_actor* bonny;
    NOT_IMPLEMENTED();
}
