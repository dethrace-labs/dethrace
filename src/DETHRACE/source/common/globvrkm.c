#include "globvrkm.h"

br_actor *gCamera_list[2];
tCar_spec *gActive_car_list[25];
int gNum_active_cars;
tCar_spec *gCar_to_view;
float gRecovery_cost[3];
br_scalar gCamera_zoom;
br_scalar gCamera_height;
br_vector3 gView_direction;
br_scalar gMin_camera_car_distance;
int gCar_flying;
int gCamera_sign;
int gCamera_reset;
br_angle gCamera_yaw;

