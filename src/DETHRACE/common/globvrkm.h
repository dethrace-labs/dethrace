#ifndef _GLOBVRKM_H_
#define _GLOBVRKM_H_

#include "br_types.h"
#include "dr_types.h"

extern br_actor* gCamera_list[2];
extern tCar_spec* gActive_car_list[25];
extern int gNum_active_cars;
extern tCar_spec* gCar_to_view;
extern float gRecovery_cost[3];
extern br_scalar gCamera_zoom;
extern br_scalar gCamera_height;
extern br_vector3 gView_direction;
extern br_scalar gMin_camera_car_distance;
extern int gCar_flying;
extern int gCamera_sign;
extern int gCamera_reset;
extern br_angle gCamera_yaw;

#endif
