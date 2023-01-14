#include "pedestrn.h"
#include "brender/brender.h"
#include "car.h"
#include "constants.h"
#include "displays.h"
#include "errors.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "graphics.h"
#include "harness/trace.h"
#include "input.h"
#include "loading.h"
#include "network.h"
#include "opponent.h"
#include "pd/sys.h"
#include "piping.h"
#include "powerup.h"
#include "pratcam.h"
#include "raycast.h"
#include "replay.h"
#include "sound.h"
#include "spark.h"
#include "structur.h"
#include "trig.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>
#include <time.h>

#define FLAG_WAVING_BASTARD_REF 99
#define ACTIVE_PED_DXDZ 11.f

#define FOURCC(A, B, C, D) (((A & 0xff) << 24) | ((B & 0xff) << 16) | ((C & 0xff) << 8) | ((D & 0xff) << 0))
#define PEDESTRIAN_MAGIC FOURCC('P', 'e', 'd', '!')
#define ActorToPedestrianData(ACTOR) ((tPedestrian_data*)((ACTOR)->type_data))

#define GET_PEDESTRIAN_INDEX(P) ((P)-gPedestrian_array)

int gDetect_peds = 1;
int gReally_stupid_ped_bug_enable = 0;
int gPed_sound_disable = 0;
int gVesuvians_last_time = 0;
int gSend_peds = 1;
tU32 gLast_ped_message_send = 0;
tPedestrian_instruction* gInitial_instruction = NULL;
char* gRate_commands[3] = { "fixed", "speed", "variable" };
char* gCollide_commands[1] = { "collide" };
char* gInstruc_commands[10] = {
    "point",
    "xpoint",
    "bchoice",
    "fchoice",
    "dead",
    "bmarker",
    "fmarker",
    "baction",
    "faction",
    "reverse"
};
float gMin_ped_gib_speeds[4] = { 10000.f, .005f, .002f, .001f };
float gPed_gib_distrib[4] = { 0.f, 0.33f, 0.34f, 0.33f };
float gPed_gib_speeds[4] = { 0.f, 1.f, 1.f, 1.f };
int gPed_size_counts[4] = { 3, 5, 5, 5 };
char* gPed_gib_names[4][5] = {
    { "BIGGIBS.PIX", "BIGGIBS2.PIX", "BIGGIBS3.PIX", "", "" },
    { "ELBOW.PIX", "PELVIS.PIX", "HEAD.PIX", "RIBS.PIX", "BONE.PIX" },
    { "HAND.PIX", "CHUNK02.PIX", "EYEBALL.PIX", "COLON.PIX", "CHUNK01.PIX" },
    { "CHUNK04.PIX", "SPLAT1.PIX", "CHUNK03.PIX", "SPLAT2.PIX", "SPLATOUT.PIX" }
};
char* gPed_geb_names[4][5] = {
    { "GBIGGIBS.PIX", "GBIGGIB2.PIX", "GBIGGIB3.PIX", "", "" },
    { "GELBOW.PIX", "GPELVIS.PIX", "GHEAD.PIX", "GRIBS.PIX", "GBONE.PIX" },
    { "GHAND.PIX", "GCHUNK02.PIX", "GEYEBALL.PIX", "GCOLON.PIX", "GCHUNK01.PIX" },
    { "GCHUNK04.PIX", "GSPLAT1.PIX", "GCHUNK03.PIX", "GSPLAT2.PIX", "GSPLATOT.PIX" }
};
int gPed_gib_maxes[4][5] = {
    { 1, 1, 1, 0, 0 },
    { 3, 3, 3, 3, 3 },
    { 3, 3, 2, 3, 3 },
    { 3, 3, 3, 3, 3 },
};
br_scalar gExploding_ped_scale[3] = { 0.0028f, 0.0033f, 0.0045f };
br_vector3 gZero_v__pedestrn; // suffix added to avoid duplicate symbol
int gPed_instruc_count = 0;
int gPed_count;
br_actor* gPath_actor = NULL;
br_actor* gCurrent_ped_path_actor = NULL;
int gPedestrians_on = 0;
int gVesuvian_corpses = 0;
br_material* gPed_material = NULL;
int gPed_gib_counts[4][5];
tPedestrian_instruction gPed_instrucs[100];
tPed_gib gPed_gibs[30];
tPed_gib_materials gPed_gib_materials[4];
tProximity_ray gProximity_rays[20];
int gPed_colliding;
float gZombie_factor;
int gRespawn_variance;
br_scalar gPed_scale_factor;
int gTotal_peds;
int gPedestrian_harvest;
br_vector3 gPed_pos_camera;
int gMin_respawn_time;
br_material* gPath_mat_calc;
float gPedestrian_speed_factor;
int gExploding_pedestrians;
int gBlind_pedestrians;
br_material* gPath_mat_normal;
br_material* gInit_pos_mat_calc;
int gPed_other;
int gAttracted_pedestrians;
int gPed_ref_num;
br_scalar gMax_distance_squared;
br_model* gPed_model;
float gDanger_level;
br_vector3 gDanger_direction;
int gInit_ped_instruc;
int gCurrent_lollipop_index;
int gVesuvians_this_time;
int gNumber_of_ped_gibs;
tPedestrian_data* gFlag_waving_bastard;
int gNumber_of_pedestrians = 0;
br_pixelmap* gProx_ray_shade_table;
tPedestrian_data* gPedestrian_array;
tU32 gLast_ped_splat_time;
int gCurrent_ped_multiplier;

// IDA: void __usercall PedModelUpdate(br_model *pModel@<EAX>, br_scalar x0, br_scalar y0, br_scalar x1, br_scalar y1, br_scalar x2, br_scalar y2, br_scalar x3, br_scalar y3)
void PedModelUpdate(br_model* pModel, br_scalar x0, br_scalar y0, br_scalar x1, br_scalar y1, br_scalar x2, br_scalar y2, br_scalar x3, br_scalar y3) {
    LOG_TRACE("(%p, %f, %f, %f, %f, %f, %f, %f, %f)", pModel, x0, y0, x1, y1, x2, y2, x3, y3);

    BrVector2Set(&pModel->vertices[0].p, x0, y0);
    BrVector2Set(&pModel->vertices[1].p, x1, y1);
    BrVector2Set(&pModel->vertices[2].p, x2, y2);
    BrVector2Set(&pModel->vertices[3].p, x3, y3);
    BrModelUpdate(pModel, BR_MATF_I_FROM_U);
}

// IDA: int __usercall ActorIsPedestrian@<EAX>(br_actor *pActor@<EAX>)
int ActorIsPedestrian(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);

    if (pActor->model == NULL) {
        return 0;
    }
    if (pActor->type_data == NULL) {
        return 0;
    }
    return ActorToPedestrianData(pActor)->magic_number == PEDESTRIAN_MAGIC;
}

// IDA: br_scalar __usercall PedHeightFromActor@<ST0>(br_actor *pActor@<EAX>)
br_scalar PedHeightFromActor(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);
    if (!ActorIsPedestrian(pActor)) {
        return 0.f;
    }
    return ActorToPedestrianData(pActor)->height2;
}

// IDA: int __usercall GetPedestrianValue@<EAX>(br_actor *pActor@<EAX>)
int GetPedestrianValue(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);

    return ActorToPedestrianData(pActor)->credits_value;
}

// IDA: int __usercall PedestrianActorIsPerson@<EAX>(br_actor *pActor@<EAX>)
int PedestrianActorIsPerson(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);

    return ActorToPedestrianData(pActor)->ref_number < 100 && ActorToPedestrianData(pActor)->hit_points != -100;
}

// IDA: br_actor* __usercall GetPedestrianActor@<EAX>(int pIndex@<EAX>)
br_actor* GetPedestrianActor(int pIndex) {
    LOG_TRACE("(%d)", pIndex);

    if (pIndex >= 0 && pIndex < gPed_count) {
        return gPedestrian_array[pIndex].actor;
    } else {
        return NULL;
    }
}

// IDA: br_pixelmap* __usercall GetPedestrianTexture@<EAX>(br_actor *pActor@<EAX>, int *pFlipped@<EDX>)
br_pixelmap* GetPedestrianTexture(br_actor* pActor, int* pFlipped) {
    LOG_TRACE("(%p, %p)", pActor, pFlipped);

    if (!ActorIsPedestrian(pActor)) {
        return NULL;
    }
    *pFlipped = ActorToPedestrianData(pActor)->flipped;
    return ActorToPedestrianData(pActor)->colour_map;
}

// IDA: void __cdecl TogglePedestrians()
void TogglePedestrians() {
    LOG_TRACE("()");

    if (!gProgram_state.sausage_eater_mode) {
        gPedestrians_on = !gPedestrians_on;
        if (gPedestrians_on) {
            NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(24));
        } else {
            NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(25));
        }
    }
}

// IDA: void __cdecl InitPedGibs()
void InitPedGibs() {
    int i;
    int j;
    br_model* the_model;
    br_pixelmap* the_pix;
    br_material* the_material;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gPed_gibs); i++) {
        PossibleService();
        gPed_gibs[i].size = -1;
        the_model = BrModelAllocate(NULL, 4, 2);
        the_model->flags |= BR_MODF_UPDATEABLE;
        the_model->faces[0].vertices[0] = 0;
        the_model->faces[0].vertices[1] = 2;
        the_model->faces[0].vertices[2] = 1;
        the_model->faces[0].material = NULL;
        the_model->faces[0].smoothing = 1;
        the_model->faces[1].vertices[0] = 0;
        the_model->faces[1].vertices[1] = 3;
        the_model->faces[1].vertices[2] = 2;
        the_model->faces[1].material = NULL;
        the_model->faces[1].smoothing = 1;
        BrVector3Set(&the_model->vertices[0].p, -1.0f, 0.0f, 0.0f);
        BrVector2Set(&the_model->vertices[0].map, 0.0f, 1.0f);
        BrVector3Set(&the_model->vertices[1].p, -1.0f, 1.0f, 0.0f);
        BrVector2Set(&the_model->vertices[1].map, 0.0f, 0.0f);
        BrVector3Set(&the_model->vertices[2].p, 1.0f, 1.0f, 0.0f);
        BrVector2Set(&the_model->vertices[2].map, 1.0f, 0.0f);
        BrVector3Set(&the_model->vertices[3].p, 1.0f, 0.0f, 0.0f);
        BrVector2Set(&the_model->vertices[3].map, 1.0f, 1.0f);
        BrModelAdd(the_model);
        gPed_gibs[i].actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        gPed_gibs[i].actor->parent = NULL;
        gPed_gibs[i].actor->model = the_model;
        gPed_gibs[i].actor->render_style = BR_RSTYLE_NONE;
    }
    for (i = 0; i < COUNT_OF(gPed_size_counts); i++) {
        for (j = 0; j < gPed_size_counts[i]; j++) {
            PossibleService();
            if (gProgram_state.sausage_eater_mode) {
                the_pix = LoadPixelmap(gPed_geb_names[i][j]);
            } else {
                the_pix = LoadPixelmap(gPed_gib_names[i][j]);
            }
            BrMapAdd(the_pix);
            gPed_gib_materials[i].count = gPed_size_counts[i];
            gPed_gib_materials[i].materials[j] = BrMaterialAllocate(NULL);
            BrMaterialAdd(gPed_gib_materials[i].materials[j]);
            the_material = gPedestrian_array[0].actor->material;
            gPed_gib_materials[i].materials[j]->flags = the_material->flags;
            gPed_gib_materials[i].materials[j]->ka = the_material->ka;
            gPed_gib_materials[i].materials[j]->kd = the_material->kd;
            gPed_gib_materials[i].materials[j]->ks = the_material->ks;
            gPed_gib_materials[i].materials[j]->power = the_material->power;
            gPed_gib_materials[i].materials[j]->index_base = the_material->index_base;
            gPed_gib_materials[i].materials[j]->index_range = the_material->index_range;
            gPed_gib_materials[i].materials[j]->colour_map = the_pix;
            gPed_gib_materials[i].materials[j]->map_transform = the_material->map_transform;
            gPed_gib_materials[i].materials[j]->index_shade = the_material->index_shade;
            BrMaterialUpdate(gPed_gib_materials[i].materials[j], BR_MATU_ALL);
        }
    }
}

// IDA: void __usercall SetPedMaterialForRender(br_actor *pActor@<EAX>)
void SetPedMaterialForRender(br_actor* pActor) {
    tPedestrian_data* ped;
    int changed;
    LOG_TRACE("(%p)", pActor);

    ped = ActorToPedestrianData(pActor);
    pActor->material->colour_map = ped->colour_map;
    if (ped->flipped) {
        changed = pActor->material->map_transform.m[0][0] != 1.f;
        if (changed) {
            pActor->material->map_transform.m[0][0] = 1.f;
        }
    } else {
        changed = pActor->material->map_transform.m[0][0] != -1.f;
        if (changed) {
            pActor->material->map_transform.m[0][0] = -1.f;
        }
    }
    if (changed) {
        BrMaterialUpdate(pActor->material, BR_MATU_MAP_TRANSFORM);
    }
    BrMaterialUpdate(pActor->material, BR_MATU_COLOURMAP);
}

// IDA: void __cdecl PedCallBack(br_actor *pActor, br_model *pModel, br_material *pMaterial, void *pRender_data, br_uint_8 pStyle, int pOn_screen)
void PedCallBack(br_actor* pActor, br_model* pModel, br_material* pMaterial, void* pRender_data, br_uint_8 pStyle, int pOn_screen) {
    tPedestrian_data* ped;
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", pActor, pModel, pMaterial, pRender_data, pStyle, pOn_screen);

    ped = ActorToPedestrianData(pActor);
    pActor->material->colour_map = ped->colour_map;
    if (ped->flipped) {
        pActor->material->map_transform.m[0][0] = 1.f;
    } else {
        pActor->material->map_transform.m[0][0] = -1.f;
    }
    BrMaterialUpdate(pActor->material, BR_MATU_ALL);
    BrZbModelRender(pActor, pModel, pMaterial, pStyle, BrOnScreenCheck(&pModel->bounds), 0);
}

// IDA: void __cdecl InitPeds()
void InitPeds() {
    LOG_TRACE("()");

    gPed_model = BrModelAllocate(NULL, 4, 2);
    gPed_model->faces[0].vertices[0] = 0;
    gPed_model->faces[0].vertices[1] = 2;
    gPed_model->faces[0].vertices[2] = 1;
    gPed_model->faces[0].material = NULL;
    gPed_model->faces[0].smoothing = 1;
    gPed_model->faces[1].vertices[0] = 0;
    gPed_model->faces[1].vertices[1] = 3;
    gPed_model->faces[1].vertices[2] = 2;
    gPed_model->faces[1].material = NULL;
    gPed_model->faces[1].smoothing = 1;
    BrVector3Set(&gPed_model->vertices[0].p, -0.5f, 0.0f, 0.0f);
    BrVector2Set(&gPed_model->vertices[0].map, 0.0001f, 0.9999f);
    BrVector3Set(&gPed_model->vertices[1].p, -0.5f, 1.0f, 0.0f);
    BrVector2Set(&gPed_model->vertices[1].map, 0.0001f, 0.0001f);
    BrVector3Set(&gPed_model->vertices[2].p, 0.5f, 1.0f, 0.0f);
    BrVector2Set(&gPed_model->vertices[2].map, 0.9999f, 0.0001f);
    BrVector3Set(&gPed_model->vertices[3].p, 0.5f, 0.0f, 0.0f);
    BrVector2Set(&gPed_model->vertices[3].map, 0.9999f, 0.9999f);
    BrModelAdd(gPed_model);
}

// IDA: void __usercall MungeModelSize(br_actor *pActor@<EAX>, br_scalar pScaling_factor)
void MungeModelSize(br_actor* pActor, br_scalar pScaling_factor) {
    br_pixelmap* the_pix;
    br_scalar half_width;
    br_scalar half_height;
    LOG_TRACE("(%p, %f)", pActor, pScaling_factor);

    the_pix = pActor->material->colour_map;
    half_width = (the_pix->width / 2) * pScaling_factor * gPed_scale_factor;
    half_height = (the_pix->height / 2) * pScaling_factor * gPed_scale_factor;
    PedModelUpdate(pActor->model,
        -half_width, -half_height,
        -half_width, half_height,
        half_width, half_height,
        half_width, -half_height);
}

// IDA: int __usercall BurstPedestrian@<EAX>(tPedestrian_data *pPedestrian@<EAX>, float pSplattitudinalitude, int pAllow_explosion)
int BurstPedestrian(tPedestrian_data* pPedestrian, float pSplattitudinalitude, int pAllow_explosion) {
    int i;
    int j;
    int gib_index;
    int next_gib_index;
    int size_threshold;
    int current_size;
    int max_size;
    int gib_count;
    int exploded;
    tPed_gib* the_ped_gib;
    br_scalar min_speed;
    br_scalar max_speed;
    tU32 the_time;
    LOG_TRACE("(%p, %f, %d)", pPedestrian, pSplattitudinalitude, pAllow_explosion);

    #if defined(DETHRACE_FIX_BUGS)
        min_speed = 0;
        max_speed = 0;
    #endif
    exploded = 0;
    for (i = 0; i < COUNT_OF(gPed_gib_counts); i++) {
        for (j = 0; j < gPed_size_counts[i]; j++) {
            gPed_gib_counts[i][j] = 0;
        }
    }

    the_time = GetTotalTime();
    size_threshold = COUNT_OF(gMin_ped_gib_speeds) - 1;
    for (i = 1; i < COUNT_OF(gMin_ped_gib_speeds); i++) {
        if (gMin_ped_gib_speeds[i] <= pSplattitudinalitude) {
            size_threshold = i;
            break;
        }
    }
    if (gExploding_pedestrians) {
        size_threshold = 1;
    } else if (size_threshold == 1 && (!pAllow_explosion || !PercentageChance(50))) {
        size_threshold = 2;
    }
    gib_count = pSplattitudinalitude * 10000.f;
    if (gib_count > 15) {
        gib_count = 15;
    }
    gib_index = 0;
    the_ped_gib = &gPed_gibs[gib_index];
    if (size_threshold == 1) {
        while (the_ped_gib->size >= 0) {
            gib_index++;
            the_ped_gib++;
            if (gib_index >= COUNT_OF(gPed_gibs)) {
                return 0;
            }
        }
        if (pPedestrian->number_of_exploding_sounds != 0) {
            DRS3StopSound(pPedestrian->last_sound);
            pPedestrian->last_sound = DRS3StartSound2(gPedestrians_outlet,
                pPedestrian->exploding_sounds[IRandomBetween(0, pPedestrian->number_of_exploding_sounds - 1)],
                1,
                -1,
                -1,
                65536.f / gPed_scale_factor,
                -1);
        }
        the_ped_gib->size = 0;
        the_ped_gib->start_time = the_time;
        the_ped_gib->end_time = -1;
        the_ped_gib->last_frame = 0;
        the_ped_gib->flip_period = 0;
        the_ped_gib->x_speed = 0.f;
        the_ped_gib->y_speed = 0.f;
        if (the_ped_gib->actor->parent != NULL) {
            BrActorRemove(the_ped_gib->actor);
        }
        BrActorAdd(pPedestrian->actor, the_ped_gib->actor);
        the_ped_gib->actor->render_style = BR_RSTYLE_FACES;
        BrMatrix34Identity(&the_ped_gib->actor->t.t.mat);
        the_ped_gib->actor->t.t.translate.t.v[V_Y] += pPedestrian->actor->t.t.mat.m[1][1] * 2.f / (pPedestrian->height2 * 3.f);
        the_ped_gib->actor->t.t.translate.t.v[V_Z] = 0.01f;
        the_ped_gib->actor->material = gPed_gib_materials[0].materials[0];
        the_ped_gib->gib_index = 0;
        the_ped_gib->parent_index = GET_PEDESTRIAN_INDEX(pPedestrian);
        MungeModelSize(the_ped_gib->actor, gExploding_ped_scale[0]);
        exploded = 1;
    }
    current_size = 4;
    j = 0;
    for (i = 0; i < gib_count; i++) {
        while (the_ped_gib->size >= 0) {
            gib_index++;
            the_ped_gib++;
            if (gib_index >= COUNT_OF(gPed_gibs)) {
                return exploded;
            }
        }
        if (j <= i && size_threshold < current_size && current_size > 1) {
            current_size--;
            j += gib_count * gPed_gib_distrib[current_size];
            min_speed = gPed_gib_speeds[current_size] * pSplattitudinalitude * .003f + .00005f;
            max_speed = gPed_gib_speeds[current_size] * pSplattitudinalitude * .2f + .00030f;
        }
        the_ped_gib->size = current_size;
        the_ped_gib->start_time = the_time + 150;
        the_ped_gib->end_time = the_time + 2000;
        the_ped_gib->flip_period = IRandomBetween(50, 200);
        the_ped_gib->x_speed = SRandomBetween(min_speed, max_speed);
        the_ped_gib->y_speed = SRandomBetween(min_speed, max_speed);
        if (PercentageChance(50)) {
            the_ped_gib->x_speed = -the_ped_gib->x_speed;
        }
        if (PercentageChance(25)) {
            the_ped_gib->y_speed = -the_ped_gib->y_speed;
        }
        if (the_ped_gib->actor->parent != NULL) {
            BrActorRemove(the_ped_gib->actor);
        }
        BrActorAdd(pPedestrian->actor, the_ped_gib->actor);
        the_ped_gib->actor->render_style = BR_RSTYLE_FACES;
        BrMatrix34Identity(&the_ped_gib->actor->t.t.mat);
        the_ped_gib->actor->t.t.translate.t.v[V_Y] += pPedestrian->actor->t.t.mat.m[1][1] / (pPedestrian->height2 * 2.f);
        do {
            next_gib_index = IRandomBetween(0, gPed_gib_materials[the_ped_gib->size].count - 1);
        } while (gPed_gib_maxes[the_ped_gib->size][next_gib_index] <= gPed_gib_counts[the_ped_gib->size][next_gib_index]);
        gPed_gib_counts[the_ped_gib->size][next_gib_index]++;
        the_ped_gib->actor->material = gPed_gib_materials[the_ped_gib->size].materials[next_gib_index];
        the_ped_gib->gib_index = next_gib_index;
        the_ped_gib->parent_index = GET_PEDESTRIAN_INDEX(pPedestrian);
        MungeModelSize(the_ped_gib->actor, .0023f);
    }
    pPedestrian->last_sound = DRS3StartSound3D(gPedestrians_outlet,
        4020,
        &pPedestrian->pos,
        &gZero_v__pedestrn,
        1,
        255,
        65536,
        -1);
    PipeSingleSound(gPedestrians_outlet, 4020, 255, 0, 65536, &pPedestrian->pos);
    return exploded;
}

