#include "raycast.h"
#include "brender.h"
#include "brucetrk.h"
#include "formats.h"
#include "globvars.h"
#include "harness/trace.h"
#include "shortcut.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>

// GLOBAL: CARM95 0x53d028
br_matrix34 gPick_model_to_view__raycast; // suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x53d010
int gBelow_face_index;

// GLOBAL: CARM95 0x53d018
br_scalar gCurrent_y;

// GLOBAL: CARM95 0x53d020
int gAbove_face_index;

// GLOBAL: CARM95 0x53d01c
br_model* gAbove_model;

// GLOBAL: CARM95 0x53d00c
br_model* gBelow_model;

// GLOBAL: CARM95 0x53d058
br_scalar gHighest_y_below;

// GLOBAL: CARM95 0x53d008
br_actor* gY_picking_camera;

// GLOBAL: CARM95 0x53d014
br_scalar gLowest_y_above;

// Added, probably can be replaced with NULL

// GLOBAL: CARM95 0x553aa4
br_model* model_unk1;

// GLOBAL: CARM95 0x553aa8
br_material* material_unk1;

// IDA: int __usercall DRActorToRoot@<EAX>(br_actor *a@<EAX>, br_actor *world@<EDX>, br_matrix34 *m@<EBX>)
// FUNCTION: CARM95 0x494230
int DRActorToRoot(br_actor* a, br_actor* world, br_matrix34* m) {
    LOG_TRACE("(%p, %p, %p)", a, world, m);

    if (world == a) {
        BrMatrix34Identity(m);
        return 1;
    } else {
        BrTransformToMatrix34(m, &a->t);
        for (a = a->parent; a && world != a; a = a->parent) {
            if (a->t.type != BR_TRANSFORM_IDENTITY) {
                BrMatrix34PostTransform(m, &a->t);
            }
        }
        return world == a;
    }
}

// IDA: void __cdecl InitRayCasting()
// FUNCTION: CARM95 0x4942e4
void InitRayCasting(void) {
    br_camera* camera_ptr;
    LOG_TRACE("()");

    gY_picking_camera = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
    camera_ptr = gY_picking_camera->type_data;
    camera_ptr->type = BR_CAMERA_PERSPECTIVE_FOV;
    camera_ptr->field_of_view = BrDegreeToAngle(70.0f);
    camera_ptr->hither_z = 0.001f;
    camera_ptr->yon_z = 1000.0f;
    camera_ptr->aspect = 1.0f;
    gY_picking_camera->t.t.mat.m[0][0] = 1.0;
    gY_picking_camera->t.t.mat.m[0][1] = 0.0;
    gY_picking_camera->t.t.mat.m[0][2] = 0.0;
    gY_picking_camera->t.t.mat.m[1][0] = 0.0;
    gY_picking_camera->t.t.mat.m[1][1] = 0.0;
    gY_picking_camera->t.t.mat.m[1][2] = -1.0;
    gY_picking_camera->t.t.mat.m[2][0] = 0.0;
    gY_picking_camera->t.t.mat.m[2][1] = 1.0;
    gY_picking_camera->t.t.mat.m[2][2] = 0.0;
}

// IDA: int __cdecl BadDiv(br_scalar a, br_scalar b)
// Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x4952ca
int BadDiv__raycast(br_scalar a, br_scalar b) {
    // LOG_TRACE("(%f, %f)", a, b);

    return fabs(b) < 1.0 && fabs(a) > fabs(b) * BR_SCALAR_MAX;
}

// IDA: void __usercall DRVector2AccumulateScale(br_vector2 *a@<EAX>, br_vector2 *b@<EDX>, br_scalar s)
//  Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x495319
void DRVector2AccumulateScale__raycast(br_vector2* a, br_vector2* b, br_scalar s) {
    LOG_TRACE("(%p, %p, %f)", a, b, s);

    a->v[0] = b->v[0] * s + a->v[0];
    a->v[1] = b->v[1] * s + a->v[1];
}

