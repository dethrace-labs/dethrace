#ifndef _PREPMESH_H_
#define _PREPMESH_H_

#include "br_types.h"

// Offset: 8
// Size: 246
// EAX: first
// EDX: last
int addEdge(br_uint_16 first, br_uint_16 last);

// Offset: 267
// Size: 461
// EAX: group
// EDX: model
void prepareEdges(v11group* group, br_model* model);

// Offset: 743
// Size: 115
// EAX: model
void BrPrepareEdges(br_model* model);

// Offset: 871
// Size: 102
int FacesCompare(void* p1, void* p2);

// Offset: 987
// Size: 268
int TVCompare_XYZ(void* p1, void* p2);

// Offset: 1273
// Size: 610
int TVCompare_MXYZUVN(void* p1, void* p2);

// Offset: 1897
// Size: 383
int TVCompare_MVN(void* p1, void* p2);

// Offset: 2304
// Size: 88
br_fraction BrScalarToFractionClamp(br_scalar s);

// Offset: 2411
// Size: 284
// EAX: model
void PrepareFaceNormals(br_model* model);

// Offset: 2705
// Size: 296
// EAX: model
void Smoothing(br_model* model, br_scalar crease_limit, prep_vertex** start, prep_vertex** end);

// Offset: 3018
// Size: 500
// EAX: model
void SmoothingCreased(br_model* model, br_scalar crease_limit, prep_vertex** start, prep_vertex** end);

// Offset: 3529
// Size: 479
// EAX: group
// EDX: v
// EBX: src
// ECX: model
void CopyVertex(v11group* group, int v, prep_vertex* src, br_model* model);

// Offset: 4017
// Size: 293
// EAX: group
// EDX: f
// EBX: src
// ECX: model
void CopyFace(v11group* group, int f, br_face* src, br_model* model);

// Offset: 4324
// Size: 2516
// EAX: model
void PrepareGroups(br_model* model);

// Offset: 6862
// Size: 170
// EAX: model
void PrepareBoundingRadius(br_model* model);

// Offset: 7051
// Size: 272
// EAX: model
void PrepareBoundingBox(br_model* model);

// Offset: 7345
// Size: 268
// EAX: v11m
void RegenerateFaceNormals(v11model* v11m);

// Offset: 7637
// Size: 762
// EAX: v11m
void RegenerateVertexNormals(v11model* v11m);

// Offset: 8413
// Size: 1585
void BrModelUpdate(br_model* model, br_uint_16 flags);

// Offset: 10011
// Size: 109
// EAX: model
void BrModelClear(br_model* model);

#endif
