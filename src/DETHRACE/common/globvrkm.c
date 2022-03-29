#include "globvrkm.h"
#include "globvars.h"
#include <stdlib.h>

br_scalar gCamera_zoom = 0.2f;
br_angle gCamera_yaw;
br_vector3 gView_direction = { { 0.0, 0.0, -1.0 } };
int gCamera_sign;
int gCar_flying;
int gCamera_reset;
tCar_spec* gCar_to_view = &gProgram_state.current_car;
br_actor* gCamera_list[2];
tCar_spec* gActive_car_list[25];
int gNum_active_cars;
float gRecovery_cost[3];
br_scalar gCamera_height;
br_scalar gMin_camera_car_distance;
