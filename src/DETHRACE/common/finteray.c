#include "finteray.h"
#include "brender.h"
#include "brucetrk.h"
#include "car.h"
#include "formats.h"
#include "globvars.h"
#include "harness/trace.h"
#include "raycast.h"
#include "world.h"
#include <math.h>
#include <stdlib.h>

// GLOBAL: CARM95 0x0051e8fc
int gPling_materials = 1;

// GLOBAL: CARM95 0x0051e900
br_material* gSub_material;

// GLOBAL: CARM95 0x0051e904
br_material* gReal_material;

// GLOBAL: CARM95 0x0051e908
int gNfaces;

// GLOBAL: CARM95 0x0053e558
br_matrix34 gPick_model_to_view__finteray; // suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x0053e588
int gTemp_group;

// GLOBAL: CARM95 0x0053e54c
br_model* gNearest_model;

// GLOBAL: CARM95 0x0053e58c
br_model* gSelected_model;

// GLOBAL: CARM95 0x0053e554
int gNearest_face_group;

// GLOBAL: CARM95 0x0053e548
int gNearest_face;

// GLOBAL: CARM95 0x0053e550
br_scalar gNearest_T;

// GLOBAL: CARM95 0x00550240
tFace_ref* gPling_face;

// IDA: int __cdecl BadDiv(br_scalar a, br_scalar b)
// Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004abe0c
int BadDiv__finteray(br_scalar a, br_scalar b) {
    //
    return fabs(b) < 1.0f && fabs(a) > fabs(b) * BR_SCALAR_MAX;
}

// IDA: void __usercall DRVector2AccumulateScale(br_vector2 *a@<EAX>, br_vector2 *b@<EDX>, br_scalar s)
// Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004abe5b
void DRVector2AccumulateScale__finteray(br_vector2* a, br_vector2* b, br_scalar s) {

    a->v[0] = b->v[0] * s + a->v[0];
    a->v[1] = b->v[1] * s + a->v[1];
}

// IDA: int __usercall PickBoundsTestRay@<EAX>(br_bounds *b@<EAX>, br_vector3 *rp@<EDX>, br_vector3 *rd@<EBX>, br_scalar t_near, br_scalar t_far, br_scalar *new_t_near, br_scalar *new_t_far)
//  Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004ab244
int PickBoundsTestRay__finteray(br_bounds* b, br_vector3* rp, br_vector3* rd, br_scalar t_near, br_scalar t_far, br_scalar* new_t_near, br_scalar* new_t_far) {
    int i;
    float s;
    float t;

    for (i = 0; i < 3; i++) {
        if (rd->v[i] >= -0.00000023841858) {
            if (rd->v[i] <= 0.00000023841858) {
                if (b->max.v[i] < rp->v[i] || rp->v[i] < b->min.v[i]) {
                    return 0;
                }
            } else {
                s = (-1.0f / rd->v[i]) * (rp->v[i] - b->max.v[i]);
                if (s >= BR_SCALAR_MIN) {
                    if (s < t_far) {
                        t_far = (-1.0f / rd->v[i]) * (rp->v[i] - b->max.v[i]);
                    }
                } else {
                    t_far = BR_SCALAR_MIN;
                }
                t = (-1.0f / rd->v[i]) * (rp->v[i] - b->min.v[i]);
                if (t <= BR_SCALAR_MAX) {
                    if (t > t_near) {
                        t_near = (-1.0f / rd->v[i]) * (rp->v[i] - b->min.v[i]);
                    }
                } else {
                    t_near = BR_SCALAR_MAX;
                }
            }
        } else {
            s = (-1.0f / rd->v[i]) * (rp->v[i] - b->max.v[i]);
            if (s <= BR_SCALAR_MAX) {
                if (s > t_near) {
                    t_near = (-1.0f / rd->v[i]) * (rp->v[i] - b->max.v[i]);
                }
            } else {
                t_near = BR_SCALAR_MAX;
            }
            t = (-1.0f / rd->v[i]) * (rp->v[i] - b->min.v[i]);
            if (t >= BR_SCALAR_MIN) {
                if (t < t_far) {
                    t_far = (-1.0f / rd->v[i]) * (rp->v[i] - b->min.v[i]);
                }
            } else {
                t_far = BR_SCALAR_MIN;
            }
        }
    }
    if (t_far < t_near) {
        return 0;
    }
    *new_t_near = t_near;
    *new_t_far = t_far;
    return 1;
}

// IDA: int __usercall ActorRayPick2D@<EAX>(br_actor *ap@<EAX>, br_vector3 *pPosition@<EDX>, br_vector3 *pDir@<EBX>, br_model *model@<ECX>, br_material *material, dr_pick2d_cbfn *callback)
// FUNCTION: CARM95 0x004aaf5a
int ActorRayPick2D(br_actor* ap, br_vector3* pPosition, br_vector3* pDir, br_model* model, br_material* material, dr_pick2d_cbfn* callback) {
    br_actor* a;
    br_model* this_model;
    br_material* this_material;
    br_scalar t_near;
    br_scalar t_far;
    int r;
    br_matrix34 mat;
    br_matrix34 invmat;
    br_vector3 pos;
    br_vector3 dir;
    void* arg;

    t_near = 0.0;
    t_far = 1.0;
    r = 0;
    arg = NULL;
    if (ap->model != NULL) {
        this_model = ap->model;
    } else {
        this_model = model;
    }
    if (ap->material != NULL) {
        this_material = ap->material;
    } else {
        this_material = material;
    }
    if (ap->render_style == BR_RSTYLE_NONE) {
        return 0;
    }
    if (ap->identifier != NULL && ap->identifier[0] == '&') {
        BrTransformToMatrix34(&mat, &ap->t);
        BrMatrix34Inverse(&invmat, &mat);
        BrMatrix34ApplyP(&pos, pPosition, &invmat);
        BrMatrix34ApplyV(&dir, pDir, &invmat);
        pPosition = &pos;
        pDir = &dir;
    }
    if (ap->type == BR_ACTOR_MODEL) {
        if (PickBoundsTestRay__finteray(&this_model->bounds, pPosition, pDir, t_near, t_far, &t_near, &t_far)) {
            t_near = 0.0;
            t_far = MIN(1.f, gNearest_T);
            r = callback(ap, this_model, this_material, pPosition, pDir, t_near, t_far, arg);
            if (r) {
                return r;
            }
        }
        if (r) {
            return r;
        }
    } else if (ap->type >= BR_ACTOR_BOUNDS && ap->type <= BR_ACTOR_BOUNDS_CORRECT) {
        if (PickBoundsTestRay__finteray((br_bounds*)ap->type_data, pPosition, pDir, t_near, t_far, &t_near, &t_far)) {
            for (a = ap->children; a != NULL; a = a->next) {
                r = ActorRayPick2D(a, pPosition, pDir, this_model, this_material, callback);
                if (r) {
                    break;
                }
            }
        }
        return r;
    }
    for (a = ap->children; a != NULL; a = a->next) {
        r = ActorRayPick2D(a, pPosition, pDir, this_model, this_material, callback);
        if (r) {
            break;
        }
    }
    return r;
}

