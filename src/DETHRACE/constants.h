#ifndef CONSTANTS_H
#define CONSTANTS_H

// These names are not part of the symbol dump, so the names in this file are not original, except as noted.

// Names derived from DATA/64X48X8/HEADUP.TXT (and context)
typedef enum dr_headup_slot {
    eHeadupSlot_development = 0,
    eHeadupSlot_credits = 1,
    eHeadupSlot_ped_kills = 2,
    eHeadupSlot_ped_warning = 3,
    eHeadupSlot_misc = 4,
    eHeadupSlot_countdown = 5,
    eHeadupSlot_fancies = 6,
    eHeadupSlot_timer = 7,
    eHeadupSlot_lap_count = 8,
    eHeadupSlot_race_bonus = 9,
    eHeadupSlot_time_bonus = 10,
    eHeadupSlot_time_award = 11,
    eHeadupSlot_cars_out_count = 12,
    eHeadupSlot_cash_network = 13,
    eHeadupSlot_ped_network = 14,
} dr_headup_slot;

// Names from `gMem_names`. See also: `CreateStainlessClasses`
typedef enum dr_memory_classes {
    kMem_intf_pix_copy = 129,                                            //  0x81
    kMem_intf_pal_copy = 130,                                            //  0x82
    kMem_nodes_array = 131,                                              //  0x83
    kMem_sections_array = 132,                                           //  0x84
    kMem_key_names = 133,                                                //  0x85
    kMem_columns_z = 134,                                                //  0x86
    kMem_columns_x = 135,                                                //  0x87
    kMem_non_car_list = 136,                                             //  0x88
    kMem_simp_level = 137,                                               //  0x89
    kMem_crush_data = 138,                                               //  0x8a
    kMem_crush_neighbours = 139,                                         //  0x8b
    kMem_temp_fs = 140,                                                  //  0x8c
    kMem_error_pix_copy = 141,                                           //  0x8d
    kMem_error_pal_copy = 142,                                           //  0x8e
    kMem_flic_pal = 143,                                                 //  0x8f
    kMem_flic_data = 144,                                                //  0x90
    kMem_flic_data_2 = 145,                                              //  0x91
    kMem_queued_flic = 146,                                              //  0x92
    kFlic_panel_pixels = 147,                                            //  0x93
    kMem_translations = 148,                                             //  0x94
    kMem_translations_text = 149,                                        //  0x95
    kMem_cur_pal_pixels = 150,                                           //  0x96
    kMem_render_pal_pixels = 151,                                        //  0x97
    kMem_scratch_pal_pixels = 152,                                       //  0x98
    kMem_shade_table_copy = 153,                                         //  0x99
    kMem_rear_screen_pixels = 154,                                       //  0x9a
    kMem_rolling_letters = 155,                                          //  0x9b
    kMem_intf_copy_areas = 156,                                          //  0x9c
    kMem_strip_image = 157,                                              //  0x9d
    kMem_strip_image_perm = 158,                                         //  0x9e
    kMem_damage_clauses = 159,                                           //  0x9f
    kMem_undamaged_vertices = 160,                                       //  0xa0
    kMem_race_text_chunk = 161,                                          //  0xa1
    kMem_race_text_str = 162,                                            //  0xa2
    kMem_oppo_array = 163,                                               //  0xa3
    kMem_oppo_text_chunk = 164,                                          //  0xa4
    kMem_oppo_text_str = 165,                                            //  0xa5
    kMem_br_font = 166,                                                  //  0xa6
    kMem_br_font_wid = 167,                                              //  0xa7
    kMem_br_font_enc = 168,                                              //  0xa8
    kMem_br_font_glyphs = 169,                                           //  0xa9
    kMem_oppo_car_spec = 170,                                            //  0xaa
    kMem_misc_string = 171,                                              //  0xab
    kMem_mac_host_buffer_1 = 172,                                        //  0xac
    kMem_mac_host_buffer_2 = 173,                                        //  0xad
    kMem_mac_net_details = 174,                                          //  0xae
    kMem_back_pixels = 175,                                              //  0xaf
    kMem_quit_vfy_pixels = 176,                                          //  0xb0
    kMem_quit_vfy_pal = 177,                                             //  0xb1
    kMem_net_min_messages = 178,                                         //  0xb2
    kMem_net_mid_messages = 179,                                         //  0xb3
    kMem_net_max_messages = 180,                                         //  0xb4
    kMem_net_pid_details = 181,                                          //  0xb5
    kMem_net_car_spec = 182,                                             //  0xb6
    kMem_dynamic_message = 183,                                          //  0xb7
    kMem_player_list_join = 184,                                         //  0xb8
    kMem_player_list_leave = 185,                                        //  0xb9
    kMem_oppo_new_nodes = 186,                                           //  0xba
    kMem_oppo_new_sections = 187,                                        //  0xbb
    kMem_cop_car_spec = 188,                                             //  0xbc
    kMem_oppo_bit_per_node = 189,                                        //  0xbd
    kMem_ped_action_list = 190,                                          //  0xbe
    kMem_ped_sequences = 191,                                            //  0xbf
    kMem_ped_array_stain = 192,                                          //  0xc0
    kMem_ped_array = 193,                                                //  0xc1
    kMem_ped_instructions = 194,                                         //  0xc2
    kMem_ped_new_instruc = 195,                                          //  0xc3
    kMem_pipe_model_geometry = 196,                                      //  0xc4
    kMem_powerup_array = 197,                                            //  0xc5
    kMem_powerup_float_parms = 198,                                      //  0xc6
    kMem_powerup_int_parms = 199,                                        //  0xc7
    kMem_drugs_palette = 200,                                            //  0xc8
    kMem_pratcam_flic_array = 201,                                       //  0xc9
    kMem_pratcam_flic_data = 202,                                        //  0xca
    kMem_pratcam_sequence_array = 203,                                   //  0xcb
    kMem_pratcam_pixelmap = 204,                                         //  0xcc
    kMem_video_pixels = 205,                                             //  0xcd
    kMem_funk_prox_array = 206,                                          //  0xce
    kMem_new_mat_id = 207,                                               //  0xcf
    kMem_new_mat_id_2 = 208,                                             //  0xd0
    kMem_new_mat_id_3 = 209,                                             //  0xd1
    kMem_special_volume = 210,                                           //  0xd2
    kMem_special_screen = 211,                                           //  0xd3
    kMem_new_special_vol = 212,                                          //  0xd4
    kMem_saved_game = 213,                                               //  0xd5
    kMem_new_save_game = 214,                                            //  0xd6
    kMem_stor_space_pix = 215,                                           //  0xd7
    kMem_stor_space_tab = 216,                                           //  0xd8
    kMem_stor_space_mat = 217,                                           //  0xd9
    kMem_stor_space_mod = 218,                                           //  0xda
    kMem_stor_space_save = 219,                                          //  0xdb
    kMem_funk_spec = 220,                                                //  0xdc
    kMem_groove_spec = 221,                                              //  0xdd
    kMem_non_car_spec = 222,                                             //  0xde
    kMem_S3_scan_name = 223,                                             //  0xdf
    kMem_S3_sound_header = 224,                                          //  0xe0
    kMem_S3_sample = 225,                                                //  0xe1
    kMem_S3_mac_channel = 226,                                           //  0xe2
    kMem_S3_mac_path = 227,                                              //  0xe3
    kMem_S3_sentinel = 228,                                              //  0xe4
    kMem_S3_outlet = 229,                                                //  0xe5
    kMem_S3_channel = 230,                                               //  0xe6
    kMem_S3_descriptor = 231,                                            //  0xe7
    kMem_S3_reverse_buffer = 232,                                        //  0xe8
    kMem_S3_source = 233,                                                //  0xe9
    kMem_S3_DOS_SOS_channel = 234,                                       //  0xea
    kMem_S3_PC_DOS_path = 235,                                           //  0xeb
    kMem_S3_DOS_SOS_patch = 236,                                         //  0xec
    kMem_S3_DOS_SOS_song_structure = 237,                                //  0xed
    kMem_S3_DOS_SOS_song_data = 238,                                     //  0xee
    kMem_S3_Windows_95_load_WAV_file = 239,                              //  0xef
    kMem_S3_Windows_95_create_temp_buffer_space_to_reverse_sample = 240, //  0xf0
    kMem_S3_Windows_95_path = 241,                                       //  0xf1
    kMem_DOS_HMI_file_open = 242,                                        //  0xf2
    kMem_abuse_text = 243,                                               //  0xf3
    kMem_action_replay_buffer = 244,                                     //  0xf4
    kMem_misc = 245                                                      //  0xf5
} dr_memory_classes;

