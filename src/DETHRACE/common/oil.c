#include "oil.h"
#include "brender.h"
#include "common/globvars.h"
#include "common/loading.h"
#include <stdlib.h>

int gNext_oil_pixie;
char* gOil_pixie_names[1] = { "OIL.PIX" };
br_scalar gZ_buffer_diff;
br_scalar gMin_z_diff;
br_pixelmap* gOil_pixies[1];
tOil_spill_info gOily_spills[15];

void InitOilSpills() {
    int i;
    br_model* the_model;
    br_material* the_material;
    LOG_TRACE("()");

    gOil_pixies[0] = LoadPixelmap(gOil_pixie_names[0]);
    BrMapAdd(gOil_pixies[0]);
    for (i = 0; i < 15; i++) {
        the_material = BrMaterialAllocate(NULL);
        BrMaterialAdd(the_material);
        the_material->ka = 0.99000001;
        the_material->kd = 0.0;
        the_material->ks = 0.0;
        the_material->power = 0.0;
        the_material->index_base = 0;
        //LOBYTE(the_material->flags) = v2 | 0x25;
        the_material->flags |= 0x25;
        the_material->index_range = 0;
        the_material->colour_map = 0;
        BrMatrix23Identity(&the_material->map_transform);
        the_material->index_shade = BrTableFind("IDENTITY.TAB");
        BrMaterialUpdate(the_material, BR_MATU_ALL);
        the_model = BrModelAllocate(NULL, 4, 2);
        the_model->flags |= BR_MODF_KEEP_ORIGINAL;

        the_model->faces->vertices[0] = 2;
        the_model->faces->vertices[1] = 1;
        the_model->faces->vertices[2] = 0;
        the_model->faces->material = 0;
        the_model->faces->smoothing = 1;
        the_model->faces[1].vertices[0] = 3;
        the_model->faces[1].vertices[1] = 2;
        the_model->faces[1].vertices[2] = 0;
        the_model->faces[1].material = 0;
        the_model->faces[1].smoothing = 1;
        the_model->vertices->p.v[0] = -1.0;
        the_model->vertices->p.v[1] = 0.0;
        the_model->vertices->p.v[2] = -1.0;
        the_model->vertices->map.v[0] = 0.0;
        the_model->vertices->map.v[1] = 1.0;
        the_model->vertices[1].p.v[0] = 1.0;
        the_model->vertices[1].p.v[1] = 0.0;
        the_model->vertices[1].p.v[2] = 1.0;
        the_model->vertices[1].map.v[0] = 0.0;
        the_model->vertices[1].map.v[1] = 0.0;
        the_model->vertices[2].p.v[0] = 1.0;
        the_model->vertices[2].p.v[1] = 0.0;
        the_model->vertices[2].p.v[2] = -1.0;
        the_model->vertices[2].map.v[0] = 1.0;
        the_model->vertices[2].map.v[1] = 0.0;
        the_model->vertices[3].p.v[0] = -1.0;
        the_model->vertices[3].p.v[1] = 0.0;
        the_model->vertices[3].p.v[2] = 1.0;
        the_model->vertices[3].map.v[0] = 1.0;
        the_model->vertices[3].map.v[1] = 1.0;
        gOily_spills[i].actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        gOily_spills[i].actor->model = the_model;
        gOily_spills[i].actor->render_style = 1;
        gOily_spills[i].actor->material = the_material;
        BrActorAdd(gNon_track_actor, gOily_spills[i].actor);
    }
}

void ResetOilSpills() {
    NOT_IMPLEMENTED();
}

void QueueOilSpill(tCar_spec* pCar) {
    int i;
    int oily_index;
    int oldest_one;
    tU32 the_time;
    tU32 oldest_time;
    NOT_IMPLEMENTED();
}

int OKToSpillOil(tOil_spill_info* pOil) {
    br_scalar temp;
    br_scalar size_with_margin;
    br_scalar distance;
    br_scalar mr_dotty;
    br_vector3 v;
    br_vector3 ray_pos;
    br_vector3 ray_dir;
    br_vector3 normal;
    tCar_spec* car;
    int i;
    int face_count;
    int found_one;
    br_angle angle_to_rotate_by;
    tBounds kev_bounds;
    tFace_ref the_list[10];
    tFace_ref* face_ref;
    NOT_IMPLEMENTED();
}

void Vector3Interpolate(br_vector3* pDst, br_vector3* pFrom, br_vector3* pTo, br_scalar pP) {
    NOT_IMPLEMENTED();
}

void EnsureGroundDetailVisible(br_vector3* pNew_pos, br_vector3* pGround_normal, br_vector3* pOld_pos) {
    br_scalar factor;
    br_scalar s;
    br_scalar dist;
    br_vector3 to_camera;
    NOT_IMPLEMENTED();
}

void MungeOilsHeightAboveGround(tOil_spill_info* pOil) {
    NOT_IMPLEMENTED();
}

void MungeIndexedOilsHeightAboveGround(int pIndex) {
    NOT_IMPLEMENTED();
}

void SetInitialOilStuff(tOil_spill_info* pOil, br_model* pModel) {
    NOT_IMPLEMENTED();
}

void ProcessOilSpills(tU32 pFrame_period) {
    int i;
    tU32 time;
    br_model* the_model;
    br_scalar grow_amount;
    br_scalar initial_size;
    br_scalar this_size;
    br_vector3 v;
    tNet_message* message;
    NOT_IMPLEMENTED();
}

int GetOilSpillCount() {
    NOT_IMPLEMENTED();
}

void GetOilSpillDetails(int pIndex, br_actor** pActor, br_scalar* pSize) {
    NOT_IMPLEMENTED();
}

int PointInSpill(br_vector3* pV, int pSpill) {
    NOT_IMPLEMENTED();
}

void GetOilFrictionFactors(tCar_spec* pCar, br_scalar* pFl_factor, br_scalar* pFr_factor, br_scalar* pRl_factor, br_scalar* pRr_factor) {
    int i;
    br_vector3 wheel_world;
    NOT_IMPLEMENTED();
}

void AdjustOilSpill(int pIndex, br_matrix34* pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec* pCar, br_vector3* pOriginal_pos, br_pixelmap* pPixelmap) {
    NOT_IMPLEMENTED();
}

void ReceivedOilSpill(tNet_contents* pContents) {
    int i;
    int oily_index;
    int oldest_one;
    tU32 the_time;
    tU32 oldest_time;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}