// IDA: int __usercall PickBoundsTestRay@<EAX>(br_bounds *b@<EAX>, br_vector3 *rp@<EDX>, br_vector3 *rd@<EBX>, br_scalar t_near, br_scalar t_far, br_scalar *new_t_near, br_scalar *new_t_far)
// Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x494765
int PickBoundsTestRay__raycast(br_bounds* b, br_vector3* rp, br_vector3* rd, br_scalar t_near, br_scalar t_far, br_scalar* new_t_near, br_scalar* new_t_far) {
    int i;
    float s;
    float t;
    LOG_TRACE("(%p, %p, %p, %f, %f, %p, %p)", b, rp, rd, t_near, t_far, new_t_near, new_t_far);

    for (i = 0; i < 3; i++) {
        if (rd->v[i] > 0.00000023841858) {
            s = (1.0f / rd->v[i]) * (rp->v[i] - b->max.v[i]);
            if (s > BR_SCALAR_MAX) {
                t_near = BR_SCALAR_MAX;
            } else if (s > t_near) {
                t_near = s;
            }
            t = (1.0f / rd->v[i]) * (rp->v[i] - b->min.v[i]);
            if (t < BR_SCALAR_MIN) {
                t_far = BR_SCALAR_MIN;
            } else if (t < t_far) {
                t_far = t;
            }
        } else if (rd->v[i] < -0.00000023841858) {
            s = (1.0f / rd->v[i]) * (rp->v[i] - b->max.v[i]);
            if (s < BR_SCALAR_MIN) {
                t_far = BR_SCALAR_MIN;
            } else if (s < t_far) {
                t_far = s;
            }
            t = (1.0f / rd->v[i]) * (rp->v[i] - b->min.v[i]);
            if (t > BR_SCALAR_MAX) {
                t_near = BR_SCALAR_MAX;
            } else if (t > t_near) {
                t_near = t;
            }
        } else if (rp->v[i] > b->max.v[i] || rp->v[i] < b->min.v[i]) {
            return 0;
        }
    }
    if (t_far < t_near) {
        return 0;
    }
    *new_t_near = t_near;
    *new_t_far = t_far;
    return 1;
}

// IDA: int __usercall ActorPick2D@<EAX>(br_actor *ap@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, dr_pick2d_cbfn *callback@<ECX>, void *arg)
// FUNCTION: CARM95 0x4944de
int ActorPick2D(br_actor* ap, br_model* model, br_material* material, dr_pick2d_cbfn* callback, void* arg) {
    br_actor* a;
    br_model* this_model;
    br_material* this_material;
    br_matrix34 m_to_v;
    br_matrix34 v_to_m;
    br_scalar t_near;
    br_scalar t_far;
    int r;
    br_vector3 dir;
    LOG_TRACE("(%p, %p, %p, %p, %p)", ap, model, material, callback, arg);

    r = 0;
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
    m_to_v = gPick_model_to_view__raycast;

    BrMatrix34PreTransform(&gPick_model_to_view__raycast, &ap->t);
    if (ap->type == BR_ACTOR_MODEL) {
        BrMatrix34Inverse(&v_to_m, &gPick_model_to_view__raycast);
        if (PickBoundsTestRay__raycast(
                &this_model->bounds,
                (br_vector3*)v_to_m.m[3],
                (br_vector3*)v_to_m.m[2],
                0.0,
                BR_SCALAR_MAX,
                &t_near,
                &t_far)) {
            dir.v[0] = -v_to_m.m[2][0];
            dir.v[1] = -v_to_m.m[2][1];
            dir.v[2] = -v_to_m.m[2][2];

            r = callback(
                ap,
                this_model,
                this_material,
                (br_vector3*)v_to_m.m[3],
                &dir,
                t_near,
                t_far,
                arg);
            if (r) {
                gPick_model_to_view__raycast = m_to_v;
                return r;
            }
        }
        if (r) {
            gPick_model_to_view__raycast = m_to_v;
            return r;
        }
    } else if (ap->type == BR_ACTOR_BOUNDS || ap->type == BR_ACTOR_BOUNDS_CORRECT) {
        BrMatrix34Inverse(&v_to_m, &gPick_model_to_view__raycast);
        if (PickBoundsTestRay__raycast(
                (br_bounds*)ap->type_data,
                (br_vector3*)v_to_m.m[3],
                (br_vector3*)v_to_m.m[2],
                0.0,
                BR_SCALAR_MAX,
                &t_near,
                &t_far)) {
            for (a = ap->children; a != NULL; a = a->next) {
                r = ActorPick2D(a, this_model, this_material, callback, arg);
                if (r) {
                    break;
                }
            }
        }
        gPick_model_to_view__raycast = m_to_v;
        return r;
    }
    for (a = ap->children; a != NULL; a = a->next) {
        r = ActorPick2D(a, this_model, this_material, callback, arg);
        if (r) {
            break;
        }
    }
    gPick_model_to_view__raycast = m_to_v;
    return r;
}

