#ifndef HARNESS_HOOKS_H
#define HARNESS_HOOKS_H

#include "brender/br_types.h"

// Hooks are called from original game code.

// Dethrace hooks
void Harness_Init(int* argc, char* argv[]);
int Harness_Hook_KeyDown(unsigned char pScan_code);

// Poll windows message events (glfwPollEvents, SDL_PollEvents etc).
void PlatformHooks_PollEvents();

// BRender hooks
void Harness_Hook_DOSGfxBegin();
void Harness_Hook_BrDevPaletteSetOld(br_pixelmap* pm);
void Harness_Hook_BrDevPaletteSetEntryOld(int i, br_colour colour);
void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src);
void Harness_Hook_BrV1dbRendererBegin();
void Harness_Hook_BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer);
void Harness_Hook_BrZbSceneRenderAdd(br_actor* tree);
void Harness_Hook_renderFaces(br_model* model, br_material* material, br_token type);
void Harness_Hook_BrZbSceneRenderEnd();

// Sound hooks
void Harness_Hook_S3Service(int unk1, int unk2);
void Harness_Hook_S3StopAllOutletSounds();

#endif