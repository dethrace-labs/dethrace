#include "init.h"

#include <stdlib.h>
#include <time.h>

#include "car.h"
#include "controls.h"
#include "depth.h"
#include "displays.h"
#include "drfile.h"
#include "drmem.h"
#include "errors.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrbm.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/trace.h"
#include "loading.h"
#include "loadsave.h"
#include "netgame.h"
#include "network.h"
#include "oil.h"
#include "opponent.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "piping.h"
#include "powerup.h"
#include "pratcam.h"
#include "raycast.h"
#include "replay.h"
#include "skidmark.h"
#include "sound.h"
#include "spark.h"
#include "trig.h"
#include "utility.h"
#include "world.h"

#include "brender.h"

// GLOBAL: CARM95 0x0052115c
int gGame_initialized;

// GLOBAL: CARM95 0x00521160
int gBr_initialized;

// GLOBAL: CARM95 0x00521164
int gBrZb_initialized;

// GLOBAL: CARM95 0x00521168
int gInitialisation_finished;

// GLOBAL: CARM95 0x0052116c
int gRender_indent;

// GLOBAL: CARM95 0x0053fdd0
tU32 gAustere_time;

// GLOBAL: CARM95 0x0054b2bc
int gInitial_rank;

// GLOBAL: CARM95 0x0054b2b0
int gCredits_per_rank[3];

// GLOBAL: CARM95 0x0054b2c0
int gInitial_credits[3];

// GLOBAL: CARM95 0x0054b2a0
int gNet_mode_of_last_game;

// GLOBAL: CARM95 0x0054b29c
br_material* gDefault_track_material;

// IDA: void __cdecl AllocateSelf()
// FUNCTION: CARM95 0x004bbebf
void AllocateSelf(void) {
    LOG_TRACE("()");

    gSelf = BrActorAllocate(BR_ACTOR_NONE, NULL);
    if (gSelf == NULL) {
        FatalError(kFatalError_AllocateSelf);
    }
    gSelf = BrActorAdd(gNon_track_actor, gSelf);
    if (gSelf == NULL) {
        FatalError(kFatalError_AllocateSelf);
    }
}

// IDA: void __cdecl AllocateCamera()
// FUNCTION: CARM95 0x004bbf22
void AllocateCamera(void) {
    br_camera* camera_ptr;
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gCamera_list); i++) {
        gCamera_list[i] = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
        if (gCamera_list[i] == NULL) {
            FatalError(kFatalError_AllocateCamera);
        }

        camera_ptr = gCamera_list[i]->type_data;
        camera_ptr->type = BR_CAMERA_PERSPECTIVE_FOV;
        camera_ptr->field_of_view = BrDegreeToAngle(gCamera_angle);
        camera_ptr->hither_z = gCamera_hither;
        camera_ptr->yon_z = gCamera_yon;
        camera_ptr->aspect = (double)gWidth / (double)gHeight;
    }

    gCamera_list[0] = BrActorAdd(gSelf, gCamera_list[0]);
    if (gCamera_list[0] == NULL) {
        FatalError(kFatalError_AllocateCamera);
    }
    gCamera_list[1] = BrActorAdd(gUniverse_actor, gCamera_list[1]);
    if (gCamera_list[1] == NULL) {
        FatalError(kFatalError_AllocateCamera);
    }
    gCamera = gCamera_list[0];
    gRearview_camera = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
    if (gRearview_camera == NULL) {
        FatalError(kFatalError_AllocateCamera);
    }

    gRearview_camera->t.t.mat.m[2][2] = -1.0f;
    camera_ptr = (br_camera*)gRearview_camera->type_data;
    camera_ptr->hither_z = gCamera_hither;
    camera_ptr->type = BR_CAMERA_PERSPECTIVE_FOV;
    camera_ptr->yon_z = gCamera_yon;
    camera_ptr->field_of_view = BrDegreeToAngle(gCamera_angle);
    camera_ptr->aspect = (double)gWidth / (double)gHeight;
    gRearview_camera = BrActorAdd(gSelf, gRearview_camera);
    if (gRearview_camera == NULL) {
        FatalError(kFatalError_AllocateCamera);
    }
    SetSightDistance(camera_ptr->yon_z);
}