// IDA: int __usercall DRScenePick2DXY@<EAX>(br_actor *world@<EAX>, br_actor *camera@<EDX>, br_pixelmap *viewport@<EBX>, int pick_x@<ECX>, int pick_y, dr_pick2d_cbfn *callback, void *arg)
// FUNCTION: CARM95 0x4943a8
int DRScenePick2DXY(br_actor* world, br_actor* camera, br_pixelmap* viewport, int pick_x, int pick_y, dr_pick2d_cbfn* callback, void* arg) {
    br_matrix34 camera_tfm;
    br_scalar scale;
    br_scalar cos_angle;
    br_scalar sin_angle;
    br_camera* camera_data;
    br_angle view_over_2;
    LOG_TRACE("(%p, %p, %p, %d, %d, %p, %p)", world, camera, viewport, pick_x, pick_y, callback, arg);

    camera_data = camera->type_data;
    DRActorToRoot(camera, world, &camera_tfm);
    BrMatrix34Inverse(&gPick_model_to_view__raycast, &camera_tfm);
    view_over_2 = camera_data->field_of_view / 2;
    cos_angle = BR_COS(view_over_2);
    sin_angle = BR_SIN(view_over_2);
    scale = cos_angle / sin_angle;
    BrMatrix34PostScale(&gPick_model_to_view__raycast, scale / camera_data->aspect, scale, 1.f);
    BrMatrix34PostShearZ(&gPick_model_to_view__raycast,
        2 * pick_x / (float)viewport->width,
        -2 * pick_y / (float)viewport->height);
    return ActorPick2D(world, model_unk1, material_unk1, callback, arg);
}

// IDA: int __usercall DRScenePick2D@<EAX>(br_actor *world@<EAX>, br_actor *camera@<EDX>, dr_pick2d_cbfn *callback@<EBX>, void *arg@<ECX>)
// FUNCTION: CARM95 0x49499e
int DRScenePick2D(br_actor* world, br_actor* camera, dr_pick2d_cbfn* callback, void* arg) {
    br_matrix34 camera_tfm;
    br_scalar scale;
    br_camera* camera_data;
    LOG_TRACE("(%p, %p, %p, %p)", world, camera, callback, arg);

    camera_data = (br_camera*)camera->type_data;
    DRActorToRoot(camera, world, &camera_tfm);
    BrMatrix34Inverse(&gPick_model_to_view__raycast, &camera_tfm);
    scale = cosf(BrAngleToRadian(camera_data->field_of_view / 2)) / sinf(BrAngleToRadian(camera_data->field_of_view / 2));

    BrMatrix34PostScale(&gPick_model_to_view__raycast, scale / camera_data->aspect, scale, 1.0f);
    return ActorPick2D(world, model_unk1, material_unk1, callback, arg);
}