// IDA: int __usercall DRSceneRayPick2D@<EAX>(br_actor *world@<EAX>, br_vector3 *pPosition@<EDX>, br_vector3 *pDir@<EBX>, dr_pick2d_cbfn *callback@<ECX>)
// FUNCTION: CARM95 0x004aaf10
int DRSceneRayPick2D(br_actor* world, br_vector3* pPosition, br_vector3* pDir, dr_pick2d_cbfn* callback) {

    BrMatrix34Inverse(&gPick_model_to_view__finteray, &world->t.t.mat);
    // LOG_WARN_ONCE("Missing material and model pointers to ActorRayPick2D");
    return ActorRayPick2D(world, pPosition, pDir, NULL, NULL, callback);
}

// IDA: int __usercall DRModelPick2D@<EAX>(br_model *model@<EAX>, br_material *material@<EDX>, br_vector3 *ray_pos@<EBX>, br_vector3 *ray_dir@<ECX>, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn *callback, void *arg)
//  Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004ab5e4
int DRModelPick2D__finteray(br_model* model, br_material* material, br_vector3* ray_pos, br_vector3* ray_dir, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn* callback, void* arg) {
    // DR_FACE* fp;
    int f;
    int axis_m;
    int axis_0;
    int axis_1;
    br_scalar t;
    br_scalar n;
    br_scalar d;
    br_vector3 p;
    float u0;
    float u1;
    float u2;
    float v0;
    float v1;
    float v2;
    br_scalar v0i1;
    br_scalar v0i2;
    float alpha;
    float beta;
    float f_d;
    float f_n;
    br_scalar s_alpha;
    br_scalar s_beta;
    br_vector2 map;
    int v;
    int e;
    int r;
    br_material* this_material;
    br_scalar numerator;
    float f_numerator;
    int group;

    struct v11group* grp_ptr;
    br_vector4* eqn;

    t_near -= 0.00001f;
    t_far += 0.00001f;
    for (group = 0; group < V11MODEL(model)->ngroups; group++) {
        grp_ptr = &V11MODEL(model)->groups[group];
        for (f = 0; f < V11MODEL(model)->groups[group].nfaces; f++) {
            eqn = &V11MODEL(model)->groups[group].eqn[f];
            if (V11MODEL(model)->groups[group].user) {
                this_material = V11MODEL(model)->groups[group].user;
            } else {
                this_material = material;
            }
            d = BrVector3Dot(eqn, ray_dir);
            if (fabs(d) >= 0.00000023841858 && (!this_material || !this_material->identifier || *this_material->identifier != '!' || !gPling_materials)
                && (!this_material || (this_material->flags & (BR_MATF_TWO_SIDED | BR_MATF_ALWAYS_VISIBLE)) != 0 || d <= 0.0)) {
                numerator = eqn->v[1] * ray_pos->v[1]
                    + eqn->v[2] * ray_pos->v[2]
                    + eqn->v[0] * ray_pos->v[0]
                    - eqn->v[3];
                if (!BadDiv__finteray(numerator, d)) {
                    t = -(numerator / d);
                    if (t >= t_near && t <= t_far) {
                        BrVector3Scale(&p, ray_dir, t);
                        BrVector3Accumulate(&p, ray_pos);
                        axis_m = fabs(eqn->v[0]) < fabs(eqn->v[1]);
                        if (fabs(eqn->v[2]) > fabs(eqn->v[axis_m])) {
                            axis_m = 2;
                        }
                        if (axis_m) {
                            axis_0 = 0;
                            if (axis_m == 1) {
                                axis_1 = 2;
                            } else {
                                axis_1 = 1;
                            }
                        } else {
                            axis_0 = 1;
                            axis_1 = 2;
                        }

                        v0 = grp_ptr->position[grp_ptr->vertex_numbers[f].v[0]].v[axis_0];
                        u0 = grp_ptr->position[grp_ptr->vertex_numbers[f].v[0]].v[axis_1];
                        v1 = grp_ptr->position[grp_ptr->vertex_numbers[f].v[1]].v[axis_0] - v0;
                        u1 = grp_ptr->position[grp_ptr->vertex_numbers[f].v[1]].v[axis_1] - u0;
                        v2 = grp_ptr->position[grp_ptr->vertex_numbers[f].v[2]].v[axis_0] - v0;
                        u2 = grp_ptr->position[grp_ptr->vertex_numbers[f].v[2]].v[axis_1] - u0;

                        v0i1 = p.v[axis_0] - v0;
                        v0i2 = p.v[axis_1] - u0;
                        if (fabs(v1) > 0.0000002384185791015625) {
                            f_n = u2 * v1 - u1 * v2;
                            f_d = v0i2 * v1 - u1 * v0i1;
                            if (fabs(f_n) < fabs(f_d)) {
                                continue;
                            }
                            if (f_n == 0) {
                                continue;
                            }
                            beta = f_d / f_n;
                            if (beta < 0.0 || beta > 1.0 || v1 == 0.0) {
                                continue;
                            }
                            alpha = (v0i1 - beta * v2) / v1;
                        } else {
                            if (fabs(v2) < fabs(v0i1)) {
                                continue;
                            }
                            if (v2 == 0) {
                                continue;
                            }

                            beta = v0i1 / v2;
                            if (beta < 0.0 || beta > 1.0 || u1 == 0.0) {
                                continue;
                            }
                            alpha = (v0i2 - beta * u2) / u1;
                        }

                        if (alpha >= 0.0 && beta + alpha <= 1.0) {
                            s_alpha = alpha;
                            s_beta = beta;
                            BrVector2Scale(&map, &grp_ptr->map[grp_ptr->vertex_numbers[f].v[1]], s_alpha);
                            DRVector2AccumulateScale__finteray(
                                &map,
                                &grp_ptr->map[grp_ptr->vertex_numbers[f].v[2]],
                                s_beta);
                            DRVector2AccumulateScale__finteray(
                                &map,
                                &grp_ptr->map[grp_ptr->vertex_numbers[f].v[0]],
                                1.0 - (s_alpha + s_beta));
                            v = 0;
                            e = 1;
                            if (s_alpha <= s_beta) {
                                if (0.5 - s_beta / 2.0 > s_alpha) {
                                    e = 0;
                                }
                                if (1.0 - s_beta * 2.0 < s_alpha) {
                                    v = 1;
                                }
                            } else {
                                if (1.0 - s_beta * 2.0 > s_alpha) {
                                    e = 2;
                                }
                                if (0.5 - s_beta / 2.0 < s_alpha) {
                                    v = 2;
                                }
                            }
                            gTemp_group = group;
                            r = callback(model, this_material, ray_pos, ray_dir, t, f, e, v, &p, &map, arg);
                            if (r) {
                                return r;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

// IDA: int __cdecl FindHighestPolyCallBack(br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3 *pPoint, br_vector2 *pMap, void *pArg)
// Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004abe8d
int FindHighestPolyCallBack__finteray(br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3* pPoint, br_vector2* pMap, void* pArg) {

    if (pT < (double)gNearest_T) {
        gNearest_T = pT;
        gNearest_model = pModel;
        gNearest_face = pF;
        gNearest_face_group = gTemp_group;
    }
    return 0;
}

// IDA: int __cdecl FindHighestCallBack(br_actor *pActor, br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT_near, br_scalar pT_far, void *pArg)
// Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004ab584
int FindHighestCallBack__finteray(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT_near, br_scalar pT_far, void* pArg) {

    if (gProgram_state.current_car.current_car_actor < 0
        || gProgram_state.current_car.car_model_actors[gProgram_state.current_car.current_car_actor].actor != pActor) {
        DRModelPick2D__finteray(pModel, pMaterial, pRay_pos, pRay_dir, pT_near, pT_far, FindHighestPolyCallBack__finteray, pArg);
    }
    return 0;
}

// IDA: void __usercall FindFace(br_vector3 *pPosition@<EAX>, br_vector3 *pDir@<EDX>, br_vector3 *nor@<EBX>, br_scalar *t@<ECX>, br_material **material)
// FUNCTION: CARM95 0x004ab47d
void FindFace(br_vector3* pPosition, br_vector3* pDir, br_vector3* nor, br_scalar* t, br_material** material) {
    int group;

    br_vector4* eqn;

    gNearest_T = 100.0f;
    DRSceneRayPick2D(gTrack_actor, pPosition, pDir, FindHighestCallBack__finteray);
    *t = gNearest_T;
    if (*t < 100.0f) {
        group = gNearest_face_group;
        eqn = &V11MODEL(gNearest_model)->groups[group].eqn[gNearest_face];
        nor->v[0] = eqn->v[0];
        nor->v[1] = eqn->v[1];
        nor->v[2] = eqn->v[2];
        *material = V11MODEL(gNearest_model)->groups[group].user;
    }
}

// IDA: void __cdecl EnablePlingMaterials()
// FUNCTION: CARM95 0x004abed5
void EnablePlingMaterials(void) {

    gPling_materials = 1;
}

// IDA: void __cdecl DisablePlingMaterials()
// FUNCTION: CARM95 0x004abeea
void DisablePlingMaterials(void) {

    gPling_materials = 0;
}

// IDA: void __usercall CheckSingleFace(tFace_ref *pFace@<EAX>, br_vector3 *ray_pos@<EDX>, br_vector3 *ray_dir@<EBX>, br_vector3 *normal@<ECX>, br_scalar *rt)
// FUNCTION: CARM95 0x004abeff
void CheckSingleFace(tFace_ref* pFace, br_vector3* ray_pos, br_vector3* ray_dir, br_vector3* normal, br_scalar* rt) {
    br_scalar t;
    br_scalar numerator;
    br_scalar d;
    br_vector3 p;
    br_vector3 tv;
    int axis_m;
    int axis_0;
    int axis_1;
    double u0;
    double u1;
    double u2;
    double v0;
    double v1;
    double v2;
    br_scalar v0i1;
    br_scalar v0i2;
    double alpha;
    double beta;
    double f_d;
    double f_n;
    double f_numerator;
    br_material* this_material;

    this_material = pFace->material;
    *rt = 100.0;

    d = pFace->normal.v[1] * ray_dir->v[1] + ray_dir->v[2] * pFace->normal.v[2] + ray_dir->v[0] * pFace->normal.v[0];
    if ((this_material == NULL || (this_material->flags & (BR_MATF_TWO_SIDED | BR_MATF_ALWAYS_VISIBLE)) != 0 || d <= 0.0)
        && (!this_material || !this_material->identifier || *this_material->identifier != '!' || !gPling_materials)
        && fabs(d) >= 0.00000023841858) {
        BrVector3Sub(&p, ray_pos, &pFace->v[0]);
        numerator = BrVector3Dot(&pFace->normal, &p);
        if (!BadDiv__finteray(numerator, d)) {
            if (d > 0.0) {
                if (-numerator < -0.001 || -numerator > d + 0.003) {
                    return;
                }
            } else if (numerator < -0.001 || 0.003 - d < numerator) {
                return;
            }
            t = -(numerator / d);
            if (t > 1.0) {
                t = 1.0;
            }
            BrVector3Scale(&tv, ray_dir, t);
            BrVector3Accumulate(&tv, ray_pos);
            axis_m = fabs(pFace->normal.v[0]) < fabs(pFace->normal.v[1]);
            if (fabs(pFace->normal.v[2]) > fabs(pFace->normal.v[axis_m])) {
                axis_m = 2;
            }
            if (axis_m) {
                axis_0 = 0;
                if (axis_m == 1) {
                    axis_1 = 2;
                } else {
                    axis_1 = 1;
                }
            } else {
                axis_0 = 1;
                axis_1 = 2;
            }
            v0i1 = pFace->v[0].v[axis_0];
            v0i2 = pFace->v[0].v[axis_1];
            u0 = pFace->v[1].v[axis_0] - v0i1;
            u1 = pFace->v[1].v[axis_1] - v0i2;
            v0 = pFace->v[2].v[axis_0] - v0i1;
            v1 = pFace->v[2].v[axis_1] - v0i2;
            u2 = tv.v[axis_0] - v0i1;
            v2 = tv.v[axis_1] - v0i2;
            if (fabs(u0) > 0.0000002384185791015625) {
                f_d = v1 * u0 - u1 * v0;
                if (f_d == 0) {
                    return;
                }
                alpha = (v2 * u0 - u1 * u2) / f_d;
                beta = (u2 - alpha * v0) / u0;
            } else {
                alpha = u2 / v0;
                beta = (v2 - alpha * v1) / u1;
            }
            if (beta >= -0.0001 && alpha >= -0.0001 && alpha + beta <= 1.0001) {
                *rt = t;
                *normal = pFace->normal;
                if (d > 0.0) {
                    BrVector3Negate(normal, normal);
                }
            }
        }
    }
}

// IDA: void __usercall MultiRayCheckSingleFace(int pNum_rays@<EAX>, tFace_ref *pFace@<EDX>, br_vector3 *ray_pos@<EBX>, br_vector3 *ray_dir@<ECX>, br_vector3 *normal, br_scalar *rt)
// FUNCTION: CARM95 0x004ac3b7
void MultiRayCheckSingleFace(int pNum_rays, tFace_ref* pFace, br_vector3* ray_pos, br_vector3* ray_dir, br_vector3* normal, br_scalar* rt) {
    int i;
    br_scalar t[4];
    br_scalar numerator;
    br_scalar d;
    br_vector3 p[4];
    br_vector3 tv;
    int axis_m;
    int axis_0;
    int axis_1;
    double u0[4];
    double u1;
    double u2;
    double v0[4];
    double v1;
    double v2;
    br_scalar v0i1;
    br_scalar v0i2;
    double alpha;
    double beta;
    double f_d;
    double f_n;
    double f_numerator;
    br_material* this_material;

    this_material = pFace->material;
    d = ray_dir->v[2] * pFace->normal.v[2] + ray_dir->v[1] * pFace->normal.v[1] + ray_dir->v[0] * pFace->normal.v[0];
    for (i = 0; i < pNum_rays; ++i) {
        rt[i] = 100.0;
    }
    if ((!this_material || (this_material->flags & (BR_MATF_TWO_SIDED | BR_MATF_ALWAYS_VISIBLE)) != 0 || d <= 0.0)
        && (!this_material || !this_material->identifier || *this_material->identifier != '!' || !gPling_materials)
        && fabs(d) >= 0.00000023841858) {
        for (i = 0;; ++i) {
            if (i >= pNum_rays) {
                axis_m = fabs(pFace->normal.v[0]) < fabs(pFace->normal.v[1]);
                if (fabs(pFace->normal.v[2]) > fabs(pFace->normal.v[axis_m])) {
                    axis_m = 2;
                }
                if (axis_m) {
                    axis_0 = 0;
                    if (axis_m == 1) {
                        axis_1 = 2;
                    } else {
                        axis_1 = 1;
                    }
                } else {
                    axis_0 = 1;
                    axis_1 = 2;
                }
                v0i1 = pFace->v[0].v[axis_0];
                v0i2 = pFace->v[0].v[axis_1];
                u1 = pFace->v[1].v[axis_0] - v0i1;
                v1 = pFace->v[1].v[axis_1] - v0i2;
                u2 = pFace->v[2].v[axis_0] - v0i1;
                v2 = pFace->v[2].v[axis_1] - v0i2;
                i = 0;
                while (1) {
                    if (i >= pNum_rays) {
                        return;
                    }
                    if (t[i] != 100.0) {
                        u0[i] = p[i].v[axis_0] - v0i1;
                        v0[i] = p[i].v[axis_1] - v0i2;
                        if (fabs(u1) <= 0.0000002384185791015625) {
                            alpha = u0[i] / u2;
                            beta = v0[i] - alpha * v2;
                            f_d = beta / v1;
                            goto LABEL_43;
                        }
                        f_numerator = v0[i] * u1 - u0[i] * v1;
                        f_n = v2 * u1 - v1 * u2;
                        if (f_n != 0) {
                            alpha = f_numerator / f_n;
                            beta = u0[i] - alpha * u2;
                            f_d = beta / u1;
                        LABEL_43:
                            if (f_d >= -0.0001 && alpha >= -0.0001 && alpha + f_d <= 1.0001) {
                                rt[i] = t[i];
                                *normal = pFace->normal;
                                if (d > 0.0) {
                                    normal->v[0] = -pFace->normal.v[0];
                                    normal->v[1] = -pFace->normal.v[1];
                                    normal->v[2] = -pFace->normal.v[2];
                                }
                            }
                        }
                    }
                    ++i;
                    continue;
                }
            }
            tv.v[0] = ray_pos[i].v[0] - pFace->v[0].v[0];
            tv.v[1] = ray_pos[i].v[1] - pFace->v[0].v[1];
            tv.v[2] = ray_pos[i].v[2] - pFace->v[0].v[2];
            numerator = pFace->normal.v[2] * tv.v[2] + pFace->normal.v[1] * tv.v[1] + pFace->normal.v[0] * tv.v[0];
            if (BadDiv__finteray(numerator, d)) {
                return;
            }
            if (d > 0.0) {
                if (-numerator < -0.001 || -numerator > d + 0.003) {
                    t[i] = 100.0;
                    continue;
                }
            } else if (numerator < -0.001 || 0.003 - d < numerator) {
                t[i] = 100.0;
                continue;
            }
            t[i] = -(numerator / d);
            if (t[i] > 1.0) {
                t[i] = 1.0;
            }
            p[i].v[0] = t[i] * ray_dir->v[0];
            p[i].v[1] = t[i] * ray_dir->v[1];
            p[i].v[2] = t[i] * ray_dir->v[2];
            p[i].v[0] = ray_pos[i].v[0] + p[i].v[0];
            p[i].v[1] = ray_pos[i].v[1] + p[i].v[1];
            p[i].v[2] = ray_pos[i].v[2] + p[i].v[2];
        }
    }
}

// IDA: void __usercall GetNewBoundingBox(br_bounds *b2@<EAX>, br_bounds *b1@<EDX>, br_matrix34 *m@<EBX>)
// FUNCTION: CARM95 0x004acaa2
void GetNewBoundingBox(br_bounds* b2, br_bounds* b1, br_matrix34* m) {
    br_vector3 a;
    br_vector3 c[3];
    int j;

    BrMatrix34ApplyP(&b2->min, &b1->min, m);
    BrVector3Copy(&b2->max, &b2->min);
    BrVector3Sub(&a, &b1->max, &b1->min);
    for (j = 0; j < 3; j++) {
        BrVector3Scale(&c[j], (br_vector3*)m->m[j], a.v[j]);
    }
    for (j = 0; j < 3; ++j) {
        b2->min.v[j] = (float)(c[2].v[j] < 0.f) * c[2].v[j]
            + (float)(c[1].v[j] < 0.f) * c[1].v[j]
            + (float)(c[0].v[j] < 0.f) * c[0].v[j]
            + b2->min.v[j];
        b2->max.v[j] = (float)(c[0].v[j] > 0.f) * c[0].v[j]
            + (float)(c[2].v[j] > 0.f) * c[2].v[j]
            + (float)(c[1].v[j] > 0.f) * c[1].v[j]
            + b2->max.v[j];
    }
}

// IDA: int __usercall FindFacesInBox@<EAX>(tBounds *bnds@<EAX>, tFace_ref *face_list@<EDX>, int max_face@<EBX>)
// FUNCTION: CARM95 0x004accae
int FindFacesInBox(tBounds* bnds, tFace_ref* face_list, int max_face) {
    br_vector3 a;
    br_vector3 b;
    br_vector3 c[3];
    int i;
    int j;
    int x;
    int z;
    tU8 cx_min;
    tU8 cx_max;
    tU8 cz_min;
    tU8 cz_max;
    tTrack_spec* track_spec;

    j = 0;
    track_spec = &gProgram_state.track_spec;
    BrVector3Add(&a, &bnds->original_bounds.min, &bnds->original_bounds.max);
    BrVector3Scale(&a, &a, 0.5f);
    BrMatrix34ApplyP(&bnds->box_centre, &a, bnds->mat);
    BrVector3Sub(&b, &bnds->original_bounds.max, &bnds->original_bounds.min);
    bnds->radius = BrVector3Length(&b) / 2.f;
    BrMatrix34ApplyP(&bnds->real_bounds.min, &bnds->original_bounds.min, bnds->mat);
    BrVector3Copy(&bnds->real_bounds.max, &bnds->real_bounds.min);
    for (i = 0; i < 3; ++i) {
        c[i].v[0] = bnds->mat->m[i][0] * b.v[i];
        c[i].v[1] = bnds->mat->m[i][1] * b.v[i];
        c[i].v[2] = bnds->mat->m[i][2] * b.v[i];
    }
    for (i = 0; i < 3; ++i) {
        bnds->real_bounds.min.v[i] += MIN(c[0].v[i], 0.f)
            + MIN(c[1].v[i], 0.f)
            + MIN(c[2].v[i], 0.f);
        bnds->real_bounds.max.v[i] += MAX(c[0].v[i], 0.f)
            + MAX(c[1].v[i], 0.f)
            + MAX(c[2].v[i], 0.f);
    }
    XZToColumnXZ(&cx_min, &cz_min, bnds->real_bounds.min.v[0], bnds->real_bounds.min.v[2], track_spec);
    XZToColumnXZ(&cx_max, &cz_max, bnds->real_bounds.max.v[0], bnds->real_bounds.max.v[2], track_spec);
    if (cx_min != 0) {
        cx_min--;
    }
    if (cz_min != 0) {
        cz_min--;
    }
    if (cx_max + 1 < track_spec->ncolumns_x) {
        cx_max++;
    }
    if (cz_max + 1 < track_spec->ncolumns_z) {
        cz_max++;
    }
    for (x = cx_min; x <= cx_max; x++) {
        for (z = cz_min; z <= cz_max; z++) {
            if (track_spec->columns[z][x] != NULL) {
                if (track_spec->blends[z][x] != NULL) {
                    track_spec->blends[z][x]->render_style = BR_RSTYLE_FACES;
                }
                j = max_face - ActorBoxPick(bnds, track_spec->columns[z][x], model_unk1, material_unk1, &face_list[j], max_face - j, NULL);
                if (track_spec->blends[z][x] != NULL) {
                    track_spec->blends[z][x]->render_style = BR_RSTYLE_NONE;
                }
            }
            if (track_spec->lollipops[z][x] != NULL) {
                j = max_face - ActorBoxPick(bnds, track_spec->lollipops[z][x], model_unk1, material_unk1, &face_list[j], max_face - j, NULL);
            }
        }
    }
    return j;
}

// IDA: int __usercall FindFacesInBox2@<EAX>(tBounds *bnds@<EAX>, tFace_ref *face_list@<EDX>, int max_face@<EBX>)
// FUNCTION: CARM95 0x004ad176
int FindFacesInBox2(tBounds* bnds, tFace_ref* face_list, int max_face) {
    br_vector3 a;
    br_vector3 b;
    br_vector3 c[3];
    int i;
    int j;

    a.v[0] = (bnds->original_bounds.min.v[0] + bnds->original_bounds.max.v[0]) * .5f;
    a.v[1] = (bnds->original_bounds.min.v[1] + bnds->original_bounds.max.v[1]) * .5f;
    a.v[2] = (bnds->original_bounds.min.v[2] + bnds->original_bounds.max.v[2]) * .5f;
    BrMatrix34ApplyP(&bnds->box_centre, &a, bnds->mat);
    BrVector3Sub(&b, &bnds->original_bounds.max, &bnds->original_bounds.min);
    bnds->radius = BrVector3Length(&b) / 2.f;
    BrMatrix34ApplyP(&bnds->real_bounds.min, &bnds->original_bounds.min, bnds->mat);
    BrVector3Copy(&bnds->real_bounds.max, &bnds->real_bounds.min);
    for (i = 0; i < 3; i++) {
        BrVector3Scale(&c[i], (br_vector3*)bnds->mat->m[i], b.v[i]);
    }
    for (i = 0; i < 3; i++) {
        bnds->real_bounds.min.v[i] += MIN(0.f, c[0].v[i]) + MIN(0.f, c[1].v[i]) + MIN(0.f, c[2].v[i]);
        bnds->real_bounds.max.v[i] += MAX(0.f, c[0].v[i]) + MAX(0.f, c[1].v[i]) + MAX(0.f, c[2].v[i]);
    }
    return max_face - ActorBoxPick(bnds, gTrack_actor, model_unk1, material_unk1, face_list, max_face, NULL);
}

// IDA: int __usercall ActorBoxPick@<EAX>(tBounds *bnds@<EAX>, br_actor *ap@<EDX>, br_model *model@<EBX>, br_material *material@<ECX>, tFace_ref *face_list, int max_face, br_matrix34 *pMat)
// FUNCTION: CARM95 0x004ad45b
int ActorBoxPick(tBounds* bnds, br_actor* ap, br_model* model, br_material* material, tFace_ref* face_list, int max_face, br_matrix34* pMat) {
    br_model* this_model;
    br_material* this_material;
    int i;
    int n;
    int test_children;
    br_actor* a;
    br_actor* next_a;
    br_matrix34 mat;
    br_matrix34 mat2;
    br_matrix34 invmat;
    br_matrix34 box_to_actor;
    tBounds new_bounds;
    br_bounds br_bnds;

    i = 0;
    test_children = 1;
    if (ap->model != NULL) {
        this_model = ap->model;
    } else {
        this_model = model;
    }
    if (ap->material != NULL) {
        this_material = ap->material;
    } else {
        this_material = material;
    }
    if (ap->render_style == BR_RSTYLE_NONE) {
        return max_face;
    }
    if (ap->identifier != NULL && ap->identifier[0] == '&') {
        if (ap->children == NULL) {
            if (ap->type != BR_ACTOR_MODEL) {
                return max_face;
            }
            if (!BoundsTransformTest(&this_model->bounds, &bnds->real_bounds, &ap->t.t.mat)) {
                return max_face;
            }
        }
        if (pMat != NULL) {
            BrMatrix34Mul(&mat, &ap->t.t.mat, pMat);
            pMat = &mat;
        } else {
            pMat = &ap->t.t.mat;
        }
        BrMatrix34LPInverse(&invmat, &ap->t.t.mat);
        BrMatrix34Mul(&mat2, bnds->mat, &invmat);
        new_bounds.mat = &mat2;
        BrVector3Copy(&new_bounds.original_bounds.min, &bnds->original_bounds.min);
        BrVector3Copy(&new_bounds.original_bounds.max, &bnds->original_bounds.max);
        BrMatrix34ApplyP(&new_bounds.box_centre, &bnds->box_centre, &invmat);
        new_bounds.radius = bnds->radius;
        GetNewBoundingBox(&new_bounds.real_bounds, &new_bounds.original_bounds, new_bounds.mat);
        if (ap->identifier[1] >= '0' && ap->identifier[1] <= '9') {
            if (!BoundsOverlapTest__finteray(&new_bounds.real_bounds, &this_model->bounds)) {
                return max_face;
            }
            BrMatrix34LPInverse(&invmat, bnds->mat);
            BrMatrix34Mul(&box_to_actor, &ap->t.t.mat, &invmat);
            GetNewBoundingBox(&br_bnds, &ap->model->bounds, &box_to_actor);
            if (!BoundsOverlapTest__finteray(&br_bnds, &bnds->original_bounds)) {
                return max_face;
            }
            if (PullActorFromWorld(ap)) {
                return max_face;
            }
        }
        bnds = &new_bounds;
    }
    if (ap->type == BR_ACTOR_MODEL) {
        if (BoundsOverlapTest__finteray(&bnds->real_bounds, &this_model->bounds)) {
            n = ModelPickBox(ap, bnds, this_model, this_material, &face_list[i], max_face, pMat);
            if (pMat && max_face != n) {
                StopGroovidelic(ap);
            }
            i += max_face - n;
            max_face = n;
        }
    } else if (ap->type == BR_ACTOR_BOUNDS || ap->type == BR_ACTOR_BOUNDS_CORRECT) {
        test_children = BoundsOverlapTest__finteray(&bnds->real_bounds, (br_bounds*)ap->type_data);
    }
    if (test_children) {
        for (a = ap->children; a != NULL; a = next_a) {
            next_a = a->next;
            n = ActorBoxPick(bnds, a, this_model, this_material, &face_list[i], max_face, pMat);
            i += max_face - n;
            max_face = n;
        }
    }
    return max_face;
}

// IDA: int __usercall ModelPickBox@<EAX>(br_actor *actor@<EAX>, tBounds *bnds@<EDX>, br_model *model@<EBX>, br_material *model_material@<ECX>, tFace_ref *face_list, int max_face, br_matrix34 *pMat)
// FUNCTION: CARM95 0x004ad8ce
int ModelPickBox(br_actor* actor, tBounds* bnds, br_model* model, br_material* model_material, tFace_ref* face_list, int max_face, br_matrix34* pMat) {
    int f;
    int i;
    int n;
    int group;
    // DR_FACE* fp;
    int v1;
    int v2;
    int v3;
    br_vector3 polygon[12];
    br_vector3 a;
    br_vector3 tv;
    br_scalar t;
    struct v11model* prepared;

    struct v11group* grp_ptr;

    prepared = model->prepared;
    if (max_face <= 0) {
        return 0;
    }
    for (group = 0; prepared->ngroups > group; group++) {
        grp_ptr = &prepared->groups[group];
        for (f = 0; f < prepared->groups[group].nfaces; f++) {
            // fp = &prepared->groups[group].faces[f];
            v1 = grp_ptr->vertex_numbers[f].v[0]; // fp->vertices[0];
            // BrVector3Sub(&a, &prepared->groups[group].vertices[v1].p, &bnds->box_centre);
            BrVector3Sub(&a, &grp_ptr->position[v1], &bnds->box_centre);
            // t = BrVector3Dot((br_vector3*)&fp->eqn, &a);
            t = BrVector3Dot((br_vector3*)&grp_ptr->eqn[f], &a);
            if (fabs(t) > bnds->radius) {
                continue;
            }
            // v2 = fp->vertices[1];
            // v3 = fp->vertices[2];
            v2 = grp_ptr->vertex_numbers[f].v[1];
            v3 = grp_ptr->vertex_numbers[f].v[2];

            t = bnds->real_bounds.min.v[0];
            if (t > grp_ptr->position[v1].v[0]
                && t > grp_ptr->position[v2].v[0]
                && t > grp_ptr->position[v3].v[0]) {
                continue;
            }
            t = bnds->real_bounds.max.v[0];
            if (t < grp_ptr->position[v1].v[0]
                && t < grp_ptr->position[v2].v[0]
                && t < grp_ptr->position[v3].v[0]) {
                continue;
            }
            t = bnds->real_bounds.min.v[1];
            if (t > grp_ptr->position[v1].v[1]
                && t > grp_ptr->position[v2].v[1]
                && t > grp_ptr->position[v3].v[1]) {
                continue;
            }
            t = bnds->real_bounds.max.v[1];
            if (t < grp_ptr->position[v1].v[1]
                && t < grp_ptr->position[v2].v[1]
                && t < grp_ptr->position[v3].v[1]) {
                continue;
            }
            t = bnds->real_bounds.min.v[2];
            if (t > grp_ptr->position[v1].v[2]
                && t > grp_ptr->position[v2].v[2]
                && t > grp_ptr->position[v3].v[2]) {
                continue;
            }
            t = bnds->real_bounds.max.v[2];
            if (t < grp_ptr->position[v1].v[2]
                && t < grp_ptr->position[v2].v[2]
                && t < grp_ptr->position[v3].v[2]) {
                continue;
            }
            BrVector3Sub(&polygon[1], &grp_ptr->position[v1], (br_vector3*)bnds->mat->m[3]);
            BrVector3Sub(&polygon[2], &grp_ptr->position[v2], (br_vector3*)bnds->mat->m[3]);
            BrVector3Sub(&polygon[3], &grp_ptr->position[v3], (br_vector3*)bnds->mat->m[3]);
            BrMatrix34TApplyV(&polygon[0], &polygon[1], bnds->mat);
            BrMatrix34TApplyV(&polygon[1], &polygon[2], bnds->mat);
            BrMatrix34TApplyV(&polygon[2], &polygon[3], bnds->mat);
            n = 3;
            for (i = 0; i < 3; i++) {
                ClipToPlaneGE(&polygon[0], &n, i, bnds->original_bounds.min.v[i]);
                if (n < 3) {
                    break;
                }
                ClipToPlaneLE(&polygon[0], &n, i, bnds->original_bounds.max.v[i]);
                if (n < 3) {
                    break;
                }
            }
            if (n >= 3) {
                if (pMat != NULL) {
                    BrMatrix34ApplyP(&face_list->v[0], &grp_ptr->position[v1], pMat);
                    BrMatrix34ApplyP(&face_list->v[1], &grp_ptr->position[v2], pMat);
                    BrMatrix34ApplyP(&face_list->v[2], &grp_ptr->position[v3], pMat);
                    BrVector3Copy(&tv, (br_vector3*)&grp_ptr->eqn[f]);
                    BrMatrix34ApplyV(&face_list->normal, &tv, pMat);
                } else {
                    BrVector3Copy(&face_list->v[0], &grp_ptr->position[v1]);
                    BrVector3Copy(&face_list->v[1], &grp_ptr->position[v2]);
                    BrVector3Copy(&face_list->v[2], &grp_ptr->position[v3]);
                    BrVector3Copy(&face_list->normal, (br_vector3*)&grp_ptr->eqn[f]);
                }
                if (prepared->groups[group].user != NULL) {
                    face_list->material = prepared->groups[group].user;
                } else {
                    face_list->material = model_material;
                }
                face_list->flags = 0;
                if (face_list->material != NULL && (face_list->material->flags & (BR_MATF_TWO_SIDED | BR_MATF_ALWAYS_VISIBLE)) == 0) {
                    face_list->flags |= (v1 < v2) | (v2 < v3) << 1 | (v3 < v1) << 2;
                }
                if (pMat != NULL) {
                    face_list->d = BrVector3LengthSquared(&face_list->v[0]);
                } else {
                    face_list->d = grp_ptr->eqn[f].v[3];
                }
                face_list->map[0] = &grp_ptr->map[v1];
                face_list->map[1] = &grp_ptr->map[v2];
                face_list->map[2] = &grp_ptr->map[v3];
                if (face_list->material != NULL
                    && face_list->material->identifier != NULL
                    && face_list->material->identifier[0] == '!') {
                    gPling_face = face_list;
                }
                face_list++;
                max_face--;
                if (max_face == 0) {
                    break;
                }
            }
        }
        if (max_face == 0) {
            break;
        }
    }
    return max_face;
}

// IDA: void __usercall ClipToPlaneGE(br_vector3 *p@<EAX>, int *nv@<EDX>, int i@<EBX>, br_scalar limit)
// FUNCTION: CARM95 0x004ae5b5
void ClipToPlaneGE(br_vector3* p, int* nv, int i, br_scalar limit) {
    int last_vertex;
    int j;
    int vertex;
    int k;
    br_vector3 p2[12];

    last_vertex = *nv - 1;
    j = 0;
    for (vertex = 0; *nv > vertex; ++vertex) {
        if ((p[last_vertex].v[i] > limit) != (p[vertex].v[i] > limit)) {
            for (k = 0; k < 3; ++k) {
                if (i != k) {
                    p2[j].v[k] = (p[vertex].v[k] - p[last_vertex].v[k])
                            * (limit - p[last_vertex].v[i])
                            / (p[vertex].v[i] - p[last_vertex].v[i])
                        + p[last_vertex].v[k];
                }
            }
            p2[j++].v[i] = limit;
        }
        if (p[vertex].v[i] >= limit) {
            BrVector3Copy(&p2[j], &p[vertex]);
            j++;
        }
        last_vertex = vertex;
    }
    *nv = j;
    for (k = 0; k < j; k++) {
        BrVector3Copy(&p[k], &p2[k]);
    }
}

// IDA: void __usercall ClipToPlaneLE(br_vector3 *p@<EAX>, int *nv@<EDX>, int i@<EBX>, br_scalar limit)
// FUNCTION: CARM95 0x004ae89f
void ClipToPlaneLE(br_vector3* p, int* nv, int i, br_scalar limit) {
    int last_vertex;
    int j;
    int vertex;
    int k;
    br_vector3 p2[12];

    last_vertex = *nv - 1;
    j = 0;
    for (vertex = 0; *nv > vertex; ++vertex) {
        if ((p[vertex].v[i] > limit) != (p[last_vertex].v[i] > limit)) {
            for (k = 0; k < 3; ++k) {
                if (k != i) {
                    p2[j].v[k] = (p[vertex].v[k] - p[last_vertex].v[k])
                            * (limit - p[last_vertex].v[i])
                            / (p[vertex].v[i] - p[last_vertex].v[i])
                        + p[last_vertex].v[k];
                }
            }
            p2[j++].v[i] = limit;
        }
        if (p[vertex].v[i] <= (double)limit) {
            BrVector3Copy(&p2[j], &p[vertex]);
            j++;
        }
        last_vertex = vertex;
    }
    *nv = j;
    for (k = 0; k < j; k++) {
        BrVector3Copy(&p[k], &p2[k]);
    }
}

// IDA: int __usercall BoundsOverlapTest@<EAX>(br_bounds *b1@<EAX>, br_bounds *b2@<EDX>)
// Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004aeb89
int BoundsOverlapTest__finteray(br_bounds* b1, br_bounds* b2) {

    return b1->min.v[0] <= b2->max.v[0]
        && b2->min.v[0] <= b1->max.v[0]
        && b1->min.v[1] <= b2->max.v[1]
        && b2->min.v[1] <= b1->max.v[1]
        && b1->min.v[2] <= b2->max.v[2]
        && b2->min.v[2] <= b1->max.v[2];
}

// IDA: int __usercall BoundsTransformTest@<EAX>(br_bounds *b1@<EAX>, br_bounds *b2@<EDX>, br_matrix34 *M@<EBX>)
// FUNCTION: CARM95 0x004aec32
int BoundsTransformTest(br_bounds* b1, br_bounds* b2, br_matrix34* M) {
    br_scalar val;
    br_vector3 o;

    BrVector3Sub(&o, &b1->max, &b1->min);
    val = M->m[0][0] * b1->min.v[0] + M->m[1][0] * b1->min.v[1] + M->m[2][0] * b1->min.v[2] + M->m[3][0];

    if ((M->m[0][0] <= 0.0f ? 0.0f : M->m[0][0] * o.v[0])
            + (M->m[1][0] <= 0.0f ? 0.0f : M->m[1][0] * o.v[1])
            + (M->m[2][0] <= 0.0f ? 0.0f : M->m[2][0] * o.v[2])
            + val
        < b2->min.v[0]) {
        return 0;
    }
    if ((M->m[0][0] < 0.0f ? M->m[0][0] * o.v[0] : 0.0f)
            + (M->m[1][0] < 0.0f ? M->m[1][0] * o.v[1] : 0.0f)
            + (M->m[2][0] < 0.0f ? M->m[2][0] * o.v[2] : 0.0f)
            + val
        > b2->max.v[0]) {
        return 0;
    }

    val = M->m[0][2] * b1->min.v[0] + M->m[1][2] * b1->min.v[1] + M->m[2][2] * b1->min.v[2] + M->m[3][2];
    if ((M->m[0][2] <= 0.0f ? 0.0f : M->m[0][2] * o.v[0])
            + (M->m[1][2] <= 0.0f ? 0.0f : M->m[1][2] * o.v[1])
            + (M->m[2][2] <= 0.0f ? 0.0f : M->m[2][2] * o.v[2])
            + val
        < b2->min.v[2]) {
        return 0;
    }
    if ((M->m[0][2] < 0.0f ? M->m[0][2] * o.v[0] : 0.0f)
            + (M->m[1][2] < 0.0f ? M->m[1][2] * o.v[1] : 0.0f)
            + (M->m[2][2] < 0.0f ? M->m[2][2] * o.v[2] : 0.0f)
            + val
        > b2->max.v[2]) {
        return 0;
    }

    val = M->m[0][1] * b1->min.v[0] + M->m[1][1] * b1->min.v[1] + M->m[2][1] * b1->min.v[2] + M->m[3][1];
    if ((M->m[0][1] <= 0.0f ? 0.0f : M->m[0][1] * o.v[0])
            + (M->m[1][1] <= 0.0f ? 0.0f : M->m[1][1] * o.v[1])
            + (M->m[2][1] <= 0.0f ? 0.0f : M->m[2][1] * o.v[2])
            + val
        < b2->min.v[1]) {
        return 0;
    }
    if ((M->m[0][1] < 0.0 ? M->m[0][1] * o.v[0] : 0.0)
            + (M->m[1][1] < 0.0 ? M->m[1][1] * o.v[1] : 0.0)
            + (M->m[2][1] < 0.0 ? M->m[2][1] * o.v[2] : 0.0)
            + val
        > b2->max.v[1]) {
        return 0;
    }

    return 1;
}

// IDA: int __usercall LineBoxColl@<EAX>(br_vector3 *o@<EAX>, br_vector3 *p@<EDX>, br_bounds *pB@<EBX>, br_vector3 *pHit_point@<ECX>)
// FUNCTION: CARM95 0x004af126
int LineBoxColl(br_vector3* o, br_vector3* p, br_bounds* pB, br_vector3* pHit_point) {
    br_vector3 dir;
    int inside;
    int quad[3];
    int i;
    int which_plane;
    br_scalar max_t[3];
    br_scalar cp[3];

    inside = 1;
    BrVector3Sub(&dir, p, o);
    for (i = 0; i < 3; ++i) {
        if (pB->min.v[i] <= o->v[i]) {
            if (pB->max.v[i] >= o->v[i]) {
                quad[i] = 2;
            } else {
                quad[i] = 0;
                max_t[i] = pB->max.v[i];
                inside = 0;
            }
        } else {
            quad[i] = 1;
            max_t[i] = pB->min.v[i];
            inside = 0;
        }
    }
    if (inside) {
        BrVector3Copy(pHit_point, o);
        return 8;
    } else {
        for (i = 0; i < 3; ++i) {
            if (quad[i] == 2 || dir.v[i] == 0.0) {
                cp[i] = -1.0;
            } else {
                cp[i] = (max_t[i] - o->v[i]) / dir.v[i];
            }
        }
        which_plane = 0;
        for (i = 1; i < 3; ++i) {
            if (cp[which_plane] < cp[i]) {
                which_plane = i;
            }
        }
        if (cp[which_plane] >= 0.0 && cp[which_plane] <= 1.0) {
            for (i = 0; i < 3; ++i) {
                if (which_plane == i) {
                    pHit_point->v[i] = max_t[i];
                } else {
                    pHit_point->v[i] = dir.v[i] * cp[which_plane] + o->v[i];
                    if (pHit_point->v[i] < pB->min.v[i] || pB->max.v[i] < pHit_point->v[i]) {
                        return 0;
                    }
                }
            }
            return which_plane + 4 * quad[which_plane] + 1;
        } else {
            return 0;
        }
    }
}

// IDA: int __usercall SphereBoxIntersection@<EAX>(br_bounds *pB@<EAX>, br_vector3 *pC@<EDX>, br_scalar pR_squared, br_vector3 *pHit_point)
// FUNCTION: CARM95 0x004af3f0
int SphereBoxIntersection(br_bounds* pB, br_vector3* pC, br_scalar pR_squared, br_vector3* pHit_point) {
    int i;
    br_scalar d;

    d = 0.f;
    for (i = 0; i < 3; i++) {
        if (pC->v[i] <= pB->min.v[i]) {
            pHit_point->v[i] = pB->min.v[i];
        } else if (pC->v[i] > pB->max.v[i]) {
            pHit_point->v[i] = pB->max.v[i];
        } else {
            pHit_point->v[i] = pC->v[i];
        }
        d += (pC->v[i] - pHit_point->v[i]) * (pC->v[i] - pHit_point->v[i]);
    }
    return d <= pR_squared;
}

// IDA: int __usercall LineBoxCollWithSphere@<EAX>(br_vector3 *o@<EAX>, br_vector3 *p@<EDX>, br_bounds *pB@<EBX>, br_vector3 *pHit_point@<ECX>)
// FUNCTION: CARM95 0x004af4d2
int LineBoxCollWithSphere(br_vector3* o, br_vector3* p, br_bounds* pB, br_vector3* pHit_point) {
    int i;
    int plane;

    plane = LineBoxColl(o, p, pB, pHit_point);

    if (plane != 0) {
        return plane;
    }
    if (!SphereBoxIntersection(pB, p, 2.5e-5f, pHit_point)) {
        return 0;
    }
    for (i = 0; i < 3; i++) {
        if (pB->max.v[i] == pHit_point->v[i] && p->v[i] <= o->v[i]) {
            return i + 1;
        }
        if (pHit_point->v[i] == pB->min.v[i] && p->v[i] >= o->v[i]) {
            return i + 5;
        }
    }
    return 0;
}

// IDA: int __usercall CompVert@<EAX>(int v1@<EAX>, int v2@<EDX>)
// FUNCTION: CARM95 0x004af90e
int CompVert(int v1, int v2) {
    br_vertex* vl;
    br_vector3 tv;
    br_vector2 tv2;

    if (v1 == v2) {
        return 1;
    }
    vl = gSelected_model->vertices;
    BrVector3Sub(&tv, &vl[v1].p, &vl[v2].p);
    if (BrVector3LengthSquared(&tv) > 1e-5f) {
        return 0;
    }
    BrVector2Sub(&tv2, &vl[v1].map, &vl[v2].map);
    if (BrVector2LengthSquared(&tv2) > 1e-5f) {
        return 0;
    }
    return 1;
}

// IDA: void __usercall SetFacesGroup(int pFace@<EAX>)
// FUNCTION: CARM95 0x004af7d4
void SetFacesGroup(int pFace) {
    int f;
    int v;
    int i;
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SelectFace(br_vector3 *pDir@<EAX>)
// FUNCTION: CARM95 0x004af5e8
void SelectFace(br_vector3* pDir) {
    tCar_spec* c;
    br_vector3 dir;
    br_vector3 normal;
    br_scalar t;
    br_model* old_model;
    int i;
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetTilingLimits(br_vector2 *min@<EAX>, br_vector2 *max@<EDX>)
// FUNCTION: CARM95 0x004afa2a
void GetTilingLimits(br_vector2* min, br_vector2* max) {
    int f;
    int i;
    int j;
    br_vertex* verts;
    br_face* faces;

    verts = gSelected_model->vertices;
    faces = gSelected_model->faces;
    BrVector2Set(min, 32000.f, 32000.f);
    BrVector2Set(max, -32000.f, -32000.f);
    for (f = 0; f < gSelected_model->nfaces; f++) {
        if (faces[f].material == gSub_material) {
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 2; j++) {
                    if (verts[faces[f].vertices[i]].map.v[j] < min->v[j]) {
                        min->v[j] = verts[faces[f].vertices[i]].map.v[j];
                    }
                    if (verts[faces[f].vertices[i]].map.v[j] > max->v[j]) {
                        max->v[j] = verts[faces[f].vertices[i]].map.v[j];
                    }
                }
            }
        }
    }
}

// IDA: void __usercall Scale(int pD@<EAX>, int factor@<EDX>)
// FUNCTION: CARM95 0x004afbf3
void Scale(int pD, int factor) {
    br_vector2 min;
    br_vector2 max;
    int f;
    int v;
    br_scalar d;
    br_vertex* verts;
    br_face* faces;

    if (gSelected_model == NULL) {
        return;
    }
    if (gSelected_model->nfaces != gNfaces) {
        return;
    }
    verts = gSelected_model->vertices;
    faces = gSelected_model->faces;
    GetTilingLimits(&min, &max);
    d = max.v[pD] - min.v[pD];
    if (d <= 0.f || factor + d <= 0.f) {
        return;
    }
    for (v = 0; v < gSelected_model->nvertices; v++) {
        for (f = 0; f < gSelected_model->nfaces; f++) {
            if (faces[f].material == gSub_material
                && (faces[f].vertices[0] == v || faces[f].vertices[1] == v || faces[f].vertices[2] == v)) {
                verts[v].map.v[pD] = (factor + d) / d * verts[v].map.v[pD];
                break;
            }
        }
    }
    BrModelUpdate(gSelected_model, BR_MODU_ALL);
}

// IDA: void __cdecl ScaleUpX()
// FUNCTION: CARM95 0x004afbdc
void ScaleUpX(void) {

    Scale(0, 1);
}

// IDA: void __cdecl ScaleDnX()
// FUNCTION: CARM95 0x004afd9a
void ScaleDnX(void) {

    Scale(0, -1);
}

// IDA: void __cdecl ScaleUpY()
// FUNCTION: CARM95 0x004afdb1
void ScaleUpY(void) {

    Scale(1, 1);
}

// IDA: void __cdecl ScaleDnY()
// FUNCTION: CARM95 0x004afdc8
void ScaleDnY(void) {

    Scale(1, -1);
}

// IDA: void __cdecl SelectFaceForward()
// FUNCTION: CARM95 0x004afddf
void SelectFaceForward(void) {
    br_vector3 dir;

    BrVector3Scale(&dir, (br_vector3*)&gProgram_state.current_car.car_master_actor->t.t.mat.m[2], -2.f);
    SelectFace(&dir);
}

// IDA: void __cdecl SelectFaceDown()
// FUNCTION: CARM95 0x004afe2c
void SelectFaceDown(void) {
    br_vector3 dir;

    BrVector3Scale(&dir, (br_vector3*)&gProgram_state.current_car.car_master_actor->t.t.look_up.up, -2.f);
    SelectFace(&dir);
}