typedef enum keycodes {
    KEY_SHIFT_ANY = 0,    //  0x0
    KEY_ALT_ANY = 1,      //  0x1
    KEY_CTRL_ANY = 2,     //  0x2
    KEY_CTRL_ANY_2 = 3,   //  0x3
    KEY_CAPSLOCK = 4,     //  0x4
    KEY_RSHIFT = 5,       //  0x5
    KEY_RALT = 6,         //  0x6
    KEY_RCTRL = 7,        //  0x7
    KEY_LSHIFT = 8,       //  0x8
    KEY_LALT = 9,         //  0x9
    KEY_LCTRL = 10,       //  0xa
    KEY_0 = 11,           //  0xb
    KEY_1 = 12,           //  0xc
    KEY_2 = 13,           //  0xd
    KEY_3 = 14,           //  0xe
    KEY_4 = 15,           //  0xf
    KEY_5 = 16,           //  0x10
    KEY_6 = 17,           //  0x11
    KEY_7 = 18,           //  0x12
    KEY_8 = 19,           //  0x13
    KEY_9 = 20,           //  0x14
    KEY_A = 21,           //  0x15
    KEY_B = 22,           //  0x16
    KEY_C = 23,           //  0x17
    KEY_D = 24,           //  0x18
    KEY_E = 25,           //  0x19
    KEY_F = 26,           //  0x1a
    KEY_G = 27,           //  0x1b
    KEY_H = 28,           //  0x1c
    KEY_I = 29,           //  0x1d
    KEY_J = 30,           //  0x1e
    KEY_K = 31,           //  0x1f
    KEY_L = 32,           //  0x20
    KEY_M = 33,           //  0x21
    KEY_N = 34,           //  0x22
    KEY_O = 35,           //  0x23
    KEY_P = 36,           //  0x24
    KEY_Q = 37,           //  0x25
    KEY_R = 38,           //  0x26
    KEY_S = 39,           //  0x27
    KEY_T = 40,           //  0x28
    KEY_U = 41,           //  0x29
    KEY_V = 42,           //  0x2a
    KEY_W = 43,           //  0x2b
    KEY_X = 44,           //  0x2c
    KEY_Y = 45,           //  0x2d
    KEY_Z = 46,           //  0x2e
    KEY_GRAVE = 47,       //  0x2f
    KEY_MINUS = 48,       //  0x30
    KEY_EQUALS = 49,      //  0x31
    KEY_BACKSPACE = 50,   //  0x32
    KEY_RETURN = 51,      //  0x33
    KEY_KP_ENTER = 52,    //  0x34
    KEY_TAB = 53,         //  0x35
    KEY_SLASH = 54,       //  0x36
    KEY_UNKNOWN_55 = 55,  //  0x37
    KEY_SEMICOLON = 56,   //  0x38
    KEY_APOSTROPHE = 57,  //  0x39
    KEY_PERIOD = 58,      //  0x3a
    KEY_COMMA = 59,       //  0x3b
    KEY_LBRACKET = 60,    //  0x3c
    KEY_RBRACKET = 61,    //  0x3d
    KEY_BACKSLASH = 62,   //  0x3e
    KEY_ESCAPE = 63,      //  0x3f
    KEY_INSERT = 64,      //  0x40
    KEY_DELETE = 65,      //  0x41
    KEY_HOME = 66,        //  0x42
    KEY_END = 67,         //  0x43
    KEY_PAGEUP = 68,      //  0x44
    KEY_PAGEDOWN = 69,    //  0x45
    KEY_LEFT = 70,        //  0x46
    KEY_RIGHT = 71,       //  0x47
    KEY_UP = 72,          //  0x48
    KEY_DOWN = 73,        //  0x49
    KEY_KP_NUMLOCK = 74,  //  0x4a
    KEY_KP_DIVIDE = 75,   //  0x4b
    KEY_KP_MULTIPLY = 76, //  0x4c
    KEY_KP_MINUS = 77,    //  0x4d
    KEY_KP_PLUS = 78,     //  0x4e
    KEY_KP_PERIOD = 79,   //  0x4f
    KEY_KP_EQUALS = 80,   //  0x50
    KEY_KP_0 = 81,        //  0x51
    KEY_KP_1 = 82,        //  0x52
    KEY_KP_2 = 83,        //  0x53
    KEY_KP_3 = 84,        //  0x54
    KEY_KP_4 = 85,        //  0x55
    KEY_KP_5 = 86,        //  0x56
    KEY_KP_6 = 87,        //  0x57
    KEY_KP_7 = 88,        //  0x58
    KEY_KP_8 = 89,        //  0x59
    KEY_KP_9 = 90,        //  0x5a
    KEY_F1 = 91,          //  0x5b
    KEY_F2 = 92,          //  0x5c
    KEY_F3 = 93,          //  0x5d
    KEY_F4 = 94,          //  0x5e
    KEY_F5 = 95,          //  0x5f
    KEY_F6 = 96,          //  0x60
    KEY_F7 = 97,          //  0x61
    KEY_F8 = 98,          //  0x62
    KEY_F9 = 99,          //  0x63
    KEY_F10 = 100,        //  0x64
    KEY_F11 = 101,        //  0x65
    KEY_F12 = 102,        //  0x66
    KEY_PRTSCN = 103,     //  0x67
    KEY_SCRLK = 104,      //  0x68
    KEY_PAUSE = 105,      //  0x69
    KEY_SPACE = 106,      //  0x6a
} keycodes;