// IDA: int __usercall DRModelPick2D@<EAX>(br_model *model@<EAX>, br_material *material@<EDX>, br_vector3 *ray_pos@<EBX>, br_vector3 *ray_dir@<ECX>, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn *callback, void *arg)
// Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x494b88
int DRModelPick2D__raycast(br_model* model, br_material* material, br_vector3* ray_pos, br_vector3* ray_dir, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn* callback, void* arg) {
    // DR_FACE* fp;
    int f;
    int axis_m;
    int axis_0;
    int axis_1;
    int group;
    br_scalar t;
    br_scalar n;
    br_scalar d;
    br_vector3 p;
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
    br_scalar s_alpha;
    br_scalar s_beta;
    br_vector2 map;
    int v;
    int e;
    int r;
    br_material* this_material;
    br_scalar numerator;
    double f_numerator;
    LOG_TRACE("(%p, %p, %p, %p, %f, %f, %p, %p)", model, material, ray_pos, ray_dir, t_near, t_far, callback, arg);

    struct v11group* grp_ptr;
    br_vector4* eqn;

    t_near -= 0.001f;
    t_far += 0.001f;
    for (group = 0; group < V11MODEL(model)->ngroups; group++) {
        grp_ptr = &V11MODEL(model)->groups[group];
        for (f = 0; f < V11MODEL(model)->groups[group].nfaces; f++) {
            eqn = &V11MODEL(model)->groups[group].eqn[f];
            if (V11MODEL(model)->groups[group].user != NULL) {
                this_material = V11MODEL(model)->groups[group].user;
            } else {
                this_material = material;
            }
            d = BrVector3Dot(eqn, ray_dir);
            if (fabsf(d) >= 0.00000023841858f && ((this_material->flags & (BR_MATF_TWO_SIDED | BR_MATF_ALWAYS_VISIBLE)) != 0 || d <= 0.0)) //
            {
                numerator = BrVector3Dot(eqn, ray_pos) - eqn->v[3];
                if (!BadDiv__raycast(numerator, d)) {
                    t = -(numerator / d);
                    if (t >= t_near && t <= t_far) {
                        BrVector3Scale(&p, ray_dir, t);
                        BrVector3Accumulate(&p, ray_pos);
                        axis_m = (fabsf(eqn->v[1]) > fabsf(eqn->v[0])) ? 1 : 0;
                        if (fabsf(eqn->v[2]) > fabsf(eqn->v[axis_m])) {
                            axis_m = 2;
                        }
                        if (axis_m == 0) {
                            axis_0 = 1;
                            axis_1 = 2;
                        } else if (axis_m == 1) {
                            axis_0 = 0;
                            axis_1 = 2;
                        } else if (axis_m == 2) {
                            axis_0 = 0;
                            axis_1 = 1;
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
                            f_d = v0i2 * v1 - u1 * v0i1;
                            f_n = u2 * v1 - u1 * v2;
                            if (f_n == 0.) {
                                continue;
                            }
                            beta = f_d / f_n;
                            alpha = (v0i1 - beta * v2) / v1;
                        } else {
                            beta = v0i1 / v2;
                            alpha = (v0i2 - beta * u2) / u1;
                        }

                        if (alpha >= 0.0 && beta >= 0.0 && beta + alpha <= 1.0) {
                            s_alpha = alpha;
                            s_beta = beta;
                            BrVector2Scale(&map, &grp_ptr->map[grp_ptr->vertex_numbers[f].v[1]], s_alpha);
                            DRVector2AccumulateScale__raycast(
                                &map,
                                &grp_ptr->map[grp_ptr->vertex_numbers[f].v[2]],
                                s_beta);
                            DRVector2AccumulateScale__raycast(
                                &map,
                                &grp_ptr->map[grp_ptr->vertex_numbers[f].v[0]],
                                1.0f - (s_alpha + s_beta));
                            v = 0;
                            e = 1;
                            if (s_alpha <= s_beta) {
                                if (0.5f - s_beta / 2.0f > s_alpha) {
                                    e = 0;
                                }
                                if (1.0f - s_beta * 2.0f < s_alpha) {
                                    v = 1;
                                }
                            } else {
                                if (1.0f - s_beta * 2.0f > s_alpha) {
                                    e = 2;
                                }
                                if (0.5f - s_beta / 2.0f < s_alpha) {
                                    v = 2;
                                }
                            }
                            r = callback(model, this_material, ray_pos, ray_dir, t, f, e, v, &p, &map, arg);
                            if (r != 0) {
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
//  Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x49534b
int FindHighestPolyCallBack__raycast(br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3* pPoint, br_vector2* pMap, void* pArg) {
    br_scalar the_y;
    LOG_TRACE("(%p, %p, %p, %p, %f, %d, %d, %d, %p, %p, %p)", pModel, pMaterial, pRay_pos, pRay_dir, pT, pF, pE, pV, pPoint, pMap, pArg);

    if (pPoint->v[1] > gCurrent_y) {
        if (gLowest_y_above > pPoint->v[1]) {
            gLowest_y_above = pPoint->v[1];
            gAbove_face_index = pF;
            gAbove_model = pModel;
        }
    } else if (pPoint->v[1] > gHighest_y_below) {
        gHighest_y_below = pPoint->v[1];
        gBelow_face_index = pF;
        gBelow_model = pModel;
    }
    return 0;
}

// IDA: int __cdecl FindHighestCallBack(br_actor *pActor, br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT_near, br_scalar pT_far, void *pArg)
//  Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x494b28
int FindHighestCallBack__raycast(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT_near, br_scalar pT_far, void* pArg) {
    LOG_TRACE("(%p, %p, %p, %p, %p, %f, %f, %p)", pActor, pModel, pMaterial, pRay_pos, pRay_dir, pT_near, pT_far, pArg);

    if (gProgram_state.current_car.current_car_actor < 0
        || gProgram_state.current_car.car_model_actors[gProgram_state.current_car.current_car_actor].actor != pActor) {
        DRModelPick2D__raycast(pModel, pMaterial, pRay_pos, pRay_dir, pT_near, pT_far, FindHighestPolyCallBack__raycast, pArg);
    }
    return 0;
}

// IDA: void __usercall FindBestY(br_vector3 *pPosition@<EAX>, br_actor *gWorld@<EDX>, br_scalar pStarting_height, br_scalar *pNearest_y_above, br_scalar *pNearest_y_below, br_model **pNearest_above_model, br_model **pNearest_below_model, int *pNearest_above_face_index, int *pNearest_below_face_index)
// FUNCTION: CARM95 0x494a71
void FindBestY(br_vector3* pPosition, br_actor* gWorld, br_scalar pStarting_height, br_scalar* pNearest_y_above, br_scalar* pNearest_y_below, br_model** pNearest_above_model, br_model** pNearest_below_model, int* pNearest_above_face_index, int* pNearest_below_face_index) {
    LOG_TRACE("(%p, %p, %f, %p, %p, %p, %p, %p, %p)", pPosition, gWorld, pStarting_height, pNearest_y_above, pNearest_y_below, pNearest_above_model, pNearest_below_model, pNearest_above_face_index, pNearest_below_face_index);

    gLowest_y_above = 30000.0;
    gHighest_y_below = -30000.0;
    gCurrent_y = pPosition->v[1] + 0.000011920929;
    gY_picking_camera->t.t.euler.t = *pPosition;
    gY_picking_camera->t.t.mat.m[3][1] = gY_picking_camera->t.t.mat.m[3][1] + pStarting_height;
    DRScenePick2D(gWorld, gY_picking_camera, FindHighestCallBack__raycast, 0);
    *pNearest_y_above = gLowest_y_above;
    *pNearest_y_below = gHighest_y_below;
    *pNearest_above_model = gAbove_model;
    *pNearest_below_model = gBelow_model;
    *pNearest_above_face_index = gAbove_face_index;
    *pNearest_below_face_index = gBelow_face_index;
}

// IDA: int __cdecl FindYVerticallyBelowPolyCallBack(br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3 *pPoint, br_vector2 *pMap, void *pArg)
// FUNCTION: CARM95 0x4955eb
int FindYVerticallyBelowPolyCallBack(br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3* pPoint, br_vector2* pMap, void* pArg) {
    br_scalar the_y;
    LOG_TRACE("(%p, %p, %p, %p, %f, %d, %d, %d, %p, %p, %p)", pModel, pMaterial, pRay_pos, pRay_dir, pT, pF, pE, pV, pPoint, pMap, pArg);

    if (pMaterial->identifier == NULL || pMaterial->identifier[0] != '!') {
        the_y = pPoint->v[Y];
        if (the_y > gHighest_y_below) {
            gHighest_y_below = the_y;
        }
    }
    return 0;
}

// IDA: int __cdecl FindYVerticallyBelowCallBack(br_actor *pActor, br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT_near, br_scalar pT_far, void *pArg)
// FUNCTION: CARM95 0x49558b
int FindYVerticallyBelowCallBack(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT_near, br_scalar pT_far, void* pArg) {
    LOG_TRACE("(%p, %p, %p, %p, %p, %f, %f, %p)", pActor, pModel, pMaterial, pRay_pos, pRay_dir, pT_near, pT_far, pArg);

    if (gProgram_state.current_car.current_car_actor < 0
        || gProgram_state.current_car.car_model_actors[gProgram_state.current_car.current_car_actor].actor != pActor) {
        DRModelPick2D__raycast(pModel, pMaterial, pRay_pos, pRay_dir, pT_near, pT_far, (dr_modelpick2d_cbfn*)FindYVerticallyBelowPolyCallBack, pArg);
    }
    return 0;
}

// IDA: br_scalar __usercall FindYVerticallyBelow@<ST0>(br_vector3 *pPosition@<EAX>)
// FUNCTION: CARM95 0x4953da
br_scalar FindYVerticallyBelow(br_vector3* pPosition) {
    tU8 cx;
    tU8 cz;
    tU8 x;
    tU8 z;
    tTrack_spec* track_spec;
    LOG_TRACE("(%p)", pPosition);

    track_spec = &gProgram_state.track_spec;
    XZToColumnXZ(&cx, &cz, pPosition->v[X], pPosition->v[Z], track_spec);
    gHighest_y_below = BR_SCALAR_MIN;
    BrVector3Copy(&gY_picking_camera->t.t.translate.t, pPosition);
    for (x = MAX(cx - 1, 0); x < MIN(cx + 2, track_spec->ncolumns_x); x++) {
        for (z = MAX(cz - 1, 0); z < MIN(cz + 2, track_spec->ncolumns_z); z++) {
            if (track_spec->columns[z][x] != NULL) {
                if (track_spec->blends[z][x] != NULL) {
                    track_spec->blends[z][x]->render_style = BR_RSTYLE_FACES;
                }
                DRScenePick2D(track_spec->columns[z][x], gY_picking_camera, FindYVerticallyBelowCallBack, NULL);
                if (track_spec->blends[z][x] != NULL) {
                    track_spec->blends[z][x]->render_style = BR_RSTYLE_NONE;
                }
            }
        }
    }
    return gHighest_y_below;
}

// IDA: br_scalar __usercall FindYVerticallyBelow2@<ST0>(br_vector3 *pCast_point@<EAX>)
// FUNCTION: CARM95 0x49564b
br_scalar FindYVerticallyBelow2(br_vector3* pCast_point) {
    br_scalar result;
    int number_of_attempts;
    br_vector3 cast_point;
    LOG_TRACE("(%p)", pCast_point);

    BrVector3Copy(&cast_point, pCast_point);
    for (number_of_attempts = 0; number_of_attempts <= 10; number_of_attempts++) {
        result = FindYVerticallyBelow(&cast_point);
        cast_point.v[Y] += .2f;
        if (result >= -100.f) {
            return result;
        }
    }
    return result;
}
