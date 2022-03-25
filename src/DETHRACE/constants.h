#ifndef CONSTANTS_H
#define CONSTANTS_H

#define V11MODEL(model) (((v11model*)model->prepared))

// Jeff: These names are not part of the symbol dump, so the names in this file are not original, except as noted.

// Names from `gMem_names`. See also: `CreateStainlessClasses`
typedef enum dr_memory_classes {
    kMem_intf_pix_copy = 129, //  0x81
    kMem_intf_pal_copy = 130, //  0x82
    kMem_nodes_array = 131, //  0x83
    kMem_sections_array = 132, //  0x84
    kMem_key_names = 133, //  0x85
    kMem_columns_z = 134, //  0x86
    kMem_columns_x = 135, //  0x87
    kMem_non_car_list = 136, //  0x88
    kMem_simp_level = 137, //  0x89
    kMem_crush_data = 138, //  0x8a
    kMem_crush_neighbours = 139, //  0x8b
    kMem_temp_fs = 140, //  0x8c
    kMem_error_pix_copy = 141, //  0x8d
    kMem_error_pal_copy = 142, //  0x8e
    kMem_flic_pal = 143, //  0x8f
    kMem_flic_data = 144, //  0x90
    kMem_flic_data_2 = 145, //  0x91
    kMem_queued_flic = 146, //  0x92
    kFlic_panel_pixels = 147, //  0x93
    kMem_translations = 148, //  0x94
    kMem_translations_text = 149, //  0x95
    kMem_cur_pal_pixels = 150, //  0x96
    kMem_render_pal_pixels = 151, //  0x97
    kMem_scratch_pal_pixels = 152, //  0x98
    kMem_shade_table_copy = 153, //  0x99
    kMem_rear_screen_pixels = 154, //  0x9a
    kMem_rolling_letters = 155, //  0x9b
    kMem_intf_copy_areas = 156, //  0x9c
    kMem_strip_image = 157, //  0x9d
    kMem_strip_image_perm = 158, //  0x9e
    kMem_damage_clauses = 159, //  0x9f
    kMem_undamaged_vertices = 160, //  0xa0
    kMem_race_text_chunk = 161, //  0xa1
    kMem_race_text_str = 162, //  0xa2
    kMem_oppo_array = 163, //  0xa3
    kMem_oppo_text_chunk = 164, //  0xa4
    kMem_oppo_text_str = 165, //  0xa5
    kMem_br_font = 166, //  0xa6
    kMem_br_font_wid = 167, //  0xa7
    kMem_br_font_enc = 168, //  0xa8
    kMem_br_font_glyphs = 169, //  0xa9
    kMem_oppo_car_spec = 170, //  0xaa
    kMem_misc_string = 171, //  0xab
    kMem_mac_host_buffer_1 = 172, //  0xac
    kMem_mac_host_buffer_2 = 173, //  0xad
    kMem_mac_net_details = 174, //  0xae
    kMem_back_pixels = 175, //  0xaf
    kMem_quit_vfy_pixels = 176, //  0xb0
    kMem_quit_vfy_pal = 177, //  0xb1
    kMem_net_min_messages = 178, //  0xb2
    kMem_net_mid_messages = 179, //  0xb3
    kMem_net_max_messages = 180, //  0xb4
    kMem_net_pid_details = 181, //  0xb5
    kMem_net_car_spec = 182, //  0xb6
    kMem_dynamic_message = 183, //  0xb7
    kMem_player_list_join = 184, //  0xb8
    kMem_player_list_leave = 185, //  0xb9
    kMem_oppo_new_nodes = 186, //  0xba
    kMem_oppo_new_sections = 187, //  0xbb
    kMem_cop_car_spec = 188, //  0xbc
    kMem_oppo_bit_per_node = 189, //  0xbd
    kMem_ped_action_list = 190, //  0xbe
    kMem_ped_sequences = 191, //  0xbf
    kMem_ped_array_stain = 192, //  0xc0
    kMem_ped_array = 193, //  0xc1
    kMem_ped_instructions = 194, //  0xc2
    kMem_ped_new_instruc = 195, //  0xc3
    kMem_pipe_model_geometry = 196, //  0xc4
    kMem_powerup_array = 197, //  0xc5
    kMem_powerup_float_parms = 198, //  0xc6
    kMem_powerup_int_parms = 199, //  0xc7
    kMem_drugs_palette = 200, //  0xc8
    kMem_pratcam_flic_array = 201, //  0xc9
    kMem_pratcam_flic_data = 202, //  0xca
    kMem_pratcam_sequence_array = 203, //  0xcb
    kMem_pratcam_pixelmap = 204, //  0xcc
    kMem_video_pixels = 205, //  0xcd
    kMem_funk_prox_array = 206, //  0xce
    kMem_new_mat_id = 207, //  0xcf
    kMem_new_mat_id_2 = 208, //  0xd0
    kMem_new_mat_id_3 = 209, //  0xd1
    kMem_special_volume = 210, //  0xd2
    kMem_special_screen = 211, //  0xd3
    kMem_new_special_vol = 212, //  0xd4
    kMem_saved_game = 213, //  0xd5
    kMem_new_save_game = 214, //  0xd6
    kMem_stor_space_pix = 215, //  0xd7
    kMem_stor_space_tab = 216, //  0xd8
    kMem_stor_space_mat = 217, //  0xd9
    kMem_stor_space_mod = 218, //  0xda
    kMem_stor_space_save = 219, //  0xdb
    kMem_funk_spec = 220, //  0xdc
    kMem_groove_spec = 221, //  0xdd
    kMem_non_car_spec = 222, //  0xde
    kMem_S3_scan_name = 223, //  0xdf
    kMem_S3_sound_header = 224, //  0xe0
    kMem_S3_sample = 225, //  0xe1
    kMem_S3_mac_channel = 226, //  0xe2
    kMem_S3_mac_path = 227, //  0xe3
    kMem_S3_sentinel = 228, //  0xe4
    kMem_S3_outlet = 229, //  0xe5
    kMem_S3_channel = 230, //  0xe6
    kMem_S3_descriptor = 231, //  0xe7
    kMem_S3_reverse_buffer = 232, //  0xe8
    kMem_S3_source = 233, //  0xe9
    kMem_S3_DOS_SOS_channel = 234, //  0xea
    kMem_S3_PC_DOS_path = 235, //  0xeb
    kMem_S3_DOS_SOS_patch = 236, //  0xec
    kMem_S3_DOS_SOS_song_structure = 237, //  0xed
    kMem_S3_DOS_SOS_song_data = 238, //  0xee
    kMem_S3_Windows_95_load_WAV_file = 239, //  0xef
    kMem_S3_Windows_95_create_temp_buffer_space_to_reverse_sample = 240, //  0xf0
    kMem_S3_Windows_95_path = 241, //  0xf1
    kMem_DOS_HMI_file_open = 242, //  0xf2
    kMem_abuse_text = 243, //  0xf3
    kMem_action_replay_buffer = 244, //  0xf4
    kMem_misc = 245 //  0xf5
} dr_memory_classes;

