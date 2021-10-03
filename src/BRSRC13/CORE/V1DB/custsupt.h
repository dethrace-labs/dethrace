#ifndef _CUSTSUPT_H_
#define _CUSTSUPT_H_

#include "brender/br_types.h"

void BrModelToScreenQuery(br_matrix4* dest);

void BrModelToViewQuery(br_matrix34* dest);

br_uint_8 BrOriginToScreenXY(br_vector2* screen);

br_uint_32 BrOriginToScreenXYZO(br_vector3* screen);

br_uint_8 BrPointToScreenXY(br_vector2* screen, br_vector3* point);

br_uint_32 BrPointToScreenXYZO(br_vector3* screen, br_vector3* point);

void BrPointToScreenXYMany(br_vector2* screens, br_vector3* points, br_uint_32 npoints);

void BrPointToScreenXYZOMany(br_vector3* screens, br_uint_32* outcodes, br_vector3* points, br_uint_32 npoints);

br_scalar BrZbDepthToScreenZ(br_uint_32 depth_z, br_camera* camera);

br_uint_32 BrZbScreenZToDepth(br_scalar sz, br_camera* camera);

br_scalar BrZsDepthToScreenZ(br_scalar depth_z, br_camera* camera);

br_scalar BrZsScreenZToDepth(br_scalar sz, br_camera* camera);

br_scalar BrScreenZToCamera(br_actor* camera, br_scalar sz);

void BrScreenXYZToCamera(br_vector3* point, br_actor* camera, br_pixelmap* screen_buffer, br_int_16 x, br_int_16 y, br_scalar sz);

#endif
