#ifndef _RENDER_H_
#define _RENDER_H_

#include "brender/br_types.h"

void BrDbModelRender(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen, int use_custom);

br_uint_32 BrOnScreenCheck(br_bounds3* bounds);

br_uint_16 prependActorTransform(br_actor* ap, br_uint_16 t);

br_uint_16 prependMatrix(br_matrix34* mat, br_uint_16 mat_t, br_uint_16 t);

void actorRender(br_actor* ap, br_model* model, br_material* material, void* render_data, br_uint_8 style, br_uint_16 t);

void actorRenderOnScreen(br_actor* ap, br_model* model, br_material* material, void* render_data, br_uint_8 style, br_uint_16 t);

void sceneRenderWorld(br_actor* world);

void sceneRenderAdd(br_actor* tree);

void BrDbSceneRenderBegin(br_actor* world, br_actor* camera);

br_renderbounds_cbfn* BrDbSetRenderBoundsCallback(br_renderbounds_cbfn* new_cbfn);

void SetOrigin(br_pixelmap* buffer);

void SetViewport(br_pixelmap* buffer);

void BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer);

void BrZbSceneRenderAdd(br_actor* tree);

void BrZbSceneRenderEnd(void);

void BrZbSceneRender(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer);

void BrZsSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer);

void BrZsSceneRenderAdd(br_actor* tree);

void BrZsSceneRenderEnd(void);

void BrZsSceneRender(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer);

br_primitive_cbfn* BrZsPrimitiveCallbackSet(br_primitive_cbfn* new_cbfn);

void BrZbModelRender(br_actor* actor, br_model* model, br_material* material, br_uint_8 style, int on_screen, int use_custom);

void BrZsModelRender(br_actor* actor, br_model* model, br_material* material, br_order_table* order_table, br_uint_8 style, int on_screen, int use_custom);

br_renderbounds_cbfn* BrZbRenderBoundsCallbackSet(br_renderbounds_cbfn* new_cbfn);

br_renderbounds_cbfn* BrZsRenderBoundsCallbackSet(br_renderbounds_cbfn* new_cbfn);

#endif
