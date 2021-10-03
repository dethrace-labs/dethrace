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
    char* intro_smk_file;
} tHarness_game_info;

extern tHarness_game_info harness_game_info;
extern int harness_disable_cd_check;

#endif