// IDA: void __cdecl ReinitialiseForwardCamera()
// FUNCTION: CARM95 0x004bb510
void ReinitialiseForwardCamera(void) {
    br_camera* camera_ptr;
    float the_angle;
    float d;
    float w;
    LOG_TRACE("()");

    camera_ptr = (br_camera*)gCamera->type_data;
    if (gProgram_state.cockpit_on) {
        the_angle = gCamera_angle / 2.0;

        d = atan(
                tandeg(the_angle)
                * (double)gRender_screen->height
                / (double)(gProgram_state.current_car.render_bottom[0] - gProgram_state.current_car.render_top[0]))
            * 114.5915590261646;
        camera_ptr->field_of_view = BrDegreeToAngle(d);
        BrMatrix34Identity(&gCamera->t.t.mat);
        gCamera->t.t.mat.m[3][0] = gProgram_state.current_car.driver_x_offset;
        gCamera->t.t.mat.m[3][1] = gProgram_state.current_car.driver_y_offset;
        gCamera->t.t.mat.m[3][2] = gProgram_state.current_car.driver_z_offset;
        w = (float)(gRender_screen->base_y
            + (gRender_screen->height / 2)
            - (gProgram_state.current_car.render_bottom[0] + gProgram_state.current_car.render_top[0]) / 2);

        gCamera->t.t.mat.m[2][1] = tandeg(d / 2.0) * w * 2.0 / (float)gRender_screen->height;
        camera_ptr->aspect = (float)gWidth / gHeight;
        camera_ptr->yon_z = gYon_multiplier * gCamera_yon;
        if (gProgram_state.which_view == eView_left) {
            DRMatrix34PostRotateY(
                &gCamera->t.t.mat,
                BrDegreeToAngle(gProgram_state.current_car.head_left_angle));
        } else if (gProgram_state.which_view == eView_right) {
            DRMatrix34PostRotateY(
                &gCamera->t.t.mat,
                BrDegreeToAngle(gProgram_state.current_car.head_right_angle));
        }
        gCamera->t.t.mat.m[3][0] = gProgram_state.current_car.driver_x_offset;
        gCamera->t.t.mat.m[3][1] = gProgram_state.current_car.driver_y_offset;
        gCamera->t.t.mat.m[3][2] = gProgram_state.current_car.driver_z_offset;
        SetSightDistance(camera_ptr->yon_z);
        MungeForwardSky();
    }
    AssertYons();
}

// IDA: void __cdecl AllocateRearviewPixelmap()
// FUNCTION: CARM95 0x004bb741
void AllocateRearviewPixelmap(void) {
    char* rear_screen_pixels;
    LOG_TRACE("()");

#ifdef DETHRACE_3DFX_PATCH
    if (gRearview_screen != NULL) {
        BrPixelmapFree(gRearview_screen);
        gRearview_screen = NULL;
    }
    if (gProgram_state.mirror_on) {
        gRearview_screen = BrPixelmapAllocateSub(
            gBack_screen,
            gProgram_state.current_car.mirror_left,
            gProgram_state.current_car.mirror_top,
            gProgram_state.current_car.mirror_right - gProgram_state.current_car.mirror_left,
            gProgram_state.current_car.mirror_bottom - gProgram_state.current_car.mirror_top);
        gRearview_depth_buffer = gDepth_buffer;
        gRearview_screen->origin_x = gRearview_screen->width / 2;
        gRearview_screen->origin_y = gRearview_screen->height / 2;
    }
#else
    if (gRearview_screen) {
        BrMemFree(gRearview_screen->pixels);
        BrPixelmapFree(gRearview_screen);
        BrPixelmapFree(gRearview_depth_buffer);
        gRearview_screen = NULL;
    }
    if (gProgram_state.mirror_on) {
        rear_screen_pixels = BrMemAllocate(
            (gProgram_state.current_car.mirror_bottom - gProgram_state.current_car.mirror_top + 1)
                * (gProgram_state.current_car.mirror_right - gProgram_state.current_car.mirror_left + 4)
                * gGraf_specs[gGraf_spec_index].depth_bytes,
            kMem_rear_screen_pixels);
        if (gScreen->row_bytes < 0) {
            BrFatal("..\\..\\source\\common\\init.c", 260, "Bruce bug at line %d, file ..\\..\\source\\common\\init.c", 4);
        }
        gRearview_screen = DRPixelmapAllocate(
            gScreen->type,
            gProgram_state.current_car.mirror_right - gProgram_state.current_car.mirror_left,
            gProgram_state.current_car.mirror_bottom - gProgram_state.current_car.mirror_top,
            rear_screen_pixels,
            0);

        gRearview_screen->origin_x = gRearview_screen->width / 2;
        gRearview_screen->origin_y = gRearview_screen->height / 2;
        gRearview_depth_buffer = BrPixelmapMatch(gRearview_screen, BR_PMMATCH_DEPTH_16);
    }
#endif
}

// IDA: void __cdecl ReinitialiseRearviewCamera()
// FUNCTION: CARM95 0x004bb887
void ReinitialiseRearviewCamera(void) {
    br_camera* camera_ptr;
    LOG_TRACE("()");

    camera_ptr = gRearview_camera->type_data;
    camera_ptr->field_of_view = BrDegreeToAngle(gProgram_state.current_car.rearview_camera_angle);
    camera_ptr->aspect = (gProgram_state.current_car.mirror_right - gProgram_state.current_car.mirror_left) / (float)(gProgram_state.current_car.mirror_bottom - gProgram_state.current_car.mirror_top);
    gRearview_camera->t.t.translate.t.v[0] = gProgram_state.current_car.mirror_x_offset;
    gRearview_camera->t.t.translate.t.v[1] = gProgram_state.current_car.mirror_y_offset;
    gRearview_camera->t.t.translate.t.v[2] = gProgram_state.current_car.mirror_z_offset;
    AllocateRearviewPixelmap();
    MungeRearviewSky();
}

