#include "raycast.h"
#include "brender.h"
#include "finteray.h"
#include <stdlib.h>

br_matrix34 gPick_model_to_view_raycast; //added _raycast suffix to avoid name collision
int gBelow_face_index;
br_scalar gCurrent_y;
int gAbove_face_index;
br_model* gAbove_model;
br_model* gBelow_model;
br_scalar gHighest_y_below;
br_actor* gY_picking_camera;
br_scalar gLowest_y_above;

br_model* model_unk1;
br_material* material_unk1;

// IDA: int __usercall DRActorToRoot@<EAX>(br_actor *a@<EAX>, br_actor *world@<EDX>, br_matrix34 *m@<EBX>)
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
void InitRayCasting() {
    br_camera* camera_ptr;
    LOG_TRACE("()");
    br_actor* a;

    a = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
    camera_ptr = a->type_data;
    camera_ptr->type = BR_CAMERA_PERSPECTIVE_FOV;
    camera_ptr->field_of_view = BR_ANGLE_DEG(70.0f);
    camera_ptr->hither_z = 0.001;
    camera_ptr->yon_z = 1000.0;
    camera_ptr->aspect = 1.0;
    a->t.t.quat.q.x = 1.0;
    a->t.t.quat.q.y = 0.0;
    a->t.t.quat.q.z = 0.0;
    a->t.t.quat.q.w = 0.0;
    a->t.t.mat.m[1][1] = 0.0;
    a->t.t.mat.m[1][2] = -1.0;
    a->t.t.mat.m[2][0] = 0.0;
    a->t.t.mat.m[2][1] = 1.0;
    a->t.t.mat.m[2][2] = 0.0;
    gY_picking_camera = a;
}

// IDA: int __cdecl BadDiv(br_scalar a, br_scalar b)
int BadDiv(br_scalar a, br_scalar b) {
    //LOG_TRACE("(%f, %f)", a, b);

    return fabs(b) < 1.0 && fabs(a) > fabs(b) * 3.4028235e38;
}

