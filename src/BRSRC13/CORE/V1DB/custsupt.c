#include "custsupt.h"
#include "harness.h"

char rscid[52];

void BrModelToScreenQuery(br_matrix4* dest) {
    br_uint_32 dummy;
    br_matrix4 v2s;
    br_matrix34 m2v;
    NOT_IMPLEMENTED();
}

void BrModelToViewQuery(br_matrix34* dest) {
    br_uint_32 dummy;
    NOT_IMPLEMENTED();
}

br_uint_8 BrOriginToScreenXY(br_vector2* screen) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrOriginToScreenXYZO(br_vector3* screen) {
    br_uint_32 outcode;
    NOT_IMPLEMENTED();
}

br_uint_8 BrPointToScreenXY(br_vector2* screen, br_vector3* point) {
    br_vector4 sp;
    NOT_IMPLEMENTED();
}

br_uint_32 BrPointToScreenXYZO(br_vector3* screen, br_vector3* point) {
    br_vector4 sp;
    br_uint_32 outcode;
    NOT_IMPLEMENTED();
}

void BrPointToScreenXYMany(br_vector2* screens, br_vector3* points, br_uint_32 npoints) {
    br_vector4 sp;
    int i;
    NOT_IMPLEMENTED();
}

void BrPointToScreenXYZOMany(br_vector3* screens, br_uint_32* outcodes, br_vector3* points, br_uint_32 npoints) {
    br_vector4 sp;
    int i;
    br_uint_32 outcode;
    NOT_IMPLEMENTED();
}

br_scalar BrZbDepthToScreenZ(br_uint_32 depth_z, br_camera* camera) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrZbScreenZToDepth(br_scalar sz, br_camera* camera) {
    br_uint_32 depth;
    NOT_IMPLEMENTED();
}

br_scalar BrZsDepthToScreenZ(br_scalar depth_z, br_camera* camera) {
    br_scalar hither;
    br_scalar yon;
    NOT_IMPLEMENTED();
}

br_scalar BrZsScreenZToDepth(br_scalar sz, br_camera* camera) {
    br_scalar hither;
    br_scalar yon;
    br_scalar depth;
    NOT_IMPLEMENTED();
}

br_scalar BrScreenZToCamera(br_actor* camera, br_scalar sz) {
    br_camera* data;
    br_scalar hither;
    br_scalar yon;
    NOT_IMPLEMENTED();
}

void BrScreenXYZToCamera(br_vector3* point, br_actor* camera, br_pixelmap* screen_buffer, br_int_16 x, br_int_16 y, br_scalar sz) {
    br_scalar hx;
    br_scalar hy;
    br_scalar vz;
    br_camera* data;
    br_angle fov;
    br_scalar scale;
    NOT_IMPLEMENTED();
}