// IDA: void __cdecl ResetAllPedGibs()
void ResetAllPedGibs() {
    int i;
    tPed_gib* the_ped_gib;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gPed_gibs); i++) {
        the_ped_gib = &gPed_gibs[i];
        if (the_ped_gib->size >= 0) {
            the_ped_gib->size = -1;
            if (the_ped_gib->actor->parent != NULL) {
                BrActorRemove(the_ped_gib->actor);
            }
            the_ped_gib->actor->parent = NULL;
            the_ped_gib->actor->render_style = BR_RSTYLE_NONE;
        }
    }
}

// IDA: void __usercall AdjustPedGib(int pIndex@<EAX>, int pSize@<EDX>, int pGib_index@<EBX>, int pPed_index@<ECX>, br_matrix34 *pTrans)
void AdjustPedGib(int pIndex, int pSize, int pGib_index, int pPed_index, br_matrix34* pTrans) {
    tPed_gib* the_ped_gib;
    LOG_TRACE("(%d, %d, %d, %d, %p)", pIndex, pSize, pGib_index, pPed_index, pTrans);

    the_ped_gib = &gPed_gibs[pIndex];
    the_ped_gib->size = pSize;
    the_ped_gib->gib_index = pGib_index;
    if (the_ped_gib->actor->parent != NULL) {
        BrActorRemove(the_ped_gib->actor);
    }
    BrActorAdd(gPedestrian_array[pPed_index].actor, the_ped_gib->actor);
    the_ped_gib->actor->t.t.mat = *pTrans;
    the_ped_gib->actor->render_style = BR_RSTYLE_FACES;
    the_ped_gib->actor->material = gPed_gib_materials[gPed_gibs[pIndex].size].materials[pGib_index];
    MungeModelSize(the_ped_gib->actor, 0.0023f);
}

// IDA: void __usercall MungePedGibs(tU32 pFrame_period@<EAX>)
void MungePedGibs(tU32 pFrame_period) {
    int i;
    int frame;
    tPed_gib* the_ped_gib;
    br_scalar s_frame_period;
    tU32 the_time;
    tPedestrian_data* pedestrian;
    LOG_TRACE("(%d)", pFrame_period);

    StartPipingSession(ePipe_chunk_ped_gib);
    the_time = GetTotalTime();
    for (i = 0; i < COUNT_OF(gPed_gibs); i++) {
        the_ped_gib = &gPed_gibs[i];
        if (the_ped_gib->size > 0) {
            if (the_time >= the_ped_gib->end_time) {
                the_ped_gib->size = -1;
                if (the_ped_gib->actor->parent != NULL) {
                    BrActorRemove(the_ped_gib->actor);
                }
                the_ped_gib->actor->parent = NULL;
                the_ped_gib->actor->render_style = BR_RSTYLE_NONE;
            } else if (the_time >= the_ped_gib->start_time) {
                pedestrian = &gPedestrian_array[the_ped_gib->parent_index];
                the_ped_gib->actor->render_style = BR_RSTYLE_FACES;
                if ((the_time / the_ped_gib->flip_period) % 2 != 0) {
                    the_ped_gib->actor->t.t.mat.m[0][0] = 1.f / pedestrian->width;
                    the_ped_gib->actor->t.t.mat.m[1][1] = 1.f / pedestrian->height2;
                } else {
                    the_ped_gib->actor->t.t.mat.m[0][0] = -1.f / pedestrian->width;
                    the_ped_gib->actor->t.t.mat.m[1][1] = -1.f / pedestrian->height2;
                }

                the_ped_gib->actor->t.t.translate.t.v[V_X] += the_ped_gib->x_speed * pFrame_period / pedestrian->width;
                the_ped_gib->actor->t.t.translate.t.v[V_Y] += the_ped_gib->y_speed * pFrame_period / pedestrian->height2;
                the_ped_gib->y_speed -= gFrame_period * gGravity_multiplier * 1.420289855072464e-6;
                AddPedGibToPipingSession(i, &the_ped_gib->actor->t.t.mat, the_ped_gib->size, the_ped_gib->gib_index, the_ped_gib->parent_index);
            }
        } else if (the_ped_gib->size == 0) {
            frame = (the_time - the_ped_gib->start_time) / 100 - 1;
            if (frame < 0) {
                frame = 0;
            } else if (frame > the_ped_gib->last_frame + 1) {
                frame = the_ped_gib->last_frame + 1;
            }
            the_ped_gib->last_frame = frame;
            if (frame < gPed_gib_materials[0].count) {
                the_ped_gib->actor->t.t.mat.m[0][0] = 1.f / gPedestrian_array[the_ped_gib->parent_index].width;
                the_ped_gib->actor->t.t.mat.m[1][1] = 1.f / gPedestrian_array[the_ped_gib->parent_index].height2;
                the_ped_gib->actor->material = gPed_gib_materials[0].materials[frame];
                MungeModelSize(the_ped_gib->actor, gExploding_ped_scale[frame]);
                if (frame == 1 && the_ped_gib->actor->t.t.translate.t.v[V_Z] == 0.01f) {
                    the_ped_gib->actor->t.t.translate.t.v[V_Z] = -0.01f;
                }
                AddPedGibToPipingSession(i, &the_ped_gib->actor->t.t.mat, the_ped_gib->size, frame, the_ped_gib->parent_index);
            } else {
                the_ped_gib->size = -1;
                BrActorRemove(the_ped_gib->actor);
                the_ped_gib->actor->parent = NULL;
                the_ped_gib->actor->render_style = BR_RSTYLE_NONE;
            }
        }
    }
    EndPipingSession();
}

// IDA: void __usercall KillPedestrian(tPedestrian_data *pPedestrian@<EAX>)
void KillPedestrian(tPedestrian_data* pPedestrian) {
    LOG_TRACE("(%p)", pPedestrian);

    if (pPedestrian->hit_points != -100) {
        if (pPedestrian->ref_number < 100) {
            gProgram_state.peds_killed++;
        }
        pPedestrian->hit_points = -100;
        if (IsActionReplayAvailable()) {
            AddPedestrianToPipingSession(
                GET_PEDESTRIAN_INDEX(pPedestrian),
                &pPedestrian->actor->t.t.mat,
                pPedestrian->current_action,
                pPedestrian->current_frame,
                pPedestrian->hit_points,
                pPedestrian->done_initial,
                (pPedestrian->actor->parent == gDont_render_actor) ? -1 : pPedestrian->killers_ID,
                pPedestrian->spin_period,
                pPedestrian->jump_magnitude,
                &pPedestrian->offset);
        }
    }
    if (pPedestrian->actor->parent != gDont_render_actor) {
        pPedestrian->actor->render_style = BR_RSTYLE_NONE;
    }
}

// IDA: void __usercall CalcPedWidthNHeight(tPedestrian_data *pPedestrian@<EAX>, br_pixelmap *pPixelmap@<EDX>, br_scalar *pHeight@<EBX>, br_scalar *pWidth@<ECX>)
void CalcPedWidthNHeight(tPedestrian_data* pPedestrian, br_pixelmap* pPixelmap, br_scalar* pHeight, br_scalar* pWidth) {
    br_pixelmap* reference_pixel_bastard;
    br_scalar scale_to_use;
    LOG_TRACE("(%p, %p, %p, %p)", pPedestrian, pPixelmap, pHeight, pWidth);

    if (pPedestrian->ref_number < 100) {
        scale_to_use = gPed_scale_factor;
    } else {
        scale_to_use = 1.0f;
    }
    reference_pixel_bastard = pPedestrian->sequences->frames[0].pixelmap;
    *pHeight = (float)pPixelmap->height
        / (float)reference_pixel_bastard->height
        * pPedestrian->height
        * scale_to_use;
    *pWidth = (float)pPixelmap->width / (float)pPixelmap->height * *pHeight;
}

// IDA: int __usercall PedestrianNextInstruction@<EAX>(tPedestrian_data *pPedestrian@<EAX>, float pDanger_level, int pPosition_explicitly, int pMove_pc)
int PedestrianNextInstruction(tPedestrian_data* pPedestrian, float pDanger_level, int pPosition_explicitly, int pMove_pc) {
    tPedestrian_instruction* instruction;
    float most_dangerous;
    float chance_value;
    int result;
    int start_index;
    int end_index;
    int i;
    int j;
    int choice;
    int the_marker_ref;
    LOG_TRACE("(%p, %f, %d, %d)", pPedestrian, pDanger_level, pPosition_explicitly, pMove_pc);

    if (pPedestrian->number_of_instructions <= 1) {
        pPedestrian->current_instruction = 0;
        instruction = &pPedestrian->instruction_list[0];
        pPedestrian->to_pos = instruction->data.point_data.position;
        BrVector3Set(&pPedestrian->direction, 1.f, 0.f, 0.f);
        if (pPosition_explicitly) {
            pPedestrian->pos = pPedestrian->actor->t.t.translate.t = pPedestrian->to_pos;
        }
        return 0;
    }
    result = 0;
    if (pMove_pc) {
        pPedestrian->current_instruction += pPedestrian->instruction_direction;
        if (pPedestrian->current_instruction >= pPedestrian->number_of_instructions) {
            pPedestrian->current_instruction = 0;
            result = 1;
        } else if (pPedestrian->current_instruction < 0) {
            pPedestrian->current_instruction = pPedestrian->number_of_instructions - 1;
            result = 1;
        }
    }
    instruction = &pPedestrian->instruction_list[pPedestrian->current_instruction];
    pPedestrian->irreversable = 0;
    if (instruction->type != ePed_instruc_point && instruction->type != ePed_instruc_reverse) {
        KillPedestrian(pPedestrian);
        return 0;
    }

    switch (instruction->type) {

    case ePed_instruc_xpoint:
        pPedestrian->irreversable = 1;
        // fall through
    case ePed_instruc_point:
        BrVector3Copy(&pPedestrian->to_pos, &instruction->data.point_data.position);
        BrVector3Sub(&pPedestrian->direction, &pPedestrian->to_pos, &pPedestrian->actor->t.t.translate.t);
        if (pPedestrian->to_pos.v[V_Y] >= 500.f) {
            pPedestrian->direction.v[V_Y] = 0.f;
        }
        if (Vector3IsZero(&pPedestrian->direction) && instruction != gInitial_instruction) {
            if (gInitial_instruction == NULL) {
                gInitial_instruction = instruction;
            }
            PedestrianNextInstruction(pPedestrian, pDanger_level, pPosition_explicitly, 1);
        } else {
            BrVector3Normalise(&pPedestrian->direction, &pPedestrian->direction);
            if (pPosition_explicitly) {
                pPedestrian->pos = pPedestrian->actor->t.t.translate.t = pPedestrian->to_pos;
            }
        }
        break;
    case ePed_instruc_bchoice:
    case ePed_instruc_fchoice:
        if ((instruction->type == ePed_instruc_fchoice && pPedestrian->instruction_direction > 0)
            || (instruction->type == ePed_instruc_bchoice && pPedestrian->instruction_direction < 0)) {
            most_dangerous = -1.f;
#if defined(DETHRACE_FIX_BUGS)
            start_index = 0;
            end_index = 0;
#endif
            for (i = 0; i < instruction->data.choice_data.number_of_choices; i++) {
                if (instruction->data.choice_data.choices[i].danger_level <= pDanger_level) {
                    if (instruction->data.choice_data.choices[i].danger_level > most_dangerous) {
                        most_dangerous = instruction->data.choice_data.choices[i].danger_level;
                        start_index = i;
                        end_index = i;
                    } else if (instruction->data.choice_data.choices[i].danger_level == most_dangerous) {
                        end_index = i;
                    }
                }
            }
            pPedestrian->current_instruction = -1;
            chance_value = FRandomBetween(0.f, 99.f);
            choice = start_index;
            for (i = start_index; i <= end_index; i++) {
                chance_value -= instruction->data.choice_data.choices[i].percentage_chance;
                if (chance_value < 0.f) {
                    choice = i;
                    break;
                }
            }
            for (i = 0; i < pPedestrian->number_of_instructions; i++) {
                if (((pPedestrian->instruction_list[i].type == ePed_instruc_bmarker && pPedestrian->instruction_direction < 0)
                        || (pPedestrian->instruction_list[i].type == ePed_instruc_fmarker && pPedestrian->instruction_direction > 0))) {
                    if (pPedestrian->instruction_list[i].data.marker_data.marker_ref == instruction->data.choice_data.choices[choice].marker_ref) {
                        pPedestrian->current_instruction = i;
                        break;
                    }
                }
            }
            if (pPedestrian->current_instruction < 0) {
                FatalError(kFatalError_PedInstructionMarkerNotFound);
            }
        }
        result = PedestrianNextInstruction(pPedestrian, pDanger_level, pPosition_explicitly, 1);
        break;
    case ePed_instruc_dead:
        KillPedestrian(pPedestrian);
        break;
    case ePed_instruc_bmarker:
        if (pPedestrian->instruction_direction > 0) {
            for (i = 0; i < pPedestrian->number_of_instructions; i++) {
                if (pPedestrian->instruction_list[i].type == ePed_instruc_bchoice) {
                    for (j = 0; j < pPedestrian->instruction_list[i].data.choice_data.number_of_choices; j++) {
                        if (pPedestrian->instruction_list[i].data.choice_data.choices[j].marker_ref == pPedestrian->instruction_list[pPedestrian->current_instruction].data.marker_data.marker_ref) {
                            pPedestrian->current_instruction = i;
                        }
                    }
                }
            }
        }
        result = PedestrianNextInstruction(pPedestrian, pDanger_level, pPosition_explicitly, 1);
        break;
    case ePed_instruc_fmarker:
        if (pPedestrian->instruction_direction < 0) {
            for (i = 0; i < pPedestrian->number_of_instructions; i++) {
                if (pPedestrian->instruction_list[i].type == ePed_instruc_fchoice) {
                    for (j = 0; j < pPedestrian->instruction_list[i].data.choice_data.number_of_choices; j++) {
                        if (pPedestrian->instruction_list[i].data.choice_data.choices[j].marker_ref == pPedestrian->instruction_list[pPedestrian->current_instruction].data.marker_data.marker_ref) {
                            pPedestrian->current_instruction = i;
                        }
                    }
                }
            }
        }
        result = PedestrianNextInstruction(pPedestrian, pDanger_level, pPosition_explicitly, 1);
        break;
    case ePed_instruc_baction:
        if (pPedestrian->instruction_direction < 0) {
            pPedestrian->current_action_mode = instruction->data.action_data.action_index;
        }
        result = PedestrianNextInstruction(pPedestrian, pDanger_level, pPosition_explicitly, 1);
        break;
    case ePed_instruc_faction:
        if (pPedestrian->instruction_direction > 0) {
            pPedestrian->current_action_mode = instruction->data.action_data.action_index;
        }
        result = PedestrianNextInstruction(pPedestrian, pDanger_level, pPosition_explicitly, 1);
        break;
    case ePed_instruc_reverse:
        pPedestrian->instruction_direction = -pPedestrian->instruction_direction;
        PedestrianNextInstruction(pPedestrian, pDanger_level, pPosition_explicitly, 1);
        result = 1;
        break;
    }
    return result;
}

// IDA: void __usercall MungePedestrianSequence(tPedestrian_data *pPedestrian@<EAX>, int pAction_changed@<EDX>)
void MungePedestrianSequence(tPedestrian_data* pPedestrian, int pAction_changed) {
    tPedestrian_action* the_action;
    int i;
    int the_sequence;
    int current_looping;
    br_scalar ped_movement_angle;
    float heading_difference;
    tPedestrian_sequence* sequence_ptr;
    LOG_TRACE("(%p, %d)", pPedestrian, pAction_changed);

    if (pPedestrian->ref_number < 100) {
        ped_movement_angle = FastScalarArcTan2(pPedestrian->direction.v[V_X], pPedestrian->direction.v[V_Z]);
        if (ped_movement_angle < pPedestrian->car_to_ped) {
            ped_movement_angle += 360.f;
        }
        heading_difference = ped_movement_angle - pPedestrian->car_to_ped;
    } else {
        heading_difference = gCamera_to_horiz_angle;
    }
    for (i = 0; i < pPedestrian->action_list[pPedestrian->current_action].number_of_bearings; i++) {
        if (heading_difference <= pPedestrian->action_list[pPedestrian->current_action].sequences[i].max_bearing) {
            the_sequence = pPedestrian->action_list[pPedestrian->current_action].sequences[i].sequence_index;
            if (pPedestrian->current_sequence == the_sequence) {
                break;
            }
            sequence_ptr = &pPedestrian->sequences[the_sequence];
            current_looping = pPedestrian->sequences[pPedestrian->current_sequence].looping_frame_start;
            if (pAction_changed || current_looping > pPedestrian->current_frame) {
                pPedestrian->current_frame = -1;
                pPedestrian->done_initial = 0;
            } else if (pPedestrian->current_action != pPedestrian->fatal_car_impact_action
                && pPedestrian->current_action != pPedestrian->fatal_ground_impact_action
                && pPedestrian->current_action != pPedestrian->giblets_action) {
                pPedestrian->current_frame += sequence_ptr->looping_frame_start - current_looping - 1;
                pPedestrian->done_initial = 1;
            } else if (pPedestrian->current_frame >= sequence_ptr->number_of_frames) {
                pPedestrian->current_frame = sequence_ptr->number_of_frames - 1;
            }
            pPedestrian->last_frame = 0;
            pPedestrian->current_sequence = the_sequence;
            break;
        }
    }
}

// IDA: void __usercall DetachPedFromCar(tPedestrian_data *pPedestrian@<EAX>)
void DetachPedFromCar(tPedestrian_data* pPedestrian) {
    LOG_TRACE("(%p)", pPedestrian);

    if (pPedestrian->actor->parent != gDont_render_actor) {
        BrActorRelink(gDont_render_actor, pPedestrian->actor);
        pPedestrian->actor->render_style = BR_RSTYLE_NONE;
        pPedestrian->mid_air = 1;
        if (pPedestrian->to_pos.v[V_Y] < 500.f) {
            pPedestrian->to_pos.v[V_Y] += 1000.4f;
        }
        BrVector3Set(&pPedestrian->offset, 0.f, 0.f, 0.f);
    }
}

// IDA: void __usercall SetPedPos(tPedestrian_data *pPedestrian@<EAX>)
void SetPedPos(tPedestrian_data* pPedestrian) {
    br_vector3 temp_v;
    LOG_TRACE("(%p)", pPedestrian);

    if (pPedestrian->actor->parent == gDont_render_actor) {
        pPedestrian->pos = pPedestrian->actor->t.t.translate.t;
    } else {
        BrMatrix34ApplyP(&pPedestrian->pos,
            &pPedestrian->actor->t.t.translate.t,
            &pPedestrian->actor->parent->t.t.mat);
    }
}

// IDA: void __usercall DetachPedActorFromCar(br_actor *pActor@<EAX>)
void DetachPedActorFromCar(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);

    DetachPedFromCar(ActorToPedestrianData(pActor));
    SetPedPos(ActorToPedestrianData(pActor));
}

// IDA: void __usercall MungePedestrianFrames(tPedestrian_data *pPedestrian@<EAX>)
void MungePedestrianFrames(tPedestrian_data* pPedestrian) {
    tPedestrian_sequence* the_sequence;
    float f_the_time;
    float frame_period;
    int frame_offset;
    int number_of_frames;
    int new_frame;
    tU32 the_time;
    LOG_TRACE("(%p)", pPedestrian);

    the_time = GetTotalTime();
    f_the_time = the_time;
    the_sequence = &pPedestrian->sequences[pPedestrian->current_sequence];
    if (pPedestrian->done_initial) {
        frame_offset = the_sequence->number_of_frames - the_sequence->looping_frame_start;
        if (frame_offset == 0 && pPedestrian->current_frame < the_sequence->looping_frame_start) {
            pPedestrian->current_frame = the_sequence->looping_frame_start - 1;
        }
    } else {
        frame_offset = the_sequence->looping_frame_start;
    }
    switch (the_sequence->frame_rate_type) {
    case ePed_frame_fixed:
        frame_period = 0.f;
        break;
    case ePed_frame_speed:
        if (pPedestrian->done_initial) {
            frame_period = the_sequence->frame_rate_factor1 / pPedestrian->current_speed * gPed_scale_factor;
        } else {
            frame_period = 1000.f / the_sequence->frame_rate_factor2;
        }
        break;
    case ePed_frame_variable:
        frame_period = 1000.f / FRandomBetween(the_sequence->frame_rate_factor1, the_sequence->frame_rate_factor2);
        break;
    default:
        TELL_ME_IF_WE_PASS_THIS_WAY();
        break;
    }
    if (frame_period == 0.f) {
        pPedestrian->current_frame = 0;
    } else if (!pPedestrian->done_initial) {
        if (f_the_time - pPedestrian->last_frame >= frame_period) {
            if (pPedestrian->actor->parent != gDont_render_actor
                || !pPedestrian->mid_air
                || pPedestrian->current_frame < frame_offset - 2
                || (pPedestrian->current_action != pPedestrian->fatal_ground_impact_action && pPedestrian->current_action != pPedestrian->fatal_car_impact_action)) {
                if (pPedestrian->reverse_frames) {
                    pPedestrian->current_frame--;
                    if (pPedestrian->current_frame < 0) {
                        pPedestrian->reverse_frames = 0;
                        ChangeActionTo(pPedestrian, 0, 1);
                    }
                } else {
                    pPedestrian->current_frame++;
                }
            }
            if (pPedestrian->last_frame == 0 || the_sequence->frame_rate_type == ePed_frame_variable) {
                pPedestrian->last_frame = the_time;
            } else {
                pPedestrian->last_frame += frame_period;
            }
        }
        if (gPedestrian_harvest
            && pPedestrian->ref_number < 100
            && pPedestrian->actor->parent != gDont_render_actor
            && pPedestrian->current_action == pPedestrian->fatal_car_impact_action
            && pPedestrian->current_frame >= 2) {
            pPedestrian->current_frame = 0;
            if (PercentageChance(20)
                && pPedestrian->number_of_actions >= 2
                && pPedestrian->action_list[1].number_of_sounds != 0) {
                DRS3StartSound3D(gPedestrians_outlet, pPedestrian->action_list[1].sounds[0],
                    &pPedestrian->pos, &gZero_v__pedestrn, 1, 255, gZombie_factor * 65536.f, -1);
            }
        } else if (frame_offset <= pPedestrian->current_frame) {
            pPedestrian->done_initial = 1;
            if (the_sequence->number_of_frames == the_sequence->looping_frame_start) {
                pPedestrian->current_frame--;
            }
            if (pPedestrian->current_action != pPedestrian->fatal_car_impact_action) {
                if (pPedestrian->current_action != pPedestrian->non_fatal_car_impact_action
                    && pPedestrian->current_action != pPedestrian->non_fatal_ground_impact_action) {
                    MungePedestrianFrames(pPedestrian);
                } else {
                    ChangeActionTo(pPedestrian, pPedestrian->after_impact_action, 1);
                    MungePedestrianPath(pPedestrian, gDanger_level, &gDanger_direction);
                }
            } else {
                if (pPedestrian->actor->parent != gDont_render_actor) {
                    pPedestrian->actor->t.t.translate.t = pPedestrian->offset;
                }
                DetachPedFromCar(pPedestrian);
                pPedestrian->spin_period = 0.f;
                if (pPedestrian->ref_number >= 100) {
                    KillPedestrian(pPedestrian);
                }
            }
            return;
        }
    } else if (frame_offset != 0) {
        if (f_the_time - pPedestrian->last_frame >= frame_period) {
            if (pPedestrian->reverse_frames) {
                pPedestrian->current_frame--;
                if (pPedestrian->current_frame < 0) {
                    pPedestrian->reverse_frames = 0;
                    ChangeActionTo(pPedestrian, 0, 1);
                }
            } else {
                pPedestrian->current_frame++;
                if (pPedestrian->last_frame == 0 || the_sequence->frame_rate_type == ePed_frame_variable) {
                    pPedestrian->last_frame = the_time;
                } else {
                    pPedestrian->last_frame += frame_period;
                }
            }
        }
        if (pPedestrian->current_frame >= the_sequence->looping_frame_start + frame_offset) {
            pPedestrian->current_frame = the_sequence->looping_frame_start;
        }
    }
    new_frame = pPedestrian->current_frame;
    pPedestrian->colour_map = the_sequence->frames[MAX(0, new_frame)].pixelmap;
}

