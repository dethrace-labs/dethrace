#include "dr_types.h"
#include "br_types.h"
// Offset: 21
// Size: 222
void BrModelToScreenQuery(br_matrix4 *dest);

// Offset: 262
// Size: 161
void BrModelToViewQuery(br_matrix34 *dest);

// Offset: 442
// Size: 251
br_uint_8 BrOriginToScreenXY(br_vector2 *screen);

// Offset: 714
// Size: 423
br_uint_32 BrOriginToScreenXYZO(br_vector3 *screen);

// Offset: 1155
// Size: 295
br_uint_8 BrPointToScreenXY(br_vector2 *screen, br_vector3 *point, char sp);

// Offset: 1470
// Size: 425
br_uint_32 BrPointToScreenXYZO(br_vector3 *screen, br_vector3 *point);

// Offset: 1917
// Size: 313
void BrPointToScreenXYMany(br_vector2 *screens, br_vector3 *points, br_uint_32 npoints, enum sp);

// Offset: 2254
// Size: 513
void BrPointToScreenXYZOMany(br_vector3 *screens, br_uint_32 *outcodes, br_vector3 *points);

// Offset: 2786
// Size: 98
br_scalar BrZbDepthToScreenZ(br_uint_32 depth_z, br_camera *camera, short __unk2__);

// Offset: 2903
// Size: 139
br_uint_32 BrZbScreenZToDepth(br_scalar sz, br_camera *camera, short depth);

// Offset: 3061
// Size: 207
br_scalar BrZsDepthToScreenZ(br_scalar depth_z, br_camera *camera, unsigned long hither);

// Offset: 3287
// Size: 178
br_scalar BrZsScreenZToDepth(br_scalar sz, br_camera *camera, unsigned long hither);

// Offset: 3483
// Size: 459
br_scalar BrScreenZToCamera(br_actor *camera);

// Offset: 3962
// Size: 766
void BrScreenXYZToCamera(br_vector3 *point, br_actor *camera, br_pixelmap *screen_buffer, br_int_16 x);

