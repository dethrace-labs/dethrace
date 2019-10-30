#include "spark.h"

// Global variables
br_pixelmap *gFlame_map[20];
tBRender_smoke *gBR_smoke_pointers[30];
tSplash gSplash[32];
br_material *gSplash_material[20];
tBRender_smoke gBR_smoke_structs[30];
tSmoke_column gSmoke_column[25];
br_matrix4 gCameraToScreen;
tSpark gSparks[32];
br_model *gShrapnel_model[2];
br_pixelmap *gShade_list[16];
int gN_BR_smoke_structs;
br_pixelmap **gDust_table;
tSmoke gSmoke[25];
tU32 gSplash_flags;
int gSmoke_num;
int gNext_column;
int gColumn_flags;
tU32 gNext_splash;
int gOffset;
int gIt_type;
br_pixelmap *gBlack_smoke_shade_table;
br_pixelmap *gGrey_smoke_shade_table;
br_model *gLollipop_model;
int gNum_splash_types;
int gNum_dust_tables;
int gSmoke_on;
int gSmoke_flags;
br_model *gSplash_model;
int gDust_rotate;
br_pixelmap *gIt_shade_table;
br_pixelmap *gDark_smoke_shade_table;
int gShrapnel_flags;
int gSpark_flags;
int gNext_shrapnel;
br_camera *gSpark_cam;
int gNext_spark;
br_material *gBlack_material;
tShrapnel gShrapnel[15];

// Offset: 0
// Size: 119
void DrawDot(br_scalar z, tU8 *scr_ptr, tU16 *depth_ptr, tU8 *shade_ptr) {
}

// Offset: 120
// Size: 234
// EAX: pScreen
void SetWorldToScreen(br_pixelmap *pScreen) {
  br_matrix4 mat;
  br_matrix4 mat2;
}

// Offset: 356
// Size: 171
// EAX: pStart
// EDX: pEnd
void DrawLine3DThroughBRender(br_vector3 *pStart, br_vector3 *pEnd) {
}

// Offset: 528
// Size: 490
// EAX: start
// EDX: end
// EBX: pScreen
// ECX: pDepth_buffer
int DrawLine3D(br_vector3 *start, br_vector3 *end, br_pixelmap *pScreen, br_pixelmap *pDepth_buffer, br_pixelmap *shade_table) {
  br_vector3 o;
  br_vector3 p;
  br_vector3 tv;
  br_vector4 o2;
  br_vector4 p2;
  br_scalar ts;
}

// Offset: 1020
// Size: 2221
// EAX: o
// EDX: p
// EBX: pScreen
// ECX: pDepth_buffer
int DrawLine2D(br_vector3 *o, br_vector3 *p, br_pixelmap *pScreen, br_pixelmap *pDepth_buffer, br_scalar brightness, br_pixelmap *shade_table) {
  tU8 *scr_ptr;
  tU16 *depth_ptr;
  tU8 *shade_ptr;
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
}

// Offset: 3244
// Size: 209
// EAX: pCol
void SetLineModelCols(tU8 pCol) {
}

// Offset: 3456
// Size: 633
// EAX: pRender_screen
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pTime
void ReplaySparks(br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_actor *pCamera, tU32 pTime) {
  int i;
  br_vector3 pos;
  br_vector3 o;
  br_vector3 p;
  br_vector3 tv;
  br_vector3 new_pos;
}

// Offset: 4092
// Size: 2299
// EAX: pRender_screen
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void RenderSparks(br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_actor *pCamera, br_matrix34 *pCamera_to_world, tU32 pTime) {
  int i;
  int time;
  br_vector3 tv;
  br_vector3 o;
  br_vector3 p;
  br_vector3 pos;
  br_vector3 new_pos;
  br_scalar ts;
}

// Offset: 6392
// Size: 309
// EAX: pCar
// EDX: pPos
// EBX: pVel
void CreateSingleSpark(tCar_spec *pCar, br_vector3 *pPos, br_vector3 *pVel) {
}