typedef enum keycodes {
    KEY_SHIFT_ANY = 0, //  0x0
    KEY_ALT_ANY = 1, //  0x1
    KEY_CTRL_ANY = 2, //  0x2
    KEY_CTRL_ANY_2 = 3, //  0x3
    KEY_CAPSLOCK = 4, //  0x4
    KEY_RSHIFT = 5, //  0x5
    KEY_RALT = 6, //  0x6
    KEY_RCTRL = 7, //  0x7
    KEY_LSHIFT = 8, //  0x8
    KEY_LALT = 9, //  0x9
    KEY_LCTRL = 10, //  0xa
    KEY_0 = 11, //  0xb
    KEY_1 = 12, //  0xc
    KEY_2 = 13, //  0xd
    KEY_3 = 14, //  0xe
    KEY_4 = 15, //  0xf
    KEY_5 = 16, //  0x10
    KEY_6 = 17, //  0x11
    KEY_7 = 18, //  0x12
    KEY_8 = 19, //  0x13
    KEY_9 = 20, //  0x14
    KEY_A = 21, //  0x15
    KEY_B = 22, //  0x16
    KEY_C = 23, //  0x17
    KEY_D = 24, //  0x18
    KEY_E = 25, //  0x19
    KEY_F = 26, //  0x1a
    KEY_G = 27, //  0x1b
    KEY_H = 28, //  0x1c
    KEY_I = 29, //  0x1d
    KEY_J = 30, //  0x1e
    KEY_K = 31, //  0x1f
    KEY_L = 32, //  0x20
    KEY_M = 33, //  0x21
    KEY_N = 34, //  0x22
    KEY_O = 35, //  0x23
    KEY_P = 36, //  0x24
    KEY_Q = 37, //  0x25
    KEY_R = 38, //  0x26
    KEY_S = 39, //  0x27
    KEY_T = 40, //  0x28
    KEY_U = 41, //  0x29
    KEY_V = 42, //  0x2a
    KEY_W = 43, //  0x2b
    KEY_X = 44, //  0x2c
    KEY_Y = 45, //  0x2d
    KEY_Z = 46, //  0x2e
    KEY_GRAVE = 47, //  0x2f
    KEY_MINUS = 48, //  0x30
    KEY_EQUALS = 49, //  0x31
    KEY_BACKSPACE = 50, //  0x32
    KEY_RETURN = 51, //  0x33
    KEY_KP_ENTER = 52, //  0x34
    KEY_TAB = 53, //  0x35
    KEY_SLASH = 54, //  0x36
    KEY_UNKNOWN_55 = 55, //  0x37
    KEY_SEMICOLON = 56, //  0x38
    KEY_APOSTROPHE = 57, //  0x39
    KEY_PERIOD = 58, //  0x3a
    KEY_COMMA = 59, //  0x3b
    KEY_LBRACKET = 60, //  0x3c
    KEY_RBRACKET = 61, //  0x3d
    KEY_BACKSLASH = 62, //  0x3e
    KEY_ESCAPE = 63, //  0x3f
    KEY_INSERT = 64, //  0x40
    KEY_DELETE = 65, //  0x41
    KEY_HOME = 66, //  0x42
    KEY_END = 67, //  0x43
    KEY_PAGEUP = 68, //  0x44
    KEY_PAGEDOWN = 69, //  0x45
    KEY_LEFT = 70, //  0x46
    KEY_RIGHT = 71, //  0x47
    KEY_UP = 72, //  0x48
    KEY_DOWN = 73, //  0x49
    KEY_KP_NUMLOCK = 74, //  0x4a
    KEY_KP_DIVIDE = 75, //  0x4b
    KEY_KP_MULTIPLY = 76, //  0x4c
    KEY_KP_MINUS = 77, //  0x4d
    KEY_KP_PLUS = 78, //  0x4e
    KEY_KP_PERIOD = 79, //  0x4f
    KEY_KP_EQUALS = 80, //  0x50
    KEY_KP_0 = 81, //  0x51
    KEY_KP_1 = 82, //  0x52
    KEY_KP_2 = 83, //  0x53
    KEY_KP_3 = 84, //  0x54
    KEY_KP_4 = 85, //  0x55
    KEY_KP_5 = 86, //  0x56
    KEY_KP_6 = 87, //  0x57
    KEY_KP_7 = 88, //  0x58
    KEY_KP_8 = 89, //  0x59
    KEY_KP_9 = 90, //  0x5a
    KEY_F1 = 91, //  0x5b
    KEY_F2 = 92, //  0x5c
    KEY_F3 = 93, //  0x5d
    KEY_F4 = 94, //  0x5e
    KEY_F5 = 95, //  0x5f
    KEY_F6 = 96, //  0x60
    KEY_F7 = 97, //  0x61
    KEY_F8 = 98, //  0x62
    KEY_F9 = 99, //  0x63
    KEY_F10 = 100, //  0x64
    KEY_F11 = 101, //  0x65
    KEY_F12 = 102, //  0x66
    KEY_PRTSCN = 103, //  0x67
    KEY_SCRLK = 104, //  0x68
    KEY_PAUSE = 105, //  0x69
    KEY_SPACE = 106, //  0x6a
} keycodes;

