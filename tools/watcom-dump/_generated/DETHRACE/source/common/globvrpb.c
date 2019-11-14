#include "globvrpb.h"

tNet_game_player_info gNet_players[6];
br_matrix34 gRoot_to_camera;
tCar_detail_info gCar_details[60];
int gThis_net_player_index;
br_scalar gPedestrian_distance_squared;
int gPending_race;
tPlayer_ID gLocal_net_ID;
tNet_mode gNet_mode;
int gNumber_of_net_players;
int gStart_race_sent;
int gSynch_race_start;
tNet_game_details *gCurrent_net_game;
int gReceived_car_details;
int gWaiting_for_unpause;
tNet_game_options *gNet_options;
br_vector3 gCamera_direction;
int gNetwork_available;
int gPedestrian_image;
int gHighest_pedestrian_value;
int gNeed_to_send_start_race;
int gRendering_mirror;

