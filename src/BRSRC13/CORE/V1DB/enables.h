#ifndef _ENABLES_H_
#define _ENABLES_H_

#include "br_types.h"

void actorEnable(br_v1db_enable* e, br_actor* a);

void actorDisable(br_v1db_enable* e, br_actor* a);

void BrLightEnable(br_actor* l);

void BrLightDisable(br_actor* l);

void BrClipPlaneEnable(br_actor* c);

void BrClipPlaneDisable(br_actor* c);

void BrHorizonPlaneEnable(br_actor* h);

void BrHorizonPlaneDisable(br_actor* h);

br_actor* BrEnvironmentSet(br_actor* a);

br_boolean setupView(br_matrix34* view_to_this, br_matrix34* this_to_view, br_matrix34* world_to_view, br_int_32 w2vt, br_actor* world, br_actor* a);

void BrSetupLights(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt);

void BrSetupClipPlanes(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt, br_matrix4* view_to_screen);

void BrSetupEnvironment(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt);

void BrSetupHorizons(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt);

void BrActorEnableCheck(br_actor* a);

#endif
