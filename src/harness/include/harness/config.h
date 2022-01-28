#ifndef HARNESS_CONFIG_H
#define HARNESS_CONFIG_H

typedef enum tHarness_game_type {
    eGame_none,
    eGame_carmageddon,
    eGame_splatpack,
    eGame_carmageddon_demo
} tHarness_game_type;

typedef struct tHarness_game_info {
    tHarness_game_type mode;
    struct {
        // different between carmageddon and splatpack
        char* INTRO_SMK_FILE;
        // different between demo and full game
        char* GERMAN_LOADSCRN;
    } defines;
} tHarness_game_info;

typedef struct tHarness_game_config {
    int disable_cd_check;
    float physics_step_time;
    float fps;
    int freeze_timer;
} tHarness_game_config;

extern tHarness_game_info harness_game_info;
extern tHarness_game_config harness_game_config;

#endif