// IDA: void __cdecl ReinitialiseRenderStuff()
// FUNCTION: CARM95 0x004bb916
void ReinitialiseRenderStuff(void) {
    int x_diff;
    int y_diff;
    LOG_TRACE("()");

    if (gProgram_state.cockpit_on) {
        gProgram_state.current_render_left = gProgram_state.current_car.render_left[gProgram_state.cockpit_image_index];
        gProgram_state.current_render_top = gProgram_state.current_car.render_top[gProgram_state.cockpit_image_index];
        gProgram_state.current_render_right = gProgram_state.current_car.render_right[gProgram_state.cockpit_image_index];
        gProgram_state.current_render_bottom = gProgram_state.current_car.render_bottom[gProgram_state.cockpit_image_index];
    } else {
#ifdef DETHRACE_3DFX_PATCH
        if (gSmall_frames_are_slow) {
            gProgram_state.current_render_top = 0;
            gProgram_state.current_render_right = gGraf_specs[gGraf_spec_index].total_width;
            gProgram_state.current_render_left = 0;
            gProgram_state.current_render_bottom = gGraf_specs[gGraf_spec_index].total_height;
            return;
        }
#endif
        gProgram_state.current_render_top = (gGraf_specs[gGraf_spec_index].total_height / 18 & ~1) * gRender_indent;
        gProgram_state.current_render_left = (gGraf_specs[gGraf_spec_index].total_width / 18 & ~3) * gRender_indent;
        x_diff = gGraf_specs[gGraf_spec_index].total_width - gProgram_state.current_render_left;
        y_diff = gGraf_specs[gGraf_spec_index].total_height - gProgram_state.current_render_top;
        gProgram_state.current_render_right = x_diff;
        gProgram_state.current_render_bottom = y_diff;
    }
}