typedef enum keymapcodes {
    KEYMAP_ESCAPE = 0, // default: ESC

    KEYMAP_CONTROL_ANY = 7, // default: LCTRL or RCTRL (any)
    KEYMAP_SHIFT_ANY = 8,   // default: LSHIFT or RSHIFT (any)

    //
    // The following items are meant to be combined with KEYMAP_CONTROL_ANY
    //

    KEYMAP_CTRL_QUIT = 1,           // default: Q
    KEYMAP_CTRL_SAVE = 2,           // default: S
    KEYMAP_CTRL_LOAD = 3,           // default: L
    KEYMAP_CTRL_ABORT_RACE = 4,     // default: A
    KEYMAP_CTRL_DRAW_SOME_TEXT = 9, // default: KP0
    KEYMAP_CTRL_FLY = 10,           // default: KP1

    //
    // The following items are meant to be combined with KEYMAP_SHIFT_ANY
    //

    KEYMAP_SHIFT_DUMPSCREEN = 5, // default: D

    //
    // The following items are meant to be used standalone
    //

    KEYMAP_COCKPIT = 45,        // default C
    KEYMAP_SAVE = 28,           // default F2
    KEYMAP_LOAD = 29,           // default F3
    KEYMAP_MOVE_UP = 30,        // default UP
    KEYMAP_MOVE_DOWN = 31,      // default DOWN
    KEYMAP_MOVE_LEFT = 32,      // default LEFT
    KEYMAP_MOVE_RIGHT = 33,     // default RIGHT
    KEYMAP_MIRROR = 34,         // default M
    KEYMAP_PRATCAM = 35,        // default P
    KEYMAP_LOOK_LEFT = 50,      // default Q
    KEYMAP_LOOK_FORWARD = 51,   // default W
    KEYMAP_LOOK_RIGHT = 52,     // default E
    KEYMAP_SOUND = 36,          // default S
    KEYMAP_REPAIR = 44,         // default BACKSPACE
    KEYMAP_PEDESTRIANS = 54,    // default KP_MINUS
    KEYMAP_F4 = 17,             // default F4
    KEYMAP_F5 = 18,             // default F5
    KEYMAP_F6 = 19,             // default F6
    KEYMAP_F7 = 20,             // default F7
    KEYMAP_F8 = 21,             // default F8
    KEYMAP_F10 = 22,            // default F10
    KEYMAP_F11 = 23,            // default F1
    KEYMAP_F12 = 24,            // default F12
    KEYMAP_EDITMODE_0 = 14,     // default 0
    KEYMAP_EDITMODE_1 = 37,     // default 1
    KEYMAP_EDITMODE_2 = 38,     // default 2
    KEYMAP_EDITMODE_3 = 39,     // default 3
    KEYMAP_EDITMODE_4 = 40,     // default 4
    KEYMAP_EDITMODE_5 = 41,     // default 5
    KEYMAP_EDITMODE_6 = 42,     // default 6
    KEYMAP_EDITMODE_7 = 43,     // default 7
    KEYMAP_EDITMODE_8 = 15,     // default 8
    KEYMAP_EDITMODE_9 = 16,     // default 9
    KEYMAP_SCREEN_SMALLER = 60, // default -
    KEYMAP_SCREEN_LARGER = 61,  // default =
    KEYMAP_MAP = 56,            // default TAB
    KEYMAP_RECOVERY = 59,       // default Insert
    KEYMAP_BUY_ARMOUR = 62,     // default Delete
    KEYMAP_BUY_POWER = 63,      // default End
    KEYMAP_BUY_OFFENSE = 64,    // default PgDn
    KEYMAP_VIEW_NETPLAYER = 65, // default Space
    KEYMAP_SEND_MESSAGE = 66,   // default `
    KEYMAP_ARROW = 25,          // default R
    KEYMAP_INFO = 26,           // default I
    KEYMAP_REPLAYMODE = 57,     // default KP_ENTER
} keymapcodes;

