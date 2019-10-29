#include "dr_types.h"
#include "br_types.h"
// Offset: 12
// Size: 158
br_uint_32 BrActorEnum(br_actor *parent, br_actor_enum_cbfn *callback);

// Offset: 188
// Size: 321
br_uint_32 BrActorSearchMany(br_actor *root, char *pattern, br_actor **actors);

// Offset: 523
// Size: 103
br_actor* BrActorSearch(br_actor *root);

// Offset: 640
// Size: 117
// EAX: a
// EDX: d
void RenumberActor(br_actor *a);

// Offset: 768
// Size: 230
br_actor* BrActorAdd(br_actor *parent, br_actor *a);

// Offset: 1012
// Size: 177
br_actor* BrActorRemove(br_actor *a);

// Offset: 1203
// Size: 167
void BrActorRelink(br_actor *parent, br_actor *a);

// Offset: 1386
// Size: 503
br_actor* BrActorAllocate(br_uint_8 type, void *type_data);

// Offset: 1907
// Size: 152
// EAX: a
void InternalActorFree(br_actor *a);

// Offset: 2071
// Size: 103
void BrActorFree(br_actor *a);

// Offset: 2186
// Size: 283
// EAX: a
// EDX: world
// EBX: m
br_boolean ActorToRoot(br_actor *a, br_actor *world, br_matrix34 *m);

// Offset: 2486
// Size: 339
// EAX: a
// EDX: world
// EBX: m
// ECX: type
br_boolean ActorToRootTyped(br_actor *a, br_actor *world, br_matrix34 *m, br_int_32 *type);

// Offset: 2847
// Size: 572
// EAX: mat
// EDX: field_of_view
void Matrix4PerspectiveNew(br_matrix4 *mat, br_angle field_of_view, br_scalar aspect, br_scalar hither);

// Offset: 3441
// Size: 944
// EAX: mat
// EDX: camera
br_token CameraToScreenMatrix4(br_matrix4 *mat, br_actor *camera);

// Offset: 4408
// Size: 866
br_uint_16 BrActorToActorMatrix34(br_matrix34 *m, br_actor *a, br_actor *b);

// Offset: 5297
// Size: 178
void BrActorToScreenMatrix4(br_matrix4 *m, br_actor *a, br_actor *camera);

// Offset: 5497
// Size: 409
// EAX: d
// EDX: s
// EBX: m
void BrMatrix34ApplyBounds(br_bounds *d, br_bounds *s, br_matrix34 *m);

// Offset: 5920
// Size: 397
// EAX: dest
// EDX: ap
// EBX: model
void ActorToBounds(br_bounds *dest, br_actor *ap, br_model *model);

// Offset: 6333
// Size: 310
br_bounds* BrActorToBounds(br_bounds *b, br_actor *ap);

