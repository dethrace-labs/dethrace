#ifndef _ENABLES_H_
#define _ENABLES_H_

#include "br_types.h"

// Offset: 12
// Size: 295
// EAX: e
// EDX: a
void actorEnable(br_v1db_enable* e, br_actor* a);

// Offset: 320
// Size: 191
// EAX: e
// EDX: a
void actorDisable(br_v1db_enable* e, br_actor* a);

// Offset: 525
// Size: 71
void BrLightEnable(br_actor* l);

// Offset: 611
// Size: 71
void BrLightDisable(br_actor* l);

// Offset: 700
// Size: 71
void BrClipPlaneEnable(br_actor* c);

// Offset: 790
// Size: 71
void BrClipPlaneDisable(br_actor* c);

// Offset: 882
// Size: 71
void BrHorizonPlaneEnable(br_actor* h);

// Offset: 975
// Size: 74
void BrHorizonPlaneDisable(br_actor* h);

// Offset: 1066
// Size: 55
br_actor* BrEnvironmentSet(br_actor* a);

// Offset: 1131
// Size: 284
// EAX: view_to_this
// EDX: this_to_view
// EBX: world_to_view
// ECX: w2vt
br_boolean setupView(br_matrix34* view_to_this, br_matrix34* this_to_view, br_matrix34* world_to_view, br_int_32 w2vt, br_actor* world, br_actor* a);

// Offset: 1429
// Size: 1051
// EAX: world
// EDX: world_to_view
// EBX: w2vt
void BrSetupLights(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt);

// Offset: 2498
// Size: 535
// EAX: world
// EDX: world_to_view
// EBX: w2vt
// ECX: view_to_screen
void BrSetupClipPlanes(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt, br_matrix4* view_to_screen);

// Offset: 3052
// Size: 293
// EAX: world
// EDX: world_to_view
// EBX: w2vt
void BrSetupEnvironment(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt);

// Offset: 3361
// Size: 39
// EAX: world
// EDX: world_to_view
// EBX: w2vt
void BrSetupHorizons(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt);

// Offset: 3419
// Size: 136
// EAX: a
void BrActorEnableCheck(br_actor* a);

#endif