// IDA: void __cdecl InstallFindFailedHooks()
void InstallFindFailedHooks(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AllocateStandardLamp()
// FUNCTION: CARM95 0x004bbcdb
void AllocateStandardLamp(void) {
    br_actor* lamp;
    int i;

    for (i = 0; i < gNumber_of_lights; i++) {
        BrActorAdd(gUniverse_actor, gLight_array[i]);
        lamp = gLight_array[i]->children;
        if (lamp) {
            BrActorRemove(lamp);
            gLight_array[i]->children = NULL;
            BrActorFree(lamp);
        }
        BrLightEnable(gLight_array[i]);
    }
}

// IDA: void __cdecl InitializeBRenderEnvironment()
// FUNCTION: CARM95 0x004bbd6c
void InitializeBRenderEnvironment(void) {
    br_model* arrow_model;
    LOG_TRACE("()");

    gBr_initialized = 1;
    InstallDRMemCalls();
    InstallDRFileCalls();
    SetBRenderScreenAndBuffers(0, 0, 0, 0);
    gUniverse_actor = BrActorAllocate(BR_ACTOR_NONE, NULL);
    if (gUniverse_actor == NULL) {
        FatalError(kFatalError_AllocateRootActor);
    }
    gUniverse_actor->identifier = BrResStrDup(gUniverse_actor, "Root");
    BrEnvironmentSet(gUniverse_actor);
    gNon_track_actor = BrActorAllocate(BR_ACTOR_NONE, NULL);
    if (gNon_track_actor == NULL) {
        FatalError(kFatalError_AllocateRootActor);
    }
    BrActorAdd(gUniverse_actor, gNon_track_actor);
    gDont_render_actor = BrActorAllocate(BR_ACTOR_NONE, 0);
    if (gDont_render_actor == NULL) {
        FatalError(kFatalError_AllocateRootActor);
    }
    gDont_render_actor->render_style = BR_RSTYLE_NONE;
    BrActorAdd(gUniverse_actor, gDont_render_actor);
    AllocateSelf();
    AllocateCamera();
    arrow_model = LoadModel("CPOINT.DAT");
    BrModelAdd(arrow_model);
    gArrow_actor = LoadActor("CPOINT.ACT");
    gArrow_actor->model = arrow_model;
}

// IDA: void __cdecl InitBRFonts()
// FUNCTION: CARM95 0x004bc12c
void InitBRFonts(void) {
    LOG_TRACE("()");
    gBig_font = LoadBRFont("BIGFONT.FNT");
    gFont_7 = LoadBRFont("FONT7.FNT");
    gHeadup_font = LoadBRFont("HEADUP.FNT");
}

// IDA: void __cdecl AustereWarning()
// FUNCTION: CARM95 0x004bc16d
void AustereWarning(void) {
    LOG_TRACE("()");

    ClearEntireScreen();

    if (gAusterity_mode) {
        LoadFont(FONT_GRNLIT);
        TransDRPixelmapText(gBack_screen,
            (gBack_screen->width - DRTextWidth(&gFonts[FONT_GRNLIT], GetMiscString(kMiscString_LOW_MEMORY_MODE_INVOKED))) / 2,
            0,
            &gFonts[FONT_GRNLIT],
            GetMiscString(kMiscString_LOW_MEMORY_MODE_INVOKED),
            gBack_screen->width);
        PDScreenBufferSwap(0);
        gAustere_time = PDGetTotalTime();
        DisposeFont(FONT_GRNLIT);
    } else {
        gAustere_time = gAusterity_mode;
    }
}

// IDA: void __cdecl InitLineStuff()
void InitLineStuff(void) {
    LOG_TRACE("()");

    // HACK: originally 2 vertices
    gLine_model = BrModelAllocate("gLine_model", 3 /*2*/, 1);
    gLine_material = BrMaterialAllocate("gLine_material");
    gLine_actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
    if (!gLine_model || !gLine_material || !gLine_actor) {
        FatalError(kFatalError_OOMCarmageddon_S);
    }
    gLine_actor->identifier = "gLine_actor";
    gLine_actor->render_style = BR_RSTYLE_EDGES;
    gLine_actor->model = gLine_model;
    gLine_actor->material = gLine_material;
    gLine_model->flags = BR_MODF_QUICK_UPDATE | BR_MODF_KEEP_ORIGINAL;
    gLine_model->faces->vertices[0] = 0;
    gLine_model->faces->vertices[1] = 0;
    gLine_model->faces->vertices[2] = 1;

    // HACK: override the 2 vertices + EDGES with 3 vertices + FACES
    gLine_model->faces->vertices[1] = 2;
    gLine_actor->render_style = BR_RSTYLE_FACES;
    // HACK end

    gLine_material->flags = BR_MATF_TWO_SIDED | BR_MATF_SMOOTH | BR_MATF_PRELIT | BR_MATF_LIGHT;
    gLine_model->faces[0].flags = BR_FACEF_COPLANAR_0 | BR_FACEF_COPLANAR_2;
    BrModelAdd(gLine_model);
    BrMaterialAdd(gLine_material);
    BrActorAdd(gDont_render_actor, gLine_actor);
}

// IDA: void __cdecl InitSmokeStuff()
void InitSmokeStuff(void) {
    static br_token_value fadealpha[3] = { { BRT_BLEND_B, { .u32 = 1 } }, { BRT_OPACITY_X, { .x = 0x4B0000 } }, { 0 } };
    tPath_name path;
    LOG_TRACE("()");

    gBlend_model = BrModelAllocate("gBlend_model", 4, 2);
    gBlend_material = BrMaterialAllocate("gBlend_material");
    gBlend_actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
    if (!gBlend_model || !gBlend_material || !gBlend_actor) {
        FatalError(kFatalError_OOMCarmageddon_S);
    }
    gBlend_actor->identifier = "gBlend_actor";
    gBlend_actor->model = gBlend_model;
    gBlend_actor->material = gBlend_material;
    gBlend_model->faces[0].vertices[0] = 0;
    gBlend_model->faces[0].vertices[1] = 1;
    gBlend_model->faces[0].vertices[2] = 2;
    gBlend_model->faces[1].vertices[0] = 2;
    gBlend_model->faces[1].vertices[1] = 3;
    gBlend_model->faces[1].vertices[2] = 0;
    gBlend_model->vertices[0].p.v[0] = -1.0f;
    gBlend_model->vertices[0].p.v[1] = 1.0f;
    gBlend_model->vertices[0].p.v[2] = 0.0f;
    gBlend_model->vertices[1].p.v[0] = -1.0f;
    gBlend_model->vertices[1].p.v[1] = -1.0f;
    gBlend_model->vertices[1].p.v[2] = 0.0f;
    gBlend_model->vertices[2].p.v[0] = 1.0f;
    gBlend_model->vertices[2].p.v[1] = -1.0f;
    gBlend_model->vertices[2].p.v[2] = 0.0f;
    gBlend_model->vertices[3].p.v[0] = 1.0f;
    gBlend_model->vertices[3].p.v[1] = 1.0f;
    gBlend_model->vertices[3].p.v[2] = 0.0f;
    gBlend_material->flags = BR_MATF_PERSPECTIVE | BR_MATF_SMOOTH;
    gBlend_material->flags |= (BR_MATF_LIGHT | BR_MATF_PRELIT);
    gBlend_model->flags |= BR_MODF_KEEP_ORIGINAL;
    gBlend_material->extra_prim = fadealpha;
    PathCat(path, gApplication_path, "PIXELMAP");
    PathCat(path, path, "SMOKE.PIX");
    gBlend_material->colour_map = DRPixelmapLoad(path);
    if (!gBlend_material->colour_map) {
        FatalError(kFatalError_LoadPixelmapFile_S, path);
    }
    gBlend_material->colour_map->map = gRender_palette;
    BrMapAdd(gBlend_material->colour_map);
    gBlend_model->vertices[0].map.v[0] = 0.0f;
    gBlend_model->vertices[0].map.v[1] = 1.0f - 1.0f / (float)gBlend_material->colour_map->height;
    gBlend_model->vertices[1].map.v[0] = 0.0f;
    gBlend_model->vertices[1].map.v[1] = 0.0f;
    gBlend_model->vertices[2].map.v[0] = 1.0f - 1.0f / (float)gBlend_material->colour_map->width;
    gBlend_model->vertices[2].map.v[1] = 0.0f;
    gBlend_model->vertices[3].map.v[0] = 1.0f - 1.0f / (float)gBlend_material->colour_map->width;
    gBlend_model->vertices[3].map.v[1] = 1.0f - 1.0f / (float)gBlend_material->colour_map->height;
    BrModelAdd(gBlend_model);
    BrMaterialAdd(gBlend_material);
    BrActorAdd(gDont_render_actor, gBlend_actor);
}

// IDA: void __cdecl Init2DStuff()
void Init2DStuff(void) {
    br_camera* camera;
    static br_token_value fadealpha[3] = { { BRT_BLEND_B, { .u32 = 1u } }, { BRT_OPACITY_X, { .x = 0x800000 } }, { 0 } };
    tPath_name path;
    br_scalar prat_u;
    br_scalar prat_v;
    LOG_TRACE("()");

    g2d_camera = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
    gDim_model = BrModelAllocate("gDim_model", 4, 2);
    gDim_material = BrMaterialAllocate("gDim_material");
    gDim_actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
    gPrat_model = BrModelAllocate("gPrat_model", 4, 2);
    gPrat_material = BrMaterialAllocate("gPrat_material");
    gPrat_actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
    if (!gDim_model || !gDim_material || !gDim_actor || !gPrat_model || !gPrat_material || !gPrat_actor || !g2d_camera) {
        FatalError(kFatalError_OOMCarmageddon_S);
    }
    g2d_camera->identifier = "g2d_camera";
    camera = g2d_camera->type_data;
    camera->type = BR_CAMERA_PARALLEL;
    camera->hither_z = 1.0f;
    camera->yon_z = 3.0f;
    camera->width = gScreen->width;
    camera->height = gScreen->height;
    gDim_actor->identifier = "gDim_actor";
    gDim_actor->model = gDim_model;
    gDim_actor->material = gDim_material;

    gDim_model->faces->vertices[0] = 0;
    gDim_model->faces->vertices[1] = 1;
    gDim_model->faces->vertices[2] = 2;
    gDim_model->faces[1].vertices[0] = 2;
    gDim_model->faces[1].vertices[1] = 3;
    gDim_model->faces[1].vertices[2] = 0;
    gDim_model->vertices->p.v[0] = 150.0f;
    gDim_model->vertices->p.v[1] = -20.0f;
    gDim_model->vertices->p.v[2] = -2.0f;
    gDim_model->vertices[1].p.v[0] = 150.0f;
    gDim_model->vertices[1].p.v[1] = -100.0f;
    gDim_model->vertices[1].p.v[2] = -2.0f;
    gDim_model->vertices[2].p.v[0] = 200.0f;
    gDim_model->vertices[2].p.v[1] = -100.0f;
    gDim_model->vertices[2].p.v[2] = -2.0f;
    gDim_model->vertices[3].p.v[0] = 200.0f;
    gDim_model->vertices[3].p.v[1] = -20.0f;
    gDim_model->vertices[3].p.v[2] = -2.0f;
    gDim_material->colour = 0;
    gDim_material->flags = BR_MATF_FORCE_FRONT;
    gDim_model->flags |= BR_MODF_KEEP_ORIGINAL;
    gDim_material->extra_prim = fadealpha;
    BrModelAdd(gDim_model);
    BrMaterialAdd(gDim_material);
    BrActorAdd(g2d_camera, gDim_actor);
    gDim_actor->render_style = BR_RSTYLE_NONE;
    gPrat_actor->identifier = "gPrat_actor";
    gPrat_actor->model = gPrat_model;
    gPrat_actor->material = gPrat_material;
    gPrat_model->faces->vertices[0] = 0;
    gPrat_model->faces->vertices[1] = 1;
    gPrat_model->faces->vertices[2] = 2;
    gPrat_model->faces[1].vertices[0] = 2;
    gPrat_model->faces[1].vertices[1] = 3;
    gPrat_model->faces[1].vertices[2] = 0;
    gPrat_model->vertices->p.v[0] = 150.0f;
    gPrat_model->vertices->p.v[1] = -20.0f;
    gPrat_model->vertices->p.v[2] = -2.0f;
    gPrat_model->vertices[1].p.v[0] = 150.0f;
    gPrat_model->vertices[1].p.v[1] = -100.0f;
    gPrat_model->vertices[1].p.v[2] = -2.0f;
    gPrat_model->vertices[2].p.v[0] = 200.0f;
    gPrat_model->vertices[2].p.v[1] = -100.0f;
    gPrat_model->vertices[2].p.v[2] = -2.0f;
    gPrat_model->vertices[3].p.v[0] = 200.0f;
    gPrat_model->vertices[3].p.v[1] = -20.0f;
    gPrat_model->vertices[3].p.v[2] = -2.0f;
    gPrat_material->colour = 0xFFFFFF;
    gPrat_material->flags = BR_MATF_FORCE_FRONT;
    gPrat_model->flags |= BR_MODF_KEEP_ORIGINAL;
    prat_u = 104.0f / (float)HighResPratBufferWidth();
    prat_v = 110.0f / (float)HighResPratBufferHeight();
    gPrat_model->vertices->map.v[0] = 0.0f;
    gPrat_model->vertices->map.v[1] = 0.0f;
    gPrat_model->vertices[1].map.v[0] = 0.0f;
    gPrat_model->vertices[1].map.v[1] = prat_v;
    gPrat_model->vertices[2].map.v[0] = prat_u;
    gPrat_model->vertices[2].map.v[1] = prat_v;
    gPrat_model->vertices[3].map.v[0] = prat_u;
    gPrat_model->vertices[3].map.v[1] = 0.0f;
    BrModelAdd(gPrat_model);
    BrMaterialAdd(gPrat_material);
    BrActorAdd(g2d_camera, gPrat_actor);
    gPrat_actor->render_style = BR_RSTYLE_NONE;
}

// IDA: void __usercall InitialiseApplication(int pArgc@<EAX>, char **pArgv@<EDX>)
// FUNCTION: CARM95 0x004bba54
void InitialiseApplication(int pArgc, char** pArgv) {

    if (harness_game_config.gore_check) {
        gProgram_state.sausage_eater_mode = gSausage_override ? 1 : (PDGetGorePassword() ? 0 : 1);
        PDDisplayGoreworthiness(!gProgram_state.sausage_eater_mode);
    } else {
        gProgram_state.sausage_eater_mode = gSausage_override;
    }

    MAMSInitMem();
    PrintMemoryDump(gSausage_override, *pArgv);
    if (gAustere_override || PDDoWeLeadAnAustereExistance() != 0) {
        gAusterity_mode = 1;
    }

    srand(time(NULL));
    BrV1dbBeginWrapper_Float();
    CreateStainlessClasses();
    InitWobbleStuff();
    LoadGeneralParameters();
    DefaultNetName();
    strcpy(gProgram_state.player_name[0], "MAX DAMAGE");
    strcpy(gProgram_state.player_name[1], "DIE ANNA");
    RestoreOptions();
    LoadKeyMapping();
    if (!PDInitScreenVars(pArgc, pArgv)) {
        FatalError(kFatalError_UnsupportedScreenDepth);
    }
    CalcGrafDataIndex();
    PDInitScreen();
    InitializeBRenderEnvironment();
    InitDRFonts();
    InitBRFonts();
    LoadMiscStrings();
    LoadInRegistees();

#ifdef DETHRACE_3DFX_PATCH

    // dethrace: if statement added to support all types of games
    if (harness_game_config.opengl_3dfx_mode) {
        InitLineStuff();
        InitSmokeStuff();
        Init2DStuff();
    }
#endif

    FinishLoadingGeneral();
#ifndef DETHRACE_3DFX_PATCH
    // 3dfx patch calls this earlier
    InitializePalettes();
#endif
    AustereWarning();
    LoadInterfaceStrings();
    InitializeActionReplay();
    FlicPaletteAllocate();
    InitInterfaceLoadState();
    InitTransientBitmaps();
    InitSound();
    InitHeadups();
    gDefault_track_material = BrMaterialAllocate("gDefault_track_material");
    gDefault_track_material->index_base = 227;
    gDefault_track_material->index_range = 1;

#ifdef DETHRACE_3DFX_PATCH
    gDefault_track_material->ka = 1.0;
    gDefault_track_material->kd = 0.0;
    gDefault_track_material->ks = 0.0;
    gDefault_track_material->colour = ((br_colour*)gRender_palette->pixels)[227];
#endif

    BrMaterialAdd(gDefault_track_material);
    InitShadow();
    InitFlics();
    AllocateStandardLamp();
    InitAmbience();
    LoadOpponents();
    LoadPowerups();
    LoadRaces(gRace_list, &gNumber_of_races, -1);
    InitPaletteAnimate();
    InitRayCasting();
    InitDepthEffects();
    InitialiseStorageSpace(&gOur_car_storage_space, 40, 2, 40, 30);
    InitialiseStorageSpace(&gTheir_cars_storage_space, 300, 50, 500, 200);
    InitialiseStorageSpace(&gPedestrians_storage_space, 500, 10, 0, 0);
    InitialiseStorageSpace(&gTrack_storage_space, 400, 50, 400, 1000);
    InitOilSpills();
    if (gAustere_time) {
        while (PDGetTotalTime() - gAustere_time < 2000) {
        }
    }
    ClearEntireScreen();
    InitSkids();
    InitPeds();
    gProgram_state.track_spec.the_actor = NULL;
    gCD_is_in_drive = TestForOriginalCarmaCDinDrive();
    SwitchToLoresMode();
    PrintMemoryDump(0, "AFTER APPLICATION INITIALISATION");
}

// IDA: void __usercall InitialiseDeathRace(int pArgc@<EAX>, char **pArgv@<EDX>)
// FUNCTION: CARM95 0x004bba24
void InitialiseDeathRace(int pArgc, char** pArgv) {
    PDInitialiseSystem();
    InitialiseApplication(pArgc, pArgv);
    gInitialisation_finished = 1;
}

// IDA: void __usercall InitGame(int pStart_race@<EAX>)
// FUNCTION: CARM95 0x004bc22c
void InitGame(int pStart_race) {
    int i;
    LOG_TRACE("(%d)", pStart_race);

    gWaiting_for_unpause = 1;
    gWait_for_it = 1;
    if (gNet_mode != eNet_mode_none) {
        gCredit_period = gCredit_period_network[gCurrent_net_game->type];
    } else {
        gCredit_period = gCredit_period_single[gProgram_state.skill_level];
    }
    SwitchToRealResolution();
    gGame_to_load = -1;
    gCurrent_race.number_of_racers = 0;
    LoadHeadupImages();
    gGame_initialized = 1;
    gNet_mode_of_last_game = gNet_mode;
    gNo_races_yet = 1;
    NetPlayerStatusChanged(ePlayer_status_loading);
    gProgram_state.current_race_index = pStart_race;
    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) {
        gProgram_state.current_car.power_up_levels[0] = gDemo_armour;
        gProgram_state.current_car.power_up_levels[1] = gDemo_power;
        gProgram_state.current_car.power_up_levels[2] = gDemo_offensive;
    } else {
        for (i = 0; i < COUNT_OF(gProgram_state.current_car.power_up_levels); i++) {
            gProgram_state.current_car.power_up_levels[i] = 0;
        }
    }
    for (i = 0; i < gNumber_of_races; i++) {
        gRace_list[i].been_there_done_that = 0;
    }
    for (i = 0; i < gNumber_of_racers; i++) {
        gOpponents[i].dead = 0;
    }
    gProgram_state.rank = gInitial_rank;
    gProgram_state.credits = gInitial_credits[gProgram_state.skill_level];
    gProgram_state.credits_per_rank = gCredits_per_rank[gProgram_state.skill_level];
    gProgram_state.number_of_cars = 1;
    gProgram_state.cars_available[0] = gProgram_state.frank_or_anniness;
    gProgram_state.game_completed = 0;
    gProgram_state.redo_race_index = -1;
    gWait_for_it = 0;
    SwitchToLoresMode();

    // added by dethrace to support --game-completed arg
    gProgram_state.game_completed = harness_game_config.game_completed;
    // -
}

