#ifndef _RENDER_H_
#define _RENDER_H_

#include "br_types.h"

// Offset: 16
// Size: 1005
void BrDbModelRender(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen, int use_custom);

// Offset: 1037
// Size: 100
br_uint_32 BrOnScreenCheck(br_bounds3* bounds);

// Offset: 1159
// Size: 261
// EAX: ap
// EDX: t
br_uint_16 prependActorTransform(br_actor* ap, br_uint_16 t);

// Offset: 1434
// Size: 196
// EAX: mat
// EDX: mat_t
// EBX: t
br_uint_16 prependMatrix(br_matrix34* mat, br_uint_16 mat_t, br_uint_16 t);

// Offset: 1642
// Size: 1009
// EAX: ap
// EDX: model
// EBX: material
// ECX: render_data
void actorRender(br_actor* ap, br_model* model, br_material* material, void* render_data, br_uint_8 style, br_uint_16 t);

// Offset: 2671
// Size: 522
// EAX: ap
// EDX: model
// EBX: material
// ECX: render_data
void actorRenderOnScreen(br_actor* ap, br_model* model, br_material* material, void* render_data, br_uint_8 style, br_uint_16 t);

// Offset: 3210
// Size: 198
// EAX: world
void sceneRenderWorld(br_actor* world);

// Offset: 3423
// Size: 522
// EAX: tree
void sceneRenderAdd(br_actor* tree);

// Offset: 3966
// Size: 800
void BrDbSceneRenderBegin(br_actor* world, br_actor* camera);

// Offset: 4794
// Size: 160
br_renderbounds_cbfn* BrDbSetRenderBoundsCallback(br_renderbounds_cbfn* new_cbfn);

// Offset: 4964
// Size: 344
// EAX: buffer
void SetOrigin(br_pixelmap* buffer);

// Offset: 5320
// Size: 233
// EAX: buffer
void SetViewport(br_pixelmap* buffer);

// Offset: 5574
// Size: 393
void BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer);

// Offset: 5986
// Size: 103
void BrZbSceneRenderAdd(br_actor* tree);

// Offset: 6108
// Size: 108
void BrZbSceneRenderEnd();

// Offset: 6232
// Size: 234
void BrZbSceneRender(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer);

// Offset: 6487
// Size: 507
void BrZsSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer);

// Offset: 7013
// Size: 103
void BrZsSceneRenderAdd(br_actor* tree);

// Offset: 7135
// Size: 189
void BrZsSceneRenderEnd();

// Offset: 7340
// Size: 199
void BrZsSceneRender(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer);

// Offset: 7564
// Size: 55
br_primitive_cbfn* BrZsPrimitiveCallbackSet(br_primitive_cbfn* new_cbfn);

// Offset: 7635
// Size: 159
void BrZbModelRender(br_actor* actor, br_model* model, br_material* material, br_uint_8 style, int on_screen, int use_custom);

// Offset: 7810
// Size: 192
void BrZsModelRender(br_actor* actor, br_model* model, br_material* material, br_order_table* order_table, br_uint_8 style, int on_screen, int use_custom);

// Offset: 8030
// Size: 111
br_renderbounds_cbfn* BrZbRenderBoundsCallbackSet(br_renderbounds_cbfn* new_cbfn);

// Offset: 8169
// Size: 111
br_renderbounds_cbfn* BrZsRenderBoundsCallbackSet(br_renderbounds_cbfn* new_cbfn);

#endif
