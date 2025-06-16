#include "globvrkm.h"
#include "globvars.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x50d380
br_scalar gCamera_zoom = 0.2f;
// GLOBAL: CARM95 0x50d384
br_angle gCamera_yaw;
// GLOBAL: CARM95 0x50d388
br_vector3 gView_direction = { { 0.0, 0.0, -1.0 } };
// GLOBAL: CARM95 0x50d394
int gCamera_sign;
// GLOBAL: CARM95 0x50d398
int gCar_flying;
// GLOBAL: CARM95 0x50d39c
int gCamera_reset;
// GLOBAL: CARM95 0x50d3a0
tCar_spec* gCar_to_view = &gProgram_state.current_car;
br_actor* gCamera_list[2];
tCar_spec* gActive_car_list[25];
// GLOBAL: CARM95 0x5514cc
int gNum_active_cars;
float gRecovery_cost[3];
// GLOBAL: CARM95 0x5514d0
br_scalar gCamera_height;
// GLOBAL: CARM95 0x551440
br_scalar gMin_camera_car_distance;