// IDA: void __usercall MungePedModel(tPedestrian_data *pPedestrian@<EAX>)
void MungePedModel(tPedestrian_data* pPedestrian) {
    br_vector3 old_pos;
    br_vector3 delta_pos;
    br_scalar height_over2;
    br_matrix34 mat;
    br_scalar x_offset;
    br_scalar y_offset;
    br_scalar temp_scalar;
    br_scalar* component_address;
    br_pixelmap* current_pixel_bastard;
    br_pixelmap* reference_pixel_bastard;
    br_vertex* vertices;
    int model_changed;
    tPed_frame_info* the_frame;
    tCar_spec* murderer;
    br_actor* old_parent;
    LOG_TRACE("(%p)", pPedestrian);

    old_parent = NULL;
    CalcPedWidthNHeight(pPedestrian, pPedestrian->colour_map, &pPedestrian->height2, &pPedestrian->width);
    the_frame = &pPedestrian->sequences[pPedestrian->current_sequence].frames[MAX(pPedestrian->current_frame, 0)];
    if (pPedestrian->ref_number >= 100 && pPedestrian->current_action == pPedestrian->fatal_car_impact_action) {
        x_offset = the_frame->offset.v[V_X] * 2.0f;
        y_offset = the_frame->offset.v[V_Y];
        pPedestrian->height2 *= 2.0f;
        pPedestrian->width *= 2.0f;
    } else {
        x_offset = the_frame->offset.v[V_X];
        y_offset = the_frame->offset.v[V_Y];
    }
    pPedestrian->flipped = the_frame->flipped;
    if (pPedestrian->actor->parent != gDont_render_actor) {
        old_parent = pPedestrian->actor->parent;
        BrActorRelink(gDont_render_actor, pPedestrian->actor);
    }
    BrMatrix34Scale(&pPedestrian->actor->t.t.mat, pPedestrian->width, pPedestrian->height2, 1.0f);
    if (pPedestrian->spin_period != 0.0f && pPedestrian->actor->model != NULL) {
        height_over2 = pPedestrian->height2 / 2.0f;
        BrMatrix34PostTranslate(&pPedestrian->actor->t.t.mat, 0.0f, -height_over2, 0.0f);
        if (pPedestrian->spin_period >= 0.0f) {
            temp_scalar = (float)(GetTotalTime() - pPedestrian->last_action_change) / pPedestrian->spin_period * 360.0f;
        } else {
            temp_scalar = ((float)(GetTotalTime() - pPedestrian->last_action_change) / pPedestrian->spin_period * 360.0f + 360.0f);
        }
        DRMatrix34PostRotateZ(&pPedestrian->actor->t.t.mat, BrDegreeToAngle(temp_scalar));
        BrMatrix34PostTranslate(&pPedestrian->actor->t.t.mat, 0.0f, height_over2, 0.0f);
    }
    temp_scalar = FastScalarArcTan2Angle(gCamera_to_world.m[2][0], gCamera_to_world.m[2][2]);
    DRMatrix34PostRotateY(&pPedestrian->actor->t.t.mat, temp_scalar);
    if (old_parent != NULL
        && pPedestrian->current_action == pPedestrian->fatal_car_impact_action
        && !pPedestrian->done_initial
        && pPedestrian->actor->model != NULL) {

        y_offset += FastScalarSin(((br_scalar)MAX(pPedestrian->current_frame, 0)
                        / (br_scalar)(pPedestrian->sequences[pPedestrian->current_sequence].looping_frame_start - 1) * 180.0f))
            * pPedestrian->jump_magnitude;
    }
    if (old_parent != NULL) {
        BrMatrix34PostTranslate(&pPedestrian->actor->t.t.mat,
            old_parent->t.t.translate.t.v[V_X] + x_offset,
            old_parent->t.t.translate.t.v[V_Y] + y_offset,
            old_parent->t.t.translate.t.v[V_Z]);
        BrActorRelink(old_parent, pPedestrian->actor);
        BrVector3Accumulate(&pPedestrian->actor->t.t.translate.t, &pPedestrian->offset);
    } else if (pPedestrian->spin_period == 0.0f) {
        BrMatrix34PostTranslate(&pPedestrian->actor->t.t.mat,
            pPedestrian->pos.v[V_X] + x_offset - pPedestrian->offset.v[V_X],
            pPedestrian->pos.v[V_Y] + y_offset - pPedestrian->offset.v[V_Y],
            pPedestrian->pos.v[V_Z]);
        BrVector3Set(&pPedestrian->offset, x_offset, y_offset, 0.0f);
    } else {
        BrMatrix34PostTranslate(&pPedestrian->actor->t.t.mat,
            pPedestrian->offset.v[V_X] + x_offset,
            pPedestrian->offset.v[V_Y] + y_offset,
            pPedestrian->offset.v[V_Z]);
    }
    gCurrent_lollipop_index = AddToLollipopQueue(pPedestrian->actor, gCurrent_lollipop_index);
}

// IDA: void __usercall ChangeActionTo(tPedestrian_data *pPedestrian@<EAX>, int pAction_index@<EDX>, int pRedo_frames_etc@<EBX>)
void ChangeActionTo(tPedestrian_data* pPedestrian, int pAction_index, int pRedo_frames_etc) {
    tPedestrian_action* the_action;
    int the_sound;
    tU32 the_pitch;
    LOG_TRACE("(%p, %d, %d)", pPedestrian, pAction_index, pRedo_frames_etc);

    if (pAction_index == pPedestrian->current_action || pAction_index >= pPedestrian->number_of_actions) {
        return;
    }

    pPedestrian->last_action_change = GetTotalTime();
    pPedestrian->current_action = pAction_index;
    pPedestrian->current_frame = -1;
    pPedestrian->last_frame = 0;
    pPedestrian->done_initial = 0;
    if (pRedo_frames_etc) {
        MungePedestrianSequence(pPedestrian, 1);
        MungePedestrianFrames(pPedestrian);
        MungePedModel(pPedestrian);
    }
    the_action = &pPedestrian->action_list[pAction_index];
    if (!gPed_sound_disable && the_action->number_of_sounds != 0) {
        if (pAction_index == pPedestrian->last_sound_action && GetTotalTime() - pPedestrian->last_sound_make < 4000) {
            return;
        }
        DRS3StopSound(pPedestrian->last_sound);
        the_sound = the_action->sounds[IRandomBetween(0, the_action->number_of_sounds - 1)];
        the_pitch = 65536.f / gPed_scale_factor * gZombie_factor;
        pPedestrian->last_sound = DRS3StartSound3D(gPedestrians_outlet, the_sound, &pPedestrian->pos, &gZero_v__pedestrn, 1, 255, the_pitch, -1);
        pPedestrian->last_sound_action = pAction_index;
        pPedestrian->last_sound_make = GetTotalTime();
        PipeSingleSound(gPedestrians_outlet, the_sound, 255, 0, the_pitch, &pPedestrian->pos);
    }
}

// IDA: int __usercall MungePedestrianAction@<EAX>(tPedestrian_data *pPedestrian@<EAX>, float pDanger_level)
int MungePedestrianAction(tPedestrian_data* pPedestrian, float pDanger_level) {
    tU32 time_diff;
    int i;
    int choice;
    int start_index;
    int end_index;
    float chance_value;
    float most_dangerous;
    LOG_TRACE("(%p, %f)", pPedestrian, pDanger_level);

    if (pPedestrian->current_action == pPedestrian->fatal_car_impact_action
        || pPedestrian->current_action == pPedestrian->non_fatal_car_impact_action
        || pPedestrian->current_action == pPedestrian->fatal_ground_impact_action
        || pPedestrian->current_action == pPedestrian->non_fatal_car_impact_action
        || pPedestrian->current_action == pPedestrian->giblets_action
        || pPedestrian->mid_air) {
        return 0;
    }
    time_diff = GetTotalTime() - pPedestrian->last_action_change;
    if (pPedestrian->current_action_mode >= 0) {
        ChangeActionTo(pPedestrian, pPedestrian->current_action_mode, 0);
        return 1;
    }
    most_dangerous = -1.f;
#if defined(DETHRACE_FIX_BUGS)
    start_index = 0;
    end_index = 0;
#endif
    for (i = 0; i < pPedestrian->number_of_actions; i++) {
        if (pPedestrian->action_list[i].danger_level < 999.f
            && pPedestrian->action_list[i].danger_level <= pDanger_level
            && pPedestrian->action_list[i].reaction_time <= time_diff
            && (gFlag_waving_bastard != pPedestrian || pPedestrian->current_action != 7 || pPedestrian->action_list[i].danger_level != 0.f)) {
            if (pPedestrian->action_list[i].danger_level > most_dangerous) {
                most_dangerous = pPedestrian->action_list[i].danger_level;
                start_index = i;
                end_index = i;
            } else if (pPedestrian->action_list[i].danger_level == most_dangerous) {
                end_index = i;
            }
        }
    }
    if (most_dangerous <= -1.f) {
        return 0;
    }
    chance_value = FRandomBetween(0.f, 99.f);
    choice = start_index;
    for (i = start_index; i <= end_index; i++) {
        chance_value -= pPedestrian->action_list[i].percentage_chance;
        if (chance_value < 0.f) {
            choice = i;
            break;
        }
    }
    ChangeActionTo(pPedestrian, choice, 1);
    return 1;
}

// IDA: void __cdecl MakeFlagWavingBastardWaveHisFlagWhichIsTheProbablyTheLastThingHeWillEverDo()
void MakeFlagWavingBastardWaveHisFlagWhichIsTheProbablyTheLastThingHeWillEverDo() {
    LOG_TRACE("()");

    if (gFlag_waving_bastard != NULL
        && gFlag_waving_bastard->current_action != gFlag_waving_bastard->fatal_car_impact_action
        && gFlag_waving_bastard->current_action != gFlag_waving_bastard->fatal_ground_impact_action) {
        ChangeActionTo(gFlag_waving_bastard, 7, 1);
    }
}

// IDA: void __usercall MungePedestrianPath(tPedestrian_data *pPedestrian@<EAX>, float pDanger_level, br_vector3 *pDanger_direction)
void MungePedestrianPath(tPedestrian_data* pPedestrian, float pDanger_level, br_vector3* pDanger_direction) {
    tPedestrian_action* the_action;
    float terminal_speed;
    float new_falling_speed;
    float grav_times_period;
    float ped_gravity;
    br_vector3 movement_vector;
    br_vector3 over_shoot;
    br_vector3 cast_point;
    br_scalar old_y;
    br_scalar new_y;
    int damage;
    tU32 the_pitch;
    LOG_TRACE("(%p, %f, %p)", pPedestrian, pDanger_level, pDanger_direction);

    if (gAttracted_pedestrians) {
        BrVector3Scale(pDanger_direction, pDanger_direction, -1.f);
    }
    old_y = pPedestrian->actor->t.t.translate.t.v[V_Y];
    if (pPedestrian->current_action != pPedestrian->fatal_car_impact_action
        && pPedestrian->current_action != pPedestrian->fatal_ground_impact_action
        && pPedestrian->current_action != pPedestrian->giblets_action) {
        the_action = &pPedestrian->action_list[pPedestrian->current_action];
        if (!pPedestrian->done_initial) {
            terminal_speed = the_action->initial_speed;
        } else {
            terminal_speed = the_action->looping_speed;
        }
        if (pPedestrian->current_speed < terminal_speed) {
            pPedestrian->current_speed += gFrame_period * pPedestrian->acceleration;
        }
        if (pPedestrian->current_speed > terminal_speed) {
            pPedestrian->current_speed = terminal_speed;
        }
        pPedestrian->current_speed *= gPedestrian_speed_factor;
        if (!pPedestrian->irreversable && pDanger_level != 0.f && BrVector3Dot(&pPedestrian->direction, pDanger_direction) > 0.f) {
            pPedestrian->instruction_direction = -pPedestrian->instruction_direction;
            gInitial_instruction = NULL;
            if (PedestrianNextInstruction(pPedestrian, pDanger_level, 0, 1) != 0) {
                ChangeActionTo(pPedestrian, 0, 0);
            }
        }
        if (pPedestrian->last_special_volume != NULL
            && pPedestrian->last_special_volume->viscosity_multiplier != 1.f) {
            BrVector3Scale(&movement_vector, &pPedestrian->direction, gFrame_period * pPedestrian->current_speed * 20.f / pPedestrian->last_special_volume->viscosity_multiplier);
        } else {
            BrVector3Scale(&movement_vector, &pPedestrian->direction, gFrame_period * pPedestrian->current_speed);
        }
        BrVector3Accumulate(&pPedestrian->actor->t.t.translate.t, &movement_vector);
        BrVector3Accumulate(&pPedestrian->pos, &movement_vector);
        BrVector3Sub(&over_shoot, &pPedestrian->actor->t.t.translate.t, &pPedestrian->to_pos);
        if (BrVector3Dot(&pPedestrian->direction, &over_shoot) > 0.f) {
            gInitial_instruction = NULL;
            PedestrianNextInstruction(pPedestrian, pDanger_level, 0, 1);
        }
    }

    pPedestrian->last_special_volume = FindSpecialVolume(&pPedestrian->actor->t.t.translate.t, pPedestrian->last_special_volume);
    if (pPedestrian->last_special_volume == NULL) {
        ped_gravity = 1.42029e-6f;
    } else {
        ped_gravity = pPedestrian->last_special_volume->gravity_multiplier * 1.42029e-6f;
    }
    ped_gravity *= gGravity_multiplier;
    if (gVesuvian_corpses
        && (pPedestrian->current_action == pPedestrian->fatal_car_impact_action || pPedestrian->current_action == pPedestrian->fatal_ground_impact_action)) {
        if (PercentageChance(100 / (gVesuvians_last_time != 0 ? gVesuvians_last_time : 1))) {
            BurstPedestrian(pPedestrian, .001f, 0);
        }
        gVesuvians_this_time++;
    }
    if (pPedestrian->to_pos.v[V_Y] < 500.f) {
        pPedestrian->falling_speed = 0.f;
        pPedestrian->mid_air = 0;
    } else {
        BrVector3Copy(&cast_point, &pPedestrian->actor->t.t.translate.t);
        if (cast_point.v[V_Y] >= 500.f) {
            cast_point.v[V_Y] -= 999.6f;
            pPedestrian->pos.v[V_Y] = pPedestrian->actor->t.t.translate.t.v[V_Y] = FindYVerticallyBelow2(&cast_point);
            pPedestrian->falling_speed = 0.f;
            pPedestrian->mid_air = 0;
            pPedestrian->spin_period = 0;
            BrVector3Set(&pPedestrian->offset, 0.f, 0.f, 0.f);
        } else {
            if (pPedestrian->current_speed == 0.f && !pPedestrian->mid_air) {
                pPedestrian->mid_air = 0;
            } else {
                cast_point.v[V_Y] = old_y + .4f;
                new_y = FindYVerticallyBelow2(&cast_point);
                if (new_y > old_y) {
                    pPedestrian->spin_period = 0.f;
                    BrVector3Set(&pPedestrian->offset, 0.f, 0.f, 0.f);
                    pPedestrian->mid_air = 0;
                    pPedestrian->falling_speed = 0.f;
                    pPedestrian->pos.v[V_Y] = pPedestrian->actor->t.t.translate.t.v[V_Y] = new_y;
                } else {
                    grav_times_period = ped_gravity * gFrame_period;
                    pPedestrian->falling_speed += grav_times_period;
                    pPedestrian->actor->t.t.translate.t.v[V_Y] -= gFrame_period * pPedestrian->falling_speed;
                    pPedestrian->pos.v[V_Y] = pPedestrian->actor->t.t.translate.t.v[V_Y];
                    if (pPedestrian->spin_period != 0.f) {
                        pPedestrian->offset.v[V_Y] -= gFrame_period * pPedestrian->falling_speed;
                    }
                    if (pPedestrian->hit_points > 0
                        && pPedestrian->falling_sound >= 0
                        && pPedestrian->falling_speed >= 0.0008f) {
                        DRS3StopSound(pPedestrian->last_sound);
                        the_pitch = 65536.f / gPed_scale_factor;
                        pPedestrian->last_sound = DRS3StartSound3D(gPedestrians_outlet,
                            pPedestrian->falling_sound,
                            &pPedestrian->pos,
                            &gZero_v__pedestrn,
                            1,
                            255,
                            the_pitch,
                            -1);
                        PipeSingleSound(gPedestrians_outlet,
                            pPedestrian->falling_sound,
                            255,
                            0,
                            the_pitch,
                            &pPedestrian->pos);
                        pPedestrian->falling_sound = -1;
                    }
                    if (new_y <= pPedestrian->actor->t.t.translate.t.v[V_Y]) {
                        pPedestrian->mid_air = 1;
                    } else {
                        pPedestrian->pos.v[V_Y] = pPedestrian->actor->t.t.translate.t.v[V_Y] = new_y;
                        if (pPedestrian->spin_period != 0.f) {
                            pPedestrian->offset.v[V_Y] = pPedestrian->pos.v[V_Y];
                        }
                        if (pPedestrian->hit_points < 0) {
                            BrVector3Copy(&pPedestrian->to_pos, &pPedestrian->pos);
                        }
                        damage = (pPedestrian->falling_speed - 0.0015f) * 100000.f;
                        if (damage > 0 || pPedestrian->hit_points < 0) {
                            if (pPedestrian->hit_points >= 0 && (pPedestrian->hit_points -= damage, pPedestrian->hit_points > 0)) {
                                if (pPedestrian->hit_points < damage) {
                                    ChangeActionTo(pPedestrian, pPedestrian->non_fatal_ground_impact_action, 1);
                                }
                            } else {
                                if (pPedestrian->hit_points <= -100) {
                                    pPedestrian->hit_points = -99;
                                }
                                if (pPedestrian->ref_number < 100
                                    && pPedestrian->current_action != pPedestrian->fatal_car_impact_action
                                    && pPedestrian->current_action != pPedestrian->fatal_ground_impact_action
                                    && pPedestrian->current_action != pPedestrian->giblets_action) {
                                    gProgram_state.peds_killed++;
                                    ChangeActionTo(pPedestrian, pPedestrian->fatal_ground_impact_action, 1);
                                }
                                pPedestrian->spin_period = 0.f;
                                BrVector3Set(&pPedestrian->offset, 0.f, 0.f, 0.f);
                                if (pPedestrian->mid_air) {
                                    BurstPedestrian(pPedestrian, pPedestrian->falling_speed * 3.f, 0);
                                }
                            }
                        }
                        pPedestrian->falling_speed = 0.f;
                        pPedestrian->mid_air = 0;
                    }
                }
            }
        }
    }
}

// IDA: float __usercall CalcPedestrianDangerLevel@<ST0>(tPedestrian_data *pPedestrian@<EAX>, br_vector3 *pDanger_direction@<EDX>)
float CalcPedestrianDangerLevel(tPedestrian_data* pPedestrian, br_vector3* pDanger_direction) {
    br_vector3* ped_pos;
    int i;
    float most_dangerous;
    float this_danger;
    br_scalar distance_squared;
    br_scalar car_movement_angle;
    br_scalar car_to_pedestrian_angle;
    br_scalar heading_difference;
    br_scalar camera_view_angle;
    tCar_spec* car;
    LOG_TRACE("(%p, %p)", pPedestrian, pDanger_direction);

    most_dangerous = 0.f;
    ped_pos = &pPedestrian->actor->t.t.translate.t;
    for (i = 0; i < gNum_active_cars; i++) {
        car = gActive_car_list[i];
        if (car->driver == eDriver_local_human) {
            camera_view_angle = FastScalarArcTan2(ped_pos->v[V_X] - gCamera_to_world.m[3][V_X], ped_pos->v[V_Z] - gCamera_to_world.m[3][V_Z]);
            pPedestrian->car_to_ped = camera_view_angle;
        }
        if (gBlind_pedestrians) {
            return car->keys.horn ? 100.f : 0.f;
        }
        distance_squared = (ped_pos->v[V_X] - car->pos.v[V_X]) * (ped_pos->v[V_X] - car->pos.v[V_X])
            + 10.f * (ped_pos->v[V_Y] - car->pos.v[V_Y]) * 10.f * (ped_pos->v[V_Y] - car->pos.v[V_Y])
            + (ped_pos->v[V_Z] - car->pos.v[V_Z]) * (ped_pos->v[V_Z] - car->pos.v[V_Z]);
        if (distance_squared < gMax_distance_squared) {
            car_movement_angle = FastScalarArcTan2(car->direction.v[V_X], car->direction.v[V_Z]);
            car_to_pedestrian_angle = FastScalarArcTan2(ped_pos->v[V_X] - car->pos.v[V_X], ped_pos->v[V_Z] - car->pos.v[V_Z]);
            if (car_to_pedestrian_angle > car_movement_angle) {
                car_movement_angle += 360.f;
            }
            heading_difference = car_movement_angle - car_to_pedestrian_angle;
            if (heading_difference > 180.f) {
                heading_difference = 360.f - heading_difference;
            }
            if (heading_difference < 30.f || car->speed == 0.f || car->keys.horn) {
                if (car->keys.horn) {
                    this_danger = 10.f / distance_squared;
                } else {
                    if (car->speed != 0.f) {
                        this_danger = 30.f - heading_difference + 5.f;
                    } else {
                        this_danger = 5.f;
                    }
                    this_danger = (fabsf(car->speed) + 3e-5f) * this_danger / distance_squared * 400.f;
                }
                if (this_danger > most_dangerous) {
                    most_dangerous = this_danger;
                    BrVector3Sub(pDanger_direction, &car->pos, ped_pos);
                    if (car->driver == eDriver_local_human) {
                        pPedestrian->frightened_of_us = 1;
                    } else if (!pPedestrian->mid_air) {
                        pPedestrian->frightened_of_us = 0;
                    }
                }
            }
        }
    }
    if (most_dangerous > 999.f) {
        most_dangerous = 999.f;
    }
    return most_dangerous;
}

