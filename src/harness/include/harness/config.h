#ifndef HARNESS_CONFIG_H
#define HARNESS_CONFIG_H

typedef enum tHarness_game_type {
    eGame_none,
    eGame_carmageddon,
    eGame_splatpack,
    eGame_carmageddon_demo,
    eGame_splatpack_demo,
    eGame_splatpack_xmas_demo,
} tHarness_game_type;

typedef enum {
    eGameLocalization_none,
    eGameLocalization_german,
} tHarness_game_localization;

typedef struct tHarness_game_info {
    tHarness_game_type mode;
    tHarness_game_localization localization;
    struct {
        // different between carmageddon and splatpack
        char* INTRO_SMK_FILE;
        // different between demo and full game
        char* GERMAN_LOADSCRN;
        // some versions have an ascii table built-in, others provide it through KEYBOARD.COK
        int requires_ascii_table;
        // built-in keyboard look-up table for certain localized Carmageddon releases
        int* ascii_table;
        // built-in shifted keyboard look-up table for certain localized Carmageddon releases
        int* ascii_shift_table;
    } defines;
} tHarness_game_info;

typedef struct tHarness_game_config {
    int enable_cd_check;
    float physics_step_time;
    float fps;
    int freeze_timer;
    unsigned demo_timeout;
    int enable_diagnostics;
    float volume_multiplier;
    int start_full_screen;
    int enable_replay;
    int dos_mode;

    int install_signalhandler;
} tHarness_game_config;

extern tHarness_game_info harness_game_info;
extern tHarness_game_config harness_game_config;

#endif
