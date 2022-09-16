
#ifndef BRENDER_H
#define BRENDER_H

#include "br_inline_funcs.h"
#include "br_types.h"

extern struct br_font* BrFontFixed3x5;
extern struct br_font* BrFontProp4x6;
extern struct br_font* BrFontProp7x9;

// BrActor
br_actor* BrActorLoad(char* filename);
br_uint_32 BrActorLoadMany(char* filename, br_actor** actors, br_uint_16 num);
br_uint_32 BrActorSave(char* filename, br_actor* ptr);
br_actor* BrActorAllocate(br_uint_8 type, void* type_data);
br_actor* BrActorAdd(br_actor* parent, br_actor* a);
void BrActorRelink(br_actor* parent, br_actor* a);
br_actor* BrActorRemove(br_actor* a);
void BrActorFree(br_actor* a);
br_uint_32 BrActorEnum(br_actor* parent, br_actor_enum_cbfn* callback, void* arg);
br_bounds* BrActorToBounds(br_bounds* b, br_actor* ap);
br_uint_16 BrActorToActorMatrix34(br_matrix34* m, br_actor* a, br_actor* b);

// BrAllocator
br_allocator* BrAllocatorSet(br_allocator* newal);

// BrClip
void BrClipPlaneEnable(br_actor* c);
void BrClipPlaneDisable(br_actor* c);

// BrDev
void BrDevPaletteSetOld(br_pixelmap* pm);
void BrDevPaletteSetEntryOld(int i, br_colour colour);

// BrEnvironment
br_actor* BrEnvironmentSet(br_actor* a);

// BrFileSystem
br_filesystem* BrFilesystemSet(br_filesystem* newfs);

// BrMap
br_pixelmap* BrMapAdd(br_pixelmap* pixelmap);
br_pixelmap* BrMapRemove(br_pixelmap* pixelmap);
br_pixelmap* BrMapFind(char* pattern);
br_uint_32 BrMapAddMany(br_pixelmap** items, int n);
br_map_find_cbfn* BrMapFindHook(br_map_find_cbfn* hook);

// BrMaterial
br_material* BrMaterialAllocate(char* name);
br_material* BrMaterialLoad(char* filename);
br_uint_32 BrMaterialLoadMany(char* filename, br_material** materials, br_uint_16 num);
br_material* BrMaterialAdd(br_material* material);
br_material* BrMaterialRemove(br_material* material);
void BrMaterialUpdate(br_material* mat, br_uint_16 flags);
br_material* BrMaterialFind(char* pattern);
void BrMaterialFree(br_material* m);
br_uint_32 BrMaterialAddMany(br_material** items, int n);
br_uint_32 BrMaterialEnum(char* pattern, br_material_enum_cbfn* callback, void* arg);
br_material_find_cbfn* BrMaterialFindHook(br_material_find_cbfn* hook);

// BrMatrix236
void BrMatrix23Copy(br_matrix23* A, br_matrix23* B);
void BrMatrix23Identity(br_matrix23* mat);
void BrMatrix23Mul(br_matrix23* A, br_matrix23* B, br_matrix23* C);

// BrMatrix34
void BrMatrix34Identity(br_matrix34* mat);
void BrMatrix34Pre(br_matrix34* mat, br_matrix34* A);
void BrMatrix34ApplyV(br_vector3* A, br_vector3* B, br_matrix34* C);
br_scalar BrMatrix34Inverse(br_matrix34* B, br_matrix34* A);
void BrMatrix34LPInverse(br_matrix34* A, br_matrix34* B);
void BrMatrix34ApplyP(br_vector3* A, br_vector3* B, br_matrix34* C);
void BrMatrix34Scale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz);
void BrMatrix34PostTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z);
void BrMatrix34Mul(br_matrix34* A, br_matrix34* B, br_matrix34* C);
void BrMatrix34Copy(br_matrix34* A, br_matrix34* B);
void BrMatrix34PreRotateY(br_matrix34* mat, br_angle ry);
void BrMatrix34RotateY(br_matrix34* mat, br_angle ry);
void BrMatrix34PostScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz);
void BrMatrix34PreScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz);
void BrMatrix34PreTransform(br_matrix34* mat, br_transform* xform);
void BrMatrix34PostTransform(br_matrix34* mat, br_transform* xform);
void BrMatrix34PreRotateX(br_matrix34* mat, br_angle rx);
void BrMatrix34TApplyV(br_vector3* A, br_vector3* B, br_matrix34* C);
void BrMatrix34LPNormalise(br_matrix34* A, br_matrix34* B);
void BrMatrix34PreRotate(br_matrix34* mat, br_angle r, br_vector3* axis);
void BrMatrix34Rotate(br_matrix34* mat, br_angle r, br_vector3* a);
void BrMatrix34PreTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z);
void BrMatrix34Post(br_matrix34* mat, br_matrix34* A);
void BrMatrix34PostShearX(br_matrix34* mat, br_scalar sy, br_scalar sz);
void BrMatrix34PostShearY(br_matrix34* mat, br_scalar sx, br_scalar sz);
void BrMatrix34PostShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy);
void BrMatrix34PreScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz);
void BrMatrix34PreShearX(br_matrix34* mat, br_scalar sy, br_scalar sz);
void BrMatrix34PreRotateY(br_matrix34* mat, br_angle ry);
void BrMatrix34PreRotateZ(br_matrix34* mat, br_angle rz);
void BrMatrix34RollingBall(br_matrix34* mat, int dx, int dy, int radius);