// IDA: tPed_hit_position __usercall MoveToEdgeOfCar@<EAX>(tPedestrian_data *pPedestrian@<EAX>, tCollision_info *pCar@<EDX>, br_actor *pCar_actor@<EBX>, br_scalar pPed_x, br_scalar pPed_z, br_scalar pCar_bounds_min_x, br_scalar pCar_bounds_max_x, br_scalar pCar_bounds_min_z, br_scalar pCar_bounds_max_z, br_vector3 *pMin_ped_bounds_car, br_vector3 *pMax_ped_bounds_car)
tPed_hit_position MoveToEdgeOfCar(tPedestrian_data* pPedestrian, tCollision_info* pCar, br_actor* pCar_actor, br_scalar pPed_x, br_scalar pPed_z, br_scalar pCar_bounds_min_x, br_scalar pCar_bounds_max_x, br_scalar pCar_bounds_min_z, br_scalar pCar_bounds_max_z, br_vector3* pMin_ped_bounds_car, br_vector3* pMax_ped_bounds_car) {
    br_vector3 car_plus_ped;
    br_vector3 delta_vector;
    br_vector3 ped_move_in_car;
    br_vector3 ped_move_in_global;
    br_vector3 scaled_car_direction;
    br_vector3 scaled_ped_direction;
    br_matrix34 global_to_car;
    br_scalar z_to_use;
    br_scalar x_to_use;
    br_scalar t;
    br_scalar x;
    br_scalar z;
    tPed_hit_position result;

#ifdef DETHRACE_FIX_BUGS
    x_to_use = 0.0f;
#endif
    if (fabsf(pPedestrian->current_speed) >= fabsf(pCar->speed)) {
        BrVector3Scale(&ped_move_in_global, &pPedestrian->direction, -fabsf(pPedestrian->current_speed));
    } else {
        BrVector3Scale(&ped_move_in_global, &pCar->direction, fabsf(pCar->speed));
    }
    if (fabsf(ped_move_in_global.v[V_X]) < 5e-5f || fabsf(ped_move_in_global.v[V_Z]) < 5e-5f) {
        return ePed_hit_unknown;
    }
    BrActorToActorMatrix34(&global_to_car, gDont_render_actor, pCar_actor);
    BrMatrix34ApplyV(&ped_move_in_car, &ped_move_in_global, &global_to_car);
    if (ped_move_in_car.v[V_X] >= 0.f) {
        x = pCar_bounds_max_x - pMin_ped_bounds_car->v[V_X];
    } else {
        x = pCar_bounds_min_x - pMax_ped_bounds_car->v[V_X];
    }
    if (ped_move_in_car.v[V_Z] >= 0.f) {
        z = pCar_bounds_max_z - pMin_ped_bounds_car->v[V_Z];
    } else {
        z = pCar_bounds_min_z - pMax_ped_bounds_car->v[V_Z];
    }

    if (ped_move_in_car.v[V_Z] != 0.f) {
        t = z / ped_move_in_car.v[V_Z] * ped_move_in_car.v[V_X];
    }
    if (ped_move_in_car.v[V_Z] == 0.f || t + pPed_x < pCar_bounds_min_x || t + pPed_x > pCar_bounds_max_x) {
        if (ped_move_in_car.v[V_X] == 0.f) {
            return ePed_hit_unknown;
        }
        t = x / ped_move_in_car.v[V_X];
        z_to_use = t * ped_move_in_car.v[V_Z];
        if (z_to_use + pPed_z < pCar_bounds_min_z || z_to_use + pPed_z > pCar_bounds_max_z) {
            return ePed_hit_unknown;
        }
        x_to_use = x;
        if (ped_move_in_car.v[V_X] >= 0.f) {
            result = ePed_hit_rside;
        } else {
            result = ePed_hit_lside;
        }
    } else {
        z_to_use = z;
        if (ped_move_in_car.v[V_Z] >= 0.f) {
            result = ePed_hit_back;
        } else {
            result = ePed_hit_front;
        }
    }
    BrVector3Set(&scaled_car_direction, 1.01f * x_to_use, 0.f, 1.01f * z_to_use);
    BrMatrix34TApplyV(&scaled_ped_direction, &scaled_car_direction, &global_to_car);
    scaled_ped_direction.v[V_Y] = 0.f;
    if (pCar->speed == 0.f || gFrame_period * fabsf(pCar->speed) > BrVector3Length(&scaled_ped_direction) / 10.f) {
        BrVector3Accumulate(&pPedestrian->actor->t.t.translate.t, &scaled_ped_direction);
        BrVector3Accumulate(&pPedestrian->pos, &scaled_ped_direction);
    }
    return result;
}

// IDA: void __cdecl CheckLastPed()
void CheckLastPed() {
    LOG_TRACE("()");

    if (gNet_mode == eNet_mode_none && gProgram_state.peds_killed >= gTotal_peds) {
        NewTextHeadupSlot(4, 0, 5000, -4, GetMiscString(26));
        RaceCompleted(eRace_over_peds);
    }
}

// IDA: int __usercall BloodyWheels@<EAX>(tCar_spec *pCar@<EAX>, br_vector3 *pPed_car@<EDX>, br_scalar pSize, br_vector3 *pPed_glob)
int BloodyWheels(tCar_spec* pCar, br_vector3* pPed_car, br_scalar pSize, br_vector3* pPed_glob) {
    int wheel;
    int squish;
    br_scalar ped_m_z;
    br_scalar ped_m_x;
    br_scalar dist_sqr;
    br_scalar size_sqr;
    LOG_TRACE("(%p, %p, %f, %p)", pCar, pPed_car, pSize, pPed_glob);

    size_sqr = pSize + .05f;
    dist_sqr = size_sqr * WORLD_SCALE * size_sqr * WORLD_SCALE;
    squish = 0;
    for (wheel = 0; wheel < COUNT_OF(pCar->blood_remaining); wheel++) {
        ped_m_x = pCar->wpos[wheel].v[V_X] - pPed_car->v[V_X] * WORLD_SCALE;
        ped_m_z = pCar->wpos[wheel].v[V_Z] - pPed_car->v[V_Z] * WORLD_SCALE;
        if (pCar->blood_remaining[wheel] == 0.f && ped_m_x * ped_m_x + ped_m_z * ped_m_z < dist_sqr) {
            pCar->blood_remaining[wheel] = SRandomBetween(2.f, 8.f);
            pCar->special_start[wheel] = *pPed_glob;
            squish = 1;
        }
    }
    return squish;
}

// IDA: int __usercall FancyATossOffMate@<EAX>(tPedestrian_data *pPedestrian@<EAX>, tCollision_info *pCar@<EDX>, float pImpact_speed)
int FancyATossOffMate(tPedestrian_data* pPedestrian, tCollision_info* pCar, float pImpact_speed) {
    LOG_TRACE("(%p, %p, %f)", pPedestrian, pCar, pImpact_speed);

    return !gPedestrian_harvest
        && pPedestrian->ref_number < 100
        && (pCar->driver > 1 || ((tCar_spec*)pCar)->number_of_wheels_on_ground >= 4)
        && PercentageChance(50)
        && pImpact_speed >= .0035f;
}

// IDA: void __usercall CheckPedestrianDeathScenario(tPedestrian_data *pPedestrian@<EAX>)
void CheckPedestrianDeathScenario(tPedestrian_data* pPedestrian) {
    int norman;
    int i;
    int tossing;
    int orig_gib_flag;
    int exploded;
    int proximity_rayed;
    int fated;
    int billiards_shot;
    int credits_value;
    br_vector3* car_pos;
    br_vector3* ped_pos;
    br_vector3 min_ped_bounds;
    br_vector3 max_ped_bounds;
    br_vector3 min_ped_bounds_car;
    br_vector3 max_ped_bounds_car;
    br_vector3 ped_to_car_vector;
    br_vector3 up;
    br_vector3 zero_v;
    br_scalar attitude;
    br_scalar distance_squared;
    br_scalar ped_centre_x;
    br_scalar ped_centre_y;
    br_scalar car_bounds_min_x;
    br_scalar car_bounds_max_x;
    br_scalar car_bounds_min_z;
    br_scalar car_bounds_max_z;
    br_scalar prev_car_bounds_min_x;
    br_scalar prev_car_bounds_max_x;
    br_scalar prev_car_bounds_min_z;
    br_scalar prev_car_bounds_max_z;
    br_scalar scalar_frame_time;
    br_scalar gross_dismiss;
    br_actor* car_actor;
    br_actor* incident_actor;
    br_matrix34 ped_to_car;
    tCollision_info* the_car;
    float impact_speed;
    float volume_damage;
    tU32 the_time;
    tPed_hit_position hit_pos;
    LOG_TRACE("(%p)", pPedestrian);

    tossing = 0;
    billiards_shot = 0;
    the_time = GetTotalTime();
    orig_gib_flag = pPedestrian->giblets_being_sat_upon;
    pPedestrian->giblets_being_sat_upon = 0;
    ped_pos = &pPedestrian->actor->t.t.translate.t;
    if (!gDetect_peds || pPedestrian->giblets_action == pPedestrian->current_action || pPedestrian->actor->model == NULL) {
        return;
    }
    scalar_frame_time = gFrame_period;
    BrVector3Set(&min_ped_bounds, -0.5f, 0.f, 0.f);
    BrVector3Set(&max_ped_bounds, 0.5f, 1.f, 0.f);
    if (pPedestrian->last_special_volume != NULL) {
        volume_damage = gFrame_period * pPedestrian->last_special_volume->ped_damage_per_ms;
        if (volume_damage != 0.f
            && pPedestrian->current_action != pPedestrian->fatal_car_impact_action
            && pPedestrian->current_action != pPedestrian->fatal_ground_impact_action) {
            if (volume_damage <= 1.0f) {
                volume_damage = 1.0f;
            }
            pPedestrian->hit_points -= volume_damage;
            if (pPedestrian->hit_points <= 0) {
                ChangeActionTo(pPedestrian, pPedestrian->fatal_ground_impact_action, 1);
            }
        }
    }
    for (norman = 0, i = 0; 1; i++) {
        while (i >= (norman == 0 ? gNum_active_cars : gNum_active_non_cars) && norman < 2) {
            norman++;
            i = 0;
        }
        if (norman > 1) {
            pPedestrian->collided_last_time = 0;
            return;
        }
        the_car = (norman == 0 ? (tCollision_info*)gActive_car_list[i] : &gActive_non_car_list[i]->collision_info);
        if (the_car->doing_nothing_flag && the_car->driver != eDriver_local_human) {
            continue;
        }
        car_actor = the_car->car_master_actor;
        car_pos = &car_actor->t.t.translate.t;
        impact_speed = fabsf(the_car->speed);
        if (BrVector3Dot(&pPedestrian->direction, &the_car->direction) > 0.f
            && impact_speed < fabsf(pPedestrian->current_speed)) {
            impact_speed = 0.0f;
        }
        distance_squared = Vector3DistanceSquared(ped_pos, car_pos);
        proximity_rayed = pPedestrian->ref_number < 100
            && pPedestrian->current_action != pPedestrian->fatal_car_impact_action
            && pPedestrian->current_action != pPedestrian->fatal_ground_impact_action
            && the_car->driver > eDriver_non_car
            && !((tCar_spec*)the_car)->knackered
            && !gCountdown
            && distance_squared < ((tCar_spec*)the_car)->proxy_ray_distance;
        fated = pPedestrian->fate == (tCar_spec*)the_car;
        if (MAX(scalar_frame_time * impact_speed * scalar_frame_time * impact_speed * 2.f, 1.44f) <= distance_squared && !proximity_rayed && !fated) {
            continue;
        }
        if (car_actor == NULL) {
            continue;
        }
        BrActorToActorMatrix34(&ped_to_car, pPedestrian->actor, car_actor);
        BrMatrix34ApplyP(&min_ped_bounds_car, &min_ped_bounds, &ped_to_car);
        BrMatrix34ApplyP(&max_ped_bounds_car, &max_ped_bounds, &ped_to_car);
        // use gross_dismiss as temporary
        if (max_ped_bounds_car.v[V_X] < min_ped_bounds_car.v[V_X]) {
            SwapValuesUsingTemporary(max_ped_bounds_car.v[V_X], min_ped_bounds_car.v[V_X], gross_dismiss);
        }
        if (max_ped_bounds_car.v[V_Y] < min_ped_bounds_car.v[V_Y]) {
            SwapValuesUsingTemporary(max_ped_bounds_car.v[V_Y], min_ped_bounds_car.v[V_Y], gross_dismiss);
        }
        if (max_ped_bounds_car.v[V_Z] < min_ped_bounds_car.v[V_Z]) {
            SwapValuesUsingTemporary(max_ped_bounds_car.v[V_Z], min_ped_bounds_car.v[V_Z], gross_dismiss);
        }
        car_bounds_min_x = the_car->bounds[0].min.v[V_X];
        car_bounds_max_x = the_car->bounds[0].max.v[V_X];
        car_bounds_min_z = the_car->bounds[0].min.v[V_Z];
        car_bounds_max_z = the_car->bounds[0].max.v[V_Z];
        prev_car_bounds_min_x = car_bounds_min_x - the_car->velocity_car_space.v[V_X] * scalar_frame_time;
        prev_car_bounds_max_x = car_bounds_max_x - the_car->velocity_car_space.v[V_X] * scalar_frame_time;
        prev_car_bounds_min_z = car_bounds_min_z - the_car->velocity_car_space.v[V_Z] * scalar_frame_time;
        prev_car_bounds_max_z = car_bounds_max_z - the_car->velocity_car_space.v[V_Z] * scalar_frame_time;
        if (!proximity_rayed) {
            if (!fated) {
                if (the_car->velocity_car_space.v[V_X] <= 0.0f) {
                    if (the_car->velocity_car_space.v[V_Z] <= 0.0f) {
                        if (max_ped_bounds_car.v[V_X] <= car_bounds_min_x
                            || min_ped_bounds_car.v[V_X] >= prev_car_bounds_max_x
                            || max_ped_bounds_car.v[V_Z] <= car_bounds_min_z
                            || min_ped_bounds_car.v[V_Z] >= prev_car_bounds_max_z
                            || (min_ped_bounds_car.v[V_X] > car_bounds_max_x
                                && max_ped_bounds_car.v[V_Z] < prev_car_bounds_min_z
                                && prev_car_bounds_max_x - car_bounds_max_x != 0.0f
                                && (prev_car_bounds_min_z - car_bounds_min_z) / (prev_car_bounds_max_x - car_bounds_max_x) > (max_ped_bounds_car.v[V_Z] - car_bounds_min_z) / (min_ped_bounds_car.v[V_X] - car_bounds_max_x))
                            || (max_ped_bounds_car.v[V_X] < prev_car_bounds_min_x
                                && min_ped_bounds_car.v[V_Z] > car_bounds_max_z
                                && car_bounds_min_x - prev_car_bounds_min_x != 0.0f
                                && (car_bounds_max_z - prev_car_bounds_max_z) / (car_bounds_min_x - prev_car_bounds_min_x) > (min_ped_bounds_car.v[V_Z] - prev_car_bounds_max_z) / (max_ped_bounds_car.v[V_X] - prev_car_bounds_min_x))) {
                            continue;
                        }
                    } else if (max_ped_bounds_car.v[V_X] <= car_bounds_min_x
                        || min_ped_bounds_car.v[V_X] >= prev_car_bounds_max_x
                        || max_ped_bounds_car.v[V_Z] <= prev_car_bounds_min_z
                        || min_ped_bounds_car.v[V_Z] >= car_bounds_max_z
                        || (max_ped_bounds_car.v[V_X] < prev_car_bounds_min_x
                            && max_ped_bounds_car.v[V_Z] < car_bounds_min_z
                            && car_bounds_min_x - prev_car_bounds_min_x != 0.0f
                            && ((car_bounds_min_z - prev_car_bounds_min_z) / (car_bounds_min_x - prev_car_bounds_min_x) < (max_ped_bounds_car.v[V_Z] - prev_car_bounds_min_z) / (max_ped_bounds_car.v[V_X] - prev_car_bounds_min_x)))
                        || (min_ped_bounds_car.v[V_X] > car_bounds_max_x
                            && min_ped_bounds_car.v[V_Z] > prev_car_bounds_max_z
                            && prev_car_bounds_max_x - car_bounds_max_x != 0.0f
                            && (prev_car_bounds_max_z - car_bounds_max_z) / (prev_car_bounds_max_x - car_bounds_max_x) < (min_ped_bounds_car.v[V_Z] - car_bounds_max_z) / (min_ped_bounds_car.v[V_X] - car_bounds_max_x))) {
                        continue;
                    }
                } else if (the_car->velocity_car_space.v[V_Z] <= 0.0f) {
                    if (max_ped_bounds_car.v[V_X] <= prev_car_bounds_min_x
                        || min_ped_bounds_car.v[V_X] >= car_bounds_max_x
                        || max_ped_bounds_car.v[V_Z] <= car_bounds_min_z
                        || min_ped_bounds_car.v[V_Z] >= prev_car_bounds_max_z
                        || (max_ped_bounds_car.v[V_X] < car_bounds_min_x
                            && max_ped_bounds_car.v[V_Z] < prev_car_bounds_min_z
                            && prev_car_bounds_min_x - car_bounds_min_x != 0.0f
                            && (prev_car_bounds_min_z - car_bounds_min_z) / (prev_car_bounds_min_x - car_bounds_min_x) < (max_ped_bounds_car.v[V_Z] - car_bounds_min_z) / (max_ped_bounds_car.v[V_X] - car_bounds_min_x))
                        || (min_ped_bounds_car.v[V_X] > prev_car_bounds_max_x
                            && min_ped_bounds_car.v[V_Z] > car_bounds_max_z
                            && car_bounds_max_x - prev_car_bounds_max_x != 0.0f
                            && (car_bounds_max_z - prev_car_bounds_max_z) / (car_bounds_max_x - prev_car_bounds_max_x) < (min_ped_bounds_car.v[V_Z] - prev_car_bounds_max_z) / (min_ped_bounds_car.v[V_X] - prev_car_bounds_max_x))) {
                        continue;
                    }
                } else if (max_ped_bounds_car.v[V_X] <= prev_car_bounds_min_x
                    || min_ped_bounds_car.v[V_X] >= car_bounds_max_x
                    || max_ped_bounds_car.v[V_Z] <= prev_car_bounds_min_z
                    || min_ped_bounds_car.v[V_Z] >= car_bounds_max_z
                    || (min_ped_bounds_car.v[V_X] > prev_car_bounds_max_x
                        && max_ped_bounds_car.v[V_Z] < car_bounds_min_z
                        && car_bounds_max_x - prev_car_bounds_max_x != 0.0f
                        && (car_bounds_min_z - prev_car_bounds_min_z) / (car_bounds_max_x - prev_car_bounds_max_x) > (max_ped_bounds_car.v[V_Z] - prev_car_bounds_min_z) / (min_ped_bounds_car.v[V_X] - prev_car_bounds_max_x))
                    || (max_ped_bounds_car.v[V_X] < car_bounds_min_x
                        && min_ped_bounds_car.v[V_Z] > prev_car_bounds_max_z
                        && prev_car_bounds_min_x - car_bounds_min_x != 0.0f
                        && (prev_car_bounds_max_z - car_bounds_max_z) / (prev_car_bounds_min_x - car_bounds_min_x) > (min_ped_bounds_car.v[V_Z] - car_bounds_max_z) / (max_ped_bounds_car.v[V_X] - car_bounds_min_x))) {
                    continue;
                }
            }
            proximity_rayed = 0;
        }
        if (pPedestrian->fate != (tCar_spec*)the_car
            && !proximity_rayed
            && (the_car->bounds[0].max.v[V_Y] < min_ped_bounds_car.v[V_Y]
                || the_car->bounds[0].min.v[V_Y] > max_ped_bounds_car.v[V_Y])) {
            continue;
        }
        pPedestrian->fate = NULL;
        if (pPedestrian->current_action == pPedestrian->fatal_car_impact_action
            || pPedestrian->current_action == pPedestrian->fatal_ground_impact_action) {
            if (pPedestrian->ref_number < 100) {
                if (the_car->driver <= 1 || BloodyWheels((tCar_spec*)the_car, (br_vector3*)ped_to_car.m[3], MIN(-pPedestrian->min_x, pPedestrian->max_x), &pPedestrian->actor->t.t.translate.t)) {
                    if (!orig_gib_flag) {
                        BurstPedestrian(pPedestrian, impact_speed, 0);
                    }
                    pPedestrian->giblets_being_sat_upon = 1;
                }
            }
            continue;
        }
        break;
    }
    pPedestrian->killers_ID = the_car->car_ID;
    ped_centre_x = (max_ped_bounds_car.v[V_X] + min_ped_bounds_car.v[V_X]) / 2.0f;
    ped_centre_y = (max_ped_bounds_car.v[V_Z] + min_ped_bounds_car.v[V_Z]) / 2.0f;
    if (proximity_rayed) {
        hit_pos = ePed_hit_unknown;
    } else {
        hit_pos = MoveToEdgeOfCar(pPedestrian,
            the_car,
            car_actor,
            ped_centre_x,
            ped_centre_y,
            car_bounds_min_x,
            car_bounds_max_x,
            car_bounds_min_z,
            car_bounds_max_z,
            &min_ped_bounds_car,
            &max_ped_bounds_car);
        pPedestrian->mid_air = 0;
    }
    if (the_car->driver <= eDriver_non_car && the_car->who_last_hit_me != NULL && pPedestrian->ref_number != 114) {
        the_car = (tCollision_info*)the_car->who_last_hit_me;
        billiards_shot = 1;
    }
    pPedestrian->hit_points -= impact_speed * 35000.0f;
    if (pPedestrian->hit_points < -99) {
        pPedestrian->hit_points = -99;
    }
    if (impact_speed == 0.0f && !proximity_rayed && !pPedestrian->collided_last_time) {
        pPedestrian->instruction_direction = -pPedestrian->instruction_direction;
        gInitial_instruction = 0;
        PedestrianNextInstruction(pPedestrian, 500.0f, 0, 1);
        pPedestrian->collided_last_time = 1;
        return;
    }

    pPedestrian->hit_points = -1;
    if (proximity_rayed || gPed_scale_factor != 1.0f || pPedestrian->ref_number >= 100) {
        pPedestrian->jump_magnitude = 0.0f;
    } else {
        pPedestrian->jump_magnitude = impact_speed * 30.0f;
    }
    if (pPedestrian->jump_magnitude > 0.4f) {
        pPedestrian->jump_magnitude = 0.4f;
    }
    pPedestrian->jump_magnitude /= gGravity_multiplier;
    if (pPedestrian->ref_number < 100 && (gNet_mode == eNet_mode_none || pPedestrian->murderer == -1)) {
        gProgram_state.peds_killed++;
    }
    if (pPedestrian->ref_number < 100 && the_car->driver >= eDriver_net_human) {
        if (pPedestrian->murderer == -1 && gNet_mode == eNet_mode_host && gCurrent_net_game->type == eNet_game_type_carnage) {
            NetPlayerFromCar((tCar_spec*)the_car)->score++;
        }
        if (gNet_mode != eNet_mode_none) {
            pPedestrian->murderer = NetPlayerFromCar((tCar_spec*)the_car)->ID;
        }
    }
    if (proximity_rayed) {
        for (i = 0; i < COUNT_OF(gProximity_rays); ++i) {
            if (gProximity_rays[i].start_time == 0) {
                gProximity_rays[i].start_time = GetTotalTime();
                gProximity_rays[i].car = (tCar_spec*)the_car;
                gProximity_rays[i].ped = pPedestrian;
                if (the_car->driver == eDriver_local_human) {
                    DRS3StartSound(gCar_outlet, 5500);
                } else {
                    DRS3StartSound3D(
                        gCar_outlet, 5500, &the_car->pos, &gZero_v__pedestrn,
                        1, 255, 65536, 65536);
                }
                break;
            }
        }
    } else {
        if (FancyATossOffMate(pPedestrian, the_car, impact_speed)) {
            pPedestrian->mid_air = 1;
            if (pPedestrian->to_pos.v[V_Y] < 500.0f) {
                pPedestrian->to_pos.v[V_Y] += 1000.4f;
            }
            pPedestrian->falling_speed -= impact_speed * 0.02f;
            if (pPedestrian->falling_speed > -0.001f) {
                pPedestrian->falling_speed = -0.001f;
            }
            tossing = 1;
            pPedestrian->actor->t.t.translate.t.v[V_Y] += impact_speed * 15.0f;
            pPedestrian->pos = pPedestrian->actor->t.t.translate.t;
        } else {
            pPedestrian->actor->render_style = BR_RSTYLE_NONE;
            BrActorRelink(car_actor, pPedestrian->actor);
        }
        pPedestrian->offset = pPedestrian->actor->t.t.translate.t;
    }
    if (proximity_rayed
        || ((impact_speed < 0.004f || !PercentageChance(50) || gPed_scale_factor != 1.0f) && !tossing)
        || pPedestrian->ref_number >= 100
        || gPedestrian_harvest) {
        pPedestrian->spin_period = 0.0f;
    } else if (PercentageChance(50)) {
        pPedestrian->spin_period = FRandomBetween(100.0f, tossing ? 300.f : 2000.f);
        ;
    } else {
        pPedestrian->spin_period = -FRandomBetween(100.0f, tossing ? 300.0f : 2000.f);
    }
    if (proximity_rayed || tossing) {
        ChangeActionTo(pPedestrian, pPedestrian->fatal_ground_impact_action, 1);
    } else {
        ChangeActionTo(pPedestrian, pPedestrian->fatal_car_impact_action, 1);
    }
    if (pPedestrian->ref_number >= 100) {
        exploded = 0;
    } else {
        exploded = BurstPedestrian(pPedestrian, impact_speed, 1);
        ReportMurderToPoliceDepartment((tCar_spec*)the_car);
    }
    BrVector3Set(&zero_v, 0.f, 0.f, 0.f);
    if (gNet_mode != eNet_mode_none && gCurrent_net_game->options.powerup_respawn && pPedestrian->ref_number >= 100) {
        pPedestrian->respawn_time = GetRaceTime() + IRandomBetween(0, gRespawn_variance) + gMin_respawn_time;
    }
    if (the_car->driver != eDriver_local_human || gRace_finished || pPedestrian->ref_number >= 100) {
        DRS3StartSound3D(
            gPedestrians_outlet,
            pPedestrian->exploding_sounds[IRandomBetween(0, pPedestrian->number_of_exploding_sounds - 1)],
            &pPedestrian->pos,
            &zero_v,
            1,
            255,
            65536,
            65536);
        if ((the_car->driver == eDriver_local_human || pPedestrian->ref_number == 114) && !gRace_finished && !billiards_shot) {
            GotPowerup((tCar_spec*)the_car, pPedestrian->ref_number - 100);
        }
    } else {
        PipeSinglePedIncident(pPedestrian - gPedestrian_array, car_actor);
        AwardTime(gPed_time_value[gProgram_state.skill_level]);
        if (the_time - gLast_ped_splat_time > 1000) {
            gCurrent_ped_multiplier = 1;
        } else if (gCurrent_ped_multiplier < 5) {
            gCurrent_ped_multiplier++;
        }
        gLast_ped_splat_time = the_time;
        credits_value = pPedestrian->credits_value;
        credits_value *= gCurrent_ped_multiplier;
        if (exploded) {
            credits_value *= 2;
        }
        BrVector3Set(&up, 0.f, 1.f, 0.f);
        if (billiards_shot) {
            credits_value *= 4;
            PratcamEvent(30);
            DoFancyHeadup(8);
        } else if (fabsf(the_car->omega.v[V_X]) <= 5.0f
            && fabsf(the_car->omega.v[V_Z]) <= 5.0f
            && BrVector3Dot(&the_car->car_master_actor->t.t.look_up.up, &up) >= 0.1f
            && pPedestrian->offset.v[1] >= -0.1f) {
            if (((hit_pos != ePed_hit_lside && hit_pos != ePed_hit_rside)
                    || (fabsf(the_car->velocity_car_space.v[V_X]) <= fabsf(the_car->velocity_car_space.v[V_Z])
                        && fabsf(the_car->omega.v[V_Y] / the_car->velocity_car_space.v[V_Z]) <= 600.0f))
                && (hit_pos != ePed_hit_back || the_car->velocity_car_space.v[V_Z] <= 0.0f)) {
                if (gCurrent_ped_multiplier >= 2) {
                    DoFancyHeadup(gCurrent_ped_multiplier + kFancyHeadup2xComboBonus - 2);
                } else {
                    PratcamEvent(30);
                    if (exploded) {
                        DoFancyHeadup(kFancyHeadupSplatterBonus);
                        PratcamEvent(30);
                    }
                }
            } else {
                credits_value *= 2;
                DoFancyHeadup(kFancyHeadupExtraStyleBonus);
            }
        } else {
            credits_value *= 4;
            PratcamEvent(30);
            DoFancyHeadup(kFancyHeadupBonusForArtisticImpression);
        }
        PratcamEvent(29);
        EarnCredits(credits_value);
    }
    if (the_car->driver == eDriver_local_human && gProgram_state.cockpit_on) {
        NewScreenWobble(FRandomBetween(300.f * impact_speed, 500.f * impact_speed),
            FRandomBetween(1000.f * impact_speed, 2000.f * impact_speed),
            FRandomBetween(15.f / impact_speed, 25.f / impact_speed));
    }
    if (!gRace_finished) {
        CheckLastPed();
    }
    pPedestrian->collided_last_time = 1;
}

