#ifndef _ACTSUPT_H_
#define _ACTSUPT_H_

#include "br_types.h"

br_uint_32 BrActorEnum(br_actor* parent, br_actor_enum_cbfn* callback, void* arg);

br_uint_32 BrActorSearchMany(br_actor* root, char* pattern, br_actor** actors, int max);

br_actor* BrActorSearch(br_actor* root, char* pattern);

void RenumberActor(br_actor* a, int d);

br_actor* BrActorAdd(br_actor* parent, br_actor* a);

br_actor* BrActorRemove(br_actor* a);

void BrActorRelink(br_actor* parent, br_actor* a);

br_actor* BrActorAllocate(br_uint_8 type, void* type_data);

void InternalActorFree(br_actor* a);

void BrActorFree(br_actor* a);

br_boolean ActorToRoot(br_actor* a, br_actor* world, br_matrix34* m);

br_boolean ActorToRootTyped(br_actor* a, br_actor* world, br_matrix34* m, br_int_32* type);

void Matrix4PerspectiveNew(br_matrix4* mat, br_angle field_of_view, br_scalar aspect, br_scalar hither, br_scalar yon, br_scalar origin_x, br_scalar origin_y);

br_token CameraToScreenMatrix4(br_matrix4* mat, br_actor* camera);

br_uint_16 BrActorToActorMatrix34(br_matrix34* m, br_actor* a, br_actor* b);

void BrActorToScreenMatrix4(br_matrix4* m, br_actor* a, br_actor* camera);

void BrMatrix34ApplyBounds(br_bounds* d, br_bounds* s, br_matrix34* m);

void ActorToBounds(br_bounds* dest, br_actor* ap, br_model* model);

br_bounds* BrActorToBounds(br_bounds* b, br_actor* ap);

#endif
