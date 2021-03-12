#include "pedestrn.h"
#include "brender.h"
#include "constants.h"
#include "errors.h"
#include "globvars.h"
#include "graphics.h"
#include "input.h"
#include "loading.h"
#include "pd/sys.h"
#include "sound.h"
#include "trig.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>
#include <time.h>

#define FLAG_WAVING_BASTARD_REF 99

int gPed_gib_counts[4][5];
int gPed_gib_maxes[4][5];
tPedestrian_instruction gPed_instrucs[100];
char* gPed_geb_names[4][5] = {
    { "GBIGGIBS.PIX", "GBIGGIB2.PIX", "GBIGGIB3.PIX", NULL, NULL },
    { "GELBOW.PIX", "GPELVIS.PIX", "GHEAD.PIX", "GRIBS.PIX", "GBONE.PIX" },
    { "GHAND.PIX", "GCHUNK02.PIX", "GEYEBALL.PIX", "GCOLON.PIX", "GCHUNK01.PIX" },
    { "GCHUNK04.PIX",
        "GSPLAT1.PIX",
        "GCHUNK03.PIX",
        "GSPLAT2.PIX",
        "GSPLATOT.PIX" }
};
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

float gMin_ped_gib_speeds[4];
tPed_gib gPed_gibs[30];
float gPed_gib_speeds[4];
char* gPed_gib_names[4][5] = {
    { "BIGGIBS.PIX", "BIGGIBS2.PIX", "BIGGIBS3.PIX", NULL, NULL },
    { "ELBOW.PIX", "PELVIS.PIX", "HEAD.PIX", "RIBS.PIX", "BONE.PIX" },
    { "HAND.PIX", "CHUNK02.PIX", "EYEBALL.PIX", "COLON.PIX", "CHUNK01.PIX" },
    { "CHUNK04.PIX", "SPLAT1.PIX", "CHUNK03.PIX", "SPLAT2.PIX", "SPLATOUT.PIX" }
};
float gPed_gib_distrib[4];
tPed_gib_materials gPed_gib_materials[4];
int gPed_size_counts[4] = { 3, 5, 5, 5 };

tProximity_ray gProximity_rays[20];
int gVesuvian_corpses;
int gPed_colliding;
float gZombie_factor;
int gRespawn_variance;
br_scalar gPed_scale_factor;
int gTotal_peds;
int gPedestrian_harvest;
br_actor* gPath_actor;
br_vector3 gPed_pos_camera;
int gMin_respawn_time;
br_material* gPath_mat_calc;
br_actor* gCurrent_ped_path_actor;
float gPedestrian_speed_factor;
int gExploding_pedestrians;
int gBlind_pedestrians;
br_material* gPed_material;
br_material* gPath_mat_normal;
int gPedestrians_on;
br_material* gInit_pos_mat_calc;
int gPed_other;
int gAttracted_pedestrians;
int gPed_count;
int gPed_ref_num;
br_scalar gMax_distance_squared;
br_model* gPed_model;
float gDanger_level;
br_scalar gExploding_ped_scale[3];
br_vector3 gDanger_direction;
int gPed_instruc_count;
int gInit_ped_instruc;
br_vector3 gZero_v_pedestrn; // added _pedestrn to avoid name collision with car.c
char* gRate_commands[3] = { "fixed", "speed", "variable" };
char* gCollide_commands[1] = { "collide" };
int gCurrent_lollipop_index;
int gDetect_peds;
int gVesuvians_this_time;
int gNumber_of_ped_gibs;
tU32 gLast_ped_message_send;
tPedestrian_data* gFlag_waving_bastard;
int gReally_stupid_ped_bug_enable;
int gNumber_of_pedestrians;
br_pixelmap* gProx_ray_shade_table;
tPedestrian_data* gPedestrian_array;
int gVesuvians_last_time;
tU32 gLast_ped_splat_time;
int gCurrent_ped_multiplier;
int gPed_sound_disable;
tPedestrian_instruction* gInitial_instruction;
int gSend_peds;

