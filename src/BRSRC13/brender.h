
#ifndef BRENDER_H
#define BRENDER_H

#include "br_types.h"

extern struct br_font* BrFontFixed3x5;
extern struct br_font* BrFontProp4x6;
extern struct br_font* BrFontProp7x9;

void BrDevPaletteSetOld(br_pixelmap* pm);
void BrDevPaletteSetEntryOld(int i, br_colour colour);

void* BrResAllocate(void* vparent, br_size_t size, br_uint_8 res_class);
void BrResFree(void* vres);

void BrMemFree(void* block);
void* BrMemAllocate(br_size_t size, br_uint_8 type);
void* BrMemCalloc(int nelems, br_size_t size, br_uint_8 type);

br_uint_32 BrSwap32(br_uint_32 l);

br_actor* BrActorLoad(char* filename);
br_actor* BrActorAllocate(br_uint_8 type, void* type_data);
br_actor* BrActorAdd(br_actor* parent, br_actor* a);
void BrActorRelink(br_actor* parent, br_actor* a);
br_actor* BrActorRemove(br_actor* a);
void BrActorFree(br_actor* a);

br_pixelmap* BrPixelmapLoad(char* filename);
br_uint_32 BrPixelmapLoadMany(char* filename, br_pixelmap** pixelmaps, br_uint_16 num);
void BrPixelmapFree(br_pixelmap* src);
br_pixelmap* BrPixelmapMatch(br_pixelmap* src, br_uint_8 match_type);
br_pixelmap* BrPixelmapAllocateSub(br_pixelmap* src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h);
void BrPixelmapFill(br_pixelmap* dst, br_uint_32 colour);
void BrPixelmapRectangleCopy(br_pixelmap* dst, br_int_32 dx, br_int_32 dy, br_pixelmap* src, br_int_32 sx, br_int_32 sy, br_int_32 w, br_int_32 h);
void BrPixelmapLine(br_pixelmap* dst, br_int_32 x1, br_int_32 y1, br_int_32 x2, br_int_32 y2, br_uint_32 colour);
void BrPixelmapRectangleFill(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour);
br_pixelmap* BrPixelmapAllocate(br_uint_8 type, br_int_32 w, br_int_32 h, void* pixels, int flags);

br_pixelmap* BrMapAdd(br_pixelmap* pixelmap);
br_pixelmap* BrMapRemove(br_pixelmap* pixelmap);
br_pixelmap* BrMapFind(char* pattern);
br_uint_32 BrMapAddMany(br_pixelmap** items, int n);

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

void BrMatrix23Identity(br_matrix23* mat);

void BrMatrix34Identity(br_matrix34* mat);
br_scalar BrMatrix34Inverse(br_matrix34* B, br_matrix34* A);
void BrMatrix34ApplyP(br_vector3* A, br_vector3* B, br_matrix34* C);
void BrMatrix34Scale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz);
void BrMatrix34PostTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z);
void BrMatrix34Mul(br_matrix34* A, br_matrix34* B, br_matrix34* C);
void BrMatrix34Copy(br_matrix34* A, br_matrix34* B);

br_pixelmap* BrTableAdd(br_pixelmap* pixelmap);
br_pixelmap* BrTableFind(char* pattern);
br_pixelmap* BrTableRemove(br_pixelmap* pixelmap);
br_uint_32 BrTableAddMany(br_pixelmap** items, int n);

void BrFailure(const char* s, ...);
void BrFatal(const char* name, int line, const char* s, ...);

br_model* BrModelAllocate(char* name, int nvertices, int nfaces);
br_model* BrModelAdd(br_model* model);
br_model* BrModelRemove(br_model* model);
br_model* BrModelLoad(char* filename);
void BrModelFree(br_model* model);
void BrModelUpdate(br_model* model, br_uint_16 flags);
br_uint_32 BrModelLoadMany(char* filename, br_model** models, br_uint_16 num);

void BrZbBegin(br_uint_8 colour_type, br_uint_8 depth_type);

void BrClipPlaneDisable(br_actor* c);

void BrVector3SetFloat(br_vector3* v1, float f1, float f2, float f3);

#endif
