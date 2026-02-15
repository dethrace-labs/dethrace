#include "globvrpb.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x0050dd94
tNet_mode gNet_mode;

// GLOBAL: CARM95 0x00550fa0
tNet_game_player_info gNet_players[6];

br_matrix34 gRoot_to_camera;

// GLOBAL: CARM95 0x00550af0
tCar_detail_info gCar_details[MAXIMUM_NUMBER_OF_CARS];

// GLOBAL: CARM95 0x00550ae0
int gThis_net_player_index;

br_scalar gPedestrian_distance_squared;

// GLOBAL: CARM95 0x00551434
int gPending_race;

// GLOBAL: CARM95 0x00550aec
tPlayer_ID gLocal_net_ID;

// GLOBAL: CARM95 0x00550ae4
int gNumber_of_net_players;

// GLOBAL: CARM95 0x00550ad8
int gStart_race_sent;

// GLOBAL: CARM95 0x00551424
int gSynch_race_start;

// GLOBAL: CARM95 0x00550adc
tNet_game_details* gCurrent_net_game;

// GLOBAL: CARM95 0x00550ad4
int gReceived_car_details;

// GLOBAL: CARM95 0x00550ae8
int gWaiting_for_unpause;

// GLOBAL: CARM95 0x00551428
tNet_game_options* gNet_options;

// GLOBAL: CARM95 0x00550ac0
br_vector3 gCamera_direction;

int gNetwork_available;

int gPedestrian_image;

int gHighest_pedestrian_value;

// GLOBAL: CARM95 0x00551430
int gNeed_to_send_start_race;

// GLOBAL: CARM95 0x00550ad0
int gRendering_mirror;
