#ifndef HARNESS_HOOKS_H
#define HARNESS_HOOKS_H

#include "brender/br_types.h"

#include <stdio.h>

void Harness_Init(int* argc, char* argv[]);

// Hooks are called from original game code.

// Dethrace hooks
int Harness_Hook_KeyDown(unsigned char pScan_code);
void Harness_Hook_PDServiceSystem();
void Harness_Hook_PDSetKeyArray();
// void Harness_Hook_MainGameLoop(); // limit FPS
void Harness_Hook_FlushRenderer(); // synchronize in-memory framebuffer and depthbuffer
void Harness_Hook_GraphicsInit(int render_width, int render_height);
void Harness_Hook_PDShutdownSystem();

// BRender hooks
void Harness_Hook_BrDevPaletteSetOld(br_pixelmap* pm);
void Harness_Hook_BrDevPaletteSetEntryOld(int i, br_colour colour);
void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src);
void Harness_Hook_BrV1dbRendererBegin(br_v1db_state* v1db);
void Harness_Hook_BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer);
void Harness_Hook_renderActor(br_actor* actor, br_model* model, br_material* material, br_token type);
void Harness_Hook_BrZbSceneRenderEnd();
void Harness_Hook_BrBufferUpdate(br_pixelmap* pm, br_token use, br_uint_16 flags);
void Harness_Hook_BrMaterialUpdate(br_material* mat, br_uint_16 flags);
void Harness_Hook_BrModelUpdate(br_model* model);

// Input hooks
void Harness_Hook_GetMousePosition(int* pX, int* pY);
void Harness_Hook_GetMouseButtons(int* pButton1, int* pButton2);

// Sound hooks
void Harness_Hook_S3Service(int unk1, int unk2);
void Harness_Hook_S3StopAllOutletSounds();

// Filesystem hooks
FILE* Harness_Hook_fopen(const char* pathname, const char* mode);

#endif