typedef enum keymapcodes {
    KEYMAP_ESCAPE = 0,              // default: ESC

    KEYMAP_CONTROL_ANY = 7,         // default: LCTRL or RCTRL (any)
    KEYMAP_SHIFT_ANY = 8,           // default: LSHIFT or RSHIFT (any)

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

    KEYMAP_SHIFT_DUMPSCREEN = 5,    // default: D

    //
    // The following items are meant to be used standalone
    //

    KEYMAP_COCKPIT = 45,            // default C
    KEYMAP_SAVE = 28,               // default F2
    KEYMAP_LOAD = 29,               // default F3
    KEYMAP_MOVE_UP = 30,            // default UP
    KEYMAP_MOVE_DOWN = 31,          // default DOWN
    KEYMAP_MOVE_LEFT = 32,          // default LEFT
    KEYMAP_MOVE_RIGHT = 33,         // default RIGHT
    KEYMAP_MIRROR = 34,             // default M
    KEYMAP_PRATCAM = 35,            // default P
    KEYMAP_LOOK_LEFT = 50,          // default Q
    KEYMAP_LOOK_FORWARD = 51,       // default W
    KEYMAP_LOOK_RIGHT = 52,         // default E
    KEYMAP_SOUND = 36,              // default S
    KEYMAP_PEDESTRIANS = 54,        // default KP_MINUS
    KEYMAP_F4 = 17,                 // default F4
    KEYMAP_F5 = 18,                 // default F5
    KEYMAP_F6 = 19,                 // default F6
    KEYMAP_F7 = 20,                 // default F7
    KEYMAP_F8 = 21,                 // default F8
    KEYMAP_F10 = 22,                // default F10
    KEYMAP_F11 = 23,                // default F1
    KEYMAP_F12 = 24,                // default F12
    KEYMAP_EDITMODE_0 = 14,         // default 0
    KEYMAP_EDITMODE_1 = 37,         // default 1
    KEYMAP_EDITMODE_2 = 38,         // default 2
    KEYMAP_EDITMODE_3 = 39,         // default 3
    KEYMAP_EDITMODE_4 = 40,         // default 4
    KEYMAP_EDITMODE_5 = 41,         // default 5
    KEYMAP_EDITMODE_6 = 42,         // default 6
    KEYMAP_EDITMODE_7 = 43,         // default 7
    KEYMAP_EDITMODE_8 = 15,         // default 8
    KEYMAP_EDITMODE_9 = 16,         // default 9
    KEYMAP_SCREEN_SMALLER = 60,     // default -
    KEYMAP_SCREEN_LARGER = 61,      // default =
    KEYMAP_MAP = 56,                // default TAB
    KEYMAP_RECOVERY = 59,           // default Insert
    KEYMAP_BUY_ARMOUR = 62,         // default Delete
    KEYMAP_BUY_POWER = 63,          // default End
    KEYMAP_BUY_OFFENSE = 64,        // default PgDn
    KEYMAP_VIEW_NETPLAYER = 65,     // default Space
    KEYMAP_SEND_MESSAGE = 66,       // default `
    KEYMAP_ARROW = 25,              // default R
    KEYMAP_INFO = 26,               // default I
} keymapcodes;

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