// IDA: void __usercall SendPedestrian(tPedestrian_data *pPedestrian@<EAX>, int pIndex@<EDX>)
void SendPedestrian(tPedestrian_data* pPedestrian, int pIndex) {
    tNet_contents* the_contents;
    tNet_message* the_message;
    int size_decider;
    LOG_TRACE("(%p, %d)", pPedestrian, pIndex);

    if (!gSend_peds) {
        return;
    }
    if (pPedestrian->ref_number >= 100 && pPedestrian->hit_points >= 1) {
        return;
    }
    if (pPedestrian->sent_dead_message >= 4) {
        return;
    }
    if (pPedestrian->hit_points >= 0 || pPedestrian->hit_points == -100) {
        if (Vector3AreEqual(&pPedestrian->instruction_list[pPedestrian->current_instruction].data.point_data.position, &pPedestrian->to_pos)) {
            size_decider = 0;
        } else {
            size_decider = 1;
        }
        the_message = NULL;
        the_contents = NetGetBroadcastContents(0x19, size_decider);
    } else {
        size_decider = 2;
        if (pPedestrian->current_frame == pPedestrian->sequences[pPedestrian->current_sequence].number_of_frames - 1) {
            pPedestrian->sent_dead_message++;
        }
        the_message = NetBuildMessage(0x19, size_decider);
        the_contents = &the_message->contents;
    }
    the_contents->data.pedestrian.index = pIndex;
    the_contents->data.pedestrian.action_instruction = (pPedestrian->current_instruction << 4) + pPedestrian->current_action + 1;
    the_contents->data.pedestrian.flags = 0;
    if (pPedestrian->done_initial) {
        the_contents->data.pedestrian.flags |= 0x01;
    }
    if (pPedestrian->mid_air) {
        the_contents->data.pedestrian.flags |= 0x02;
    }
    if (pPedestrian->instruction_direction < 0) {
        the_contents->data.pedestrian.flags |= 0x04;
    }
    if (pPedestrian->hit_points == -100) {
        the_contents->data.pedestrian.flags |= 0x10;
    } else if (pPedestrian->hit_points < 0) {
        the_contents->data.pedestrian.flags |= 0x08;
    }
    if (size_decider != 0) {
        the_contents->data.pedestrian.flags |= 0x20;
    }
    if (size_decider == 2) {
        the_contents->data.pedestrian.flags |= 0x40;
    }
    the_contents->data.pedestrian.pos = pPedestrian->pos;
    the_contents->data.pedestrian.speed = pPedestrian->current_speed;
    if (size_decider != 0) {
        the_contents->data.pedestrian.to_pos = pPedestrian->to_pos;
        if (size_decider == 2) {
            the_contents->data.pedestrian.frame = pPedestrian->current_frame;
            the_contents->data.pedestrian.offset = pPedestrian->offset;
            the_contents->data.pedestrian.murderer = pPedestrian->murderer;
            if (pPedestrian->ref_number < 100) {
                the_contents->data.pedestrian.respawn_time_or_spin_period = pPedestrian->spin_period;
            } else {
                the_contents->data.pedestrian.respawn_time_or_spin_period = pPedestrian->respawn_time;
            }
        }
    }
    if (the_message != NULL) {
        NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, the_message, NULL);
    }
}

// IDA: void __usercall DoPedestrian(tPedestrian_data *pPedestrian@<EAX>, int pIndex@<EDX>)
void DoPedestrian(tPedestrian_data* pPedestrian, int pIndex) {
    float danger_level;
    float start_speed;
    int action_changed;
    int alive;
    int old_frame;
    int start_ins;
    int start_act;
    int start_hp;
    int start_ins_dir;
    br_vector3 danger_direction;
    br_vector3 old_pos;
    LOG_TRACE("(%p, %d)", pPedestrian, pIndex);

    pPedestrian->active = 1;
    pPedestrian->munged = 1;
    if (pPedestrian->done_initial
        && pPedestrian->sequences[pPedestrian->current_sequence].number_of_frames == pPedestrian->sequences[pPedestrian->current_sequence].looping_frame_start) {
        pPedestrian->done_initial = 0;
    }
    if (gAction_replay_mode) {
        old_frame = pPedestrian->current_frame;
        pPedestrian->car_to_ped = FastScalarArcTan2(
            pPedestrian->pos.v[V_X] - gCamera_to_world.m[3][V_X],
            pPedestrian->pos.v[V_Z] - gCamera_to_world.m[3][V_Z]);
        MungePedestrianSequence(pPedestrian, 0);
        if (old_frame <= pPedestrian->sequences[pPedestrian->current_sequence].number_of_frames) {
            pPedestrian->current_frame = old_frame;
        } else {
            pPedestrian->current_frame = 0;
        }
        pPedestrian->colour_map = pPedestrian->sequences[MAX(pPedestrian->current_sequence, 0)].frames[MAX(pPedestrian->current_frame, 0)].pixelmap;
        gCurrent_lollipop_index = -1;
        MungePedModel(pPedestrian);
    } else {
        old_pos = pPedestrian->pos;
        start_speed = pPedestrian->current_speed;
        start_ins = pPedestrian->current_instruction;
        start_act = pPedestrian->current_action;
        start_hp = pPedestrian->hit_points;
        start_ins_dir = pPedestrian->instruction_direction;
        alive = pPedestrian->current_action != pPedestrian->fatal_car_impact_action
            && pPedestrian->current_action != pPedestrian->fatal_ground_impact_action
            && pPedestrian->current_action != pPedestrian->giblets_action;
        if (alive && pPedestrian->ref_number < 100) {
            gDanger_level = CalcPedestrianDangerLevel(pPedestrian, &gDanger_direction);
            action_changed = MungePedestrianAction(pPedestrian, gDanger_level);
        } else {
            action_changed = 0;
        }
        MungePedestrianSequence(pPedestrian, action_changed);
        MungePedestrianFrames(pPedestrian);
        if (pPedestrian->ref_number < 100) {
            MungePedestrianPath(pPedestrian, gDanger_level, &gDanger_direction);
            if (Vector3AreEqual(&pPedestrian->pos, &old_pos)
                && (gReally_stupid_ped_bug_enable || (pPedestrian->actor->parent == gDont_render_actor && pPedestrian->done_initial && pPedestrian->sequences[pPedestrian->current_sequence].frame_rate_type == ePed_frame_speed))) {
                ChangeActionTo(pPedestrian, 0, 0);
            }
        }
        MungePedModel(pPedestrian);
        if (pPedestrian->current_action != pPedestrian->giblets_action) {
            CheckPedestrianDeathScenario(pPedestrian);
        }
        SetPedPos(pPedestrian);
        if (IsActionReplayAvailable()) {
            AddPedestrianToPipingSession(pIndex,
                &pPedestrian->actor->t.t.mat,
                pPedestrian->current_action,
                pPedestrian->current_frame,
                pPedestrian->hit_points,
                pPedestrian->done_initial,
                pPedestrian->actor->parent != gDont_render_actor ? pPedestrian->killers_ID : -1,
                pPedestrian->spin_period,
                pPedestrian->jump_magnitude,
                &pPedestrian->offset);
        }
        if (gNet_mode != eNet_mode_none && !pPedestrian->reverse_frames
            && !(Vector3AreEqual(&pPedestrian->pos, &old_pos)
                && pPedestrian->current_speed == start_speed
                && pPedestrian->current_instruction == start_ins
                && pPedestrian->current_action == start_act
                && pPedestrian->hit_points == start_hp
                && pPedestrian->instruction_direction == start_ins_dir)) {
            SendPedestrian(pPedestrian, pIndex);
        }
    }
}

// IDA: void __usercall AdjustPedestrian(int pIndex@<EAX>, int pAction_index@<EDX>, int pFrame_index@<EBX>, int pHit_points@<ECX>, int pDone_initial, tU16 pParent, br_actor *pParent_actor, float pSpin_period, br_scalar pJump_magnitude, br_vector3 *pOffset, br_vector3 *pTrans)
void AdjustPedestrian(int pIndex, int pAction_index, int pFrame_index, int pHit_points, int pDone_initial, tU16 pParent, br_actor* pParent_actor, float pSpin_period, br_scalar pJump_magnitude, br_vector3* pOffset, br_vector3* pTrans) {
    tPedestrian_data* pedestrian;
    br_actor* parent;
    br_vector3 old_pos;
    LOG_TRACE("(%d, %d, %d, %d, %d, %d, %p, %f, %f, %p, %p)", pIndex, pAction_index, pFrame_index, pHit_points, pDone_initial, pParent, pParent_actor, pSpin_period, pJump_magnitude, pOffset, pTrans);

    pedestrian = &gPedestrian_array[pIndex];
    if (pAction_index < 0 || pAction_index >= pedestrian->number_of_actions) {
        pAction_index = 0;
    }
    pedestrian->current_action = pAction_index;
    pedestrian->hit_points = pHit_points;
    pedestrian->done_initial = pDone_initial;
    old_pos = pedestrian->actor->t.t.translate.t;
    parent = pedestrian->actor->parent;
    if (pParent == (tU16)-1) {
        if (parent != gDont_render_actor) {
            BrActorRelink(gDont_render_actor, pedestrian->actor);
        }
        pedestrian->actor->render_style = BR_RSTYLE_NONE;
    } else {
        if (parent != pParent_actor) {
            BrActorRelink(pParent_actor, pedestrian->actor);
        }
        pedestrian->actor->render_style = BR_RSTYLE_NONE;
    }
    if (pHit_points > 0) {
        pedestrian->spin_period = 0.f;
        pedestrian->jump_magnitude = 0.f;
        BrVector3Set(&pedestrian->offset, 0.f, 0.f, 0.f);
    } else {
        pedestrian->spin_period = pSpin_period;
        pedestrian->jump_magnitude = pJump_magnitude;
        pedestrian->offset = *pOffset;
    }
    if (pedestrian->hit_points == -100) {
        pedestrian->actor->render_style = BR_RSTYLE_NONE;
    }
    pedestrian->actor->t.t.translate.t = *pTrans;
    SetPedPos(pedestrian);
    if (pFrame_index >= pedestrian->sequences[pedestrian->current_sequence].number_of_frames || pFrame_index < 0) {
        pFrame_index = 0;
    }
    pedestrian->current_frame = pFrame_index;
    BrVector3Sub(&pedestrian->direction, &pedestrian->actor->t.t.translate.t, &old_pos);
    BrVector3Normalise(&pedestrian->direction, &pedestrian->direction);
    if (GetReplayRate() < 0.f) {
        BrVector3Scale(&pedestrian->direction, &pedestrian->direction, -1.f);
    }
    MungePedestrianSequence(pedestrian, 0);
}

// IDA: void __usercall SquirtPathVertex(br_vertex *pFirst_vertex@<EAX>, br_vector3 *pPoint@<EDX>)
void SquirtPathVertex(br_vertex* pFirst_vertex, br_vector3* pPoint) {
    LOG_TRACE("(%p, %p)", pFirst_vertex, pPoint);

    pFirst_vertex[0].p = *pPoint;
    pFirst_vertex[1].p = *pPoint;
    pFirst_vertex[1].p.v[V_Y] += .1f;
    pFirst_vertex[2].p = *pPoint;
    pFirst_vertex[2].p.v[V_X] += -.05f;
    pFirst_vertex[2].p.v[V_Y] += .05f;
    pFirst_vertex[2].p.v[V_Z] += -.05f;
    pFirst_vertex[3].p = *pPoint;
    pFirst_vertex[3].p.v[V_X] += .05f;
    pFirst_vertex[3].p.v[V_Y] += .05f;
    pFirst_vertex[3].p.v[V_Z] += .05f;
}

// IDA: void __cdecl ResetAllPedestrians()
void ResetAllPedestrians() {
    int i;
    tPedestrian_data* the_pedestrian;
    LOG_TRACE("()");

    for (i = 0; i < gPed_count; i++) {
        the_pedestrian = &gPedestrian_array[i];
        the_pedestrian->actor->render_style = BR_RSTYLE_NONE;
    }
}

// IDA: void __usercall GroundPedestrian(tPedestrian_data *pPedestrian@<EAX>)
void GroundPedestrian(tPedestrian_data* pPedestrian) {
    br_scalar new_y;
    br_vector3 cast_point;
    tPedestrian_sequence* sequence;
    LOG_TRACE("(%p)", pPedestrian);

    if (pPedestrian->actor->parent != gDont_render_actor) {
        pPedestrian->actor->render_style = BR_RSTYLE_NONE;
        BrActorRelink(gDont_render_actor, pPedestrian->actor);
    }
    pPedestrian->mid_air = 0;
    BrVector3Set(&cast_point,
        pPedestrian->pos.v[V_X],
        pPedestrian->pos.v[V_Y] + 0.4f,
        pPedestrian->pos.v[V_Z]);
    new_y = FindYVerticallyBelow2(&cast_point);
    pPedestrian->actor->t.t.translate.t.v[V_Y] = pPedestrian->pos.v[V_Y] = new_y;
    pPedestrian->spin_period = 0.f;
    sequence = &pPedestrian->sequences[pPedestrian->current_sequence];
    if (sequence->number_of_frames == sequence->looping_frame_start) {
        pPedestrian->current_frame = sequence->number_of_frames - 1;
        pPedestrian->done_initial = 0;
    } else {
        pPedestrian->current_frame = sequence->looping_frame_start;
        pPedestrian->done_initial = 1;
    }
    BrVector3Set(&pPedestrian->offset, 0.f, 0.f, 0.f);
    if (IsActionReplayAvailable()) {
        AddPedestrianToPipingSession(GET_PEDESTRIAN_INDEX(pPedestrian),
            &pPedestrian->actor->t.t.mat,
            pPedestrian->current_action,
            pPedestrian->current_frame,
            pPedestrian->hit_points,
            pPedestrian->done_initial,
            pPedestrian->actor->parent == gDont_render_actor ? -1 : pPedestrian->killers_ID,
            pPedestrian->spin_period,
            pPedestrian->jump_magnitude,
            &pPedestrian->offset);
    }
}

