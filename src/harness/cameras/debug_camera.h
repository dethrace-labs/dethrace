#ifndef HARNESS_DEBUG_CAMERA_H
#define HARNESS_DEBUG_CAMERA_H

#include <cglm/cglm.h>

extern int gDebugCamera_active;

void DebugCamera_Update();
float* DebugCamera_Projection();
float* DebugCamera_View();
void DebugCamera_SetPosition(float x, float y, float z);

#endif