enum {
    kFancyHeadupSplatterBonus = 0,
    kFancyHeadupPileDriverBonus = 1,
    kFancyHeadupExtraStyleBonus = 2,
    kFancyHeadupBonusForArtisticImpression = 3,
    kFancyHeadup2xComboBonus = 4,
    kFancyHeadup3xComboBonus = 5,
    kFancyHeadup4xComboBonus = 6,
    kFancyHeadup5xComboBonus = 7,
    kFancyHeadupNiceShotSir = 8,
    kFancyHeadupCunningStuntBonus = 9,
    kFancyHeadupHeadOnBonus = 10,
    kFancyHeadupYouWastedEm = 11,
    kFancyHeadupCheckpoint = 12,
    kFancyHeadupOutOfTime = 13,
    kFancyHeadupRaceCompleted = 14,
    kFancyHeadupYouAreWasted = 15,
    kFancyHeadupYouAreBothWasted = 16,
    kFancyHeadupNetworkRaceOverNetworkLoss = 17,
    kFancyHeadupNetworkRaceNoMoreMoney = 18,
    kFancyHeadupYouLost = 19,
    kFancyHeadupNetworkVictory = 20,
    kFancyHeadupDemoTimeout = 21,
};

enum {
    kFatalError_UnsupportedScreenDepth = 0,
    kFatalError_AllocateOffScreenBuffer = 1,
    kFatalError_AllocateZBuffer = 2,
    kFatalError_AllocateRootActor = 3,
    kFatalError_ApplicationPathname = 4,
    kFatalError_AllocateCamera = 5,
    kFatalError_AllocateSelf = 6,
    kFatalError_PreferencesFile = 7,
    kFatalError_SettingsFile = 8,
    kFatalError_OpenKeyMapFile = 9,
    kFatalError_RequiredPalette = 10,
    kFatalError_AllocateInstallStandardLamp = 11,
    kFatalError_AllocateInstallStupiedTestBastard = 12,
    kFatalError_LoadFlicFile_S = 13,
    kFatalError_AllocateFlicFile_S = 14,
    kFatalError_FlicFileNot8Bits_S = 15,
    kFatalError_FlicFileNoFrameRate_S = 16,
    kFatalError_FreeFlics_S = 17,
    kFatalError_FindSpareTransientBitmap = 18,
    kFatalError_SaveFileTooBig = 19,
    kFatalError_LoadFontImage_S = 20,
    kFatalError_LoadFontWidthTable_S = 21,
    kFatalError_LoadCursorImage = 22,
    kFatalError_LoadCursorGiblet = 23,
    kFatalError_ScreenGrafDataWidthHeightMismatch = 24,
    kFatalError_LoadResolutionIndependentFile = 25,
    kFatalError_LoadCockpitImage = 26,
    kFatalError_LoadDamageImage = 27,
    kFatalError_OpenPratCamTextFile = 28,
    kFatalError_TooFewFlicSpecsInPratCamFile = 29,
    kFatalError_OpenPratCamFlicFile_S = 30,
    kFatalError_AllocateMemoryPratCamFlicFile = 31,
    kFatalError_TooManyFlicSpecsInPratCamTextFIle = 32,
    kFatalError_TooFewSequencesInPratCamTextFile = 33,
    kFatalError_TooManySequencesInPratCamTextFile = 34,
    kFatalError_LoadSpeedoImage = 35,
    kFatalError_LoadTachoImage = 36,
    kFatalError_PratCamSequenceTooManyChunks_S = 37,
    kFatalError_PratCamSequenceTooManyAlternatives_S = 38,
    kFatalError_PratCamSequenceTooManySounds_S = 39,
    kFatalError_LoadHandsImage = 40,
    kFatalError_FlicPaletteDisposeBeforeAllocation = 41,
    kFatalError_LoadDialsPix = 42,
    kFatalError_FreeNULLPointer = 43,
    kFatalError_FreeInvalidPointer_S = 44,
    kFatalError_FreeAlreadyFreedBlock_S = 45,
    kFatalError_LockInvalidPointer_S = 46,
    kFatalError_UnlockInvalidPointer_S = 47,
    kFatalError_OOMHeapSpace = 48,
    kFatalError_OOMBlockSlots = 49,
    kFatalError_OpenRacesFile = 50,
    kFatalError_LoadRaceSceneImage = 51,
    kFatalError_LoadRaceMapImage = 52,
    kFatalError_loadRaceInfoImage = 53,
    kFatalError_OpenOpponentsFile = 54,
    kFatalError_OpponentCountMismatch = 55,
    kFatalError_LoadOpponentMugShotFile = 56,
    kFatalError_LoadGridImageFile = 57,
    kFatalError_LoadPartImageFile = 58,
    kFatalError_LoadChromeFontFIle = 59,
    kFatalError_LoadFunkotronicFile = 60,
    kFatalError_LoadGroovadelicFile = 61,
    kFatalError_FunkotronicFile = 62,
    kFatalError_GroovidelicFile = 63,
    kFatalError_FindMaterialUsedByFunkotronicFile_S = 64,
    kFatalError_FindActorUsedByGroovidelicFile_S = 65,
    kFatalError_AnimationFramePixelmapUsedByFunkotronicFile = 66,
    kFatalError_InsufficientPixelmapSlots = 67,
    kFatalError_InsufficientShadeTableSlots = 68,
    kFatalError_InsufficientMaterialSlots = 69,
    kFatalError_InsufficientModelSlots = 70,
    kFatalError_LoadCarActor = 71,
    kFatalError_DefinedRefNumGrooveFunkOutOfRange = 72,
    kFatalError_UsedRefNumGrooveFunkOutOfRange = 73,
    kFatalError_MAMSHeapCorrupt = 74,
    kFatalError_OpenPedestrianFile = 75,
    kFatalError_FindPedestrianRefNum_S = 76,
    kFatalError_FindPedestrianMaterial_S = 77,
    kFatalError_FindPedestrianPixelmap_S = 78,
    kFatalError_LoadPixelmapFile_S = 79,
    kFatalError_LoadShadeTableFile_S = 80,
    kFatalError_LoadMaterialFile_S = 81,
    kFatalError_LoadModelFile_S = 82,
    kFatalError_PedSeqTooManyFrames = 83,
    kFatalError_PedSeqTooManyChoices = 84,
    kFatalError_PedSeqTooManyBearings = 85,
    kFatalError_PedInstructionMarkerNotFound = 86,
    kFatalError_LoadDepthCueShadeTable = 87,
    kFatalError_UnrecognizedOpponentPathType_S = 88,
    kFatalError_FindSkyMaterial_S = 89,
    kFatalError_FindSkyPixelmap_S = 90,
    kFatalError_UnknownDamageType_S = 91,
    kFatalError_ConditionalCarDamageFileFormat = 92,
    kFatalError_TooManyExtraPointsForCar_S = 93,
    kFatalError_OOMCarmageddon_S = 94,
    kFatalError_PipeBufferPrematureEnd = 95,
    kFatalError_PipeBufferPrematureBegin = 96,
    kFatalError_PipedPedestrian = 97,
    kFatalError_PipingSystem = 98,
    kFatalError_OpenTextTxt = 99,
    kFatalError_OpenDareTxt = 100,
    kFatalError_FindFlicUsedInTranslationFile_S = 101,
    kFatalError_OpenHeadupsFile = 102,
    kFatalError_OpenPartsshopFile = 103,
    kFatalError_OpenResolutionDependentFile = 104,
    kFatalError_NotInReceivedPlayerList = 105,
    kFatalError_OpenKeyNamesFile = 106,
    kFatalError_Open_S = 107,
    kFatalError_SpecialVolumeTooManyPlanes_S = 108,
    kFatalError_LoadGeneratedShadeTable = 109,
    kFatalError_NetworkCodeSelfCheck = 110,
    kFatalError_NetworkMessageCorrupt = 111,
    kFatalError_MemoryCorrupt = 112,
    kFatalError_NoFunkGrooveSlotBunchesLeft = 113,
    kFatalError_NetContentsTooBig_S = 114,
    kFatalError_FileCorrupt_S = 115,
    kFatalError_RandomNumberOutOfRange_S = 116,
#ifdef DETHRACE_3DFX_PATCH
    kFatalError_CouldntLockPixelmap_S = 117,
    kFatalError_ShouldBeLockedButIsnt_S = 118,
    kFatalError_CannotPurifyPixelmap_S = 119,
    kFatalError_FileMustStartWith_S = 120,
    kFatalError_CantCopeWithVersionFor_S = 121,
    kFatalError_CannotTilePixelmap_S = 122,
    kFatalError_Mysterious_S_S = 123,
    kFatalError_CanOnlyDimRectanglesOfgBack_screen = 124,
    kFatalError_InvalidMaterialAlpha = 125
#endif
};