// IDA: void __usercall RevivePedestrian(tPedestrian_data *pPedestrian@<EAX>, int pAnimate@<EDX>)
void RevivePedestrian(tPedestrian_data* pPedestrian, int pAnimate) {
    LOG_TRACE("(%p, %d)", pPedestrian, pAnimate);

    pPedestrian->sent_dead_message = 0;
    pPedestrian->respawn_time = 0;
    pPedestrian->current_sequence = 0;
    if (pAnimate) {
        if (pPedestrian->fatal_ground_impact_action >= 0) {
            pPedestrian->current_action = pPedestrian->fatal_ground_impact_action;
        } else {
            pPedestrian->current_action = pPedestrian->fatal_car_impact_action;
        }
        MungePedestrianSequence(pPedestrian, 1);
        pPedestrian->current_frame = pPedestrian->sequences[pPedestrian->current_sequence].number_of_frames - 1;
        pPedestrian->reverse_frames = 1;
        MungePedestrianFrames(pPedestrian);
    } else {
        pPedestrian->current_action = 0;
        pPedestrian->current_frame = 0;
        pPedestrian->colour_map = pPedestrian->sequences->frames[0].pixelmap;
        pPedestrian->reverse_frames = 0;
    }
    pPedestrian->current_action_mode = -1;
    pPedestrian->done_initial = 0;
    pPedestrian->current_speed = 0.f;
    pPedestrian->instruction_direction = 1;
    pPedestrian->spin_period = 0.f;
    pPedestrian->falling_speed = 0.f;
    pPedestrian->mid_air = 0;
    pPedestrian->last_special_volume = NULL;
    pPedestrian->last_sound = 0;
    pPedestrian->giblets_being_sat_upon = 0;
    pPedestrian->last_sound_action = -1;
    pPedestrian->last_sound_make = 0;
    pPedestrian->hit_points = 10;
    pPedestrian->current_instruction = pPedestrian->first_instruction;
    pPedestrian->killers_ID = -1;
    pPedestrian->fate = NULL;
    gInitial_instruction = NULL;
    PedestrianNextInstruction(pPedestrian, 0.f, 1, 0);
    BrVector3Copy(&pPedestrian->from_pos, &pPedestrian->actor->t.t.translate.t);
    MungePedModel(pPedestrian);
    pPedestrian->pos.v[V_Y] += pPedestrian->sequences[pPedestrian->current_sequence].frames[0].offset.v[V_Y];
}

// IDA: void __usercall MungePedestrians(tU32 pFrame_period@<EAX>)
void MungePedestrians(tU32 pFrame_period) {
    int i;
    int mirror_on;
    tPedestrian_data* the_pedestrian;
    br_vector3 ped_pos;
    br_vector3 ped_pos_car;
    br_vector3 ped_pos_temp;
    br_matrix34 ped_pos_matrix;
    br_camera* camera_ptr;
    br_scalar max_distance;
    br_scalar distance_squared;
    br_scalar x_delta;
    br_scalar y_delta;
    br_scalar z_delta;
    tS32 diff;
    LOG_TRACE("(%d)", pFrame_period);

    gVesuvians_this_time = 0;
    // dword_550A9C = 32;
    gMax_distance_squared = 121.f;
    if (!gAction_replay_mode) {
        MungePedGibs(pFrame_period);
    }
    if (!gAction_replay_mode) {
        StartPipingSession(ePipe_chunk_pedestrian);
    }
    gSend_peds = PDGetTotalTime() - gLast_ped_message_send >= 50;
    if (gSend_peds) {
        gLast_ped_message_send = PDGetTotalTime();
    }
    // BrVector3(&br_vector3_00550ac0, 0.f, 0.f, 0.f);
    if (gAction_replay_mode) {
        for (i = 0; i < gPed_count; i++) {
            the_pedestrian = &gPedestrian_array[i];
            x_delta = fabsf(the_pedestrian->pos.v[V_X] - gCamera_to_world.m[3][V_X]);
            z_delta = fabsf(the_pedestrian->pos.v[V_Z] - gCamera_to_world.m[3][V_Z]);
            if ((the_pedestrian->actor->parent != gDont_render_actor || (x_delta <= ACTIVE_PED_DXDZ && z_delta <= ACTIVE_PED_DXDZ))
                && (gPedestrians_on || the_pedestrian->ref_number >= 100)
                && the_pedestrian->hit_points != -100) {
                gCurrent_lollipop_index = -1;
                DoPedestrian(the_pedestrian, i);
            }
        }
    } else {
        for (i = 0; i < gPed_count; i++) {
            the_pedestrian = &gPedestrian_array[i];
            x_delta = fabsf(the_pedestrian->pos.v[V_X] - gCamera_to_world.m[3][V_X]);
            z_delta = fabsf(the_pedestrian->pos.v[V_Z] - gCamera_to_world.m[3][V_Z]);
            if (the_pedestrian->actor->parent == gDont_render_actor
                && (x_delta > ACTIVE_PED_DXDZ || z_delta > ACTIVE_PED_DXDZ)) {
                the_pedestrian->active = 0;
            } else if (the_pedestrian->hit_points == -100) {
                if (the_pedestrian->respawn_time == 0) {
                    if (the_pedestrian->mid_air || the_pedestrian->actor->parent != gDont_render_actor) {
                        KillPedestrian(the_pedestrian);
                    }
                    the_pedestrian->active = 0;
                } else {
                    diff = GetRaceTime() - the_pedestrian->respawn_time;
                    if (diff >= 0) {
                        RevivePedestrian(the_pedestrian, diff < 1000);
                    } else {
                        if (the_pedestrian->mid_air || the_pedestrian->actor->parent != gDont_render_actor) {
                            KillPedestrian(the_pedestrian);
                        }
                        the_pedestrian->active = 0;
                    }
                }
            } else if (!gPedestrians_on && the_pedestrian->ref_number < 100) {
                if (the_pedestrian->mid_air || the_pedestrian->actor->parent != gDont_render_actor) {
                    KillPedestrian(the_pedestrian);
                }
                the_pedestrian->active = 0;
            } else if (!the_pedestrian->mid_air || the_pedestrian->active) {
                gCurrent_lollipop_index = -1;
                DoPedestrian(the_pedestrian, i);
            } else {
                GroundPedestrian(the_pedestrian);
            }
        }
    }
    if (!gAction_replay_mode) {
        EndPipingSession();
    }
    gVesuvians_last_time = gVesuvians_this_time;
    if (gCurrent_ped_path_actor != NULL) {
        SquirtPathVertex(&gCurrent_ped_path_actor->model->vertices[gCurrent_ped_path_actor->model->nvertices - 4],
            gOur_pos);
        BrModelUpdate(gCurrent_ped_path_actor->model, BR_MODU_ALL);
    }
}

// IDA: void __cdecl RespawnPedestrians()
void RespawnPedestrians() {
    int i;
    tPedestrian_data* the_pedestrian;
    LOG_TRACE("()");

    for (i = 0; i < gPed_count; i++) {
        the_pedestrian = &gPedestrian_array[i];
        if (the_pedestrian->ref_number < 100) {
#if defined(DETHRACE_FIX_BUGS)
            // Only animate the respawn when we are in viewing distance.
            // This is done such that the "Peds visible on map" powerup draws far away items.
            // (far away animated pedestrians would otherwise remain invisible on the map)
            br_scalar x_delta;
            br_scalar z_delta;
            int ped_respawn_animate;
            x_delta = fabsf(the_pedestrian->pos.v[V_X] - gCamera_to_world.m[3][V_X]);
            z_delta = fabsf(the_pedestrian->pos.v[V_Z] - gCamera_to_world.m[3][V_Z]);
            ped_respawn_animate = x_delta <= ACTIVE_PED_DXDZ && z_delta <= ACTIVE_PED_DXDZ;
#else
#define ped_respawn_animate 1
#endif
            if (the_pedestrian->hit_points == -100) {
                RevivePedestrian(the_pedestrian, ped_respawn_animate);
            } else if ((the_pedestrian->current_action == the_pedestrian->fatal_car_impact_action || the_pedestrian->current_action == the_pedestrian->fatal_ground_impact_action || the_pedestrian->current_action == the_pedestrian->giblets_action)
                && the_pedestrian->actor->parent == gDont_render_actor) {
                RevivePedestrian(the_pedestrian, ped_respawn_animate);
            }
        }
    }
}

// IDA: int __cdecl GetPedCount()
int GetPedCount() {
    LOG_TRACE("()");

    return gPed_count;
}

// IDA: int __usercall GetPedPosition@<EAX>(int pIndex@<EAX>, br_vector3 *pPos@<EDX>)
int GetPedPosition(int pIndex, br_vector3* pPos) {
    tPedestrian_data* pedestrian;
    LOG_TRACE("(%d, %p)", pIndex, pPos);

    pedestrian = &gPedestrian_array[pIndex];
    if (pedestrian->ref_number < 100) {

        // Item is a human
        if (pedestrian->hit_points == -100
            || pedestrian->current_action == pedestrian->fatal_car_impact_action
            || pedestrian->current_action == pedestrian->fatal_ground_impact_action
            || pedestrian->current_action == pedestrian->giblets_action) {
            return 0;
        } else {
            BrVector3Copy(pPos, &pedestrian->pos);
            return 1;
        }
    } else {
        // Item is a power-up/mine
        if (pedestrian->hit_points == -100) {
            return 0;
        } else {
            BrVector3Copy(pPos, &pedestrian->pos);
            return -1;
        }
    }
}

// IDA: void __usercall CreatePedestrian(FILE *pG@<EAX>, tPedestrian_instruction *pInstructions@<EDX>, int pInstruc_count@<EBX>, int pInit_instruc@<ECX>, int pRef_num, int pForce_read)
void CreatePedestrian(FILE* pG, tPedestrian_instruction* pInstructions, int pInstruc_count, int pInit_instruc, int pRef_num, int pForce_read) {
    tPath_name the_path;
    char s[256];
    char s2[256];
    char* str;
    char* str2;
    tPedestrian_data* the_pedestrian;
    tPedestrian_sequence* the_sequence;
    tPedestrian_action* the_action;
    br_model* the_model;
    int the_ref;
    int i;
    int j;
    int k;
    int l;
    int temp_int;
    int new_frames;
    tPed_collide_type collide_frame;
    float temp_float1;
    float temp_float2;
    br_scalar height;
    br_scalar width_over_2;
    br_scalar minnest_min;
    br_scalar maxest_min;
    br_scalar minnest_max;
    br_scalar maxest_max;
    LOG_TRACE("(%p, %p, %d, %d, %d, %d)", pG, pInstructions, pInstruc_count, pInit_instruc, pRef_num, pForce_read);

    PossibleService();
    the_pedestrian = &gPedestrian_array[gPed_count];
    gPed_count++;
    the_pedestrian->ref_number = pRef_num;
    the_pedestrian->magic_number = PEDESTRIAN_MAGIC;
    the_pedestrian->cloned = 0;
    if (!pForce_read) {
        for (i = 0; i < gPed_count - 1; i++) {
            if (gPedestrian_array[i].ref_number == the_pedestrian->ref_number) {
                *the_pedestrian = gPedestrian_array[i];
                the_pedestrian->cloned = 1;
                break;
            }
        }
    }
    the_pedestrian->actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
    the_pedestrian->actor->render_style = BR_RSTYLE_NONE;
    the_pedestrian->actor->type_data = the_pedestrian;
    BrActorAdd(gDont_render_actor, the_pedestrian->actor);
    the_pedestrian->actor->model = gPed_model;
    if (the_pedestrian->cloned) {
        the_pedestrian->actor->material = gPed_material;
    } else {
        rewind(pG);
        do {
            GetALineAndDontArgue(pG, s);
            str = strtok(s, "\t ,/");
            sscanf(str, "%d", &the_ref);
            if (the_pedestrian->ref_number != the_ref) {
                while (1) {
                    PossibleService();
                    GetALineAndDontArgue(pG, s);
                    if (strcmp(s, "END OF PEDESTRIANS") == 0) {
                        BrActorRemove(the_pedestrian->actor);
                        BrActorFree(the_pedestrian->actor);
                        gPed_count--;
                        DRS3StartSound(gEffects_outlet, 3100);
                        return;
                    }
                    if (strcmp(s, "NEXT PEDESTRIAN") == 0) {
                        break;
                    }
                }
            }
        } while (the_pedestrian->ref_number != the_ref);

        temp_float1 = GetAFloat(pG);
        the_pedestrian->height = temp_float1;
        the_pedestrian->credits_value = GetAnInt(pG);
        the_pedestrian->hit_points = GetAnInt(pG);
        the_pedestrian->active = 0;
        GetALineAndDontArgue(pG, s2);
        str2 = strtok(s2, "\t ,/");
        sscanf(str2, "%d", &the_pedestrian->number_of_exploding_sounds);
        for (j = 0; j < the_pedestrian->number_of_exploding_sounds; j++) {
            str2 = strtok(NULL, "\t ,/");
            sscanf(str2, "%d", &the_pedestrian->exploding_sounds[j]);
        }
        the_pedestrian->falling_sound = GetAnInt(pG);
        the_pedestrian->acceleration = GetAFloat(pG);
        the_pedestrian->image_index = GetAnInt(pG);
        LoadNShadeTables(&gPedestrians_storage_space, pG, 1);
        LoadNPixelmaps(&gPedestrians_storage_space, pG, 1);
        GetALineAndDontArgue(pG, s2);
        if (gPed_material == NULL) {
            str2 = strtok(s2, "\t ,/");
            PathCat(the_path, gApplication_path, "MATERIAL");
            PathCat(the_path, the_path, s2);
            gPed_material = BrMaterialLoad(the_path);
            if (gPed_material == NULL) {
                FatalError(kFatalError_FindPedestrianMaterial_S, s2);
            }
            gPed_material->flags &= ~BR_MATF_TWO_SIDED;
            gPed_material->flags &= ~(BR_MATF_LIGHT | BR_MATF_PRELIT | BR_MATF_SMOOTH);
            gPed_material->index_shade = NULL;
            gPed_material->colour_map = gPedestrians_storage_space.pixelmaps[0];
            BrMaterialAdd(gPed_material);
        }
        gPed_material->colour_map = gPedestrians_storage_space.pixelmaps[0];
        the_pedestrian->actor->material = gPed_material;
        the_pedestrian->fatal_car_impact_action = GetAnInt(pG);
        the_pedestrian->non_fatal_car_impact_action = GetAnInt(pG);
        the_pedestrian->after_impact_action = GetAnInt(pG);
        the_pedestrian->fatal_ground_impact_action = GetAnInt(pG);
        the_pedestrian->non_fatal_ground_impact_action = GetAnInt(pG);
        the_pedestrian->giblets_action = GetAnInt(pG);
        the_pedestrian->current_sequence = 0;
        the_pedestrian->current_frame = 0;
        the_pedestrian->current_action = 0;
        the_pedestrian->current_action_mode = -1;
        the_pedestrian->done_initial = 0;
        the_pedestrian->current_speed = 0.0f;
        the_pedestrian->instruction_direction = 1;
        the_pedestrian->spin_period = 0.0f;
        the_pedestrian->falling_speed = 0.0f;
        the_pedestrian->mid_air = 0;
        the_pedestrian->last_special_volume = NULL;
        the_pedestrian->last_sound = 0;
        the_pedestrian->giblets_being_sat_upon = 0;
        the_pedestrian->last_sound_action = -1;
        the_pedestrian->last_sound_make = 0;
        the_pedestrian->fate = NULL;
        the_pedestrian->munged = 0;
        the_pedestrian->collided_last_time = 0;
        the_pedestrian->reverse_frames = 0;
        the_pedestrian->respawn_time = 0;
        the_pedestrian->killers_ID = -1;
        the_pedestrian->murderer = -1;
        the_pedestrian->sent_dead_message = 0;
        minnest_min = BR_SCALAR_MAX;
        maxest_min = BR_SCALAR_MIN;
        minnest_max = BR_SCALAR_MAX;
        maxest_max = BR_SCALAR_MIN;
        the_pedestrian->number_of_actions = GetAnInt(pG);
        the_pedestrian->action_list = BrMemAllocate(sizeof(tPedestrian_action) * the_pedestrian->number_of_actions, kMem_ped_action_list);
        the_action = the_pedestrian->action_list;
        for (i = 0; i < the_pedestrian->number_of_actions; i++) {
            PossibleService();
            GetPairOfFloats(pG, &the_action->danger_level, &the_action->percentage_chance);
            GetPairOfFloats(pG, &the_action->initial_speed, &the_action->looping_speed);
            the_action->reaction_time = (tU32)(GetAFloat(pG) * 1000.0f);
            GetALineAndDontArgue(pG, s2);
            str2 = strtok(s2, "\t ,/");
            sscanf(str2, "%d", &the_action->number_of_sounds);
            for (j = 0; j < the_action->number_of_sounds; j++) {
                str2 = strtok(NULL, "\t ,/");
                sscanf(str2, "%d", &the_action->sounds[j]);
            }
            the_action->number_of_bearings = GetAnInt(pG);
            if (the_action->number_of_bearings > COUNT_OF(the_action->sequences)) {
                FatalError(kFatalError_PedSeqTooManyBearings);
            }
            for (j = 0; j < the_action->number_of_bearings; j++) {
                GetPairOfFloats(pG, &the_action->sequences[j].max_bearing, &temp_float1);
                the_action->sequences[j].sequence_index = (int)temp_float1;
            }
            the_action++;
        }
        the_pedestrian->number_of_sequences = GetAnInt(pG);
        the_pedestrian->last_frame = 0;
        the_pedestrian->last_action_change = 0;
        the_pedestrian->sequences = BrMemAllocate(sizeof(tPedestrian_sequence) * the_pedestrian->number_of_sequences, kMem_ped_sequences);
        the_sequence = the_pedestrian->sequences;
        for (i = 0; i < the_pedestrian->number_of_sequences; i++) {
            PossibleService();
            collide_frame = GetALineAndInterpretCommand(pG, gCollide_commands, COUNT_OF(gCollide_commands));
            the_sequence->frame_rate_type = GetALineAndInterpretCommand(pG, gRate_commands, COUNT_OF(gRate_commands));

            if (the_sequence->frame_rate_type == ePed_frame_speed || the_sequence->frame_rate_type == ePed_frame_variable) {
                GetPairOfFloats(pG, &the_sequence->frame_rate_factor1, &the_sequence->frame_rate_factor2);
            }
            the_sequence->number_of_frames = 0;
            the_sequence->looping_frame_start = -1;
            for (j = 0; j < 2; j++) {
                new_frames = GetAnInt(pG);
                if (new_frames + the_sequence->number_of_frames > COUNT_OF(the_sequence->frames)) {
                    FatalError(kFatalError_PedSeqTooManyFrames);
                }
                for (k = the_sequence->number_of_frames; k < new_frames + the_sequence->number_of_frames; k++) {
                    GetAString(pG, s);
                    the_sequence->frames[k].pixelmap = BrMapFind(s);
                    if (the_sequence->frames[k].pixelmap == NULL) {
                        the_sequence->frames[k].pixelmap = LoadPixelmap(s);
                        if (the_sequence->frames[k].pixelmap == NULL) {
                            FatalError(kFatalError_FindPedestrianPixelmap_S, s);
                        }
                        BrMapAdd(the_sequence->frames[k].pixelmap);
                    }
                    GetALineAndDontArgue(pG, s);
                    str = strtok(s, "\t ,/");
                    sscanf(str, "%f", &temp_float1);
                    str = strtok(NULL, "\t ,/");
                    sscanf(str, "%f", &temp_float2);
                    str = strtok(NULL, "\t ,/");
                    the_sequence->frames[k].offset.v[0] = temp_float1;
                    the_sequence->frames[k].offset.v[1] = temp_float2;
                    the_sequence->frames[k].flipped = strcmp(str, "flipped") == 0;
                    if (collide_frame) {
                        CalcPedWidthNHeight(
                            the_pedestrian,
                            the_sequence->frames[k].pixelmap,
                            &height,
                            &width_over_2);
                        if (the_sequence->frames[k].offset.v[0] - width_over_2 < minnest_min) {
                            minnest_min = the_sequence->frames[k].offset.v[0] - width_over_2;
                        }
                        if (the_sequence->frames[k].offset.v[0] - width_over_2 > maxest_min) {
                            maxest_min = the_sequence->frames[k].offset.v[0] - width_over_2;
                        }
                        if (the_sequence->frames[k].offset.v[0] + width_over_2 < minnest_max) {
                            minnest_max = the_sequence->frames[k].offset.v[0] + width_over_2;
                        }
                        if (the_sequence->frames[k].offset.v[0] + width_over_2 > maxest_max) {
                            maxest_max = the_sequence->frames[k].offset.v[0] + width_over_2;
                        }
                    }
                }
                the_sequence->number_of_frames += new_frames;
                if (the_sequence->looping_frame_start < 0) {
                    the_sequence->looping_frame_start = the_sequence->number_of_frames;
                }
            }
            the_sequence++;
        }
        the_pedestrian->min_x = maxest_min;
        the_pedestrian->max_x = minnest_max;
    }
    PossibleService();
    the_pedestrian->number_of_instructions = pInstruc_count;
    the_pedestrian->first_instruction = pInit_instruc;
    the_pedestrian->current_instruction = pInit_instruc;
    the_pedestrian->instruction_list = pInstructions;
    the_pedestrian->colour_map = the_pedestrian->sequences->frames[0].pixelmap;
    BrVector3Set(&the_pedestrian->offset, 0.0f, 0.0f, 0.0f);
    gInitial_instruction = 0;
    PedestrianNextInstruction(the_pedestrian, 0.0f, 1, 0);
    the_pedestrian->from_pos = the_pedestrian->actor->t.t.translate.t;
    MungePedModel(the_pedestrian);
}

// IDA: void __cdecl ResetProxRay()
void ResetProxRay() {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gProximity_rays); i++) {
        gProximity_rays[i].start_time = 0;
    }
}

// IDA: void __cdecl PedMaterialFromHell()
void PedMaterialFromHell() {
    LOG_TRACE("()");
}

// IDA: void __cdecl ResetPedMaterial()
void ResetPedMaterial() {
    int i;
    int j;
    LOG_TRACE("()");
}