// Offset: 6704
// Size: 1570
// EAX: pos
// EDX: v
// EBX: pForce
void CreateSparks(br_vector3 *pos, br_vector3 *v, br_vector3 *pForce, br_scalar sparkiness, tCar_spec *pCar) {
  br_vector3 norm;
  br_vector3 normal;
  br_vector3 tv;
  br_vector3 tv2;
  br_vector3 pos2;
  br_scalar ts;
  br_scalar ts2;
  int num;
  int i;
}

// Offset: 8276
// Size: 1009
// EAX: pos
// EDX: v
// EBX: pForce
// ECX: pCar1
void CreateSparkShower(br_vector3 *pos, br_vector3 *v, br_vector3 *pForce, tCar_spec *pCar1, tCar_spec *pCar2) {
  br_scalar ts;
  br_scalar ts2;
  int num;
  int i;
  tCar_spec *c;
  br_vector3 tv;
  br_vector3 tv2;
  br_vector3 normal;
}

// Offset: 9288
// Size: 305
// EAX: pSpark_num
// EDX: pos
// EBX: length
void AdjustSpark(int pSpark_num, br_vector3 *pos, br_vector3 *length, wchar_t tv, br_memory_classes mat) {
  int i;
}

// Offset: 9596
// Size: 206
// EAX: pShrapnel_num
// EDX: pos
// EBX: pAge
// ECX: pMaterial
void AdjustShrapnel(int pShrapnel_num, br_vector3 *pos, tU16 pAge, br_material *pMaterial, ldiv_t i, br_memory_classes __unk5__) {
}

// Offset: 9804
// Size: 44
void ResetSparks() {
}

// Offset: 9848
// Size: 114
void ResetShrapnel() {
}

// Offset: 9964
// Size: 854
// EAX: pos
// EDX: v
// EBX: pNormal
void CreateShrapnelShower(br_vector3 *pos, br_vector3 *v, br_vector3 *pNormal, br_scalar pForce, tCar_spec *c1, tCar_spec *c2) {
  br_scalar ts;
  br_scalar ts2;
  br_scalar rnd;
  int num;
  int i;
  tCar_spec *c;
  br_vector3 tv;
  br_vector3 tv2;
  br_vector3 vel;
}

// Offset: 10820
// Size: 57
br_scalar random() {
}

// Offset: 10880
// Size: 523
void InitShrapnel() {
  int j;
}

// Offset: 11404
// Size: 111
void LoadInShrapnel() {
}

// Offset: 11516
// Size: 73
// EAX: i
void KillShrapnel(int i) {
}

// Offset: 11592
// Size: 179
void DisposeShrapnel() {
}

// Offset: 11772
// Size: 249
// EAX: pTime
void ReplayShrapnel(tU32 pTime) {
  int i;
  br_matrix34 *mat;
}

// Offset: 12024
// Size: 958
// EAX: pTime
void MungeShrapnel(tU32 pTime) {
  br_vector3 disp;
  int i;
  br_matrix34 *mat;
  br_scalar ts;
}

// Offset: 12984
// Size: 295
// EAX: mat
// EDX: r
// EBX: a
void DrMatrix34Rotate(br_matrix34 *mat, br_angle r, br_vector3 *a) {
  br_scalar t;
  br_scalar s;
  br_scalar c;
  br_scalar txy;
  br_scalar txz;
  br_scalar tyz;
  br_scalar sx;
  br_scalar sy;
  br_scalar sz;
}

// Offset: 13280
// Size: 245
// EAX: l
// EDX: x
void SmokeLine(int l, int x, br_scalar zbuff, int r_squared, tU8 *scr_ptr, tU16 *depth_ptr, tU8 *shade_ptr, br_scalar r_multiplier, br_scalar z_multiplier, br_scalar shade_offset, void (i)(), br_memory_classes r_multiplier_int, unsigned short z, br_vector3 *__unk14__, br_scalar __unk15__, br_scalar __unk16__) {
}