// IDA: void __cdecl DisposeGameIfNecessary()
// FUNCTION: CARM95 0x004bc3d0
void DisposeGameIfNecessary(void) {
    int i;
    LOG_TRACE("()");

    if (gNet_mode != eNet_mode_none) {
        NetLeaveGame(gCurrent_net_game);
    }
    if (gGame_initialized) {
        DisposeHeadupImages();
        gGame_initialized = 0;
        if (gNet_mode_of_last_game) {
            for (i = 0; i < gNumber_of_net_players; i++) {
                DisposeCar(gNet_players[i].car, gNet_players[i].car_index);
            }
            PossibleService();
            LoadRaces(gRace_list, &gNumber_of_races, -1);
            DisposeStorageSpace(&gNet_cars_storage_space);
        }
    }
    ShutdownNetIfRequired();
}

// IDA: void __cdecl LoadInTrack()
// FUNCTION: CARM95 0x004bc93a
void LoadInTrack(void) {
    LOG_TRACE("()");

    LoadTrack(gProgram_state.track_file_name, &gProgram_state.track_spec, &gCurrent_race);
}

// IDA: void __cdecl DisposeTrack()
// FUNCTION: CARM95 0x004bc493
void DisposeTrack(void) {
    LOG_TRACE("()");

    FreeTrack(&gProgram_state.track_spec);
}

