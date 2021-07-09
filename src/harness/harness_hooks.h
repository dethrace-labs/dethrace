#ifndef HARNESS_HOOKS_H
#define HARNESS_HOOKS_H

#include "br_types.h"

// Hooks are called from original game code.
int Harness_Hook_HandleCommandLineArg(char* arg);
void Harness_Hook_DOSGfxBegin();
void Harness_Hook_BrDevPaletteSetOld(br_pixelmap* pm);
void Harness_Hook_BrDevPaletteSetEntryOld(int i, br_colour colour);
void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src);
void Harness_Hook_BrV1dbRendererBegin();
void Harness_Hook_BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer);
void Harness_Hook_BrZbSceneRenderAdd(br_actor* tree);

void Harness_Hook_renderFaces(v11model* model, br_material* material, br_token type);
void Harness_Hook_BrZbSceneRenderEnd();

void Harness_Hook_KeyBegin();
int Harness_Hook_KeyDown(unsigned char pScan_code);

void Harness_Hook_PDServiceSystem(int pTime_since_last_call);

// Sound hooks
void Harness_Hook_S3Service(int unk1, int unk2);
void Harness_Hook_S3StopAllOutletSounds();

#endif