// Offset: 13528
// Size: 2390
// EAX: o
void SmokeCircle(br_vector3 *o, br_scalar r, br_scalar extra_z, br_scalar strength, br_scalar pAspect, br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_pixelmap *pShade_table) {
  tU8 *scr_ptr;
  tU16 *depth_ptr;
  tU8 *shade_ptr;
  tU8 *osp;
  tU16 *odp;
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
  void (*line)(int, int, br_scalar, int, tU8*, tU16*, tU8*, br_scalar, br_scalar, br_scalar, void ()(), br_memory_classes, unsigned short, br_vector3*, br_scalar, br_scalar);
}

// Offset: 15920
// Size: 112
int CmpSmokeZ(void *p1, void *p2) {
  tBRender_smoke **a;
  tBRender_smoke **b;
}

// Offset: 16032
// Size: 506
void RenderRecordedSmokeCircles() {
  tBRender_smoke *smoke;
  tU8 red;
  tU8 grn;
  tU8 blu;
}

// Offset: 16540
// Size: 266
// EAX: pCent
void RecordSmokeCircle(br_vector3 *pCent, br_scalar pR, br_scalar pStrength, br_pixelmap *pShade, br_scalar pAspect) {
  tU8 shade_index;
  br_colour shade_rgb;
}

// Offset: 16808
// Size: 506
// EAX: o
void SmokeCircle3D(br_vector3 *o, br_scalar r, br_scalar strength, br_scalar pAspect, br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_pixelmap *pShade_table, br_actor *pCam) {
  br_vector3 tv;
  br_vector3 p;
  br_vector4 o2;
  br_camera *cam;
  int scaled_r;
  br_scalar extra_z;
}

// Offset: 17316
// Size: 319
// EAX: pRender_screen
// EDX: pDepth_buffer
// EBX: pCamera
void ReplaySmoke(br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_actor *pCamera) {
  br_scalar aspect;
  int i;
}

// Offset: 17636
// Size: 935
// EAX: pCar
// EDX: wheel
// EBX: pTime
void GenerateContinuousSmoke(tCar_spec *pCar, int wheel) {
  tU32 pTime;
  br_vector3 pos;
  br_vector3 v;
  br_vector3 vcs;
  br_vector3 tv;
  br_scalar decay_factor;
  br_scalar ts;
  br_scalar alpha;
  br_scalar beta;
  int colour;
}

// Offset: 18572
// Size: 91
void DustRotate() {
}

// Offset: 18664
// Size: 1813
// EAX: pRender_screen
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void RenderSmoke(br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_actor *pCamera, br_matrix34 *pCamera_to_world, tU32 pTime) {
  int i;
  int j;
  br_vector3 tv;
  br_scalar aspect;
  br_scalar ts;
  tU32 seed;
  tU32 not_lonely;
}

// Offset: 20480
// Size: 672
// EAX: pos
// EDX: v
void CreatePuffOfSmoke(br_vector3 *pos, br_vector3 *v, br_scalar strength, br_scalar pDecay_factor, int pType) {
  tCar_spec *pC;
  br_vector3 tv;
  int pipe_me;
}

// Offset: 21152
// Size: 44
void ResetSmoke() {
}

// Offset: 21196
// Size: 143
// EAX: pIndex
// EDX: pType
// EBX: pPos
void AdjustSmoke(int pIndex, tU8 pType, br_vector3 *pPos, br_scalar pRadius, br_scalar pStrength, int __unk5__, br_memory_classes __unk6__, char __unk8__) {
}

// Offset: 21340
// Size: 34
void ActorError() {
}

// Offset: 21376
// Size: 254
// EAX: pIndex
// EDX: pCar
// EBX: pVertex
// ECX: pColour
void AdjustSmokeColumn(int pIndex, tCar_spec *pCar, int pVertex, int pColour, unsigned int i) {
  br_actor *actor;
}