// IDA: void __usercall PedModelUpdate(br_model *pModel@<EAX>, br_scalar x0, br_scalar y0, br_scalar x1, br_scalar y1, br_scalar x2, br_scalar y2, br_scalar x3, br_scalar y3)
void PedModelUpdate(br_model* pModel, br_scalar x0, br_scalar y0, br_scalar x1, br_scalar y1, br_scalar x2, br_scalar y2, br_scalar x3, br_scalar y3) {
    LOG_TRACE("(%p, %f, %f, %f, %f, %f, %f, %f, %f)", pModel, x0, y0, x1, y1, x2, y2, x3, y3);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ActorIsPedestrian@<EAX>(br_actor *pActor@<EAX>)
int ActorIsPedestrian(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall PedHeightFromActor@<ST0>(br_actor *pActor@<EAX>)
br_scalar PedHeightFromActor(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GetPedestrianValue@<EAX>(br_actor *pActor@<EAX>)
int GetPedestrianValue(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PedestrianActorIsPerson@<EAX>(br_actor *pActor@<EAX>)
int PedestrianActorIsPerson(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);
    NOT_IMPLEMENTED();
}

// IDA: br_actor* __usercall GetPedestrianActor@<EAX>(int pIndex@<EAX>)
br_actor* GetPedestrianActor(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __usercall GetPedestrianTexture@<EAX>(br_actor *pActor@<EAX>, int *pFlipped@<EDX>)
br_pixelmap* GetPedestrianTexture(br_actor* pActor, int* pFlipped) {
    LOG_TRACE("(%p, %p)", pActor, pFlipped);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TogglePedestrians()
void TogglePedestrians() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
        the_model = BrModelAllocate(0, 4, 2);
        the_model->flags |= BR_MODF_UPDATEABLE;
        the_model->faces->vertices[0] = 0;
        the_model->faces->vertices[1] = 2;
        the_model->faces->vertices[2] = 1;
        the_model->faces->material = 0;
        the_model->faces->smoothing = 1;
        the_model->faces[1].vertices[0] = 0;
        the_model->faces[1].vertices[1] = 3;
        the_model->faces[1].vertices[2] = 2;
        the_model->faces[1].material = 0;
        the_model->faces[1].smoothing = 1;
        the_model->vertices->p.v[0] = -1.0;
        the_model->vertices->p.v[1] = 0.0;
        the_model->vertices->p.v[2] = 0.0;
        the_model->vertices->map.v[0] = 0.0;
        the_model->vertices->map.v[1] = 1.0;
        the_model->vertices[1].p.v[0] = -1.0;
        the_model->vertices[1].p.v[1] = 1.0;
        the_model->vertices[1].p.v[2] = 0.0;
        the_model->vertices[1].map.v[0] = 0.0;
        the_model->vertices[1].map.v[1] = 0.0;
        the_model->vertices[2].p.v[0] = 1.0;
        the_model->vertices[2].p.v[1] = 1.0;
        the_model->vertices[2].p.v[2] = 0.0;
        the_model->vertices[2].map.v[0] = 1.0;
        the_model->vertices[2].map.v[1] = 0.0;
        the_model->vertices[3].p.v[0] = 1.0;
        the_model->vertices[3].p.v[1] = 0.0;
        the_model->vertices[3].p.v[2] = 0.0;
        the_model->vertices[3].map.v[0] = 1.0;
        the_model->vertices[3].map.v[1] = 1.0;
        BrModelAdd(the_model);
        gPed_gibs[i].actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        gPed_gibs[i].actor->parent = NULL;
        gPed_gibs[i].actor->model = the_model;
        gPed_gibs[i].actor->render_style = BR_RSTYLE_NONE;
    }
    for (i = 0; i < 4; i++) {
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PedCallBack(br_actor *pActor, br_model *pModel, br_material *pMaterial, void *pRender_data, br_uint_8 pStyle, int pOn_screen)
void PedCallBack(br_actor* pActor, br_model* pModel, br_material* pMaterial, void* pRender_data, br_uint_8 pStyle, int pOn_screen) {
    tPedestrian_data* ped;
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", pActor, pModel, pMaterial, pRender_data, pStyle, pOn_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitPeds()
void InitPeds() {
    LOG_TRACE("()");
    gPed_model = BrModelAllocate(0, 4, 2);
    gPed_model->faces->vertices[0] = 0;
    gPed_model->faces->vertices[1] = 2;
    gPed_model->faces->vertices[2] = 1;
    gPed_model->faces->material = 0;
    gPed_model->faces->smoothing = 1;
    gPed_model->faces[1].vertices[0] = 0;
    gPed_model->faces[1].vertices[1] = 3;
    gPed_model->faces[1].vertices[2] = 2;
    gPed_model->faces[1].material = 0;
    gPed_model->faces[1].smoothing = 1;
    gPed_model->vertices->p.v[0] = -0.5;
    gPed_model->vertices->p.v[1] = 0.0;
    gPed_model->vertices->p.v[2] = 0.0;
    gPed_model->vertices->map.v[0] = 0.000099999997;
    gPed_model->vertices->map.v[1] = 0.99989998;
    gPed_model->vertices[1].p.v[0] = -0.5;
    gPed_model->vertices[1].p.v[1] = 1.0;
    gPed_model->vertices[1].p.v[2] = 0.0;
    gPed_model->vertices[1].map.v[0] = 0.000099999997;
    gPed_model->vertices[1].map.v[1] = 0.000099999997;
    gPed_model->vertices[2].p.v[0] = 0.5;
    gPed_model->vertices[2].p.v[1] = 1.0;
    gPed_model->vertices[2].p.v[2] = 0.0;
    gPed_model->vertices[2].map.v[0] = 0.99989998;
    gPed_model->vertices[2].map.v[1] = 0.000099999997;
    gPed_model->vertices[3].p.v[0] = 0.5;
    gPed_model->vertices[3].p.v[1] = 0.0;
    gPed_model->vertices[3].p.v[2] = 0.0;
    gPed_model->vertices[3].map.v[0] = 0.99989998;
    gPed_model->vertices[3].map.v[1] = 0.99989998;
    BrModelAdd(gPed_model);
}

// IDA: void __usercall MungeModelSize(br_actor *pActor@<EAX>, br_scalar pScaling_factor)
void MungeModelSize(br_actor* pActor, br_scalar pScaling_factor) {
    br_pixelmap* the_pix;
    br_scalar half_width;
    br_scalar half_height;
    LOG_TRACE("(%p, %f)", pActor, pScaling_factor);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResetAllPedGibs()
void ResetAllPedGibs() {
    int i;
    tPed_gib* the_ped_gib;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AdjustPedGib(int pIndex@<EAX>, int pSize@<EDX>, int pGib_index@<EBX>, int pPed_index@<ECX>, br_matrix34 *pTrans)
void AdjustPedGib(int pIndex, int pSize, int pGib_index, int pPed_index, br_matrix34* pTrans) {
    tPed_gib* the_ped_gib;
    LOG_TRACE("(%d, %d, %d, %d, %p)", pIndex, pSize, pGib_index, pPed_index, pTrans);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall KillPedestrian(tPedestrian_data *pPedestrian@<EAX>)
void KillPedestrian(tPedestrian_data* pPedestrian) {
    LOG_TRACE("(%p)", pPedestrian);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CalcPedWidthNHeight(tPedestrian_data *pPedestrian@<EAX>, br_pixelmap *pPixelmap@<EDX>, br_scalar *pHeight@<EBX>, br_scalar *pWidth@<ECX>)
void CalcPedWidthNHeight(tPedestrian_data* pPedestrian, br_pixelmap* pPixelmap, br_scalar* pHeight, br_scalar* pWidth) {
    br_pixelmap* reference_pixel_bastard;
    br_scalar scale_to_use;
    LOG_TRACE("(%p, %p, %p, %p)", pPedestrian, pPixelmap, pHeight, pWidth);

    reference_pixel_bastard = pPedestrian->sequences->frames[0].pixelmap;
    if (pPedestrian->ref_number >= 0x64u) {
        scale_to_use = 1.0;
    } else {
        scale_to_use = gPed_scale_factor;
    }
    *pHeight = (double)pPixelmap->height
        / (double)reference_pixel_bastard->height
        * pPedestrian->height
        * scale_to_use;
    *pWidth = (double)pPixelmap->width / (double)pPixelmap->height * *pHeight;
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DetachPedFromCar(tPedestrian_data *pPedestrian@<EAX>)
void DetachPedFromCar(tPedestrian_data* pPedestrian) {
    LOG_TRACE("(%p)", pPedestrian);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetPedPos(tPedestrian_data *pPedestrian@<EAX>)
void SetPedPos(tPedestrian_data* pPedestrian) {
    br_vector3 temp_v;
    LOG_TRACE("(%p)", pPedestrian);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DetachPedActorFromCar(br_actor *pActor@<EAX>)
void DetachPedActorFromCar(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
        x_offset = the_frame->offset.v[X] * 2.0;
        y_offset = the_frame->offset.v[Y];
        pPedestrian->height2 = pPedestrian->height2 * 2.0;
        pPedestrian->width = pPedestrian->width * 2.0;
    } else {
        x_offset = the_frame->offset.v[X];
        y_offset = the_frame->offset.v[Y];
    }
    pPedestrian->flipped = the_frame->flipped;
    if (pPedestrian->actor->parent != gDont_render_actor) {
        old_parent = pPedestrian->actor->parent;
        BrActorRelink(gDont_render_actor, pPedestrian->actor);
    }
    BrMatrix34Scale(&pPedestrian->actor->t.t.mat, pPedestrian->width, pPedestrian->height2, 1.0);
    if (pPedestrian->spin_period != 0.0 && pPedestrian->actor->model) {
        height_over2 = pPedestrian->height2 / 2.0;
        BrMatrix34PostTranslate(&pPedestrian->actor->t.t.mat, 0.0, -height_over2, 0.0);
        if (pPedestrian->spin_period >= 0.0) {
            temp_scalar = (double)(GetTotalTime() - pPedestrian->last_action_change) / pPedestrian->spin_period * 360.0;
        } else {
            temp_scalar = ((double)(GetTotalTime() - pPedestrian->last_action_change) / pPedestrian->spin_period * 360.0 + 360.0);
        }
        DRMatrix34PostRotateZ(&pPedestrian->actor->t.t.mat, BR_ANGLE_DEG(temp_scalar));
        BrMatrix34PostTranslate(&pPedestrian->actor->t.t.mat, 0.0, height_over2, 0.0);
    }
    temp_scalar = FastScalarArcTan2Angle(gCamera_to_world.m[2][0], gCamera_to_world.m[2][2]);
    DRMatrix34PostRotateY(&pPedestrian->actor->t.t.mat, temp_scalar);
    if (old_parent
        && pPedestrian->current_action == pPedestrian->fatal_car_impact_action
        && !pPedestrian->done_initial
        && pPedestrian->actor->model) {

        y_offset += FastScalarSin(((double)MAX(pPedestrian->current_frame, 0)
                        / (double)(pPedestrian->sequences[pPedestrian->current_sequence].looping_frame_start - 1) * 180.0))
            * pPedestrian->jump_magnitude;
    }
    if (old_parent) {
        BrMatrix34PostTranslate(&pPedestrian->actor->t.t.mat, old_parent->t.t.translate.t.v[0] + x_offset, old_parent->t.t.translate.t.v[1] + y_offset, old_parent->t.t.translate.t.v[2]);
        BrActorRelink(old_parent, pPedestrian->actor);
        pPedestrian->actor->t.t.translate.t.v[0] += pPedestrian->offset.v[0];
        pPedestrian->actor->t.t.translate.t.v[1] += pPedestrian->offset.v[1];
        pPedestrian->actor->t.t.translate.t.v[2] += pPedestrian->offset.v[2];
    } else if (pPedestrian->spin_period == 0.0) {
        BrMatrix34PostTranslate(&pPedestrian->actor->t.t.mat, pPedestrian->pos.v[0] + x_offset - pPedestrian->offset.v[0], pPedestrian->pos.v[1] + y_offset - pPedestrian->offset.v[1], pPedestrian->pos.v[2]);
        pPedestrian->offset.v[0] = x_offset;
        pPedestrian->offset.v[1] = y_offset;
        pPedestrian->offset.v[2] = 0.0;
    } else {
        BrMatrix34PostTranslate(&pPedestrian->actor->t.t.mat, pPedestrian->offset.v[0] + x_offset, pPedestrian->offset.v[1] + y_offset, pPedestrian->offset.v[2]);
    }
    gCurrent_lollipop_index = AddToLollipopQueue(pPedestrian->actor, gCurrent_lollipop_index);
}

// IDA: void __usercall ChangeActionTo(tPedestrian_data *pPedestrian@<EAX>, int pAction_index@<EDX>, int pRedo_frames_etc@<EBX>)
void ChangeActionTo(tPedestrian_data* pPedestrian, int pAction_index, int pRedo_frames_etc) {
    tPedestrian_action* the_action;
    int the_sound;
    tU32 the_pitch;
    LOG_TRACE("(%p, %d, %d)", pPedestrian, pAction_index, pRedo_frames_etc);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MakeFlagWavingBastardWaveHisFlagWhichIsTheProbablyTheLastThingHeWillEverDo()
void MakeFlagWavingBastardWaveHisFlagWhichIsTheProbablyTheLastThingHeWillEverDo() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    LOG_TRACE("(%p, %p, %p, %f, %f, %f, %f, %f, %f, %p, %p)", pPedestrian, pCar, pCar_actor, pPed_x, pPed_z, pCar_bounds_min_x, pCar_bounds_max_x, pCar_bounds_min_z, pCar_bounds_max_z, pMin_ped_bounds_car, pMax_ped_bounds_car);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckLastPed()
void CheckLastPed() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FancyATossOffMate@<EAX>(tPedestrian_data *pPedestrian@<EAX>, tCollision_info *pCar@<EDX>, float pImpact_speed)
int FancyATossOffMate(tPedestrian_data* pPedestrian, tCollision_info* pCar, float pImpact_speed) {
    LOG_TRACE("(%p, %p, %f)", pPedestrian, pCar, pImpact_speed);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SendPedestrian(tPedestrian_data *pPedestrian@<EAX>, int pIndex@<EDX>)
void SendPedestrian(tPedestrian_data* pPedestrian, int pIndex) {
    tNet_contents* the_contents;
    tNet_message* the_message;
    int size_decider;
    LOG_TRACE("(%p, %d)", pPedestrian, pIndex);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AdjustPedestrian(int pIndex@<EAX>, int pAction_index@<EDX>, int pFrame_index@<EBX>, int pHit_points@<ECX>, int pDone_initial, tU16 pParent, br_actor *pParent_actor, float pSpin_period, br_scalar pJump_magnitude, br_vector3 *pOffset, br_vector3 *pTrans)
void AdjustPedestrian(int pIndex, int pAction_index, int pFrame_index, int pHit_points, int pDone_initial, tU16 pParent, br_actor* pParent_actor, float pSpin_period, br_scalar pJump_magnitude, br_vector3* pOffset, br_vector3* pTrans) {
    tPedestrian_data* pedestrian;
    br_actor* parent;
    br_vector3 old_pos;
    LOG_TRACE("(%d, %d, %d, %d, %d, %d, %p, %f, %f, %p, %p)", pIndex, pAction_index, pFrame_index, pHit_points, pDone_initial, pParent, pParent_actor, pSpin_period, pJump_magnitude, pOffset, pTrans);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SquirtPathVertex(br_vertex *pFirst_vertex@<EAX>, br_vector3 *pPoint@<EDX>)
void SquirtPathVertex(br_vertex* pFirst_vertex, br_vector3* pPoint) {
    LOG_TRACE("(%p, %p)", pFirst_vertex, pPoint);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResetAllPedestrians()
void ResetAllPedestrians() {
    int i;
    tPedestrian_data* the_pedestrian;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GroundPedestrian(tPedestrian_data *pPedestrian@<EAX>)
void GroundPedestrian(tPedestrian_data* pPedestrian) {
    br_scalar new_y;
    br_vector3 cast_point;
    tPedestrian_sequence* sequence;
    LOG_TRACE("(%p)", pPedestrian);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RevivePedestrian(tPedestrian_data *pPedestrian@<EAX>, int pAnimate@<EDX>)
void RevivePedestrian(tPedestrian_data* pPedestrian, int pAnimate) {
    LOG_TRACE("(%p, %d)", pPedestrian, pAnimate);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RespawnPedestrians()
void RespawnPedestrians() {
    int i;
    tPedestrian_data* the_pedestrian;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl GetPedCount()
int GetPedCount() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GetPedPosition@<EAX>(int pIndex@<EAX>, br_vector3 *pPos@<EDX>)
int GetPedPosition(int pIndex, br_vector3* pPos) {
    tPedestrian_data* pedestrian;
    LOG_TRACE("(%d, %p)", pIndex, pPos);
    NOT_IMPLEMENTED();
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
    the_pedestrian = &gPedestrian_array[gPed_count++];
    the_pedestrian->ref_number = pRef_num;
    the_pedestrian->magic_number = 1348822049;
    the_pedestrian->cloned = 0;
    if (!pForce_read) {
        for (i = 0; i < gPed_count - 1; i++) {
            if (gPedestrian_array[i].ref_number == the_pedestrian->ref_number) {
                memcpy(the_pedestrian, &gPedestrian_array[i], sizeof(tPedestrian_data));
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
    LABEL_69:
        PossibleService();
        the_pedestrian->number_of_instructions = pInstruc_count;
        the_pedestrian->first_instruction = pInit_instruc;
        the_pedestrian->current_instruction = pInit_instruc;
        the_pedestrian->instruction_list = pInstructions;
        the_pedestrian->colour_map = the_pedestrian->sequences->frames[0].pixelmap;
        the_pedestrian->offset.v[0] = 0.0;
        the_pedestrian->offset.v[1] = 0.0;
        the_pedestrian->offset.v[2] = 0.0;
        gInitial_instruction = 0;
        PedestrianNextInstruction(the_pedestrian, 0.0, 1, 0);
        the_pedestrian->from_pos = the_pedestrian->actor->t.t.translate.t;
        MungePedModel(the_pedestrian);
    } else {
        rewind(pG);
        while (1) {
            GetALineAndDontArgue(pG, s);
            str = strtok(s, "\t ,/");
            sscanf(str, "%d", &the_ref);
            if (the_pedestrian->ref_number != the_ref) {
                break;
            }
        LABEL_13:
            if (the_pedestrian->ref_number == the_ref) {
                temp_float1 = GetAFloat(pG);
                the_pedestrian->height = temp_float1;
                the_pedestrian->credits_value = GetAnInt(pG);
                the_pedestrian->hit_points = GetAnInt(pG);
                the_pedestrian->active = 0;
                GetALineAndDontArgue(pG, s2);
                str2 = strtok(s2, "\t ,/");
                sscanf(str2, "%d", &the_pedestrian->number_of_exploding_sounds);
                for (j = 0; j < the_pedestrian->number_of_exploding_sounds; j++) {
                    str2 = strtok(0, "\t ,/");
                    sscanf(str2, "%d", &the_pedestrian->exploding_sounds[j]);
                }
                the_pedestrian->falling_sound = GetAnInt(pG);
                the_pedestrian->acceleration = GetAFloat(pG);
                the_pedestrian->image_index = GetAnInt(pG);
                LoadNShadeTables(&gPedestrians_storage_space, pG, 1);
                LoadNPixelmaps(&gPedestrians_storage_space, pG, 1);
                GetALineAndDontArgue(pG, s2);
                if (!gPed_material) {
                    str2 = strtok(s2, "\t ,/");
                    PathCat(the_path, gApplication_path, "MATERIAL");
                    PathCat(the_path, the_path, s2);
                    gPed_material = BrMaterialLoad(the_path);
                    if (!gPed_material) {
                        FatalError(77, s2);
                    }
                    gPed_material->flags &= 0xFFFFEFFF;
                    gPed_material->flags &= 0xFFFFFFF8;
                    gPed_material->index_shade = 0;
                    gPed_material->colour_map = gPedestrians_storage_space.pixelmaps[0];
                    BrMaterialAdd(gPed_material);
                }
                gPed_material->colour_map = *gPedestrians_storage_space.pixelmaps;
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
                the_pedestrian->current_speed = 0.0;
                the_pedestrian->instruction_direction = 1;
                the_pedestrian->spin_period = 0.0;
                the_pedestrian->falling_speed = 0.0;
                the_pedestrian->mid_air = 0;
                the_pedestrian->last_special_volume = 0;
                the_pedestrian->last_sound = 0;
                the_pedestrian->giblets_being_sat_upon = 0;
                the_pedestrian->last_sound_action = -1;
                the_pedestrian->last_sound_make = 0;
                the_pedestrian->fate = 0;
                the_pedestrian->munged = 0;
                the_pedestrian->collided_last_time = 0;
                the_pedestrian->reverse_frames = 0;
                the_pedestrian->respawn_time = 0;
                the_pedestrian->killers_ID = -1;
                the_pedestrian->murderer = -1;
                the_pedestrian->sent_dead_message = 0;
                minnest_min = 3.4028235e38;
                maxest_min = -3.4028235e38;
                minnest_max = 3.4028235e38;
                maxest_max = -3.4028235e38;
                the_pedestrian->number_of_actions = GetAnInt(pG);
                the_pedestrian->action_list = BrMemAllocate(sizeof(tPedestrian_action) * the_pedestrian->number_of_actions, kMem_ped_action_list);
                the_action = the_pedestrian->action_list;
                for (i = 0; i < the_pedestrian->number_of_actions; i++) {
                    PossibleService();
                    GetPairOfFloats(pG, &the_action->danger_level, &the_action->percentage_chance);
                    GetPairOfFloats(pG, &the_action->initial_speed, &the_action->looping_speed);
                    the_action->reaction_time = (tU32)(GetAFloat(pG) * 1000.0);
                    GetALineAndDontArgue(pG, s2);
                    str2 = strtok(s2, "\t ,/");
                    sscanf(str2, "%d", &the_action->number_of_sounds);
                    for (j = 0; the_action->number_of_sounds > j; ++j) {
                        str2 = strtok(0, "\t ,/");
                        sscanf(str2, "%d", &the_action->sounds[j]);
                    }
                    the_action->number_of_bearings = GetAnInt(pG);
                    if (the_action->number_of_bearings > 7) {
                        FatalError(85);
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
                    collide_frame = GetALineAndInterpretCommand(pG, gCollide_commands, 1);
                    the_sequence->frame_rate_type = GetALineAndInterpretCommand(pG, gRate_commands, 3);

                    if (the_sequence->frame_rate_type == ePed_frame_speed || the_sequence->frame_rate_type == ePed_frame_variable) {
                        GetPairOfFloats(pG, &the_sequence->frame_rate_factor1, &the_sequence->frame_rate_factor2);
                    }
                    the_sequence->number_of_frames = 0;
                    the_sequence->looping_frame_start = -1;
                    for (j = 0; j < 2; j++) {
                        new_frames = GetAnInt(pG);
                        if (new_frames + the_sequence->number_of_frames > 10) {
                            FatalError(83);
                        }
                        for (k = the_sequence->number_of_frames; k < new_frames + the_sequence->number_of_frames; k++) {
                            GetAString(pG, s);
                            the_sequence->frames[k].pixelmap = BrMapFind(s);
                            if (!the_sequence->frames[k].pixelmap) {
                                the_sequence->frames[k].pixelmap = LoadPixelmap(s);
                                if (!the_sequence->frames[k].pixelmap) {
                                    FatalError(78, s);
                                }
                                BrMapAdd(the_sequence->frames[k].pixelmap);
                            }
                            GetALineAndDontArgue(pG, s);
                            str = strtok(s, "\t ,/");
                            sscanf(str, "%f", &temp_float1);
                            str = strtok(0, "\t ,/");
                            sscanf(str, "%f", &temp_float2);
                            str = strtok(0, "\t ,/");
                            the_sequence->frames[k].offset.v[0] = temp_float1;
                            the_sequence->frames[k].offset.v[1] = temp_float2;
                            the_sequence->frames[k].flipped = !strcmp(str, "flipped");
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
                goto LABEL_69;
            }
        }
        while (1) {
            PossibleService();
            GetALineAndDontArgue(pG, s);
            if (!strcmp(s, "END OF PEDESTRIANS")) {
                break;
            }
            if (!strcmp(s, "NEXT PEDESTRIAN")) {
                goto LABEL_13;
            }
        }
        BrActorRemove(the_pedestrian->actor);
        BrActorFree(the_pedestrian->actor);
        gPed_count--;
        DRS3StartSound(gIndexed_outlets[0], 3100);
    }
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResetPedMaterial()
void ResetPedMaterial() {
    int i;
    int j;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    gFlag_waving_bastard = 0;
    gPedestrians_on = 1;
    gPed_scale_factor = 1.0;
    gPed_ref_num = 0;
    gExploding_pedestrians = 0;
    gBlind_pedestrians = 0;
    gPedestrian_speed_factor = 1.0;
    if (gProgram_state.sausage_eater_mode) {
        PathCat(the_path, gApplication_path, "BORING.TXT");
    } else {
        PathCat(the_path, gApplication_path, "PEDESTRN.TXT");
    }
    g = DRfopen(the_path, "rt");
    if (!g) {
        FatalError(75);
    }
    gPed_count = 0;
    gLast_ped_splat_time = 0;
    temp_int = GetAnInt(pF);
    if (gAusterity_mode) {
        ped_count = temp_int / 2 + 2;
    } else {
        ped_count = temp_int;
    }
    gPedestrian_array = BrMemAllocate(sizeof(tPedestrian_data) * (ped_count + (gAusterity_mode == 0 ? 200 : 0)), kMem_ped_array_stain);
    if (PDKeyDown(KEY_LCTRL) && PDKeyDown(KEY_LSHIFT) && PDKeyDown(KEY_A)) {
        check_for_duplicates = 1;
        DRS3StartSound(gIndexed_outlets[0], 3202);
        DRS3StartSound(gIndexed_outlets[0], 3202);
    }
    for (i = 0; i < ped_count; i++) {
        PossibleService();
        ref_number = GetAnInt(pF);
        for (j = 0; pSubs_count > j; ++j) {
            if (pSubs_array[j].orig == ref_number) {
                ref_number = pSubs_array[j].subs;
                break;
            }
        }
        instruc_count = GetAnInt(pF);
        init_instruc = GetAnInt(pF) - 1;
        instructions = BrMemAllocate(sizeof(tPedestrian_instruction) * instruc_count, kMem_ped_instructions);

        the_instruction = instructions;
        for (j = 0; j < instruc_count; j++) {
            PossibleService();
            the_instruction->type = GetALineAndInterpretCommand(pF, gInstruc_commands, COUNT_OF(gInstruc_commands));
            if (the_instruction->type && the_instruction->type != 9) {
                FatalError(76);
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
                if (the_instruction->data.choice_data.number_of_choices > 2) {
                    FatalError(84);
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
            the_instruction++;
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
                if (gPedestrian_array[j].instruction_list->data.point_data.position.v[0] == instructions->data.point_data.position.v[0]
                    && gPedestrian_array[j].instruction_list->data.point_data.position.v[1] == instructions->data.point_data.position.v[1]
                    && gPedestrian_array[j].instruction_list->data.point_data.position.v[2] == instructions->data.point_data.position.v[2]) {
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
    gProx_ray_shade_table = GenerateShadeTable(8, gRender_palette, 215, 255, 233, 0.5, 0.75, 0.89999998);
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AddPed()
void AddPed() {
    tPedestrian_instruction* instructions;
    FILE* g;
    tPath_name the_path;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NewPed(int pRef_num@<EAX>)
void NewPed(int pRef_num) {
    char s[255];
    LOG_TRACE("(%d)", pRef_num);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RemoveCurrentPedPath()
void RemoveCurrentPedPath() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScrubPedestrian()
void ScrubPedestrian() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TogglePedDetect()
void TogglePedDetect() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed0()
void NewPed0() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed1()
void NewPed1() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed2()
void NewPed2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed3()
void NewPed3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed4()
void NewPed4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed5()
void NewPed5() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed6()
void NewPed6() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed7()
void NewPed7() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed8()
void NewPed8() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed9()
void NewPed9() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed0B()
void NewPed0B() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed1B()
void NewPed1B() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed2B()
void NewPed2B() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed3B()
void NewPed3B() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed4B()
void NewPed4B() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed5B()
void NewPed5B() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed6B()
void NewPed6B() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed7B()
void NewPed7B() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed8B()
void NewPed8B() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewPed9B()
void NewPed9B() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MungeShowPedPath()
void MungeShowPedPath() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropPedPoint2()
void DropPedPoint2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropPedPoint()
void DropPedPoint() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropInitPedPoint()
void DropInitPedPoint() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropPedPointAir2()
void DropPedPointAir2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropPedPointAir()
void DropPedPointAir() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropInitPedPointAir()
void DropInitPedPointAir() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl KillActorsModel(br_actor *pActor, void *pArg)
br_uint_32 KillActorsModel(br_actor* pActor, void* pArg) {
    LOG_TRACE("(%p, %p)", pActor, pArg);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposePedPaths()
void DisposePedPaths() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ShowPedPos()
void ShowPedPos() {
    int min_ped;
    int min_point;
    char s[255];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ShowPedPaths()
void ShowPedPaths() {
    int i;
    br_model* the_model;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PullPedPoint()
void PullPedPoint() {
    int the_ped;
    int the_point;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PullPedPointAir()
void PullPedPointAir() {
    int the_ped;
    int the_point;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DeletePedPath()
void DeletePedPath() {
    int the_ped;
    int the_point;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DeletePedPoint()
void DeletePedPoint() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposePedestrians()
void DisposePedestrians() {
    int i;
    int j;
    tPedestrian_data* the_pedestrian;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AdjustProxRay(int pRay_index@<EAX>, tU16 pCar_ID@<EDX>, tU16 pPed_index@<EBX>, tU32 pTime@<ECX>)
void AdjustProxRay(int pRay_index, tU16 pCar_ID, tU16 pPed_index, tU32 pTime) {
    LOG_TRACE("(%d, %d, %d, %d)", pRay_index, pCar_ID, pPed_index, pTime);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SendAllPedestrianPositions(tPlayer_ID pPlayer@<EAX>)
void SendAllPedestrianPositions(tPlayer_ID pPlayer) {
    int i;
    tPedestrian_data* the_pedestrian;
    tNet_contents* the_contents;
    LOG_TRACE("(%d)", pPlayer);
    NOT_IMPLEMENTED();
}