// BrMatrix4
void BrMatrix4Copy(br_matrix4* A, br_matrix4* B);
br_scalar BrMatrix4Inverse(br_matrix4* A, br_matrix4* B);
void BrMatrix4TApply(br_vector4* A, br_vector4* B, br_matrix4* C);
void BrMatrix4ApplyP(br_vector4* A, br_vector3* B, br_matrix4* C);
void BrMatrix4Perspective(br_matrix4* mat, br_angle field_of_view, br_scalar aspect, br_scalar hither, br_scalar yon);
void BrMatrix4Scale(br_matrix4* mat, br_scalar sx, br_scalar sy, br_scalar sz);
void BrMatrix4Mul(br_matrix4* A, br_matrix4* B, br_matrix4* C);

// BrMem
void BrMemFree(void* block);
void* BrMemAllocate(br_size_t size, br_uint_8 type);
void* BrMemCalloc(int nelems, br_size_t size, br_uint_8 type);

// BrModel
br_model* BrModelAllocate(char* name, int nvertices, int nfaces);
br_model* BrModelAdd(br_model* model);
br_uint_32 BrModelAddMany(br_model** items, int n);
br_model* BrModelRemove(br_model* model);
br_model* BrModelLoad(char* filename);
void BrModelFree(br_model* model);
void BrModelUpdate(br_model* model, br_uint_16 flags);
br_uint_32 BrModelLoadMany(char* filename, br_model** models, br_uint_16 num);
br_uint_32 BrModelSaveMany(char* filename, br_model** models, br_uint_16 num);
br_model_find_cbfn* BrModelFindHook(br_model_find_cbfn* hook);

// BrPixelmap
br_pixelmap* BrPixelmapLoad(char* filename);
br_uint_32 BrPixelmapLoadMany(char* filename, br_pixelmap** pixelmaps, br_uint_16 num);
br_uint_32 BrPixelmapSave(char* filename, br_pixelmap* ptr);
br_uint_32 BrPixelmapSaveMany(char* filename, br_pixelmap** pixelmaps, br_uint_16 num);
void BrPixelmapFree(br_pixelmap* src);
br_pixelmap* BrPixelmapMatch(br_pixelmap* src, br_uint_8 match_type);
br_pixelmap* BrPixelmapAllocateSub(br_pixelmap* src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h);
void BrPixelmapFill(br_pixelmap* dst, br_uint_32 colour);
void BrPixelmapRectangleCopy(br_pixelmap* dst, br_int_32 dx, br_int_32 dy, br_pixelmap* src, br_int_32 sx, br_int_32 sy, br_int_32 w, br_int_32 h);
void BrPixelmapCopy(br_pixelmap* dst, br_pixelmap* src);
void BrPixelmapLine(br_pixelmap* dst, br_int_32 x1, br_int_32 y1, br_int_32 x2, br_int_32 y2, br_uint_32 colour);
void BrPixelmapRectangleFill(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour);
void BrPixelmapPixelSet(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_32 colour);
br_pixelmap* BrPixelmapAllocate(br_uint_8 type, br_int_32 w, br_int_32 h, void* pixels, int flags);
void BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src);
void BrPixelmapText(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_32 colour, br_font* font, char* text);
void BrPixelmapTextF(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_32 colour, br_font* font, char* fmt, ...);
br_uint_16 BrPixelmapTextWidth(br_pixelmap* dst, br_font* font, char* text);

// BrRes
void* BrResAllocate(void* vparent, br_size_t size, br_uint_8 res_class);
br_resource_class* BrResClassAdd(br_resource_class* rclass);
void* BrResRemove(void* vres);
void BrResFree(void* vres);
br_uint_32 BrResCheck(void* vres, int no_tag);
br_uint_32 BrResSize(void* vres);
char* BrResStrDup(void* vparent, char* str);

// BrTable
br_pixelmap* BrTableAdd(br_pixelmap* pixelmap);
br_pixelmap* BrTableFind(char* pattern);
br_pixelmap* BrTableRemove(br_pixelmap* pixelmap);
br_uint_32 BrTableAddMany(br_pixelmap** items, int n);
void BrTableUpdate(br_pixelmap* table, br_uint_16 flags);
br_uint_32 BrTableEnum(char* pattern, br_table_enum_cbfn* callback, void* arg);
br_table_find_cbfn* BrTableFindHook(br_table_find_cbfn* hook);

// BrTransform
void BrTransformToMatrix34(br_matrix34* mat, br_transform* xform);

// BrV1db
void BrV1dbBeginWrapper_Float(void);

// BrVector3
// void BrVector3Cross(br_vector3* v1, br_vector3* v2, br_vector3* v3);
void BrVector3SetFloat(br_vector3* v1, float f1, float f2, float f3);
// void BrVector3Sub(br_vector3* v1, br_vector3* v2, br_vector3* v3);
// void BrVector3Accumulate(br_vector3* v1, br_vector3* v2);
// void BrVector3Normalise(br_vector3* v1, br_vector3* v2);

// BrTransform
void BrTransformToTransform(br_transform* dest, br_transform* src);

// Logging
void BrFailure(const char* s, ...);
void BrFatal(const char* name, int line, const char* s, ...);

// Z-buffer
void BrZbBegin(br_uint_8 colour_type, br_uint_8 depth_type);

void BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer);
void BrZbSceneRenderAdd(br_actor* tree);
void BrZbSceneRenderEnd(void);

void BrZbModelRender(br_actor* actor, br_model* model, br_material* material, br_uint_8 style, int on_screen, int use_custom);

br_pixelmap* DOSGfxBegin(char* setup_string);

// Various
br_uint_32 BrOnScreenCheck(br_bounds3* bounds);

int BrWriteModeSet(int mode);
br_uint_32 BrSwap32(br_uint_32 l);
br_diaghandler* BrDiagHandlerSet(br_diaghandler* newdh);

#endif