// Offset: 21632
// Size: 880
// EAX: pCar
// EDX: pColour
// EBX: pVertex_index
// ECX: pLifetime
void CreateSmokeColumn(tCar_spec *pCar, int pColour, int pVertex_index) {
  tU32 pLifetime;
  int i;
  br_actor *actor;
  tSmoke_column *col;
}

// Offset: 22512
// Size: 309
void GenerateSmokeShades() {
  int gb;
  int bb;
  int rd;
  int gd;
  int bd;
  int rg;
  int gg;
  int bg;
}

// Offset: 22824
// Size: 96
void GenerateItFoxShadeTable() {
}

// Offset: 22920
// Size: 153
// EAX: pIndex
// EDX: pFrame_count
void AdjustFlame(int pIndex, int pFrame_count, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z, unsigned short i, br_memory_classes j, union actor) {
}

// Offset: 23076
// Size: 373
// EAX: col
// EDX: actor
void ReplayFlame(tSmoke_column *col, br_actor *actor) {
  int i;
}

// Offset: 23452
// Size: 935
// EAX: c
// EDX: pPos
// EBX: pTime
void FlameAnimate(int c, br_vector3 *pPos, tU32 pTime, int col, br_memory_classes actor) {
  int i;
}

// Offset: 24388
// Size: 537
// EAX: i
// EDX: pTime
// EBX: pRet_car_pos
void DoSmokeColumn(int i, tU32 pTime, br_vector3 *pRet_car_pos, signed char c, _complex actor) {
  br_actor *bonny;
  int group;
}

// Offset: 24928
// Size: 132
// EAX: pTime
void ReplaySmokeColumn(tU32 pTime) {
  int i;
  br_vector3 dummy;
}

// Offset: 25060
// Size: 1157
// EAX: pTime
void MungeSmokeColumn(tU32 pTime) {
  int i;
  int plane;
  br_actor *actor;
  br_actor *bonny;
  br_vector3 car_pos;
  br_vector3 pos;
  br_vector3 v;
  br_vector3 up;
  br_vector3 start;
  br_vector3 end;
  br_scalar ts;
  br_scalar decay_factor;
  tCar_spec *c;
}

// Offset: 26220
// Size: 304
void DisposeFlame() {
  int j;
  br_actor *actor;
  br_material *material;
}

// Offset: 26524
// Size: 840
void InitFlame() {
  int j;
  int num;
  char the_path[256];
  br_actor *actor;
  br_material *material;
}

// Offset: 27364
// Size: 1197
// EAX: pF
void InitSplash(FILE *pF) {
  int i;
  int num_files;
  int num;
  br_actor *actor;
  char the_path[256];
  char s[256];
  br_pixelmap *splash_maps[20];
}

// Offset: 28564
// Size: 257
void DisposeSplash() {
  int i;
}

// Offset: 28824
// Size: 306
// EAX: pRender_screen
// EDX: pDepth_buffer
// EBX: pCamera
void DrawTheGlow(br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_actor *pCamera) {
  int i;
  br_scalar strength;
  br_vector3 tv;
  tU32 seed;
}

// Offset: 29132
// Size: 944
// EAX: pCar
void PipeInstantUnSmudge(tCar_spec *pCar) {
  br_model *model;
  br_model *b_model;
  br_actor *actor;
  br_actor *bonny;
  int j;
  int n;
  int v;
  int group;
  tSmudged_vertex data[1000];
}

// Offset: 30076
// Size: 1828
// EAX: pCar
// EDX: fire_point
void SmudgeCar(tCar_spec *pCar) {
  int fire_point;
  int v;
  int j;
  int real_vertex_number;
  br_model *model;
  br_model *b_model;
  br_actor *actor;
  br_actor *bonny;
  br_scalar ts;
  br_vector3 tv;
  br_vector3 bonny_pos;
  int group;
  br_vector3 point;
  tSmudged_vertex data[30];
  int n;
}