enum {
    kMiscString_FIXED_THAT_YOU_TWISTED_BASTARD = -1,
    kMiscString_Invulnerable = 0,
    kMiscString_TimerFrozen = 1,
    kMiscString_MirrorOn = 2,
    kMiscString_MirrorOff = 3,
    kMiscString_RepairingForFree = 4,
    kMiscString_RepairCostColon = 5,
    kMiscString_RepairObstructed = 6,
    kMiscString_RecoveryCostColon = 7,
    kMiscString_OpponentsVisibleOnMap = 8,
    kMiscString_OpponentsNotVisibleOnMap = 9,
    kMiscString_EveryOpponentWasted = 10,
    kMiscString_RunningDemoGame = 11,
    kMiscString_Credits = 12,
    kMiscString_Credit = 13,
    kMiscString_Lost = 14,
    kMiscString_Empty = 15,
    kMiscString_PROFIT = 16,
    kMiscString_LOSS = 17,
    kMiscString_KILLS = 18,
    kMiscString_WASTED_19 = 19,
    kMiscString_LAP = 20,
    kMiscString_CP = 21,
    kMiscString_CompletionBonusColon = 22,
    kMiscString_TimeBonus = 23,
    kMiscString_LetTheCarnageContinue = 24,
    kMiscString_NiceAndFluffyMode = 25,
    kMiscString_EveryPedestrianWasted = 26,
    kMiscString_YourCurrentRankIs = 27,
    kMiscString_RetailColon = 28,
    kMiscString_YouPayColon = 29,
    kMiscString_RefundColon = 30,
    kMiscString_CreditsColon = 31,
    kMiscString_YoursAlready = 32,
    kMiscString_YouGotIt = 33,
    kMiscString_NoOtherCarsToChooseFromYet = 34,
    kMiscString_OriginalDriverWas = 35,
    kMiscString_YourOriginalCar = 36,
    kMiscString_ACTION_REPLAY_UNAVAILABLE = 37,
    kMiscString_StandardCamera = 38,
    kMiscString_PanningCamera = 39,
    kMiscString_ActionTrackingCamera = 40,
    kMiscString_InstantRepair = 41,
    kMiscString_FinalLap = 42,
    kMiscString_WrongCheckpoint = 43,
    kMiscString_BadBoy = 44,
    kMiscString_CreditFine = 45,
    kMiscString_WASTED_46 = 46,
    kMiscString_PRFT = 47,
    kMiscString_RecoveringForFree = 48,
    kMiscString_BadGirl = 49,
    kMiscString_NoCarTextures = 50,
    kMiscString_TransparentCarTexturesOnly = 51,
    kMiscString_FullCarTextures = 52,
    kMiscString_NoRoadTextures = 53,
    kMiscString_RoadTextures = 54,
    kMiscString_NoWallTextures = 55,
    kMiscString_LinearWallTextures = 56,
    kMiscString_BestWallTextures = 57,
    kMiscString_HasJoinedTheGame = 58,
    kMiscString_NetworkGameTypeNames_START = 59,
    kMiscString_DRIVEN_TO_DESTRUCTION = 59,
    kMiscString_CAR_CRUSHER = 60,
    kMiscString_CARNAGE_ACCUMULATOR = 61,
    kMiscString_CHECKPOINT_STAMPEDE = 62,
    kMiscString_SUDDEN_DEATH = 63,
    kMiscString_TERMINAL_TAG = 64,
    kMiscString_FOX_N_HOUNDS = 65,
    kMiscString_UNUSED_66 = 66,
    kMiscString_UNUSED_67 = 67,
    kMiscString_NetworkGameStage_START = 68,
    kMiscString_STARTING = 68,
    kMiscString_READY_69 = 69,
    kMiscString_PLAYING = 70,
    kMiscString_NetworkGameOpenGame_START = 71,
    kMiscString_CLOSED = 71,
    kMiscString_OPEN = 72,
    kMiscString_GAME_TERMINATED_BY_HOST = 73,
    kMiscString_HasLeftTheGame = 74,
    kMiscString_CannotSaveGameInNetworkPlay = 75,
    kMiscString_NetworkPlayIsNotAvailable = 76,
    kMiscString_NetworkPlayerStatus_START = 77,
    kMiscString_NetworkPlayerStatus_UNKNOWN = 77,
    kMiscString_NetworkPlayerStatus_READY = 78,
    kMiscString_NetworkPlayerStatus_LOADING = 79,
    kMiscString_IN_WRECKS_GALLERY = 80,
    kMiscString_IN_RACE_SUMMARY = 81,
    kMiscString_NOT_RESPONDING = 82,
    kMiscString_RACING = 83,
    kMiscString_IN_MAIN_MENU = 84,
    kMiscString_RECOVERING = 85,
    kMiscString_IN_ACTION_REPLAY = 86,
    kMiscString_HOST_ABANDONED_RACE = 87,
    kMiscString_HOST = 88,
    kMiscString_SORRY_YOU_RE_TOO_LATE = 89,
    kMiscString_IS_NO_LONGER_RESPONDING = 90,
    kMiscString_PANIC_HOST_HAS_DISAPPEARED = 91,
    kMiscString_IN = 92,
    kMiscString_OUT = 93,
    kMiscString_CREDITS = 94,
    kMiscString_CANNOT_AFFORD_TO_REPAIR = 95,
    kMiscString_CANNOT_AFFORD_TO_RECOVER = 96,
    kMiscString_RECOVERY_IN = 97,
    kMiscString_SECOND = 98,
    kMiscString_SECONDS = 99,
    kMiscString_TrackAppearsVeryQuickly = 100,
    kMiscString_TrackAppearsQuiteQuickly = 101,
    kMiscString_TrackQppearsQuiteLate = 102,
    kMiscString_TrackAppearsVeryLate = 103,
    kMiscString_NoShadows = 104,
    kMiscString_ShadowUnderOwnCar = 105,
    kMiscString_ShadowUnderMainCars = 106,
    kMiscString_ShadowUnderAllCars = 107,
    kMiscString_SkyTextureOn = 108,
    kMiscString_SkyTextureOff = 109,
    kMiscString_ThereIsNoSkyTextureForThisRace = 110,
    kMiscString_DepthCueingOn = 111,
    kMiscString_DepthCueingOff = 112,
    kMiscString_ThereIsNoDepthCueingForThisRace = 113,
    kMiscString_YonIncreasedTo_D = 114,
    kMiscString_YonDecreasedTo_D = 115,
    kMiscString_FewestSounds = 116,
    kMiscString_PartialSound = 117,
    kMiscString_AllSounds = 118,
    kMiscString_CarSimplificationLevel_D = 119,
    kMiscString_AccessoriesOn = 120,
    kMiscString_AccessoriesOff = 121,
    kMiscString_SmokeOn = 122,
    kMiscString_SmokeOff = 123,
    kMiscString_NetGamesAlwaysAccessorized = 124,
    kMiscString_RECOVERY_CANCELLED = 125,
    kMiscString_AND = 126,
    kMiscString_WASTED_EACH_OTHER = 127,
    kMiscString_RACE_CHANGED_DURING_LOADING = 128,
    kMiscString_WRONG_TEXT_129 = 129,
    kMiscString_WRONG_TEXT_130 = 130,
    kMiscString_WRONG_TEXT_131 = 131,
    kMiscString_WRONG_TEXT_132 = 132,
    kMiscString_WRONG_TEXT_133 = 133,
    kMiscString_WRONG_TEXT_134 = 134,
    kMiscString_WRONG_TEXT_135 = 135,
    kMiscString_WRONG_TEXT_136 = 136,
    kMiscString_WRONG_TEXT_137 = 137,
    kMiscString_WRONG_TEXT_138 = 138,
    kMiscString_WRONG_TEXT_139 = 139,
    kMiscString_WRONG_TEXT_140 = 140,
    kMiscString_WRONG_TEXT_141 = 141,
    kMiscString_WRONG_TEXT_142 = 142,
    kMiscString_WRONG_TEXT_143 = 143,
    kMiscString_WRONG_TEXT_144 = 144,
    kMiscString_WRONG_TEXT_145 = 145,
    kMiscString_WRONG_TEXT_146 = 146,
    kMiscString_WRONG_TEXT_147 = 147,
    kMiscString_WRONG_TEXT_148 = 148,
    kMiscString_WRONG_TEXT_149 = 149,
    kMiscString_InputOptions_START = 150,
    kMiscString_Accelerate = 150,
    kMiscString_Brake_Rev = 151,
    kMiscString_TurnLeft = 152,
    kMiscString_TurnRight = 153,
    kMiscString_Handbrake = 154,
    kMiscString_Repair = 155,
    kMiscString_Recover = 156,
    kMiscString_ActionReplay = 157,
    kMiscString_Wheelspin = 158,
    kMiscString_CockpitToggle = 159,
    kMiscString_LookLeft = 160,
    kMiscString_LookForward = 161,
    kMiscString_LookRight = 162,
    kMiscString_MapToggle = 163,
    kMiscString_BuyArmour = 164,
    kMiscString_BuyPower = 165,
    kMiscString_BuyOffense = 166,
    kMiscString_SendMessage = 167,
    kMiscString_WRONG_TEXT_168 = 168,
    kMiscString_WRONG_TEXT_169 = 169,
    kMiscString_CarAddedToChangeCarList = 170,
    kMiscString_WastedBy = 171,
    kMiscString_COMMITTED_SUICIDE = 172,
    kMiscString_WASTED_173 = 173,
    kMiscString_YOU_ARE_ALREADY_AT_MAX = 174,
    kMiscString_APOGained_START = 175,
    kMiscString_ARMOUR_STRENGTHENED = 175,
    kMiscString_ENGINE_POWER_INCREASED = 176,
    kMiscString_INCREASED_OFFENSIVE = 177,
    kMiscString_ONLY_AVAILABLE_IN_NET_GAMES = 178,
    kMiscString_CANNOT_AFFORD_IT = 179,
    kMiscString_TARGET_180 = 180,
    kMiscString_TARGET_181 = 181,
    kMiscString_TARGET_182 = 182,
    kMiscString_LIMIT = 183,
    kMiscString_IS_OUT = 184,
    kMiscString_IS_IT = 185,
    kMiscString_HA_HA_YOU_ARE_IT = 186,
    kMiscString_IS_NOW_THE_FOX = 187,
    kMiscString_YOU_ARE_THE_FOX = 188,
    kMiscString_IS_THE_WINNER = 189,
    kMiscString_UNAVAILABLE_IN_DEMO = 190,
    kMiscString_ENTER_NAME = 191,
    kMiscString_NOT_ENOUGH_MEMORY = 192,
    kMiscString_LOW_MEMORY_MODE_INVOKED = 193,
    kMiscString_NetworkGameTypeDescriptions_START = 194,
    kMiscString_DrivenToDestruction_Description = 194,
    kMiscString_CarCrusher_Description = 195,
    kMiscString_CarnageAccumulator_Description = 196,
    kMiscString_CheckpointStampede_Description = 197,
    kMiscString_SuddenDeath_Description = 198,
    kMiscString_TerminalTag_Description = 199,
    kMiscString_FoxAndHounds_Description = 200,
    kMiscString_FIRST_RACE = 201,
    kMiscString_RACES_WILL_CONTINUE_DOWN_THIS_LIST = 202,
    kMiscString_SUBSEQUENT_RACES_WILL_BE_RANDOM = 203,
    kMiscString_THIS_CAR_ALREADY_TAKEN_BY = 204,
    kMiscString_AVAILABLE = 205,
    kMiscString_NetworkGamesTableHeading_HOST = 206,
    kMiscString_NetworkGamesTableHeading_GAME_TYPE = 207,
    kMiscString_NetworkGamesTableHeading_PLAYERS = 208,
    kMiscString_NetworkGamesTableHeading_STATUS = 209,
    kMiscString_PLAYER = 210,
    kMiscString_PLAYED = 211,
    kMiscString_WON = 212,
    kMiscString_SCORE = 213,
    kMiscString_THE_FOX_CANNOT_DO_THAT = 214,
    kMiscString_ONLY_IT_CAN_DO_THAT = 215,
    kMiscString_COP = 216,
    kMiscString_WATCHING = 217,
    kMiscString_RACE_LEADER = 218,
    kMiscString_THAT_HALVED_YOUR_KILL_COUNT = 219,
    kMiscString_THAT_LOST_YOU_A_CHECKPOINT = 220,
    kMiscString_BACK_TO_THE_START = 221,
    kMiscString_THAT_HALVED_YOUR_TIME = 222,
    kMiscString_PLEASE_INSERT_THE_CARMAGEDDON_CD = 223,
    kMiscString_TRIED_TO_CHEAT = 224,
    kMiscString_MANAGED_TO_CHEAT = 225,
    kMiscString_MESSAGE_SENT = 226,
    kMiscString_ENTER_MESSAGE = 227,
    kMiscString_PLEASE_WAIT_HOST_HAS_PAUSED = 228,
    kMiscString_USE_SYSTEM_CONTROL_PANEL_FOR_JOYSTICKS = 229,
    kMiscString_CANNOT_CALIBRATE_JOYSTICK_DURING_RACE_IN_SVGA = 230,
    kMiscString_HIT_ESCAPE_TO_ABORT = 231,
    kMiscString_HIT_ESCAPE_TO_FINISH_JOYSTICK_SETTINGS_1_WILL_BE_RETAINED = 232,
    kMiscString_PLEASE_MOVE_JOYSTICK_1_TO_LEFT_AND_HIT_ANY_KEY = 233,
    kMiscString_PLEASE_MOVE_JOYSTICK_1_TO_BOTTOM_RIGHT_AND_HIT_ANY_KEY = 234,
    kMiscString_PLEASE_MOVE_JOYSTICK_1_TO_CENTRE_AND_HIT_ANY_KEY = 235,
    kMiscString_PLEASE_MOVE_JOYSTICK_2_TO_TOP_LEFT_AND_HIT_ANY_KEY = 236,
    kMiscString_PLEASE_MOVE_JOYSTICK_2_TO_BOTTOM_RIGHT_AND_HIT_ANY_KEY = 237,
    kMiscString_PLEASE_MOVE_JOYSTICK_2_TO_CENTRE_AND_HIT_ANY_KEY = 238,
    kMiscString_A_NUMBER_FOR_YOU_TO_LOOK_AT = 239,
    kMiscString_ANOTHER_NUMBER_FOR_YOU_TO_LOOK_AT = 240,
    kMiscString_QUOTE_IT_QUOTE = 241,
    kMiscString_TOO_LATE_TO_CANCEL = 242,
    kMiscString_CouldNotFindTheCarmageddonCD = 243,
    kMiscString_NotAvailableInEvaluationCopy = 244,
    kMiscString_EvaluationCopy = 245,
};