// IDA: int __usercall PickBoundsTestRay@<EAX>(br_bounds *b@<EAX>, br_vector3 *rp@<EDX>, br_vector3 *rd@<EBX>, br_scalar t_near, br_scalar t_far, br_scalar *new_t_near, br_scalar *new_t_far)
int PickBoundsTestRay(br_bounds* b, br_vector3* rp, br_vector3* rd, br_scalar t_near, br_scalar t_far, br_scalar* new_t_near, br_scalar* new_t_far) {
    int i;
    float s;
    float t;
    LOG_TRACE("(%p, %p, %p, %f, %f, %p, %p)", b, rp, rd, t_near, t_far, new_t_near, new_t_far);

    for (i = 0; i < 3; i++) {
        if (rd->v[i] <= 0.00000023841858) {
            if (rd->v[i] >= -0.00000023841858) {
                if (b->max.v[i] < rp->v[i] || rp->v[i] < b->min.v[i]) {
                    return 0;
                }
            } else {
                s = (1.0f / rd->v[i]) * (rp->v[i] - b->max.v[i]);
                if (s >= -3.4028235e38) {
                    if (s < t_far) {
                        t_far = (1.0f / rd->v[i]) * (rp->v[i] - b->max.v[i]);
                    }
                } else {
                    t_far = -3.4028235e38;
                }
                t = (1.0f / rd->v[i]) * (rp->v[i] - b->min.v[i]);
                if (t <= 3.4028235e38) {
                    if (t > t_near) {
                        t_near = (1.0f / rd->v[i]) * (rp->v[i] - b->min.v[i]);
                    }
                } else {
                    t_near = 3.4028235e38;
                }
            }
        } else {
            s = (1.0f / rd->v[i]) * (rp->v[i] - b->max.v[i]);
            if (s <= 3.4028235e38) {
                if (s > t_near) {
                    t_near = (1.0f / rd->v[i]) * (rp->v[i] - b->max.v[i]);
                }
            } else {
                t_near = 3.4028235e38;
            }
            t = (1.0f / rd->v[i]) * (rp->v[i] - b->min.v[i]);
            if (t >= -3.4028235e38) {
                if (t < t_far) {
                    t_far = (1.0f / rd->v[i]) * (rp->v[i] - b->min.v[i]);
                }
            } else {
                t_far = -3.4028235e38;
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

// IDA: int __usercall ActorPick2D@<EAX>(br_actor *ap@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, dr_pick2d_cbfn *callback@<ECX>, void *arg)
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
    if (ap->model) {
        this_model = ap->model;
    } else {
        this_model = model;
    }
    if (ap->material) {
        this_material = ap->material;
    } else {
        this_material = material;
    }
    if (ap->render_style == BR_RSTYLE_NONE) {
        return 0;
    }
    m_to_v = gPick_model_to_view_raycast;

    BrMatrix34PreTransform(&gPick_model_to_view_raycast, &ap->t);
    if (ap->type == BR_ACTOR_MODEL) {
        BrMatrix34Inverse(&v_to_m, &gPick_model_to_view_raycast);
        if (PickBoundsTestRay(
                &this_model->bounds,
                (br_vector3*)v_to_m.m[3],
                (br_vector3*)v_to_m.m[2],
                0.0,
                3.4028235e38,
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
                gPick_model_to_view_raycast = m_to_v;
                return r;
            }
        }
        if (r) {
            gPick_model_to_view_raycast = m_to_v;
            return r;
        }
    } else if (ap->type == BR_ACTOR_BOUNDS || ap->type == BR_ACTOR_BOUNDS_CORRECT) {
        BrMatrix34Inverse(&v_to_m, &gPick_model_to_view_raycast);
        if (PickBoundsTestRay(
                (br_bounds*)ap->type_data,
                (br_vector3*)v_to_m.m[3],
                (br_vector3*)v_to_m.m[2],
                0.0,
                3.4028235e38,
                &t_near,
                &t_far)) {
            for (a = ap->children; a != NULL; a = a->next) {
                r = ActorPick2D(a, this_model, this_material, callback, arg);
                if (r) {
                    break;
                }
            }
        }
        gPick_model_to_view_raycast = m_to_v;
        return r;
    }
    for (a = ap->children; a != NULL; a = a->next) {
        r = ActorPick2D(a, this_model, this_material, callback, arg);
        if (r) {
            break;
        }
    }
    gPick_model_to_view_raycast = m_to_v;
    return r;
}

// IDA: int __usercall DRScenePick2DXY@<EAX>(br_actor *world@<EAX>, br_actor *camera@<EDX>, br_pixelmap *viewport@<EBX>, int pick_x@<ECX>, int pick_y, dr_pick2d_cbfn *callback, void *arg)
int DRScenePick2DXY(br_actor* world, br_actor* camera, br_pixelmap* viewport, int pick_x, int pick_y, dr_pick2d_cbfn* callback, void* arg) {
    br_matrix34 camera_tfm;
    br_scalar scale;
    br_scalar cos_angle;
    br_scalar sin_angle;
    br_camera* camera_data;
    br_angle view_over_2;
    LOG_TRACE("(%p, %p, %p, %d, %d, %p, %p)", world, camera, viewport, pick_x, pick_y, callback, arg);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRScenePick2D@<EAX>(br_actor *world@<EAX>, br_actor *camera@<EDX>, dr_pick2d_cbfn *callback@<EBX>, void *arg@<ECX>)
int DRScenePick2D(br_actor* world, br_actor* camera, dr_pick2d_cbfn* callback, void* arg) {
    br_matrix34 camera_tfm;
    br_scalar scale;
    br_camera* camera_data;
    LOG_TRACE("(%p, %p, %p, %p)", world, camera, callback, arg);

    camera_data = (br_camera*)camera->type_data;
    DRActorToRoot(camera, world, &camera_tfm);
    BrMatrix34Inverse(&gPick_model_to_view_raycast, &camera_tfm);
    scale = cos(BrAngleToRadian(camera_data->field_of_view / 2));
    scale = scale / sin(scale);

    BrMatrix34PostScale(&gPick_model_to_view_raycast, scale / camera_data->aspect, scale, 1.0);
    return ActorPick2D(world, model_unk1, material_unk1, callback, arg);
}

// IDA: int __usercall DRModelPick2D@<EAX>(br_model *model@<EAX>, br_material *material@<EDX>, br_vector3 *ray_pos@<EBX>, br_vector3 *ray_dir@<ECX>, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn *callback, void *arg)
int DRModelPick2D(br_model* model, br_material* material, br_vector3* ray_pos, br_vector3* ray_dir, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn* callback, void* arg) {
    DR_FACE* fp;
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

    for (group = 0; group < V11MODEL(model)->ngroups; group++) {
        for (f = 0; f < V11MODEL(model)->groups[group].nfaces; f++) {
            fp = &V11MODEL(model)->groups[group].faces[f];
            if (V11MODEL(model)->groups[group].face_colours_material) {
                this_material = V11MODEL(model)->groups[group].face_colours_material;
            } else {
                this_material = material;
            }
            d = fp->eqn.v[1] * ray_dir->v[1] + fp->eqn.v[2] * ray_dir->v[2] + fp->eqn.v[0] * ray_dir->v[0];
            if (fabs(d) >= 0.00000023841858 && ((this_material->flags & 0x1800) != 0 || d <= 0.0)) // BR_MATF_TWO_SIDED | BR_MATF_ALWAYS_VISIBLE
            {
                numerator = fp->eqn.v[1] * ray_pos->v[1]
                    + fp->eqn.v[2] * ray_pos->v[2]
                    + fp->eqn.v[0] * ray_pos->v[0]
                    - fp->eqn.v[3];
                if (!BadDiv(numerator, d)) {
                    t = -(numerator / d);
                    if (t >= (t_near - 0.001) && t <= (t_far + 0.001)) {
                        p.v[0] = ray_dir->v[0] * t;
                        p.v[1] = ray_dir->v[1] * t;
                        p.v[2] = ray_dir->v[2] * t;
                        p.v[0] = ray_pos->v[0] + p.v[0];
                        p.v[1] = ray_pos->v[1] + p.v[1];
                        p.v[2] = ray_pos->v[2] + p.v[2];
                        axis_m = fabs(fp->eqn.v[0]) < fabs(fp->eqn.v[1]);
                        if (fabs(fp->eqn.v[2]) > fabs(fp->eqn.v[axis_m])) {
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

                        v0 = V11MODEL(model)->groups[group].vertices[fp->vertices[0]].p.v[axis_0];
                        u0 = V11MODEL(model)->groups[group].vertices[fp->vertices[0]].p.v[axis_1];
                        v1 = V11MODEL(model)->groups[group].vertices[fp->vertices[1]].p.v[axis_0] - v0;
                        u1 = V11MODEL(model)->groups[group].vertices[fp->vertices[1]].p.v[axis_1] - u0;
                        v2 = V11MODEL(model)->groups[group].vertices[fp->vertices[2]].p.v[axis_0] - v0;
                        u2 = V11MODEL(model)->groups[group].vertices[fp->vertices[2]].p.v[axis_1] - u0;

                        v0i1 = p.v[axis_0] - v0;
                        v0i2 = p.v[axis_1] - u0;
                        if (fabs(v1) > 0.0000002384185791015625) {
                            f_d = v0i2 * v1 - u1 * v0i1;
                            f_n = u2 * v1 - u1 * v2;
                            if (f_d == 0) {
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
                            map.v[0] = V11MODEL(model)->groups[group].vertices[fp->vertices[1]].map.v[0] * s_alpha;
                            map.v[1] = V11MODEL(model)->groups[group].vertices[fp->vertices[1]].map.v[1] * s_alpha;
                            DRVector2AccumulateScale(
                                &map,
                                &V11MODEL(model)->groups[group].vertices[fp->vertices[2]].map,
                                s_beta);
                            DRVector2AccumulateScale(
                                &map,
                                &V11MODEL(model)->groups[group].vertices[fp->vertices[0]].map,
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
                            r = callback(
                                model,
                                this_material,
                                ray_pos,
                                ray_dir,
                                t,
                                f,
                                e,
                                v,
                                &p,
                                &map,
                                arg);
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

// IDA: void __usercall FindBestY(br_vector3 *pPosition@<EAX>, br_actor *gWorld@<EDX>, br_scalar pStarting_height, br_scalar *pNearest_y_above, br_scalar *pNearest_y_below, br_model **pNearest_above_model, br_model **pNearest_below_model, int *pNearest_above_face_index, int *pNearest_below_face_index)
void FindBestY(br_vector3* pPosition, br_actor* gWorld, br_scalar pStarting_height, br_scalar* pNearest_y_above, br_scalar* pNearest_y_below, br_model** pNearest_above_model, br_model** pNearest_below_model, int* pNearest_above_face_index, int* pNearest_below_face_index) {
    LOG_TRACE("(%p, %p, %f, %p, %p, %p, %p, %p, %p)", pPosition, gWorld, pStarting_height, pNearest_y_above, pNearest_y_below, pNearest_above_model, pNearest_below_model, pNearest_above_face_index, pNearest_below_face_index);

    gLowest_y_above = 30000.0;
    gHighest_y_below = -30000.0;
    gCurrent_y = pPosition->v[1] + 0.000011920929;
    gY_picking_camera->t.t.euler.t = *pPosition;
    gY_picking_camera->t.t.mat.m[3][1] = gY_picking_camera->t.t.mat.m[3][1] + pStarting_height;
    DRScenePick2D(gWorld, gY_picking_camera, FindHighestCallBack, 0);
    *pNearest_y_above = gLowest_y_above;
    *pNearest_y_below = gHighest_y_below;
    *pNearest_above_model = gAbove_model;
    *pNearest_below_model = gBelow_model;
    *pNearest_above_face_index = gAbove_face_index;
    *pNearest_below_face_index = gBelow_face_index;
    LOG_DEBUG("FindBestY %f %f '%s' '%s' %d %d", gLowest_y_above, gHighest_y_below, gAbove_model->identifier, gBelow_model->identifier, gAbove_face_index, gBelow_face_index);
}

// IDA: int __cdecl FindYVerticallyBelowPolyCallBack(br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3 *pPoint, br_vector2 *pMap, void *pArg)
int FindYVerticallyBelowPolyCallBack(br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3* pPoint, br_vector2* pMap, void* pArg) {
    br_scalar the_y;
    LOG_TRACE("(%p, %p, %p, %p, %f, %d, %d, %d, %p, %p, %p)", pModel, pMaterial, pRay_pos, pRay_dir, pT, pF, pE, pV, pPoint, pMap, pArg);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl FindYVerticallyBelowCallBack(br_actor *pActor, br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT_near, br_scalar pT_far, void *pArg)
int FindYVerticallyBelowCallBack(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT_near, br_scalar pT_far, void* pArg) {
    LOG_TRACE("(%p, %p, %p, %p, %p, %f, %f, %p)", pActor, pModel, pMaterial, pRay_pos, pRay_dir, pT_near, pT_far, pArg);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FindYVerticallyBelow@<ST0>(br_vector3 *pPosition@<EAX>)
br_scalar FindYVerticallyBelow(br_vector3* pPosition) {
    tU8 cx;
    tU8 cz;
    tU8 x;
    tU8 z;
    tTrack_spec* track_spec;
    LOG_TRACE("(%p)", pPosition);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FindYVerticallyBelow2@<ST0>(br_vector3 *pCast_point@<EAX>)
br_scalar FindYVerticallyBelow2(br_vector3* pCast_point) {
    br_scalar result;
    int number_of_attempts;
    br_vector3 cast_point;
    LOG_TRACE("(%p)", pCast_point);
    NOT_IMPLEMENTED();
}