// IDA: void __usercall CopyMaterialColourFromIndex(br_material *pMaterial@<EAX>)
void CopyMaterialColourFromIndex(br_material* pMaterial) {
    LOG_TRACE("(%p)", pMaterial);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitRace()
// FUNCTION: CARM95 0x004bc4b1
void InitRace(void) {
    LOG_TRACE("()");

    SwitchToRealResolution();
    // TODO: dword_5454C4 = 0;
    ClearConcussion();
    ClearWobbles();
    ClearHeadups();
    ResetOilSpills();
    HideSkids();
    PossibleService();
    BuildColourTable(gRender_palette);
    PossibleService();
    // TODO: dword_55142C = 0;
    gStart_race_sent = 0;
    gProgram_state.frame_rate_headup = NewTextHeadupSlot(eHeadupSlot_development, 0, 0, -1, "");
    if (TranslationMode()) {
        if (gAusterity_mode) {
            FlushInterfaceFonts();
        }
    } else {
        LoadFont(kFont_ORANGHED);
        LoadFont(kFont_BLUEHEAD);
        LoadFont(kFont_GREENHED);
    }
    LoadFont(kFont_MEDIUMHD);
    LoadFont(kFont_TIMER);
    PossibleService();
    LoadFont(kFont_NEWHITE);
    LoadFont(kFont_NEWRED);
    LoadFont(kFont_NEWBIGGR);
    PossibleService();
    ResetRecoveryVouchers();
    gMap_mode = 0;
    gProgram_state.cockpit_image_index = 0;
    if (gNet_mode != eNet_mode_none) {
        gNet_cash_headup = NewTextHeadupSlot(eHeadupSlot_cash_network, 0, 0, -6, "");
        gNet_ped_headup = NewTextHeadupSlot(eHeadupSlot_ped_network, 0, 0, -6, "");
    } else {
        gCredits_won_headup = NewTextHeadupSlot(eHeadupSlot_credits, 0, 0, -6, "");
        gPed_kill_count_headup = NewTextHeadupSlot(eHeadupSlot_ped_kills, 0, 0, -6, "");
        gCar_kill_count_headup = NewTextHeadupSlot(eHeadupSlot_cars_out_count, 0, 0, -6, "");
        gTimer_headup = NewTextHeadupSlot(eHeadupSlot_timer, 0, 0, -5, "");
        gTime_awarded_headup = NewTextHeadupSlot(eHeadupSlot_time_award, 0, 0, -2, "");
        gLaps_headup = NewTextHeadupSlot(eHeadupSlot_lap_count, 0, 0, -6, "");
    }
    PossibleService();
    gProgram_state.which_view = eView_forward;
    gProgram_state.new_view = eView_undefined;
    gProgram_state.pratcam_move_start = 0;
    gAction_replay_mode = 0;
    SwitchToRealResolution();
    InitPratcam();
    SwitchToLoresMode();
    gProgram_state.credits_earned = 0;
    gProgram_state.credits_lost = 0;
    gProgram_state.peds_killed = 0;
    gProgram_state.revs = 2000;
    gProgram_state.current_car.speed = 0.0;
    gProgram_state.current_car.steering_angle = 0.0;
    gProgram_state.current_car.lf_sus_position = 0.0;
    gProgram_state.current_car.rf_sus_position = 0.0;
    gProgram_state.current_car.lr_sus_position = 0.0;
    gProgram_state.current_car.rr_sus_position = 0.0;
    PossibleService();
    gAuto_repair = 0;
    SetIntegerMapRenders();
    AdjustRenderScreenSize();
    PrintMemoryDump(0, "DIRECTLY BEFORE LOADING IN TRACK");
    LoadInTrack();
    if (gYon_multiplier != 1.0) {
        AdjustRenderScreenSize();
    }
    PrintMemoryDump(0, "DIRECTLY AFTER LOADING IN TRACK");
    LoadCopCars();
#ifdef DETHRACE_3DFX_PATCH
    // In the 3dfx patch this code was moved from `LoadTrack` so that the pedestrian material would be
    // fogged correctly
    InstantDepthChange(
        gProgram_state.default_depth_effect.type,
        gProgram_state.default_depth_effect.sky_texture,
        gProgram_state.default_depth_effect.start,
        gProgram_state.default_depth_effect.end);
    gSwap_sky_texture = 0;
    if (!GetSkyTextureOn()) {
        ToggleSkyQuietly();
    }
    gSwap_depth_effect_type = -1;
    if (!GetDepthCueingOn()) {
        ToggleDepthCueingQuietly();
    }
#endif
    PrintMemoryDump(0, "AFTER LOADING IN COPS");
    SaveShadeTables();
    gCountdown = 7;
    gTimer = 1000 * gCurrent_race.initial_timer[gProgram_state.skill_level];
    gLap = 1;
    gTotal_laps = gCurrent_race.total_laps;
    gCheckpoint = 1;
    gCheckpoint_count = gCurrent_race.check_point_count;
    gFreeze_timer = 0;
    gFree_repairs = 0;
    if (gNet_mode) {
        gShow_opponents = gCurrent_net_game->options.show_players_on_map;
    } else {
        gShow_opponents = 1;
    }
    gOn_drugs = 0;
    gRace_finished = 0;
    gOpponent_speed_factor = 1.0f;
    gCop_speed_factor = 1.0f;
    gGravity_multiplier = gDefault_gravity;
    gPinball_factor = 0;
    gInstant_handbrake = 0;
    if (gNet_mode) {
        gShow_peds_on_map = gCurrent_net_game->options.show_peds_on_map;
    } else {
        gShow_peds_on_map = 0;
    }
    PossibleService();
    SetCarSuspGiveAndHeight(&gProgram_state.current_car, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
    InitPowerups();
    PossibleService();
    ResetSparks();
    PossibleService();
    MakeSavedGame(&gPre_race_saved_game);
    PossibleService();
    InitPlayers();
    if (gNet_mode) {
        gProgram_state.credits_earned = gInitial_net_credits[gCurrent_net_game->options.starting_money_index];
        InitNetGameplayStuff();
    }
    gInitialised_grid = 0;
    SwitchToLoresMode();
}

// IDA: void __cdecl DisposeRace()
// FUNCTION: CARM95 0x004bc968
void DisposeRace(void) {
    LOG_TRACE("()");

    PossibleService();
    DisposePiping();
    PossibleService();
    CloseDownPowerUps();
    PossibleService();
    if (!TranslationMode()) {
        DisposeFont(1);
        DisposeFont(2);
        DisposeFont(3);
    }
    PossibleService();
    DisposeFont(6);
    DisposeFont(7);
    DisposeFont(8);
    DisposeFont(4);
    DisposeFont(5);
    PossibleService();
    DisposeSavedShadeTables();
    PossibleService();
    DisposeSoundSources();
    PossibleService();
    DisposeOpponents();
    PossibleService();
    DisposePedestrians();
    PossibleService();
    DisposePratcam();
    PossibleService();

#ifdef DETHRACE_FIX_BUGS
    // when exiting a race, skid mark materials are unloaded, but material_modifiers is not changed.
    // In 3dfx mode, `FrobFog` is called during loading the next track, which iterates over the material_modifiers
    // causing a use-after-free
    for (int i = 0; i < COUNT_OF(gCurrent_race.material_modifiers); i++) {
        gCurrent_race.material_modifiers[i].skid_mark_material = NULL;
    }
#endif
}

// IDA: int __cdecl GetScreenSize()
// FUNCTION: CARM95 0x004bca25
int GetScreenSize(void) {
    LOG_TRACE("()");

    return gRender_indent;
}

// IDA: void __usercall SetScreenSize(int pNew_size@<EAX>)
// FUNCTION: CARM95 0x004bca3a
void SetScreenSize(int pNew_size) {
    LOG_TRACE("(%d)", pNew_size);

    gRender_indent = pNew_size;
}