enum {
    kFont_TYPEABLE = 0,
    kFont_ORANGHED = 1,
    kFont_BLUEHEAD = 2,
    kFont_GREENHED = 3,
    kFont_MEDIUMHD = 4,
    kFont_TIMER = 5,
    kFont_NEWHITE = 6,
    kFont_NEWRED = 7,
    kFont_NEWBIGGR = 8,
    kFont_GRNDK = 9,
    kFont_GRNLIT = 10,
    kFont_GRYDK = 11,
    kFont_GRYLIT = 12,
    kFont_BUTTIN = 13,
    kFont_BUTTOUT = 14,
    kFont_LITPLAQ = 15,
    kFont_DRKPLAQ = 16,
    kFont_BUTTIN1 = 17,
    kFont_BUTTOUT1 = 18,
    kFont_LITPLAQ1 = 19,
    kFont_DRKPLAQ1 = 20,
};

enum {
    kPratcam_stationary_or_below_25mph = 0,
    kPratcam_between_25_and_67mph = 1,
    kPratcam_between_67_and_167mph = 2,
    kPratcam_over_137mph = 3,
    kPratcam_accelerating_unused_4 = 4,
    kPratcam_flying = 5,
    kPratcam_falling = 6,
    kPratcam_rolling_gently = 7,
    kPratcam_rolling_medium = 8,
    kPratcam_rolling_violent = 9,
    kPratcam_spinning_gently = 10,
    kPratcam_spinning_medium = 11,
    kPratcam_spinning_violent = 12,
    kPratcam_small_hit_behind = 13,
    kPratcam_small_hit_front = 14,
    kPratcam_small_hit_left = 15,
    kPratcam_small_hit_right = 16,
    kPratcam_medium_hit_behind = 17,
    kPratcam_medium_hit_front = 18,
    kPratcam_medium_hit_left = 19,
    kPratcam_medium_hit_right = 20,
    kPratcam_big_hit_behind = 21,
    kPratcam_big_hit_front = 22,
    kPratcam_big_hit_left = 23,
    kPratcam_big_hit_right = 24,
    kPratcam_skidding_left = 25,
    kPratcam_skidding_right = 26,
    kPratcam_cockpit_head_left = 27,
    kPratcam_cockpit_head_right = 28,
    kPratcam_killed_one_ped = 29,
    kPratcam_killed_lots_of_peds = 30,
    kPratcam_apologise_for_ped_kill = 31,
    kPratcam_opponent_wasted = 32,
    kPratcam_checkpoint = 33,
    kPratcam_race_complete = 34,
    kPratcam_out_of_time = 35,
    kPratcam_network_timeout = 36,
    kPratcam_points_or_time = 37,
    kPratcam_good_powerup = 38,
    kPratcam_bad_powerup = 39,
    kPratcam_weird_powerup = 40,
    kPratcam_instant_handbrake = 41,
    kPratcam_bouncey_bouncey = 42,
    kPratcam_instant_bodywork_trash = 43,
    kPratcam_upside_down = 44,
};

