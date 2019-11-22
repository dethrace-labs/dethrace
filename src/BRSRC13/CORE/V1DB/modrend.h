#ifndef _MODREND_H_
#define _MODREND_H_

#include "br_types.h"

// Offset: 12
// Size: 226
// EAX: actor
// EDX: model
// EBX: material
// ECX: render_data
void renderFaces(br_actor *actor, br_model *model, br_material *material, void *render_data, br_uint_8 style, int on_screen);

// Offset: 250
// Size: 143
// EAX: actor
// EDX: model
// EBX: material
// ECX: render_data
void renderEdges(br_actor *actor, br_model *model, br_material *material, void *render_data, br_uint_8 style, int on_screen);

// Offset: 406
// Size: 143
// EAX: actor
// EDX: model
// EBX: material
// ECX: render_data
void renderPoints(br_actor *actor, br_model *model, br_material *material, void *render_data, br_uint_8 style, int on_screen);

// Offset: 560
// Size: 42
// EAX: actor
// EDX: model
// EBX: material
// ECX: render_data
void nullRender(br_actor *actor, br_model *model, br_material *material, void *render_data, br_uint_8 style, int on_screen);

// Offset: 621
// Size: 467
// EAX: b
br_model* makeMeshFromBounds(br_bounds *b);

// Offset: 1112
// Size: 79
// EAX: actor
// EDX: model
// EBX: material
// ECX: render_data
void boundingBoxRenderPoints(br_actor *actor, br_model *model, br_material *material, void *render_data, br_uint_8 style, int on_screen);

// Offset: 1214
// Size: 79
// EAX: actor
// EDX: model
// EBX: material
// ECX: render_data
void boundingBoxRenderEdges(br_actor *actor, br_model *model, br_material *material, void *render_data, br_uint_8 style, int on_screen);

// Offset: 1316
// Size: 79
// EAX: actor
// EDX: model
// EBX: material
// ECX: render_data
void boundingBoxRenderFaces(br_actor *actor, br_model *model, br_material *material, void *render_data, br_uint_8 style, int on_screen);

#endif