// IDA: void __usercall LoadInPedestrians(FILE *pF@<EAX>, int pSubs_count@<EDX>, tPed_subs *pSubs_array@<EBX>)
void LoadInPedestrians(FILE* pF, int pSubs_count, tPed_subs* pSubs_array) {
    tPath_name the_path;
    char s[256];
    char s2[256];
    char* str;
    char* str2;
    FILE* g;
    int the_ref;
    int i;
    int j;
    int k;
    int l;
    int knock_out;
    int check_for_duplicates;
    int duplicates_found;
    int temp_int;
    int ref_number;
    int instruc_count;
    int ped_count;
    int init_instruc;
    float temp_float1;
    float temp_float2;
    tPedestrian_instruction* instructions;
    tPedestrian_instruction* the_instruction;
    tPed_choice* the_choice;
    LOG_TRACE("(%p, %d, %p)", pF, pSubs_count, pSubs_array);

    knock_out = 0;
    check_for_duplicates = 0;
    duplicates_found = 0;
    gTotal_peds = 0;
    gFlag_waving_bastard = NULL;
    gPedestrians_on = 1;
    gPed_scale_factor = 1.0f;
    gAttracted_pedestrians = 0;
    gExploding_pedestrians = 0;
    gBlind_pedestrians = 0;
    gPedestrian_speed_factor = 1.0f;
    if (gProgram_state.sausage_eater_mode) {
        PathCat(the_path, gApplication_path, "BORING.TXT");
    } else {
        PathCat(the_path, gApplication_path, "PEDESTRN.TXT");
    }
    g = DRfopen(the_path, "rt");
    if (g == NULL) {
        FatalError(kFatalError_OpenPedestrianFile);
    }
    gPed_count = 0;
    gLast_ped_splat_time = 0;
    temp_int = GetAnInt(pF);
    if (gAusterity_mode) {
        ped_count = temp_int / 2 + 2;
    } else {
        ped_count = temp_int;
    }
    gPedestrian_array = BrMemAllocate(sizeof(tPedestrian_data) * (ped_count + (gAusterity_mode ? 0 : 200)), kMem_ped_array_stain);
    if (PDKeyDown(KEY_LCTRL) && PDKeyDown(KEY_LSHIFT) && PDKeyDown(KEY_A)) {
        check_for_duplicates = 1;
        DRS3StartSound(gEffects_outlet, 3202);
        DRS3StartSound(gEffects_outlet, 3202);
    }
    for (i = 0; i < ped_count; i++) {
        PossibleService();
        ref_number = GetAnInt(pF);
        for (j = 0; j < pSubs_count; ++j) {
            if (pSubs_array[j].orig == ref_number) {
                ref_number = pSubs_array[j].subs;
                break;
            }
        }
        instruc_count = GetAnInt(pF);
        init_instruc = GetAnInt(pF) - 1;
        instructions = BrMemAllocate(sizeof(tPedestrian_instruction) * instruc_count, kMem_ped_instructions);

        for (j = 0, the_instruction = instructions; j < instruc_count; j++, the_instruction++) {
            PossibleService();
            the_instruction->type = GetALineAndInterpretCommand(pF, gInstruc_commands, COUNT_OF(gInstruc_commands));
            if (the_instruction->type != ePed_instruc_point && the_instruction->type != ePed_instruc_reverse) {
                FatalError(kFatalError_FindPedestrianRefNum_S, "");
            }
            switch (the_instruction->type) {
            case ePed_instruc_point:
                GetThreeFloats(pF, &the_instruction->data.point_data.position.v[0], &the_instruction->data.point_data.position.v[1], &the_instruction->data.point_data.position.v[2]);
                the_instruction->data.point_data.irreversable = 0;
                break;

            case ePed_instruc_xpoint:
                GetThreeFloats(pF, &the_instruction->data.point_data.position.v[0], &the_instruction->data.point_data.position.v[1], &the_instruction->data.point_data.position.v[2]);
                the_instruction->data.point_data.irreversable = 1;
                break;

            case ePed_instruc_bchoice:
            case ePed_instruc_fchoice:
                GetALineAndDontArgue(pF, s);
                str = strtok(s, "\t ,/");
                sscanf(str, "%d", &the_instruction->data.choice_data.number_of_choices);
                if (the_instruction->data.choice_data.number_of_choices > COUNT_OF(the_instruction->data.choice_data.choices)) {
                    FatalError(kFatalError_PedSeqTooManyChoices);
                }
                for (k = 0; k < the_instruction->data.choice_data.number_of_choices; k++) {
                    str = strtok(NULL, "\t ,/");
                    sscanf(str, "%f", &temp_float1);
                    the_instruction->data.choice_data.choices[k].danger_level = (tU16)temp_float1;
                    str = strtok(NULL, "\t ,/");
                    sscanf(str, "%f", &temp_float1);
                    the_instruction->data.choice_data.choices[k].percentage_chance = (tU8)temp_float1;
                    str = strtok(NULL, "\t ,/");
                    sscanf(str, "%d", &temp_int);
                    the_instruction->data.choice_data.choices[k].marker_ref = (tU8)temp_int;
                }
                break;

            case ePed_instruc_dead:
                the_instruction->data.death_data.death_sequence = GetAnInt(pF);
                break;

            case ePed_instruc_bmarker:
            case ePed_instruc_fmarker:
                the_instruction->data.marker_data.marker_ref = GetAnInt(pF);
                break;

            case ePed_instruc_baction:
            case ePed_instruc_faction:
                the_instruction->data.action_data.action_index = GetAnInt(pF);
                break;

            case ePed_instruc_reverse:
                break;
            }
        }
        if (gAusterity_mode) {
            if (knock_out & 1 && ref_number != FLAG_WAVING_BASTARD_REF) {
                i--;
                ped_count--;
                instruc_count = 0;
                BrMemFree(instructions);
            }
            ++knock_out;
        }
        if (check_for_duplicates) {
            for (j = 0; j < i; j++) {
                if (Vector3AreEqual(&gPedestrian_array[j].instruction_list->data.point_data.position, &instructions->data.point_data.position)) {
                    i--;
                    ped_count--;
                    instruc_count = 0;
                    BrMemFree(instructions);
                    duplicates_found++;
                }
            }
        }
        if (instruc_count > 0) {
            if (gAusterity_mode && ref_number >= 100) {
                // swap MINE for LANDMINE and only one type of powerup ?
                if (ref_number == 114) {
                    CreatePedestrian(g, instructions, instruc_count, init_instruc, 146, 0);
                } else {
                    CreatePedestrian(g, instructions, instruc_count, init_instruc, 100, 0);
                }
            } else {
                CreatePedestrian(g, instructions, instruc_count, init_instruc, ref_number, 0);
            }
            gPedestrian_array[i].ref_number = ref_number;
            if (ref_number == FLAG_WAVING_BASTARD_REF) {
                gFlag_waving_bastard = &gPedestrian_array[i];
            }
            if (ref_number < 100) {
                gTotal_peds++;
            }
        }
    }
    fclose(g);
    if (duplicates_found) {
        WriteOutPeds();
        sprintf(s2,
            "DUPLICATE PED EXPLOSION!!\n"
            "%d DOPPELGANGERS FOUND AND DESTROYED.\n"
            "NEW PED LIST WRITTEN TO PEDPATHS.TXT\n"
            "\n"
            "\n"
            "NOW DON'T DO IT AGAIN!!\n",
            duplicates_found);
        PDFatalError(s2);
    }
    InitPedGibs();
    ResetProxRay();
    gPedestrian_harvest = 0;
    gVesuvian_corpses = 0;
    gProx_ray_shade_table = GenerateShadeTable(8, gRender_palette, 215, 255, 233, 0.5f, 0.75f, 0.9f);
    ResetPedMaterial();
}

// IDA: br_actor* __usercall BuildPedPaths@<EAX>(tPedestrian_instruction *pInstructions@<EAX>, int pInstruc_count@<EDX>, int pInit_instruc@<EBX>)
br_actor* BuildPedPaths(tPedestrian_instruction* pInstructions, int pInstruc_count, int pInit_instruc) {
    int vertex_count;
    int last_vertex_count;
    int face_count;
    int j;
    int point_count;
    br_vector3 the_point;
    br_material* the_mat;
    br_model* the_model;
    br_actor* the_actor;
    LOG_TRACE("(%p, %d, %d)", pInstructions, pInstruc_count, pInit_instruc);

    vertex_count = 4;
    face_count = 2;
    for (j = 0; j < pInstruc_count; j++) {
        if (pInstructions[j].type == ePed_instruc_point || pInstructions[j].type == ePed_instruc_xpoint) {
            vertex_count += 4;
            face_count += 4;
        }
    }
    face_count -= 4;
    the_model = BrModelAllocate(NULL, vertex_count, face_count);
    the_model->flags |= BR_MODU_VERTEX_COLOURS | BR_MODU_VERTEX_POSITIONS;

    vertex_count = 0;
    face_count = 0;
    point_count = 0;
    for (j = 0; j < pInstruc_count; j++) {
        if (pInstructions[j].type == ePed_instruc_point || pInstructions[j].type == ePed_instruc_xpoint) {
            the_point = pInstructions[j].data.point_data.position;
            if (the_point.v[V_Y] < 500.f) {
                the_mat = gPath_mat_normal;
            } else {
                the_point.v[V_Y] -= 999.6f;
                the_point.v[V_Y] = FindYVerticallyBelow2(&the_point);
                if (the_point.v[V_Y] < -100.f) {
                    the_point.v[V_Y] = 1000.f;
                    the_point.v[V_Y] = FindYVerticallyBelow2(&the_point);
                }
                if (point_count == 0 || pInstructions[j - 1].data.point_data.position.v[V_Y] < 500.f) {
                    the_mat = gPath_mat_normal;
                } else {
                    the_mat = gPath_mat_calc;
                }
            }
            SquirtPathVertex(&the_model->vertices[vertex_count], &the_point);
            vertex_count += 4;
#if defined(DETHRACE_FIX_BUGS)
            last_vertex_count = vertex_count;
#endif
            if (point_count != 0) {
                // Connect previous path vertex cross with current path vertex cross
                the_model->faces[face_count].vertices[0] = vertex_count - 4;
                the_model->faces[face_count].vertices[1] = vertex_count - 3;
                the_model->faces[face_count].vertices[2] = last_vertex_count - 4;
                the_model->faces[face_count].material = the_mat;
                face_count++;
                the_model->faces[face_count].vertices[0] = vertex_count - 3;
                the_model->faces[face_count].vertices[1] = last_vertex_count - 3;
                the_model->faces[face_count].vertices[2] = last_vertex_count - 4;
                the_model->faces[face_count].material = the_mat;
                face_count++;
                the_model->faces[face_count].vertices[0] = vertex_count - 2;
                the_model->faces[face_count].vertices[1] = vertex_count - 1;
                the_model->faces[face_count].vertices[2] = last_vertex_count - 2;
                the_model->faces[face_count].material = the_mat;
                face_count++;
                the_model->faces[face_count].vertices[0] = vertex_count - 1;
                the_model->faces[face_count].vertices[1] = last_vertex_count - 1;
                the_model->faces[face_count].vertices[2] = last_vertex_count - 2;
                the_model->faces[face_count].material = the_mat;
                face_count++;
            }
            point_count++;
            last_vertex_count = vertex_count;
            if (j == pInit_instruc) {
                the_model->vertices[vertex_count].p = the_point;
                the_model->vertices[vertex_count].p.v[V_Y] += .3f;
                the_model->vertices[vertex_count].p.v[V_Z] += .05f;
                the_model->vertices[vertex_count + 1].p = the_point;
                the_model->vertices[vertex_count + 1].p.v[V_Y] += .3f;
                the_model->vertices[vertex_count + 1].p.v[V_Z] += -.05f;
                the_model->vertices[vertex_count + 2].p = the_point;
                the_model->vertices[vertex_count + 2].p.v[V_Y] += .3f;
                the_model->vertices[vertex_count + 2].p.v[V_X] += .05f;
                the_model->vertices[vertex_count + 3].p = the_point;
                the_model->vertices[vertex_count + 3].p.v[V_Y] += .3f;
                the_model->vertices[vertex_count + 3].p.v[V_X] += -.05f;
                vertex_count += 4;
                the_model->faces[face_count].vertices[0] = vertex_count - 8;
                the_model->faces[face_count].vertices[1] = vertex_count - 4;
                the_model->faces[face_count].vertices[2] = vertex_count - 3;
                the_model->faces[face_count].material = gInit_pos_mat_calc;
                face_count++;
                the_model->faces[face_count].vertices[0] = vertex_count - 8;
                the_model->faces[face_count].vertices[1] = vertex_count - 2;
                the_model->faces[face_count].vertices[2] = vertex_count - 1;
                the_model->faces[face_count].material = gInit_pos_mat_calc;
                face_count++;
            }
        }
    }
    BrModelUpdate(the_model, BR_MODU_ALL);
    BrModelAdd(the_model);
    the_actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
    the_actor->model = the_model;
    BrActorAdd(gPath_actor, the_actor);
    return the_actor;
}

// IDA: void __cdecl WriteOutPeds()
void WriteOutPeds() {
    int i;
    int j;
    int k;
    int point_count;
    int min_ped;
    int min_point;
    br_vector3 the_point;
    br_vector3 last_point;
    br_scalar min_distance;
    br_scalar the_distance;
    char s[255];
    tPedestrian_data* the_pedestrian;
    tPedestrian_instruction* the_instruction;
    tPed_choice* the_choice;
    FILE* f;
    tPath_name the_path;
    LOG_TRACE("()");

    PathCat(the_path, gApplication_path, "PEDPATHS.TXT");
    f = DRfopen(the_path, "wt");
    fprintf(f, "// ****** START OF PEDESTRIAN STUFF ******\n\n");
    fprintf(f, "0\t\t\t\t// Ped subs table index\n\n");
    fprintf(f, "%d\t\t\t\t// Number of pedestrians\n\n", gPed_count);
    for (i = 0, the_pedestrian = gPedestrian_array; i < gPed_count; i++, the_pedestrian++) {
        fprintf(f, "// Pedestrian number %d\n\n", i + 1);
        fprintf(f, "%d\t\t\t\t// Ref num\n", the_pedestrian->ref_number);
        fprintf(f, "%d\t\t\t\t// Number of instructions\n", the_pedestrian->number_of_instructions);
        fprintf(f, "%d\t\t\t\t// Initial instruction\n\n", the_pedestrian->first_instruction + 1);
        for (j = 0, the_instruction = the_pedestrian->instruction_list; j < the_pedestrian->number_of_instructions; j++, the_instruction++) {
            switch (the_instruction->type) {
            case ePed_instruc_point:
                fprintf(f, "\tpoint\n");
                fprintf(f, "\t%.3f,%.3f,%.3f\n",
                    the_instruction->data.point_data.position.v[V_X],
                    the_instruction->data.point_data.position.v[V_Y],
                    the_instruction->data.point_data.position.v[V_Z]);
                break;
            case ePed_instruc_xpoint:
                fprintf(f, "\txpoint\n");
                fprintf(f, "\t%.3f,%.3f,%.3f\n",
                    the_instruction->data.point_data.position.v[V_X],
                    the_instruction->data.point_data.position.v[V_Y],
                    the_instruction->data.point_data.position.v[V_Z]);
                break;
            case ePed_instruc_bchoice:
                fprintf(f, "\tbchoice\n");
                fprintf(f, "%d", the_instruction->data.choice_data.number_of_choices);
                for (k = 0, the_choice = the_instruction->data.choice_data.choices; k < the_instruction->data.choice_data.number_of_choices; k++, the_choice++) {
                    fprintf(f, "%d,%d,%d,", the_choice->danger_level, the_choice->percentage_chance, the_choice->marker_ref);
                }
                break;
            case ePed_instruc_fchoice:
                fprintf(f, "\tfchoice\n");
                fprintf(f, "%d", the_instruction->data.choice_data.number_of_choices);
                for (k = 0, the_choice = the_instruction->data.choice_data.choices; k < the_instruction->data.choice_data.number_of_choices; k++, the_choice++) {
                    fprintf(f, "%d,%d,%d,", the_choice->danger_level, the_choice->percentage_chance, the_choice->marker_ref);
                }
                break;
            case ePed_instruc_dead:
                fprintf(f, "\tdead\n");
                fprintf(f, "%d", the_instruction->data.death_data.death_sequence);
                break;
            case ePed_instruc_bmarker:
                fprintf(f, "\tbmarker\n");
                fprintf(f, "%d", the_instruction->data.marker_data.marker_ref);
                break;
            case ePed_instruc_fmarker:
                fprintf(f, "\tfmarker\n");
                fprintf(f, "%d", the_instruction->data.marker_data.marker_ref);
                break;
            case ePed_instruc_baction:
                fprintf(f, "\tbaction\n");
                fprintf(f, "%d", the_instruction->data.action_data.action_index);
                break;
            case ePed_instruc_faction:
                fprintf(f, "\tfaction\n");
                fprintf(f, "%d", the_instruction->data.action_data.action_index);
                break;
            case ePed_instruc_reverse:
                fprintf(f, "\treverse\n");
                break;
            }
        }
        fprintf(f, "\n\n");
    }
    fclose(f);
}

// IDA: void __cdecl AddPed()
void AddPed() {
    tPedestrian_instruction* instructions;
    FILE* g;
    tPath_name the_path;
    LOG_TRACE("()");

    PathCat(the_path, gApplication_path, "PEDESTRN.TXT");
    g = DRfopen(the_path, "rt");
    if (g == NULL) {
        FatalError(kFatalError_OpenPedestrianFile);
    }
    gPed_instrucs[gPed_instruc_count].type = ePed_instruc_reverse;
    gPed_instruc_count++;
    instructions = BrMemAllocate(gPed_instruc_count * sizeof(tPedestrian_instruction), kMem_ped_new_instruc);
    memcpy(instructions, gPed_instrucs, gPed_instruc_count * sizeof(tPedestrian_instruction));
    CreatePedestrian(g, instructions, gPed_instruc_count, gInit_ped_instruc, gPed_ref_num, 1);
    fclose(g);
    gPed_instruc_count = 0;
    WriteOutPeds();
}

// IDA: void __usercall NewPed(int pRef_num@<EAX>)
void NewPed(int pRef_num) {
    char s[255];
    LOG_TRACE("(%d)", pRef_num);

    if (pRef_num >= 0) {
        if (PDKeyDown(0)) {
            pRef_num += 100;
        }
        if (PDKeyDown(2)) {
            pRef_num += 20;
        }
        if (PDKeyDown(1)) {
            pRef_num += 10;
        }
    }
    if (gCurrent_ped_path_actor != NULL) {
        gCurrent_ped_path_actor->model->nvertices -= 4;
        gCurrent_ped_path_actor->model->nfaces -= 4;
        BrModelUpdate(gCurrent_ped_path_actor->model, BR_MODU_ALL);
        gCurrent_ped_path_actor = NULL;
    }
    if (gPed_instruc_count != 0) {
        AddPed();
    }
    if (pRef_num >= 0) {
        gPed_ref_num = pRef_num;
        gPed_instruc_count = 0;
        gInit_ped_instruc = 0;
        gNumber_of_pedestrians++;
        gTotal_peds++;
        sprintf(s, "New pedestrian, refnum = %d", pRef_num);
        NewTextHeadupSlot(4, 0, 2000, -1, s);
        if (pRef_num >= 100) {
            DropInitPedPoint();
            NewPed(-1);
        }
    }
}

// IDA: void __cdecl RemoveCurrentPedPath()
void RemoveCurrentPedPath() {
    LOG_TRACE("()");

    if (gCurrent_ped_path_actor != NULL) {
        BrModelRemove(gCurrent_ped_path_actor->model);
        BrModelFree(gCurrent_ped_path_actor->model);
        BrActorRemove(gCurrent_ped_path_actor);
        BrActorFree(gCurrent_ped_path_actor);
        gCurrent_ped_path_actor = NULL;
    }
}

// IDA: void __cdecl ScrubPedestrian()
void ScrubPedestrian() {
    LOG_TRACE("()");

    gPed_instruc_count = 0;
    gNumber_of_pedestrians--;
    gTotal_peds--;
    NewTextHeadupSlot(4, 0, 2000, -1, "Scrubbed pedestrian");
    RemoveCurrentPedPath();
}

// IDA: void __cdecl TogglePedDetect()
void TogglePedDetect() {
    LOG_TRACE("()");

    gDetect_peds = !gDetect_peds;
    if (gDetect_peds) {
        NewTextHeadupSlot(4, 0, 2000, -1, "Pedestrian are meat again");
    } else {
        NewTextHeadupSlot(4, 0, 2000, -1, "Pedestrians are invulnerable");
    }
}

// IDA: void __cdecl NewPed0()
void NewPed0() {
    LOG_TRACE("()");

    NewPed(0);
}

// IDA: void __cdecl NewPed1()
void NewPed1() {
    LOG_TRACE("()");

    NewPed(1);
}

// IDA: void __cdecl NewPed2()
void NewPed2() {
    LOG_TRACE("()");

    NewPed(2);
}

// IDA: void __cdecl NewPed3()
void NewPed3() {
    LOG_TRACE("()");

    NewPed(3);
}

// IDA: void __cdecl NewPed4()
void NewPed4() {
    LOG_TRACE("()");

    NewPed(4);
}

// IDA: void __cdecl NewPed5()
void NewPed5() {
    LOG_TRACE("()");

    NewPed(5);
}

// IDA: void __cdecl NewPed6()
void NewPed6() {
    LOG_TRACE("()");

    NewPed(6);
}

// IDA: void __cdecl NewPed7()
void NewPed7() {
    LOG_TRACE("()");

    NewPed(7);
}

// IDA: void __cdecl NewPed8()
void NewPed8() {
    LOG_TRACE("()");

    NewPed(8);
}

// IDA: void __cdecl NewPed9()
void NewPed9() {
    LOG_TRACE("()");

    NewPed(9);
}

// IDA: void __cdecl NewPed0B()
void NewPed0B() {
    LOG_TRACE("()");

    NewPed(61);
}

// IDA: void __cdecl NewPed1B()
void NewPed1B() {
    LOG_TRACE("()");

    NewPed(51);
}

// IDA: void __cdecl NewPed2B()
void NewPed2B() {
    LOG_TRACE("()");

    NewPed(52);
}

// IDA: void __cdecl NewPed3B()
void NewPed3B() {
    LOG_TRACE("()");

    NewPed(53);
}

// IDA: void __cdecl NewPed4B()
void NewPed4B() {
    LOG_TRACE("()");

    NewPed(54);
}

// IDA: void __cdecl NewPed5B()
void NewPed5B() {
    LOG_TRACE("()");

    NewPed(55);
}

// IDA: void __cdecl NewPed6B()
void NewPed6B() {
    LOG_TRACE("()");

    NewPed(56);
}

// IDA: void __cdecl NewPed7B()
void NewPed7B() {
    LOG_TRACE("()");

    NewPed(57);
}

// IDA: void __cdecl NewPed8B()
void NewPed8B() {
    LOG_TRACE("()");

    NewPed(58);
}

// IDA: void __cdecl NewPed9B()
void NewPed9B() {
    LOG_TRACE("()");

    NewPed(59);
}

// IDA: void __cdecl MungeShowPedPath()
void MungeShowPedPath() {
    LOG_TRACE("()");

    RemoveCurrentPedPath();
    gPed_instrucs[gPed_instruc_count] = gPed_instrucs[gPed_instruc_count - 1];
    gCurrent_ped_path_actor = BuildPedPaths(gPed_instrucs, gPed_instruc_count + 1, gInit_ped_instruc);
}

// IDA: void __cdecl DropPedPoint2()
void DropPedPoint2() {
    LOG_TRACE("()");

    gPed_instrucs[gPed_instruc_count].type = ePed_instruc_point;
    gPed_instrucs[gPed_instruc_count].data.point_data.irreversable = 0;
    gPed_instrucs[gPed_instruc_count].data.point_data.position = *gOur_pos;
    gPed_instruc_count++;
    MungeShowPedPath();
}

