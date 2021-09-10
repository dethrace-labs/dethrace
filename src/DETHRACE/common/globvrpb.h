#ifndef _GLOBVRPB_H_
#define _GLOBVRPB_H_

#include "br_types.h"
#include "dr_types.h"



extern tNet_mode gNet_mode;
extern tNet_game_player_info gNet_players[6];
extern br_matrix34 gRoot_to_camera;
extern tCar_detail_info gCar_details[60];
extern int gThis_net_player_index;
extern br_scalar gPedestrian_distance_squared;
extern int gPending_race;
extern tPlayer_ID gLocal_net_ID;
extern int gNumber_of_net_players;
extern int gStart_race_sent;
extern int gSynch_race_start;
extern tNet_game_details *gCurrent_net_game;
extern int gReceived_car_details;
extern int gWaiting_for_unpause;
extern tNet_game_options *gNet_options;
extern br_vector3 gCamera_direction;
extern int gNetwork_available;
extern int gPedestrian_image;
extern int gHighest_pedestrian_value;
extern int gNeed_to_send_start_race;
extern int gRendering_mirror;

#endif
