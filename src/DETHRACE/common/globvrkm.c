#include "globvrkm.h"
#include "globvars.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x0050d380
br_scalar gCamera_zoom = 0.2f;

// GLOBAL: CARM95 0x0050d384
br_angle gCamera_yaw;

// GLOBAL: CARM95 0x0050d388
br_vector3 gView_direction = { { 0.0, 0.0, -1.0 } };

// GLOBAL: CARM95 0x0050d394
int gCamera_sign;

// GLOBAL: CARM95 0x0050d398
int gCar_flying;

// GLOBAL: CARM95 0x0050d39c
int gCamera_reset;

// GLOBAL: CARM95 0x0050d3a0
tCar_spec* gCar_to_view = &gProgram_state.current_car;

// GLOBAL: CARM95 0x00551438
br_actor* gCamera_list[2];

// GLOBAL: CARM95 0x00551450
tCar_spec* gActive_car_list[25];

// GLOBAL: CARM95 0x005514cc
int gNum_active_cars;

// GLOBAL: CARM95 0x005514c0
float gRecovery_cost[3];

// GLOBAL: CARM95 0x005514d0
br_scalar gCamera_height;

// GLOBAL: CARM95 0x00551440
br_scalar gMin_camera_car_distance;
