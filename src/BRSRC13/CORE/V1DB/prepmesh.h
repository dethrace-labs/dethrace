#ifndef _PREPMESH_H_
#define _PREPMESH_H_

#include "br_types.h"

int addEdge(br_uint_16 first, br_uint_16 last);

void prepareEdges(v11group* group, br_model* model);

void BrPrepareEdges(br_model* model);

int FacesCompare(void* p1, void* p2);

int TVCompare_XYZ(void* p1, void* p2);

int TVCompare_MXYZUVN(void* p1, void* p2);

int TVCompare_MVN(void* p1, void* p2);

br_fraction BrScalarToFractionClamp(br_scalar s);

void PrepareFaceNormals(br_model* model);

void Smoothing(br_model* model, br_scalar crease_limit, prep_vertex** start, prep_vertex** end);

void SmoothingCreased(br_model* model, br_scalar crease_limit, prep_vertex** start, prep_vertex** end);

void CopyVertex(v11group* group, int v, prep_vertex* src, br_model* model);

void CopyFace(v11group* group, int f, br_face* src, br_model* model);

void PrepareGroups(br_model* model);

void PrepareBoundingRadius(br_model* model);

void PrepareBoundingBox(br_model* model);

void RegenerateFaceNormals(v11model* v11m);

void RegenerateVertexNormals(v11model* v11m);

void BrModelUpdate(br_model* model, br_uint_16 flags);

void BrModelClear(br_model* model);

#endif