// IDA: void __cdecl DropPedPoint()
void DropPedPoint() {
    LOG_TRACE("()");

#if defined(DETHRACE_FIX_BUGS)
    if (gPath_actor == NULL) {
        return;
    }
#endif
    DropPedPoint2();
    NewTextHeadupSlot(4, 0, 2000, -1, "Dropped pedestrian point");
}

// IDA: void __cdecl DropInitPedPoint()
void DropInitPedPoint() {
    LOG_TRACE("()");

    #if defined(DETHRACE_FIX_BUGS)
        if (gPath_actor == NULL) {
            return;
        }
    #endif
    gInit_ped_instruc = gPed_instruc_count;
    DropPedPoint2();
    NewTextHeadupSlot(4, 0, 2000, -1, "Dropped initial pedestrian point");
}

// IDA: void __cdecl DropPedPointAir2()
void DropPedPointAir2() {
    LOG_TRACE("()");

    gPed_instrucs[gPed_instruc_count].type = ePed_instruc_point;
    gPed_instrucs[gPed_instruc_count].data.point_data.irreversable = 0;
    gPed_instrucs[gPed_instruc_count].data.point_data.position = *gOur_pos;
    gPed_instrucs[gPed_instruc_count].data.point_data.position.v[V_Y] += 1000.4f;
    gPed_instruc_count++;
    MungeShowPedPath();
}

// IDA: void __cdecl DropPedPointAir()
void DropPedPointAir() {
    LOG_TRACE("()");

#if defined(DETHRACE_FIX_BUGS)
    if (gPath_actor == NULL) {
        return;
    }
#endif
    DropPedPointAir2();
    NewTextHeadupSlot(4, 0, 2000, -1, "Dropped auto-y pedestrian point");
}

// IDA: void __cdecl DropInitPedPointAir()
void DropInitPedPointAir() {
    LOG_TRACE("()");

#if defined(DETHRACE_FIX_BUGS)
    if (gPath_actor == NULL) {
        return;
    }
#endif
    gInit_ped_instruc = gPed_instruc_count;
    DropPedPointAir2();
    NewTextHeadupSlot(4, 0, 2000, -1, "Dropped initial auto-y pedestrian point");
}

// IDA: br_uint_32 __cdecl KillActorsModel(br_actor *pActor, void *pArg)
intptr_t KillActorsModel(br_actor* pActor, void* pArg) {
    LOG_TRACE("(%p, %p)", pActor, pArg);

    if (pActor->model != NULL) {
        BrModelRemove(pActor->model);
        BrModelFree(pActor->model);
    }
    return 0;
}

// IDA: void __cdecl DisposePedPaths()
void DisposePedPaths() {
    LOG_TRACE("()");

    if (gPath_actor != NULL) {
        DRActorEnumRecurse(gPath_actor, KillActorsModel, NULL);
        BrActorRemove(gPath_actor);
        BrActorFree(gPath_actor);
        BrMaterialRemove(gPath_mat_normal);
        BrMaterialFree(gPath_mat_normal);
        BrMaterialRemove(gPath_mat_calc);
        BrMaterialFree(gPath_mat_calc);
        gPath_actor = NULL;
    }
}

// IDA: void __usercall GetPedPos(int *pPed_index@<EAX>, int *pPoint_index@<EDX>)
void GetPedPos(int* pPed_index, int* pPoint_index) {
    int i;
    int j;
    int point_count;
    br_vector3 the_point;
    br_vector3 last_point;
    br_scalar min_distance;
    br_scalar the_distance;
    LOG_TRACE("(%p, %p)", pPed_index, pPoint_index);

    min_distance = BR_SCALAR_MAX;
    for (i = 0; i < gPed_count; i++) {
        for (j = 0; j < gPedestrian_array[i].number_of_instructions; j++) {
            if (gPedestrian_array[i].instruction_list[j].type == ePed_instruc_point || gPedestrian_array[i].instruction_list[j].type == ePed_instruc_xpoint) {
                the_point = gPedestrian_array[i].instruction_list[j].data.point_data.position;
                if (the_point.v[V_Y] >= 500.f) {
                    the_point.v[V_Y] -= 999.6f;
                    the_point.v[V_Y] = FindYVerticallyBelow2(&the_point);
                    if (the_point.v[V_Y] < -100.f) {
                        the_point.v[V_Y] = 1000.f;
                        the_point.v[V_Y] = FindYVerticallyBelow2(&the_point);
                    }
                }
                the_distance = Vector3DistanceSquared(&the_point, gOur_pos);
                if (the_distance < min_distance) {
                    *pPed_index = i;
                    *pPoint_index = j;
                    min_distance = the_distance;
                }
            }
            // last_point = the_point;
        }
    }
}

// IDA: void __cdecl ShowPedPos()
void ShowPedPos() {
    int min_ped;
    int min_point;
    char s[255];
    LOG_TRACE("()");

    GetPedPos(&min_ped, &min_point);
    sprintf(s, "Nearest pedestrian is #%d, point #%d", min_ped + 1, min_point + 1);
    NewTextHeadupSlot(4, 0, 3000, -1, s);
}

// IDA: void __cdecl ShowPedPaths()
void ShowPedPaths() {
    int i;
    br_model* the_model;
    LOG_TRACE("()");

    if (gPath_actor == NULL) {
        gPath_actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        BrActorAdd(gNon_track_actor, gPath_actor);

        gPath_mat_normal = BrMaterialAllocate(NULL);
        gPath_mat_normal->index_base = 3;
        gPath_mat_normal->index_range = 4;
        gPath_mat_normal->flags |= BR_MATF_TWO_SIDED;
        BrMaterialAdd(gPath_mat_normal);

        gPath_mat_calc = BrMaterialAllocate(NULL);
        gPath_mat_calc->index_base = 93;
        gPath_mat_calc->index_range = 4;
        gPath_mat_calc->flags |= BR_MATF_TWO_SIDED;
        BrMaterialAdd(gPath_mat_calc);

        gInit_pos_mat_calc = BrMaterialAllocate(NULL);
        gInit_pos_mat_calc->index_base = 51;
        gInit_pos_mat_calc->index_range = 4;
        gInit_pos_mat_calc->flags |= BR_MATF_TWO_SIDED;
        BrMaterialAdd(gInit_pos_mat_calc);

        for (i = 0; i < gPed_count; i++) {
            BuildPedPaths(gPedestrian_array[i].instruction_list,
                gPedestrian_array[i].number_of_instructions,
                gPedestrian_array[i].first_instruction);
        }
    }
    if (gPath_actor->render_style == BR_RSTYLE_FACES) {
        gPath_actor->render_style = BR_RSTYLE_NONE;
    } else {
        gPath_actor->render_style = BR_RSTYLE_FACES;
    }
}

// IDA: void __cdecl PullPedPoint()
void PullPedPoint() {
    int the_ped;
    int the_point;
    LOG_TRACE("()");

    if (gPed_instruc_count == 0) {
        GetPedPos(&the_ped, &the_point);
        gPedestrian_array[the_ped].instruction_list[the_point].data.point_data.position = *gOur_pos;
        WriteOutPeds();
        DisposePedPaths();
        ShowPedPaths();
    }
}

// IDA: void __cdecl PullPedPointAir()
void PullPedPointAir() {
    int the_ped;
    int the_point;
    LOG_TRACE("()");

    if (gPed_instruc_count == 0) {
        GetPedPos(&the_ped, &the_point);
        gPedestrian_array[the_ped].instruction_list[the_point].data.point_data.position = *gOur_pos;
        gPedestrian_array[the_ped].instruction_list[the_point].data.point_data.position.v[V_Y] += 1000.4f;
        WriteOutPeds();
        DisposePedPaths();
        ShowPedPaths();
    }
}

// IDA: void __cdecl DeletePedPath()
void DeletePedPath() {
    int the_ped;
    int the_point;
    int i;
    LOG_TRACE("()");

    GetPedPos(&the_ped, &the_point);
    for (i = the_ped; i < gPed_count - 1; i++) {
        gPedestrian_array[i] = gPedestrian_array[i + 1];
    }
    gPed_count--;
    WriteOutPeds();
    DisposePedPaths();
    ShowPedPaths();
}

// IDA: void __cdecl DeletePedPoint()
void DeletePedPoint() {
    LOG_TRACE("()");

    if (gPed_instruc_count == 0) {
        return;
    }
    if (gPed_instruc_count == 1) {
        ScrubPedestrian();
    } else {
        if (gInit_ped_instruc == gPed_instruc_count) {
            gInit_ped_instruc--;
        }
        gPed_instruc_count--;
        MungeShowPedPath();
    }
}

// IDA: void __cdecl DisposePedestrians()
void DisposePedestrians() {
    int i;
    int j;
    tPedestrian_data* the_pedestrian;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gPed_gibs); i++) {
        PossibleService();
        BrModelRemove(gPed_gibs[i].actor->model);
        BrModelFree(gPed_gibs[i].actor->model);
        gPed_gibs[i].actor->model = NULL;
        if (gPed_gibs[i].size >= 0) {
            BrActorRemove(gPed_gibs[i].actor);
        }
        BrActorFree(gPed_gibs[i].actor);
    }
    for (i = 0; i < COUNT_OF(gPed_gib_materials); i++) {
        for (j = 0; j < gPed_size_counts[i]; j++) {
            BrMapRemove(gPed_gib_materials[i].materials[j]->colour_map);
            BrPixelmapFree(gPed_gib_materials[i].materials[j]->colour_map);
            gPed_gib_materials[i].materials[j]->colour_map = NULL;
            BrMaterialRemove(gPed_gib_materials[i].materials[j]);
            BrMaterialFree(gPed_gib_materials[i].materials[j]);
        }
    }

    // AddPed->CreatePedestrian needs the gPedestrian array.
    if (gPed_instruc_count != 0) {
        AddPed();
    }

    for (i = 0; i < gPed_count; i++) {
        the_pedestrian = &gPedestrian_array[i];
        PossibleService();
        BrActorRemove(the_pedestrian->actor);
        BrActorFree(the_pedestrian->actor);
        BrMemFree(the_pedestrian->instruction_list);
        if (!the_pedestrian->cloned) {
            BrMemFree(the_pedestrian->action_list);
            BrMemFree(the_pedestrian->sequences);
        }
    }
    ClearOutStorageSpace(&gPedestrians_storage_space);
    BrMemFree(gPedestrian_array);
    BrTableRemove(gProx_ray_shade_table);
    BrPixelmapFree(gProx_ray_shade_table);
    DisposePedPaths();
}

// IDA: void __cdecl DoPedReport()
void DoPedReport() {
    FILE* f;
    tPath_name the_path;
    time_t the_bloody_time;
    int i;
    int j;
    int count;
    int last_ref_num;
    int ped_count;     // added by dethrace
    int powerup_count; // added by dethrace
    char s[256];       // added by dethrace
    LOG_TRACE("()");

    powerup_count = 0;
    ped_count = 0;
    PathCat(the_path, gApplication_path, "PEDINFO.TXT");
    f = fopen(the_path, "at");
    time(&the_bloody_time);
    fprintf(f, "PEDESTRIAN REPORT FOR '%s' ON %s\n\n", gCurrent_race.name, ctime(&the_bloody_time));
    fprintf(f, "RefNum   Count\n===============================\n");
    last_ref_num = -1;
    for (i = 0; i < gPed_count; i++) {
        if (last_ref_num < gPedestrian_array[i].ref_number) {
            last_ref_num = gPedestrian_array[i].ref_number;
        }
    }
    for (i = 0; i <= last_ref_num; i++) {
        if (i == 100) {
            fprintf(f, "\n");
        }
        count = 0;
        for (j = 0; j < gPed_count; j++) {
            if (gPedestrian_array[j].ref_number == j) {
                count++;
            }
        }
        if (count != 0) {
            if (i < 100) {
                s[0] = '\0';
                ped_count += count;
            } else {
                GetPowerupMessage(i - 100, s);
                powerup_count += count;
            }
            fprintf(f, "%6d    %5d      %s\n", i, count, s);
        }
    }
    fprintf(f, "\n\nSUMMARY:\n\n");
    fprintf(f, "Peds:     %5d\n", ped_count);
    fprintf(f, "Powerups: %5d\n", powerup_count);
    fprintf(f, "\n\n\n\n");
    fclose(f);
}

// IDA: void __usercall RenderProximityRays(br_pixelmap *pRender_screen@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>, tU32 pTime)
void RenderProximityRays(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world, tU32 pTime) {
    int i;
    float seed;
    tU32 the_time;
    br_vector3 car_pos;
    br_vector3 ped_pos;
    br_vector3 car_pos_cam;
    br_vector3 ped_pos_cam;
    br_vector3 car_add;
    br_vector3 car_add_c;
    br_vector3 ped_add;
    br_vector3 ray;
    br_vector3 r1;
    br_vector3 from_pos;
    br_vector3 to_pos;
    br_model* car_model;
    br_scalar distance;
    br_scalar t;
    LOG_TRACE("(%p, %p, %p, %p, %d)", pRender_screen, pDepth_buffer, pCamera, pCamera_to_world, pTime);

    the_time = GetTotalTime();
    StartPipingSession(ePipe_chunk_prox_ray);
    for (i = 0; i < COUNT_OF(gProximity_rays); i++) {
        if (gProximity_rays[i].start_time == 0) {
            continue;
        }
        if (the_time - gProximity_rays[i].start_time < 500) {
            AddProxRayToPipingSession(i,
                gProximity_rays[i].car,
                GET_PEDESTRIAN_INDEX(gProximity_rays[i].ped),
                gProximity_rays[i].start_time);
            car_model = gProximity_rays[i].car->car_model_actors[gProximity_rays[i].car->principal_car_actor].actor->model;

            BrVector3Set(&car_add_c, 0.f, (car_model->bounds.max.v[V_Y] - car_model->bounds.min.v[V_Y]) / -5.f, 0.f);
            BrMatrix34ApplyV(&car_add, &car_add_c, &gProximity_rays[i].car->car_master_actor->t.t.mat);
            BrVector3Add(&car_pos, &gProximity_rays[i].car->pos, &car_add);
            DRMatrix34TApplyP(&car_pos_cam, &car_pos, &gCamera_to_world);

            BrVector3Set(&ped_add, 0.f, gProximity_rays[i].ped->actor->t.t.mat.m[1][1] / 2.f, 0.f);
            BrVector3Add(&ped_pos, &gProximity_rays[i].ped->pos, &ped_add);
            DRMatrix34TApplyP(&ped_pos_cam, &ped_pos, &gCamera_to_world);

            BrVector3Sub(&r1, &ped_pos_cam, &car_pos_cam);
            distance = BrVector3Length(&r1);
            BrVector3Normalise(&r1, &r1);

            from_pos = car_pos_cam;

            seed = the_time + ped_pos.v[V_X] + ped_pos.v[V_Y] + ped_pos.v[V_Z] + car_pos.v[V_X] + car_pos.v[V_Y] + car_pos.v[V_Z];
            srand(seed);

            t = 0.f;
            do {
                BrVector3Scale(&ray, &r1, t);
                BrVector3Add(&to_pos, &ray, &car_pos_cam);
                to_pos.v[V_X] += SRandomPosNeg(0.1f);
                to_pos.v[V_Y] += SRandomPosNeg(0.1f);
                to_pos.v[V_Z] += SRandomPosNeg(0.1f);
                DrawLine3D(&to_pos, &from_pos, pRender_screen, pDepth_buffer, gProx_ray_shade_table);
                from_pos = to_pos;
                t += 0.05f;
            } while (t < distance);
            DrawLine3D(&ped_pos_cam, &from_pos, pRender_screen, pDepth_buffer, gProx_ray_shade_table);
        } else {
            gProximity_rays[i].start_time = 0;
        }
    }
    EndPipingSession();
}

// IDA: void __usercall AdjustProxRay(int pRay_index@<EAX>, tU16 pCar_ID@<EDX>, tU16 pPed_index@<EBX>, tU32 pTime@<ECX>)
void AdjustProxRay(int pRay_index, tU16 pCar_ID, tU16 pPed_index, tU32 pTime) {
    LOG_TRACE("(%d, %d, %d, %d)", pRay_index, pCar_ID, pPed_index, pTime);

    if ((pCar_ID & 0xff00) == 0) {
        gProximity_rays[pRay_index].car = &gProgram_state.current_car;
    } else {
        gProximity_rays[pRay_index].car = GetCarSpec(pCar_ID >> 8, pCar_ID & 0xff);
    }
    gProximity_rays[pRay_index].ped = &gPedestrian_array[pPed_index];
    gProximity_rays[pRay_index].start_time = pTime;
}

// IDA: void __usercall ReceivedPedestrian(tNet_contents *pContents@<EAX>, tNet_message *pMessage@<EDX>, tU32 pReceive_time@<EBX>)
void ReceivedPedestrian(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time) {
    tPedestrian_data* the_pedestrian;
    tNet_game_player_info* sender;
    tNet_game_player_info* murderer;
    int modified_action;
    int modified_action_2;
    int action;
    int instruction;
    int dead;
    tPedestrian_sequence* the_sequence;
    LOG_TRACE("(%p, %p, %d)", pContents, pMessage, pReceive_time);

    if (pContents->data.pedestrian.index >= 0 && pContents->data.pedestrian.index < gPed_count) {
        the_pedestrian = &gPedestrian_array[pContents->data.pedestrian.index];
        action = (pContents->data.pedestrian.action_instruction & 0xf) - 1;
        if (action < 0) {
            action = 0;
        } else if (action >= the_pedestrian->number_of_actions) {
            action = the_pedestrian->number_of_actions - 1;
        }
        instruction = pContents->data.pedestrian.action_instruction >> 4;
        if (instruction < 0) {
            instruction = 0;
        } else if (instruction >= the_pedestrian->number_of_instructions) {
            instruction = the_pedestrian->number_of_instructions - 1;
        }
        if (pContents->data.pedestrian.flags & 0x8) {
            murderer = NetPlayerFromID(pContents->data.pedestrian.murderer);
            modified_action = action;
            if (action == the_pedestrian->fatal_ground_impact_action) {
                modified_action = the_pedestrian->fatal_car_impact_action;
            }
            if (the_pedestrian->reverse_frames) {
                modified_action_2 = 0;
            } else {
                modified_action_2 = the_pedestrian->current_action;
                if (modified_action_2 == the_pedestrian->fatal_ground_impact_action) {
                    modified_action_2 = the_pedestrian->fatal_car_impact_action;
                }
            }
            if (modified_action != modified_action_2
                && modified_action == the_pedestrian->fatal_car_impact_action
                && the_pedestrian->ref_number < 100
                && gNet_mode == eNet_mode_host
                && gCurrent_net_game->type == eNet_game_type_carnage
                && murderer != NULL
                && the_pedestrian->murderer == -1) {
                the_pedestrian->reverse_frames = 0;
                murderer->score++;
                gProgram_state.peds_killed++;
                the_pedestrian->murderer = murderer->ID;
            }
        }
        if ((!the_pedestrian->active || gWait_for_it) && the_pedestrian->hit_points != -100) {
            if (pContents->data.pedestrian.flags & 0x2) {
                the_pedestrian->mid_air = 1;
            } else {
                the_pedestrian->mid_air = 0;
            }
            if (pContents->data.pedestrian.flags & 0x4) {
                the_pedestrian->instruction_direction = -1;
            } else {
                the_pedestrian->instruction_direction = 1;
            }
            if (the_pedestrian->current_action != action
                && the_pedestrian->current_action != the_pedestrian->non_fatal_car_impact_action
                && the_pedestrian->current_action != the_pedestrian->fatal_ground_impact_action) {
                gPed_sound_disable = 1;
                ChangeActionTo(the_pedestrian, action, 0);
                gPed_sound_disable = 0;
            }
            if (pContents->data.pedestrian.flags & 0x10) {
                the_pedestrian->hit_points = -100;
            } else {
                if (pContents->data.pedestrian.flags & 0x8) {
                    if (the_pedestrian->hit_points > 0) {
                        the_pedestrian->current_sequence = 0;
                        the_sequence = &the_pedestrian->sequences[the_pedestrian->action_list[the_pedestrian->current_action].sequences[0].sequence_index];
                        the_pedestrian->current_frame = the_sequence->number_of_frames - 1;
                        the_pedestrian->done_initial = 0;
                        the_pedestrian->colour_map = the_sequence->frames[the_pedestrian->current_frame].pixelmap;
                    }
                    the_pedestrian->hit_points = -10;
                } else {
                    the_pedestrian->hit_points = 0;
                }
            }
            if (the_pedestrian->current_instruction != instruction) {
                the_pedestrian->current_instruction = instruction;
                if (the_pedestrian->current_action != the_pedestrian->fatal_car_impact_action) {
                    PedestrianNextInstruction(the_pedestrian, 0.f, 1, 0);
                }
            }
            the_pedestrian->current_speed = pContents->data.pedestrian.speed;
            BrVector3Copy(&the_pedestrian->pos, &pContents->data.pedestrian.pos);
            BrVector3Copy(&the_pedestrian->actor->t.t.translate.t, &pContents->data.pedestrian.pos);
            if (pContents->data.pedestrian.flags & 0x20) {
                BrVector3Copy(&the_pedestrian->to_pos, &pContents->data.pedestrian.to_pos);
            }
        } else if (the_pedestrian->current_action == the_pedestrian->fatal_car_impact_action
            || action != the_pedestrian->fatal_car_impact_action
            || the_pedestrian->reverse_frames) {
            if (the_pedestrian->action_list[the_pedestrian->current_action].danger_level < the_pedestrian->action_list[action].danger_level) {
                ChangeActionTo(the_pedestrian, action, 0);
            }
        } else {
            the_pedestrian->fate = NetCarFromPlayerID(pContents->data.pedestrian.murderer);
        }
        if (pContents->data.pedestrian.flags & 0x40
            && the_pedestrian->ref_number >= 100
            && pContents->data.pedestrian.respawn_time_or_spin_period
            && the_pedestrian->respawn_time == 0) {
            the_pedestrian->respawn_time = pContents->data.pedestrian.respawn_time_or_spin_period;
        }
    }
}

// IDA: void __usercall SendAllPedestrianPositions(tPlayer_ID pPlayer@<EAX>)
void SendAllPedestrianPositions(tPlayer_ID pPlayer) {
    int i;
    tPedestrian_data* the_pedestrian;
    tNet_contents* the_contents;
    LOG_TRACE("(%d)", pPlayer);

    gSend_peds = 1;
    for (i = 0; i < gPed_count; i++) {
        the_pedestrian = &gPedestrian_array[i];
        if (the_pedestrian->munged) {
            SendPedestrian(the_pedestrian, i);
        }
    }
}