enum {
    NETMSGID_SENDMEDETAILS = 0x00,
    NETMSGID_DETAILS = 0x01,
    NETMSGID_JOIN = 0x02,
    NETMSGID_NEWPLAYERLIST = 0x03,
    NETMSGID_GUARANTEEREPLY = 0x04,
    NETMSGID_CARDETAILSREQ = 0x05,
    NETMSGID_CARDETAILS = 0x06,
    NETMSGID_LEAVE = 0x07,
    NETMSGID_HOSTICIDE = 0x08,
    NETMSGID_RACEOVER = 0x09,
    NETMSGID_STATUSREPORT = 0x0a,
    NETMSGID_STARTRACE = 0x0b,
    NETMSGID_HEADUP = 0x0c,
    NETMSGID_HOSTQUERY = 0x0d,
    NETMSGID_HOSTREPLY = 0x0e,
    NETMSGID_MECHANICS = 0x0f,
    NETMSGID_NONCAR_INFO = 0x10,
    NETMSGID_TIMESYNC = 0x11,
    NETMSGID_CONFIRM = 0x12,
    NETMSGID_DISABLECAR = 0x13,
    NETMSGID_ENABLECAR = 0x14,
    NETMSGID_POWERUP = 0x15,
    NETMSGID_RECOVER = 0x16,
    NETMSGID_SCORES = 0x17,
    NETMSGID_WASTED = 0x18,
    NETMSGID_PEDESTRIAN = 0x19,
    NETMSGID_GAMEPLAY = 0x1a,
    NETMSGID_NONCARPOSITION = 0x1b,
    NETMSGID_COPINFO = 0x1c,
    NETMSGID_GAMESCORES = 0x1d,
    NETMSGID_OILSPILL = 0x1e,
    NETMSGID_CRUSHPOINT = 0x1f,
    NETMSGID_NONE = 0x20,
};