#define ROLLING_LETTER_LOOP_RANDOM 96
#define PLAYER_NAME_MAX_LENGTH 13
#define DAMAGE_UNIT_PERIODS 5

#define GROOVE_FUNK_MAX_PER_CAR 24

#define NONCAR_UNUSED_SLOTS 5

#define STR_STARTS_WITH(haystack, needle) strncmp(haystack, needle, strlen(needle))
#define STR_ENDS_WITH(haystack, needle) strcmp(haystack + strlen(haystack) - strlen(needle), needle)
#define MAX(a, b) ((a) > (b) ? a : b)
#define COUNT_OF(array) (sizeof((array)) / sizeof((array)[0]))
#define LEN(array) (sizeof((array)) / sizeof((array)[0]))

#define DEG_TO_RAD(degrees) ((degrees)*3.141592653589793 / 180.0)

#define ONEEIGHTTWO 182.0444444444444

#define d180_OVER_PI 57.29577951308232 // (180 / PI)

#define DR_PI 3.141592653589793

#define TIME_CONV_THING 0.0005f

#define OPPONENT_COUNT 5

#define WORLD_SCALE 6.9f

#define SHADOW_MAX_RENDER_DISTANCE 36.0f

#define DOUBLESIDED_FLAG_COLOR_MAP (br_pixelmap*)12345

#define SLOBYTE(x) (*((signed char*)&(x)))

#endif
