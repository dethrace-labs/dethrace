#ifndef _MODREND_H_
#define _MODREND_H_

#include "brender/br_types.h"

typedef void (*render_style_cbfn)(br_actor* actor, br_model* model, br_material* material,
    void* render_data, br_uint_8 style, int on_screen);

extern render_style_cbfn RenderStyleCalls[8];

void renderFaces(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen);

void renderEdges(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen);

void renderPoints(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen);

void nullRender(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen);

br_model* makeMeshFromBounds(br_bounds* b);

void boundingBoxRenderPoints(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen);

void boundingBoxRenderEdges(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen);

void boundingBoxRenderFaces(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen);

#endif