// Introduced with 3DFX patch
enum ExceptionFlags {
    ExceptionFlag_NoBilinear = 1,
    ExceptionFlag_Double = 2,
    ExceptionFlag_Quadruple = 4,
    ExceptionFlag_Mipmap = 4,
};

#define FONT_TYPEABLE 0
#define FONT_ORANGHED 1
#define FONT_BLUEHEAD 2
#define FONT_GREENHED 3
#define FONT_MEDIUMHD 4
#define FONT_TIMER 5
#define FONT_NEWHITE 6
#define FONT_NEWRED 7
#define FONT_NEWBIGGR 8
#define FONT_GRNDK 9
#define FONT_GRNLIT 10
#define FONT_GRYDK 11
#define FONT_GRYLIT 12
#define FONT_BUTTIN 13
#define FONT_BUTTOUT 14
#define FONT_LITPLAQ 15
#define FONT_DRKPLAQ 16
#define FONT_BUTTIN1 17
#define FONT_BUTTOUT1 18
#define FONT_LITPLAQ1 19
#define FONT_DRKPLAQ1 20

#define MAX_PATH_LENGTH 1024

#define ROLLING_LETTER_LOOP_RANDOM 96
#define PLAYER_NAME_MAX_LENGTH 13
#define DAMAGE_UNIT_PERIODS 5

#define GROOVE_FUNK_MAX_PER_CAR 24

#define NONCAR_UNUSED_SLOTS 5

#define DR_PI 3.141592653589793
#define DR_PI_OVER_2 1.570796326794897
#define DR_3PI_OVER_2 4.71238898038469

#define TIME_CONV_THING 0.0005f

#define OPPONENT_COUNT 5

#define WORLD_SCALE 6.9f
#define WORLD_SCALE_D 6.9

#define SHADOW_MAX_RENDER_DISTANCE 36.0f

#define DOUBLESIDED_USER_FLAG (void*)12345

// average frame time in carm95
#define MUNGE_ENGINE_INTERVAL 50

#define HIRES_Y_OFFSET 40

#endif
