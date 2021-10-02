#include "custsupt.h"
#include "harness/trace.h"

// IDA: void __cdecl BrModelToScreenQuery(br_matrix4 *dest)
void BrModelToScreenQuery(br_matrix4* dest) {
    br_uint_32 dummy;
    br_matrix4 v2s;
    br_matrix34 m2v;
    LOG_TRACE("(%p)", dest);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrModelToViewQuery(br_matrix34 *dest)
void BrModelToViewQuery(br_matrix34* dest) {
    br_uint_32 dummy;
    LOG_TRACE("(%p)", dest);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_8 __cdecl BrOriginToScreenXY(br_vector2 *screen)
br_uint_8 BrOriginToScreenXY(br_vector2* screen) {
    LOG_TRACE("(%p)", screen);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrOriginToScreenXYZO(br_vector3 *screen)
br_uint_32 BrOriginToScreenXYZO(br_vector3* screen) {
    br_uint_32 outcode;
    LOG_TRACE("(%p)", screen);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_8 __cdecl BrPointToScreenXY(br_vector2 *screen, br_vector3 *point)
br_uint_8 BrPointToScreenXY(br_vector2* screen, br_vector3* point) {
    br_vector4 sp;
    LOG_TRACE("(%p, %p)", screen, point);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrPointToScreenXYZO(br_vector3 *screen, br_vector3 *point)
br_uint_32 BrPointToScreenXYZO(br_vector3* screen, br_vector3* point) {
    br_vector4 sp;
    br_uint_32 outcode;
    LOG_TRACE("(%p, %p)", screen, point);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPointToScreenXYMany(br_vector2 *screens, br_vector3 *points, br_uint_32 npoints)
void BrPointToScreenXYMany(br_vector2* screens, br_vector3* points, br_uint_32 npoints) {
    br_vector4 sp;
    int i;
    LOG_TRACE("(%p, %p, %d)", screens, points, npoints);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPointToScreenXYZOMany(br_vector3 *screens, br_uint_32 *outcodes, br_vector3 *points, br_uint_32 npoints)
void BrPointToScreenXYZOMany(br_vector3* screens, br_uint_32* outcodes, br_vector3* points, br_uint_32 npoints) {
    br_vector4 sp;
    int i;
    br_uint_32 outcode;
    LOG_TRACE("(%p, %p, %p, %d)", screens, outcodes, points, npoints);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl BrZbDepthToScreenZ(br_uint_32 depth_z, br_camera *camera)
br_scalar BrZbDepthToScreenZ(br_uint_32 depth_z, br_camera* camera) {
    LOG_TRACE("(%d, %p)", depth_z, camera);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrZbScreenZToDepth(br_scalar sz, br_camera *camera)
br_uint_32 BrZbScreenZToDepth(br_scalar sz, br_camera* camera) {
    br_uint_32 depth;
    LOG_TRACE("(%f, %p)", sz, camera);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl BrZsDepthToScreenZ(br_scalar depth_z, br_camera *camera)
br_scalar BrZsDepthToScreenZ(br_scalar depth_z, br_camera* camera) {
    br_scalar hither;
    br_scalar yon;
    LOG_TRACE("(%f, %p)", depth_z, camera);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl BrZsScreenZToDepth(br_scalar sz, br_camera *camera)
br_scalar BrZsScreenZToDepth(br_scalar sz, br_camera* camera) {
    br_scalar hither;
    br_scalar yon;
    br_scalar depth;
    LOG_TRACE("(%f, %p)", sz, camera);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl BrScreenZToCamera(br_actor *camera, br_scalar sz)
br_scalar BrScreenZToCamera(br_actor* camera, br_scalar sz) {
    br_camera* data;
    br_scalar hither;
    br_scalar yon;
    LOG_TRACE("(%p, %f)", camera, sz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrScreenXYZToCamera(br_vector3 *point, br_actor *camera, br_pixelmap *screen_buffer, br_int_16 x, br_int_16 y, br_scalar sz)
void BrScreenXYZToCamera(br_vector3* point, br_actor* camera, br_pixelmap* screen_buffer, br_int_16 x, br_int_16 y, br_scalar sz) {
    br_scalar hx;
    br_scalar hy;
    br_scalar vz;
    br_camera* data;
    br_angle fov;
    br_scalar scale;
    LOG_TRACE("(%p, %p, %p, %d, %d, %f)", point, camera, screen_buffer, x, y, sz);
    NOT_IMPLEMENTED();
}