// Offset: 31904
// Size: 105
void ResetSmokeColumns() {
}

// Offset: 32012
// Size: 45
// EAX: pSmoke_on
void SetSmokeOn(int pSmoke_on) {
}

// Offset: 32060
// Size: 47
// EAX: pSmoke_on
void ReallySetSmokeOn(int pSmoke_on) {
}

// Offset: 32108
// Size: 53
// EAX: pSmoke_on
void SetSmoke(int pSmoke_on) {
}

// Offset: 32164
// Size: 45
int GetSmokeOn() {
}

// Offset: 32212
// Size: 107
// EAX: pCar
void StopCarSmoking(tCar_spec *pCar) {
  int i;
}

// Offset: 32320
// Size: 91
// EAX: pCar
void StopCarSmokingInstantly(tCar_spec *pCar) {
  int i;
}

// Offset: 32412
// Size: 245
// EAX: pCar
// EDX: pDamage_index
// EBX: pColour
void ConditionalSmokeColumn(tCar_spec *pCar, int pDamage_index) {
  int pColour;
  int i;
}

// Offset: 32660
// Size: 1065
// EAX: pCar
// EDX: sp
// EBX: normal
// ECX: pTime
void SingleSplash(tCar_spec *pCar, br_vector3 *sp, br_vector3 *normal, tU32 pTime) {
  br_matrix34 *mat;
  br_matrix34 *c_mat;
  br_vector3 tv;
  br_vector3 vel;
  br_scalar size;
  br_scalar speed;
  br_scalar ts;
}

// Offset: 33728
// Size: 3206
// EAX: pCar
// EDX: pTime
void CreateSplash(tCar_spec *pCar, tU32 pTime) {
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
}

// Offset: 36936
// Size: 977
// EAX: pTime
void MungeSplash(tU32 pTime) {
  int i;
  br_vector3 tv;
  br_scalar dt;
  br_scalar ts;
  tCar_spec *car;
  tVehicle_type type;
}

// Offset: 37916
// Size: 143
void RenderSplashes() {
}

// Offset: 38060
// Size: 207
// EAX: f
void GetSmokeShadeTables(FILE *f) {
  int i;
  int red;
  int green;
  int blue;
  br_scalar quarter;
  br_scalar half;
  br_scalar three_quarter;
}

// Offset: 38268
// Size: 117
void FreeSmokeShadeTables() {
  int i;
}

// Offset: 38388
// Size: 80
// EAX: pF
void LoadInKevStuff(FILE *pF) {
}

// Offset: 38468
// Size: 49
void DisposeKevStuff() {
}

// Offset: 38520
// Size: 259
// EAX: pCar
void DisposeKevStuffCar(tCar_spec *pCar) {
  int i;
}

// Offset: 38780
// Size: 829
void DoTrueColModelThing(br_actor *actor, br_model *pModel, br_material *material, void *render_data) {
  br_uint_8 style;
  int on_screen;
  int group;
  int j;
  int val;
}

// Offset: 39612
// Size: 856
void DoModelThing(br_actor *actor, br_model *pModel, br_material *material, void *render_data) {
  br_uint_8 style;
  int on_screen;
  int j;
  int i;
  int group;
  tU32 t;
  int val;
}

// Offset: 40468
// Size: 204
// EAX: pActor
// EDX: pShade
void SetModelShade(br_actor *pActor, br_pixelmap *pShade) {
  int i;
  br_material *material;
  br_model *model;
}

// Offset: 40672
// Size: 414
// EAX: pCar
void MakeCarIt(tCar_spec *pCar) {
  br_actor *actor;
  br_actor *bonny;
  br_pixelmap *shade[6];
  int shade_num;
  int i;
}

// Offset: 41088
// Size: 1093
// EAX: pCar
void StopCarBeingIt(tCar_spec *pCar) {
  int i;
  int group;
  br_actor *actor;
  br_actor *bonny;
}

