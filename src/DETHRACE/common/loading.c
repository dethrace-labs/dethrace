#include "loading.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "brender.h"
#include "brucetrk.h"
#include "controls.h"
#include "depth.h"
#include "displays.h"
#include "errors.h"
#include "globvars.h"
#include "globvrkm.h"
#include "graphics.h"
#include "init.h"
#include "input.h"
#include "newgame.h"
#include "opponent.h"
#include "pc-dos/dossys.h"
#include "pedestrn.h"
#include "sound.h"
#include "spark.h"
#include "utility.h"
#include "world.h"

#define HITHER_MULTIPLIER 2.0f
#define AMBIENT_MULTIPLIER 0.0099999998f

char* gWheel_actor_names[6];
int gFunk_groove_flags[30];
char* gNet_avail_names[4];
char* gDamage_names[12];
char* gDrivable_car_names[6];
char* gYour_car_names[2][6];
char gDef_def_water_screen_name[32];
tHeadup_info gHeadup_image_info[31];
int gAllow_open_to_fail = 1;
br_material* gDestn_screen_mat;
br_material* gSource_screen_mat;
char* gRaces_file_names[9];
char* gFloorpan_names[5];
int gCurrent_race_file_index;
int gGroove_funk_offset;
int gDemo_armour;
int gDemo_rank;
int gDecode_thing = '@';
int gDemo_opponents[5];
int gDemo_power;
int gDemo_offensive;
char gDecode_string[14] = { 0x9B, 0x52, 0x93, 0x9F, 0x52, 0x98, 0x9B, 0x96, 0x96, 0x9E, 0x9B, 0xA0, 0x99, 0x0 };

// Offset: 0
// Size: 67
// EAX: pF
tU32 ReadU32(FILE* pF) {
    tU32 raw_long;
    NOT_IMPLEMENTED();
}

// Offset: 68
// Size: 67
// EAX: pF
tU16 ReadU16(FILE* pF) {
    tU16 raw_short;
    NOT_IMPLEMENTED();
}

// Offset: 136
// Size: 67
// EAX: pF
tU8 ReadU8(FILE* pF) {
    tU8 raw_byte;
    NOT_IMPLEMENTED();
}

// Offset: 204
// Size: 67
// EAX: pF
tS32 ReadS32(FILE* pF) {
    tS32 raw_long;
    NOT_IMPLEMENTED();
}

// Offset: 272
// Size: 67
// EAX: pF
tS16 ReadS16(FILE* pF) {
    tS16 raw_short;
    NOT_IMPLEMENTED();
}

// Offset: 340
// Size: 67
// EAX: pF
tS8 ReadS8(FILE* pF) {
    tS8 raw_byte;
    NOT_IMPLEMENTED();
}

// Offset: 408
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteU32L(FILE* pF, tU32 pNumber) {
    tU32 raw_long;
    NOT_IMPLEMENTED();
}

// Offset: 476
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteU16L(FILE* pF, tU16 pNumber) {
    tU16 raw_short;
    NOT_IMPLEMENTED();
}

// Offset: 544
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteU8L(FILE* pF, tU8 pNumber) {
    tU8 raw_byte;
    NOT_IMPLEMENTED();
}

// Offset: 612
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteS32L(FILE* pF, tS32 pNumber) {
    tS32 raw_long;
    NOT_IMPLEMENTED();
}

// Offset: 680
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteS16L(FILE* pF, tS16 pNumber) {
    tS16 raw_short;
    NOT_IMPLEMENTED();
}

// Offset: 748
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteS8L(FILE* pF, tS8 pNumber) {
    tS8 raw_byte;
    NOT_IMPLEMENTED();
}

// Offset: 816
// Size: 54
// EAX: pF
// EDX: pBytes_to_skip
void SkipBytes(FILE* pF, int pBytes_to_skip) {
    NOT_IMPLEMENTED();
}

// Offset: 872
// Size: 70
// EAX: pPtr
tU32 MemReadU32(char** pPtr) {
    tU32 raw_long;
    NOT_IMPLEMENTED();
}

// Offset: 944
// Size: 70
// EAX: pPtr
tU16 MemReadU16(char** pPtr) {
    tU16 raw_short;
    NOT_IMPLEMENTED();
}

// Offset: 1016
// Size: 55
// EAX: pPtr
tU8 MemReadU8(char** pPtr) {
    NOT_IMPLEMENTED();
}

// Offset: 1072
// Size: 70
// EAX: pPtr
tS32 MemReadS32(char** pPtr) {
    tS32 raw_long;
    NOT_IMPLEMENTED();
}

// Offset: 1144
// Size: 70
// EAX: pPtr
tS16 MemReadS16(char** pPtr) {
    tS16 raw_short;
    NOT_IMPLEMENTED();
}

// Offset: 1216
// Size: 55
// EAX: pPtr
tS8 MemReadS8(char** pPtr) {
    tS8 raw_byte;
    NOT_IMPLEMENTED();
}

// Offset: 1272
// Size: 46
// EAX: pPtr
// EDX: pBytes_to_skip
void MemSkipBytes(char** pPtr, int pBytes_to_skip) {
    NOT_IMPLEMENTED();
}

// Offset: 1320
// Size: 2149
void LoadGeneralParameters() {
    FILE* f;
    tPath_name the_path;
    int i;
    int temp;
    char s[256];
    char* str;

    PathCat(the_path, gApplication_path, "ACTORS");
    PathCat(the_path, the_path, "PROG.ACT");
    f = fopen(the_path, "rb");
    if (f) {
        fgets(s, 255, f);
        fclose(f);
        for (i = 0; i < strlen(gDecode_string); i++) {
            gDecode_string[i] -= 50;
        }

        // trim trailing CRLF etc
        while (s[0] != '\0' && s[strlen(s) - 1] < 0x20) {
            s[strlen(s) - 1] = 0;
        }

        if (strcmp(s, gDecode_string) == 0) {
            gDecode_thing = 0;
        }

        for (i = 0; i < strlen(gDecode_string); i++) {
            gDecode_string[i] -= 50;
        }
    }
    PathCat(the_path, gApplication_path, "GENERAL.TXT");
    f = DRfopen(the_path, "rt");
    if (!f) {
        FatalError(8);
    }
    gCamera_hither = GetAFloat(f) * HITHER_MULTIPLIER;
    gCamera_yon = GetAFloat(f);
    gCamera_angle = GetAFloat(f);
    gAmbient_adjustment = GetAFloat(f) * AMBIENT_MULTIPLIER;
    gDim_amount = GetAnInt(f);
    gInitial_rank = GetAnInt(f);
    GetThreeInts(f, &gInitial_credits[0], &gInitial_credits[1], &gInitial_credits[2]);
    GetThreeInts(f, &gCredits_per_rank[0], &gCredits_per_rank[1], &gCredits_per_rank[2]);
    gCar_crush_min_fold = GetAFloat(f);
    gCar_crush_max_fold = GetAFloat(f);
    gCar_crush_wibble = GetAFloat(f);
    gCar_crush_limit_deviant = GetAFloat(f);
    gCar_crush_split_chance = GetAFloat(f);
    gCar_crush_softness = GetAFloat(f);
    GetThreeFloats(f, &gRepair_cost[0], &gRepair_cost[1], &gRepair_cost[2]);
    GetThreeFloats(f, &gRecovery_cost[0], &gRecovery_cost[1], &gRecovery_cost[2]);
    GetThreeInts(f, &gPed_time_value[0], &gPed_time_value[1], &gPed_time_value[2]);
    if (gProgram_state.sausage_eater_mode) {
        for (i = 0; i < 7; i++) {
            GetALineAndDontArgue(f, s);
        }

        GetThreeFloats(f, gCar_time_value, &gCar_time_value[1], &gCar_time_value[2]);
        GetThreeFloats(f, gCar_cred_value, &gCar_cred_value[1], &gCar_cred_value[2]);
        GetThreeInts(f, gWasted_time, &gWasted_time[1], &gWasted_time[2]);
        GetThreeInts(f, gWasted_creds, &gWasted_creds[1], &gWasted_creds[2]);
        GetThreeInts(f, gRoll_over_time, &gRoll_over_time[1], &gRoll_over_time[2]);
        GetThreeInts(f, gRoll_over_creds, &gRoll_over_creds[1], &gRoll_over_creds[2]);
        GetThreeInts(f, gCheck_point_cash, &gCheck_point_cash[1], &gCheck_point_cash[2]);
    } else {
        GetThreeFloats(f, gCar_time_value, &gCar_time_value[1], &gCar_time_value[2]);
        GetThreeFloats(f, gCar_cred_value, &gCar_cred_value[1], &gCar_cred_value[2]);
        GetThreeInts(f, gWasted_time, &gWasted_time[1], &gWasted_time[2]);
        GetThreeInts(f, gWasted_creds, &gWasted_creds[1], &gWasted_creds[2]);
        GetThreeInts(f, gRoll_over_time, &gRoll_over_time[1], &gRoll_over_time[2]);
        GetThreeInts(f, gRoll_over_creds, &gRoll_over_creds[1], &gRoll_over_creds[2]);
        GetThreeInts(f, gCheck_point_cash, &gCheck_point_cash[1], &gCheck_point_cash[2]);
        for (i = 0; i < 7; i++) {
            GetALineAndDontArgue(f, s);
        }
    }
    GetThreeInts(f, gJump_start_fine, &gJump_start_fine[1], &gJump_start_fine[2]);
    GetThreeInts(f, gPoints_per_second, &gPoints_per_second[1], &gPoints_per_second[2]);
    GetThreeInts(f, gCunning_stunt_bonus, &gCunning_stunt_bonus[1], &gCunning_stunt_bonus[2]);
    GetAString(f, gBasic_car_names[0]);
    GetAString(f, gBasic_car_names[1]);
    gKnobbled_frame_period = GetAnInt(f);
    if (gKnobbled_frame_period) {
        gKnobbled_frame_period = 1000 / gKnobbled_frame_period;
    }
    gOpponent_nastyness_frigger = GetAFloat(f);
    ParseSpecialVolume(f, &gDefault_default_water_spec_vol, gDef_def_water_screen_name);
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    for (i = 0; i < 5; i++) {
        sscanf(str, "%d", &gInitial_net_credits[i]);
        str = strtok(NULL, "\t ,/");
    }

    gTag_start_time = 1000 * GetAnInt(f);
    gFox_start_time = 1000 * GetAnInt(f);

    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    for (i = 0; i < 7; i++) {
        sscanf(str, "%f", &gNet_repair_cost[i]);
        str = strtok(NULL, "\t ,/");
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    for (i = 0; i < 7; i++) {
        sscanf(str, "%f", &gNet_recovery_cost[i]);
        str = strtok(NULL, "\t ,/");
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    for (i = 0; i < 7; i++) {
        sscanf(str, "%f", &gNet_softness[i]);
        str = strtok(NULL, "\t ,/");
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    for (i = 0; i < 7; i++) {
        sscanf(str, "%f", &gNet_offensive[i]);
        str = strtok(NULL, "\t ,/");
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    for (i = 0; i < 7; i++) {
        sscanf(str, "%d", &gNet_target[i]);
        str = strtok(NULL, "\t ,/");
    }

    gMin_respawn_time = 1000 * GetAnInt(f);
    gRespawn_variance = 1000 * GetAnInt(f);
    gDemo_rank = GetAnInt(f);
    gDemo_armour = GetAnInt(f);
    gDemo_power = GetAnInt(f);
    gDemo_offensive = GetAnInt(f);
    for (i = 0; i < 5; i++) {
        gDemo_opponents[i] = GetAnInt(f);
    }

    gGravity_multiplier = GetAFloat(f);
    gZombie_factor = GetAFloat(f);
    gCut_delay_1 = GetAFloat(f);
    gCut_delay_2 = GetAFloat(f);
    gCut_delay_3 = GetAFloat(f);
    gCut_delay_4 = 1.0f;
    fclose(f);
}

// Offset: 3472
// Size: 53
void FinishLoadingGeneral() {
    /*dword_1298C0 = */ BrMaterialFind(gDef_def_water_screen_name);
}

// Offset: 3528
// Size: 292
// EAX: pName
br_pixelmap* LoadPixelmap(char* pName) {
    tPath_name the_path;
    br_pixelmap* pm = NULL;
    char* end;
    LOG_TRACE("(\"%s\")", pName);

    end = strrchr(pName, '.');
    if (!end) {
        end = &pName[strlen(pName)];
    }

    if ((end - pName) != 4 || memcmp(pName, "none", end - pName) != 0) {
        PossibleService();
        PathCat(the_path, gApplication_path, gGraf_specs[gGraf_spec_index].data_dir_name);
        PathCat(the_path, the_path, "PIXELMAP");
        PathCat(the_path, the_path, pName);
        gAllow_open_to_fail = 1;
        pm = DRPixelmapLoad(the_path);
        if (!pm) {
            PathCat(the_path, gApplication_path, "PIXELMAP");
            PathCat(the_path, the_path, pName);
            pm = DRPixelmapLoad(the_path);
        }
        return pm;
    }
}

// Offset: 3820
// Size: 222
// EAX: pFile_name
// EDX: pPixelmaps
// EBX: pNum
br_uint_32 LoadPixelmaps(char* pFile_name, br_pixelmap** pPixelmaps, br_uint_16 pNum) {
    tPath_name path;
    int count;
    NOT_IMPLEMENTED();
}

// Offset: 4044
// Size: 99
// EAX: pName
br_pixelmap* LoadShadeTable(char* pName) {
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

// Offset: 4144
// Size: 129
// EAX: pName
br_material* LoadMaterial(char* pName) {
    tPath_name the_path;
    br_material* result;
    NOT_IMPLEMENTED();
}

// Offset: 4276
// Size: 123
// EAX: pName
br_model* LoadModel(char* pName) {
    tPath_name the_path;
    br_model* model;
    LOG_TRACE("(\"%s\")", pName);

    PossibleService();
    PathCat(the_path, gApplication_path, "MODELS");
    PathCat(the_path, the_path, pName);
    return BrModelLoad(the_path);
}

// Offset: 4400
// Size: 104
// EAX: pName
br_actor* LoadActor(char* pName) {
    tPath_name the_path;
    LOG_TRACE("(\"%s\")", pName);

    PossibleService();
    PathCat(the_path, gApplication_path, "ACTORS");
    PathCat(the_path, the_path, pName);
    return BrActorLoad(the_path);
}

// Offset: 4504
// Size: 78
// EAX: pPath_name
void DRLoadPalette(char* pPath_name) {
    br_pixelmap* palette_array[100];
    int number_of_palettes;

    number_of_palettes = DRPixelmapLoadMany(pPath_name, palette_array, 100);
    BrTableAddMany(palette_array, number_of_palettes);
}

// Offset: 4584
// Size: 78
// EAX: pPath_name
void DRLoadShadeTable(char* pPath_name) {
    br_pixelmap* table_array[100];
    int number_of_tables;

    number_of_tables = DRPixelmapLoadMany(pPath_name, table_array, 100);
    BrTableAddMany(table_array, number_of_tables);
}

// Offset: 4664
// Size: 84
// EAX: pPixelmap_array
// EDX: pCount
void RezeroPixelmaps(br_pixelmap** pPixelmap_array, int pCount) {
    NOT_IMPLEMENTED();
}

// Offset: 4748
// Size: 97
// EAX: pPath_name
void DRLoadPixelmaps(char* pPath_name) {
    br_pixelmap* pixelmap_array[100];
    int number_of_pixelmaps;

    int i;
    PossibleService();
    number_of_pixelmaps = DRPixelmapLoadMany(pPath_name, pixelmap_array, 100);

    for (i = 0; i < number_of_pixelmaps; i++) {
        pixelmap_array[i]->origin_x = 0;
        pixelmap_array[i]->origin_y = 0;
    }
    BrMapAddMany(pixelmap_array, number_of_pixelmaps);
}

// Offset: 4848
// Size: 103
// EAX: pPath_name
void DRLoadMaterials(char* pPath_name) {
    br_material* material_array[100];
    int number_of_materials;

    PossibleService();
    number_of_materials = BrMaterialLoadMany(pPath_name, material_array, 100);
    BrMaterialAddMany(material_array, number_of_materials);
}

// Offset: 4952
// Size: 103
// EAX: pPath_name
void DRLoadModels(char* pPath_name) {
    br_model* model_array[100];
    int number_of_models;
    NOT_IMPLEMENTED();
}

// Offset: 5056
// Size: 134
// EAX: pPath_name
void DRLoadActors(char* pPath_name) {
    br_actor* actor_array[100];
    int number_of_actors;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 5192
// Size: 134
// EAX: pPath_name
void DRLoadLights(char* pPath_name) {
    br_actor* light_array[100];
    int number_of_lights;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 5328
// Size: 70
// EAX: pThe_base_path
// EDX: pThe_dir_name
// EBX: pLoad_routine
void LoadInFiles(char* pThe_base_path, char* pThe_dir_name, void (*pLoad_routine)(char*)) {
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

// Offset: 5400
// Size: 208
// EAX: pThe_dir_path
void LoadInRegisteeDir(char* pThe_dir_path) {
    tPath_name the_path;
    tPath_name reg_path;
    LOG_TRACE("(\"%s\")", pThe_dir_path);

    PathCat(reg_path, pThe_dir_path, "REG");
    PathCat(the_path, reg_path, "PALETTES");
    PDForEveryFile(the_path, DRLoadPalette);
    PathCat(the_path, reg_path, "SHADETAB");
    PDForEveryFile(the_path, DRLoadShadeTable);
    PathCat(the_path, reg_path, "PIXELMAP");
    PDForEveryFile(the_path, DRLoadPixelmaps);
    PathCat(the_path, reg_path, "MATERIAL");
    PDForEveryFile(the_path, DRLoadMaterials);
    PathCat(the_path, reg_path, "MODELS");
    PDForEveryFile(the_path, DRLoadModels);
    PathCat(the_path, reg_path, "ACTORS");
    PDForEveryFile(the_path, DRLoadActors);
    PathCat(the_path, reg_path, "LIGHTS");
    PDForEveryFile(the_path, DRLoadLights);
}

// Offset: 5608
// Size: 44
void LoadInRegistees() {
    LoadInRegisteeDir(gApplication_path);
}

// Offset: 5652
// Size: 182
void LoadKeyMapping() {
    FILE* f;
    tPath_name the_path;
    int i;
    LOG_TRACE("()");

    PathCat(the_path, gApplication_path, "KEYMAP_X.TXT");
    the_path[strlen(the_path) - 5] = gKey_map_index + 48;
    f = DRfopen(the_path, "rt");
    if (!f) {
        FatalError(9);
    }

    for (i = 0; i < 67; i++) {
        fscanf(f, "%d", &gKey_mapping[i]);
    }

    fclose(f);
}

// Offset: 5836
// Size: 269
// EAX: pWithin_race
void LoadInterfaceStuff(int pWithin_race) {
    tPath_name path;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 6108
// Size: 272
void UnlockInterfaceStuff() {
    NOT_IMPLEMENTED();
}

// Offset: 6380
// Size: 75
void InitInterfaceLoadState() {
    NOT_IMPLEMENTED();
}

// Offset: 6456
// Size: 627
// EAX: pBr_map
tS8* ConvertPixTo16BitStripMap(br_pixelmap* pBr_map) {
    int i;
    int j;
    int new_line_length;
    int current_size;
    int counting_blanks;
    int counter;
    int chunk_counter;
    int max_line_bytes;
    tU8* next_byte;
    tU8* strip_image;
    tU8* current_strip_pointer;
    tU8* temp_strip_image;
    tU8* new_line;
    tU8 byte;
    tU16* palette_entry;
    NOT_IMPLEMENTED();
}

// Offset: 7084
// Size: 578
// EAX: pThe_br_map
tS8* ConvertPixToStripMap(br_pixelmap* pThe_br_map) {
    int i;
    int j;
    int new_line_length;
    int current_size;
    int counting_blanks;
    int counter;
    int chunk_counter;
    tU8* next_byte;
    tU8* the_strip_image;
    tU8* current_strip_pointer;
    tU8* temp_strip_image;
    tU8 new_line[800];
    tU8 the_byte;
    NOT_IMPLEMENTED();
}

// Offset: 7664
// Size: 147
// EAX: pModel
// EDX: pMaterial
void KillWindscreen(br_model* pModel, br_material* pMaterial) {
    br_face* face;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 7812
// Size: 103
// EAX: pCar
void DropOffDyingPeds(tCar_spec* pCar) {
    br_actor* child;
    br_actor* next;
    NOT_IMPLEMENTED();
}

// Offset: 7916
// Size: 1113
// EAX: pCar_spec
// EDX: pOwner
void DisposeCar(tCar_spec* pCar_spec, int pOwner) {
    int i;
    int j;
    NOT_IMPLEMENTED();
}

// Offset: 9032
// Size: 626
// EAX: pCar
void AdjustCarCoordinates(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 9660
// Size: 929
// EAX: pF
// EDX: pIndex
// EBX: pCar_spec
void LoadSpeedo(FILE* pF, int pIndex, tCar_spec* pCar_spec) {
    tPath_name the_path;
    char s[256];
    char* str;
    char the_char1;
    char the_char2;
    NOT_IMPLEMENTED();
}

// Offset: 10592
// Size: 764
// EAX: pF
// EDX: pIndex
// EBX: pCar_spec
void LoadTacho(FILE* pF, int pIndex, tCar_spec* pCar_spec) {
    tPath_name the_path;
    char s[256];
    char* str;
    char the_char1;
    char the_char2;
    NOT_IMPLEMENTED();
}

// Offset: 11356
// Size: 816
// EAX: pF
// EDX: pIndex
// EBX: pCar_spec
void LoadHeadups(FILE* pF, int pIndex, tCar_spec* pCar_spec) {
    char s[256];
    char* str;
    int j;
    int number_of_slots;
    NOT_IMPLEMENTED();
}

// Offset: 12172
// Size: 1819
// EAX: pF
// EDX: non_car
void ReadNonCarMechanicsData(FILE* pF, tNon_car_spec* non_car) {
    int number;
    int i;
    int j;
    char s[256];
    tCollision_info* c;
    br_scalar wid;
    br_scalar het;
    br_scalar len;
    br_scalar ts;
    br_scalar ts1;
    br_scalar snap_angle;
    NOT_IMPLEMENTED();
}

// Offset: 13992
// Size: 3476
// EAX: pF
// EDX: c
void ReadMechanicsData(FILE* pF, tCar_spec* c) {
    char s[256];
    char version;
    int i;
    int j;
    br_scalar ratio;
    br_scalar ts1;
    br_scalar theta_front;
    br_scalar theta_back;
    br_scalar theta_comp;
    br_scalar wid;
    br_scalar het;
    br_scalar len;
    br_vector3* actor_offset;
    br_scalar speed;
    br_scalar force;
    NOT_IMPLEMENTED();
}

// Offset: 17468
// Size: 196
// EAX: pF
// EDX: pIndex
// EBX: pCar_spec
void LoadGear(FILE* pF, int pIndex, tCar_spec* pCar_spec) {
    tPath_name the_path;
    char s[256];
    char* str;
    NOT_IMPLEMENTED();
}

// Offset: 17664
// Size: 56
// EAX: pRef_holder
void AddRefOffset(int* pRef_holder) {
    NOT_IMPLEMENTED();
}

// Offset: 17720
// Size: 697
// EAX: pF
// EDX: pCar_spec
// EBX: pImpact_location
void GetDamageProgram(FILE* pF, tCar_spec* pCar_spec, tImpact_location pImpact_location) {
    tDamage_clause* the_clause;
    int i;
    int j;
    int k;
    int count;
    char s[256];
    char delim[64];
    char* str;
    NOT_IMPLEMENTED();
}

// Offset: 18420
// Size: 178
br_uint_32 LinkModel(br_actor* pActor, tModel_pool* pModel_pool) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 18600
// Size: 198
// EAX: pModel_array
// EDX: pModel_count
void FreeUpBonnetModels(br_model** pModel_array, int pModel_count) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 18800
// Size: 67
// EAX: pActor
// EDX: pModel_array
// EBX: pModel_count
void LinkModelsToActor(br_actor* pActor, br_model** pModel_array, int pModel_count) {
    tModel_pool model_pool;
    NOT_IMPLEMENTED();
}

// Offset: 18868
// Size: 132
// EAX: pF
// EDX: pCar_spec
void ReadShrapnelMaterials(FILE* pF, tCollision_info* pCar_spec) {
    char s[256];
    char version;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 19000
// Size: 200
// EAX: pOutput_car
// EDX: pInput_car
void CloneCar(tCar_spec** pOutput_car, tCar_spec* pInput_car) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 19200
// Size: 65
// EAX: pCar
void DisposeClonedCar(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 19268
// Size: 523
// EAX: pModel
int RemoveDoubleSided(br_model* pModel) {
    br_face* faces;
    br_face* face;
    int temp;
    int num_double_sided_faces;
    int i;
    int orig_nfaces;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 19792
// Size: 201
// EAX: pModel
void MungeWindscreen(br_model* pModel) {
    br_face* face;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 19996
// Size: 141
// EAX: pModel
// EDX: pOwner
void SetModelFlags(br_model* pModel, int pOwner) {
    NOT_IMPLEMENTED();
}

// Offset: 20140
// Size: 8202
// EAX: pCar_name
// EDX: pDriver
// EBX: pCar_spec
// ECX: pOwner
void LoadCar(char* pCar_name, tDriver pDriver, tCar_spec* pCar_spec, int pOwner, char* pDriver_name, tBrender_storage* pStorage_space) {
    FILE* f;
    FILE* g;
    FILE* h;
    tPath_name the_path;
    int i;
    int j;
    int k;
    int its_a_floorpan;
    int number_of_floorpans;
    int point_num;
    int initial_vertex;
    int old_model_count;
    int old_material_count;
    int temp_index;
    int vertex_array_size;
    char s[256];
    char* str;
    br_pixelmap* the_image;
    float rate;
    float temp_float;
    br_model* model;
    br_vector3 tv;
    int v;
    int v_num;
    int group;
    int vertex_total;
    NOT_IMPLEMENTED();
}

// Offset: 28344
// Size: 186
void LoadHeadupImages() {
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

// Offset: 28532
// Size: 94
void DisposeHeadupImages() {
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

// Offset: 28628
// Size: 109
FILE* OpenRaceFile() {
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

// Offset: 28740
// Size: 219
// EAX: pF
void SkipRestOfRace(FILE* pF) {
    int j;
    int k;
    int text_chunk_count;
    int line_count;
    char s[256];
    NOT_IMPLEMENTED();
}

// Offset: 28960
// Size: 411
// EAX: pRace_list
// EDX: pCount
// EBX: pRace_type_index
void LoadRaces(tRace_list_spec* pRace_list, int* pCount, int pRace_type_index) {
    FILE* f;
    int i;
    int j;
    int k;
    int number_of_racers;
    int last_race;
    char s[256];
    char* str;
    NOT_IMPLEMENTED();
}

// Offset: 29372
// Size: 88
// EAX: pIndex
void UnlockOpponentMugshot(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 29460
// Size: 168
// EAX: pIndex
void LoadOpponentMugShot(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 29628
// Size: 136
// EAX: pRace_info
// EDX: pIndex
void DisposeOpponentGridIcon(tRace_info* pRace_info, int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 29764
// Size: 155
// EAX: pRace_info
// EDX: pIndex
void LoadOpponentGridIcon(tRace_info* pRace_info, int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 29920
// Size: 804
// EAX: pRace_index
// EDX: pRace_info
void LoadRaceInfo(int pRace_index, tRace_info* pRace_info) {
    FILE* f;
    int i;
    int j;
    int k;
    int duplicate;
    int substitute;
    int auto_scum_count;
    int old_index;
    int temp_index;
    char s[256];
    char* str;
    float temp_float;
    tText_chunk* the_chunk;
    NOT_IMPLEMENTED();
}

// Offset: 30724
// Size: 351
// EAX: pRace_info
void DisposeRaceInfo(tRace_info* pRace_info) {
    int i;
    int j;
    int k;
    tText_chunk* the_chunk;
    NOT_IMPLEMENTED();
}

// Offset: 31076
// Size: 122
// EAX: pRace_info
void LoadGridIcons(tRace_info* pRace_info) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 31200
// Size: 119
// EAX: pRace_info
void DisposeGridIcons(tRace_info* pRace_info) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 31320
// Size: 1075
void LoadOpponents() {
    tPath_name the_path;
    int i;
    int j;
    int k;
    char s[256];
    char* str;
    tText_chunk* the_chunk;
    NOT_IMPLEMENTED();
}

// Offset: 32396
// Size: 647
// EAX: pName
br_font* LoadBRFont(char* pName) {
    FILE* f;
    tPath_name the_path;
    br_font* the_font;
    tU32 data_size;
    int i;
    LOG_TRACE("(\"%s\")", pName);

    PathCat(the_path, gApplication_path, gGraf_specs[gGraf_spec_index].data_dir_name);
    PathCat(the_path, the_path, "FONTS");
    PathCat(the_path, the_path, pName);
    f = DRfopen(the_path, "rb");
    PossibleService();
    the_font = BrMemAllocate(sizeof(br_font), 0xA6u);

    // we read 0x18 bytes as that is the size of the struct in 32 bit code.
    fread(the_font, 0x18, 1u, f);
    the_font->flags = BrSwap32(the_font->flags);

    // swap endianness
    the_font->glyph_x = the_font->glyph_x >> 8 | the_font->glyph_x << 8;
    the_font->glyph_y = the_font->glyph_y >> 8 | the_font->glyph_y << 8;
    the_font->spacing_x = the_font->spacing_x >> 8 | the_font->spacing_x << 8;
    the_font->spacing_y = the_font->spacing_y >> 8 | the_font->spacing_y << 8;

    data_size = sizeof(br_int_8) * 256;
    the_font->width = BrMemAllocate(data_size, 0xA7u);
    fread(the_font->width, data_size, 1, f);
    data_size = sizeof(br_uint_16) * 256;
    the_font->encoding = BrMemAllocate(data_size, 0xA8u);
    fread(the_font->encoding, data_size, 1u, f);
    for (i = 0; i < 256; i++) {
        the_font->encoding[i] = the_font->encoding[i] >> 8 | the_font->encoding[i] << 8;
    }
    PossibleService();
    fread(&data_size, sizeof(tU32), 1u, f);
    data_size = BrSwap32(data_size);
    PossibleService();
    the_font->glyphs = BrMemAllocate(data_size, 0xA9u);
    fread(the_font->glyphs, data_size, 1u, f);
    fclose(f);
    return the_font;
}

// Offset: 33044
// Size: 262
void LoadParts() {
    int j;
    NOT_IMPLEMENTED();
}

// Offset: 33308
// Size: 149
void UnlockParts() {
    int j;
    NOT_IMPLEMENTED();
}

// Offset: 33460
// Size: 72
br_pixelmap* LoadChromeFont() {
    NOT_IMPLEMENTED();
}

// Offset: 33532
// Size: 49
// EAX: pThe_font
void DisposeChromeFont(br_pixelmap* pThe_font) {
    NOT_IMPLEMENTED();
}

// Offset: 33584
// Size: 140
// EAX: pF
// EDX: pString_list
// EBX: pCount
int GetALineAndInterpretCommand(FILE* pF, char** pString_list, int pCount) {
    int i;
    char s[256];
    char* str;
    NOT_IMPLEMENTED();
}

// Offset: 33724
// Size: 101
// EAX: pF
int GetAnInt(FILE* pF) {
    char s[256];
    char* str;
    int result;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &result);
    return result;
}

// Offset: 33828
// Size: 101
// EAX: pF
float GetAFloat(FILE* pF) {
    char s[256];
    char* str;
    float result;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%f", &result);
    return result;
}

// Offset: 33932
// Size: 107
// EAX: pF
float GetAFloatPercent(FILE* pF) {
    char s[256];
    char* str;
    float result;
    NOT_IMPLEMENTED();
}

// Offset: 34040
// Size: 131
// EAX: pF
// EDX: pF1
// EBX: pF2
void GetPairOfFloats(FILE* pF, float* pF1, float* pF2) {
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%f", pF1);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%f", pF2);
}

// Offset: 34172
// Size: 169
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetThreeFloats(FILE* pF, float* pF1, float* pF2, float* pF3) {
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%f", pF1);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%f", pF2);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%f", pF3);
}

// Offset: 34344
// Size: 131
// EAX: pF
// EDX: pF1
// EBX: pF2
void GetPairOfInts(FILE* pF, int* pF1, int* pF2) {
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", pF1);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%d", pF2);
}

// Offset: 34476
// Size: 169
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetThreeInts(FILE* pF, int* pF1, int* pF2, int* pF3) {
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", pF1);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%d", pF2);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%d", pF3);
}

// Offset: 34648
// Size: 197
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetThreeIntsAndAString(FILE* pF, int* pF1, int* pF2, int* pF3, char* pS) {
    char s[256];
    char* str;
    NOT_IMPLEMENTED();
}

// Offset: 34848
// Size: 208
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetFourInts(FILE* pF, int* pF1, int* pF2, int* pF3, int* pF4) {
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", pF1);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%d", pF2);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%d", pF3);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%d", pF4);
}

// Offset: 35056
// Size: 51
// EAX: pF
br_scalar GetAScalar(FILE* pF) {
    return GetAFloat(pF);
    NOT_IMPLEMENTED();
}

// Offset: 35108
// Size: 69
// EAX: pF
// EDX: pS1
// EBX: pS2
void GetPairOfScalars(FILE* pF, br_scalar* pS1, br_scalar* pS2) {
    NOT_IMPLEMENTED();
}

// Offset: 35180
// Size: 81
// EAX: pF
// EDX: pS1
// EBX: pS2
// ECX: pS3
void GetThreeScalars(FILE* pF, br_scalar* pS1, br_scalar* pS2, br_scalar* pS3) {
    NOT_IMPLEMENTED();
}

// Offset: 35264
// Size: 240
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetFourScalars(FILE* pF, br_scalar* pF1, br_scalar* pF2, br_scalar* pF3, br_scalar* pF4) {
    char s[256];
    char* str;
    float f1;
    float f2;
    float f3;
    float f4;
    NOT_IMPLEMENTED();
}

// Offset: 35504
// Size: 285
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetFiveScalars(FILE* pF, br_scalar* pF1, br_scalar* pF2, br_scalar* pF3, br_scalar* pF4, br_scalar* pF5) {
    char s[256];
    char* str;
    float f1;
    float f2;
    float f3;
    float f4;
    float f5;
    NOT_IMPLEMENTED();
}

// Offset: 35792
// Size: 152
// EAX: pF
// EDX: pNumber
// EBX: pScalars
void GetNScalars(FILE* pF, int pNumber, br_scalar* pScalars) {
    char s[256];
    char* str;
    float fleurting_point_numero;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 35944
// Size: 157
// EAX: pF
// EDX: pF1
// EBX: pF2
void GetPairOfFloatPercents(FILE* pF, float* pF1, float* pF2) {
    char s[256];
    char* str;
    NOT_IMPLEMENTED();
}

// Offset: 36104
// Size: 208
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetThreeFloatPercents(FILE* pF, float* pF1, float* pF2, float* pF3) {
    char s[256];
    char* str;
    NOT_IMPLEMENTED();
}

// Offset: 36312
// Size: 82
// EAX: pF
// EDX: pString
void GetAString(FILE* pF, char* pString) {
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    strcpy(pString, str);
}

// Offset: 36396
// Size: 49
void AboutToLoadFirstCar() {
    NOT_IMPLEMENTED();
}

// Offset: 36448
// Size: 275
// EAX: pRace_info
void LoadOpponentsCars(tRace_info* pRace_info) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 36724
// Size: 184
// EAX: pRace_info
void DisposeOpponentsCars(tRace_info* pRace_info) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 36908
// Size: 211
void LoadMiscStrings() {
    int i;
    FILE* f;
    char s[256];
    tPath_name the_path;
    LOG_TRACE("()");

    PathCat(the_path, gApplication_path, "TEXT.TXT");
    f = DRfopen(the_path, "rt");
    if (!f) {
        FatalError(99);
    }
    for (i = 0; i < 250; i++) {
        GetALineAndDontArgue(f, s);
        gMisc_strings[i] = BrMemAllocate(strlen(s) + 1, 0xABu);
        strcpy(gMisc_strings[i], s);
        if (feof(f)) {
            break;
        }
    }
    fclose(f);
}

// Offset: 37120
// Size: 53
// EAX: pThe_race
void FillInRaceInfo(tRace_info* pThe_race) {
    NOT_IMPLEMENTED();
}

// Offset: 37176
// Size: 1156
// EAX: pFilename
// EDX: pMode
FILE* OldDRfopen(char* pFilename, char* pMode) {
    FILE* fp;
    FILE* file_ptr;
    FILE* test1;
    FILE* test2;
    char* data_dir;
    tPath_name CD_dir;
    tPath_name path_file;
    tPath_name source_check;
    static int source_exists = 1;
    int len;
    int c;

    LOG_TRACE("(\"%s\", \"%s\")", pFilename, pMode);

    fp = fopen(pFilename, pMode);

    if (fp) {
        len = strlen(pFilename);
        if (gDecode_thing != 0) {
            if (strcmp(&pFilename[len - 4], ".TXT") == 0
                && strcmp(&pFilename[len - 12], "DKEYMAP0.TXT") != 0
                && strcmp(&pFilename[len - 12], "DKEYMAP1.TXT") != 0
                && strcmp(&pFilename[len - 12], "DKEYMAP2.TXT") != 0
                && strcmp(&pFilename[len - 12], "DKEYMAP3.TXT") != 0
                && strcmp(&pFilename[len - 12], "KEYMAP_0.TXT") != 0
                && strcmp(&pFilename[len - 12], "KEYMAP_1.TXT") != 0
                && strcmp(&pFilename[len - 12], "KEYMAP_2.TXT") != 0
                && strcmp(&pFilename[len - 12], "KEYMAP_3.TXT") != 0
                && strcmp(&pFilename[len - 11], "OPTIONS.TXT") != 0
                && strcmp(&pFilename[len - 11], "KEYNAMES.TXT") != 0
                && strcmp(&pFilename[len - 10], "KEYMAP.TXT") != 0
                && strcmp(&pFilename[len - 9], "PATHS.TXT") != 0
                && strcmp(&pFilename[len - 11], "PRATCAM.TXT") != 0) {
                c = fgetc(fp);
                if (c != gDecode_thing) {
                    fclose(fp);
                    LOG_WARN("Unexpected encoding character");
                    return NULL;
                }
                ungetc(c, fp);
                return fp;
            }
        }
    }
    if (gCD_fully_installed) {
        return fp;
    }
    // source_exists = 1 means we haven't checked the CD yet
    if (source_exists == 1) {
        strcpy(path_file, "DATA");
        strcat(path_file, gDir_separator);
        strcat(path_file, "PATHS.TXT");

        if (!PDCheckDriveExists(path_file)) {
            source_exists = 0;
            LOG_WARN("PATHS.TXT not found");
            return NULL;
        }
        test1 = fopen(path_file, "rt");
        if (!test1) {
            source_exists = 0;
            LOG_WARN("PATHS.TXT couldnt be opened");
            return NULL;
        }

        GetALineAndDontArgue(test1, source_check);
        strcat(source_check, gDir_separator);
        strcat(source_check, "DATA");
        strcat(source_check, gDir_separator);
        strcat(source_check, "GENERAL.TXT");

        fclose(test1);
        if (PDCheckDriveExists(source_check)) {
            source_exists++;
        } else {
            PDFatalError("Carmageddon CD not in drive.");
            if (gCD_fully_installed) {
                source_exists = 0;
            }
        }
    }
    if (!source_exists) {
        return fp;
    }

    data_dir = strstr(pFilename, "DATA");
    if (data_dir != NULL) {
        if (GetCDPathFromPathsTxtFile(CD_dir)) {
            strcat(CD_dir, gDir_separator);
            strcat(CD_dir, data_dir);
            if (PDCheckDriveExists(CD_dir)) {
                fp = fopen(CD_dir, pMode);
            }
        }
    }
    return fp;
}

// Offset: 38332
// Size: 44
void AllowOpenToFail() {
    NOT_IMPLEMENTED();
}

// Offset: 38376
// Size: 34
void DoNotAllowOpenToFail() {
    NOT_IMPLEMENTED();
}

// Offset: 38412
// Size: 189
// EAX: pFilename
// EDX: pMode
FILE* DRfopen(char* pFilename, char* pMode) {
    FILE* result;
    tPath_name CD_dir;
    char msg[336];

    result = OldDRfopen(pFilename, pMode);
    if (!result && !gAllow_open_to_fail) {
        if (GetCDPathFromPathsTxtFile(CD_dir) && !PDCheckDriveExists(CD_dir)) {
            if (gMisc_strings[0]) {
                PDFatalError(GetMiscString(243));
            } else {
                PDFatalError("Could not find the Carmageddon CD");
            }
            sprintf(msg, "DRfopen( \"%s\", \"%s\" ) failed", pFilename, pMode);
            PDFatalError(msg);
        }
    }
    if (!result) {
        LOG_WARN("DRfopen failed");
    }
    return result;
}

// Offset: 38604
// Size: 178
// EAX: pPath_name
int GetCDPathFromPathsTxtFile(char* pPath_name) {
    static int got_it_already = 0;
    static tPath_name cd_pathname;
    FILE* paths_txt_fp;
    tPath_name paths_txt;
    LOG_TRACE("(\"%s\")", pPath_name);

    if (!got_it_already) {
        sprintf(paths_txt, "%s%s%s", gApplication_path, gDir_separator, "PATHS.TXT");
        paths_txt_fp = fopen(paths_txt, "rt");
        if (!paths_txt_fp) {
            return 0;
        }
        GetALineAndDontArgue(paths_txt_fp, cd_pathname);
        fclose(paths_txt_fp);
        got_it_already = 1;
    }
    memcpy(pPath_name, cd_pathname, 256);
    return 1;
}

// Offset: 38784
// Size: 44
int TestForOriginalCarmaCDinDrive() {
    NOT_IMPLEMENTED();
}

// Offset: 38828
// Size: 45
int OriginalCarmaCDinDrive() {
    NOT_IMPLEMENTED();
}

// Offset: 38876
// Size: 63
int CarmaCDinDriveOrFullGameInstalled() {
    NOT_IMPLEMENTED();
}

// Offset: 38940
// Size: 191
// EAX: pF
// EDX: pOptions
void ReadNetworkSettings(FILE* pF, tNet_game_options* pOptions) {
    LOG_TRACE("(%p, %p)", pF, pOptions);

    pOptions->enable_text_messages = GetAnInt(pF);
    pOptions->show_players_on_map = GetAnInt(pF);
    pOptions->show_peds_on_map = GetAnInt(pF);
    pOptions->show_powerups_on_map = GetAnInt(pF);
    pOptions->powerup_respawn = GetAnInt(pF);
    pOptions->open_game = GetAnInt(pF);
    pOptions->grid_start = GetAnInt(pF);
    pOptions->race_sequence_type = GetAnInt(pF);
    pOptions->random_car_choice = GetAnInt(pF);
    pOptions->car_choice = GetAnInt(pF);
    pOptions->starting_money_index = GetAnInt(pF);
}

// Offset: 39132
// Size: 378
// EAX: pF
// EDX: pIndex
int PrintNetOptions(FILE* pF, int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 39512
// Size: 1398
int SaveOptions() {
    FILE* f;
    NOT_IMPLEMENTED();
}

// Offset: 40912
// Size: 1643
int RestoreOptions() {
    tPath_name the_path;
    FILE* f;
    char line[80];
    char token[80];
    char* s;
    float arg;
    LOG_TRACE("()");

    gProgram_state.music_volume = 4;
    gProgram_state.effects_volume = 4;
    DefaultNetSettings();

    PathCat(the_path, gApplication_path, "OPTIONS.TXT");
    f = DRfopen(the_path, "rt");
    if (!f) {
        LOG_WARN("Failed to open OPTIONS.TXT");
        return 0;
    }
    while (fgets(line, 80, f)) {
        if (sscanf(line, "%79s%f", token, &arg) == 2) {
            if (!strcmp(token, "YonFactor")) {
                SetYonFactor(arg);
            } else if (!strcmp(token, "SkyTextureOn")) {
                SetSkyTextureOn((int)arg);
            } else if (!strcmp(token, "CarTexturingLevel")) {
                SetCarTexturingLevel((tCar_texturing_level)arg);
            } else if (!strcmp(token, "RoadTexturingLevel")) {
                SetRoadTexturingLevel((tRoad_texturing_level)arg);
            } else if (!strcmp(token, "WallTexturingLevel")) {
                SetWallTexturingLevel((tWall_texturing_level)arg);
            } else if (!strcmp(token, "ShadowLevel")) {
                SetShadowLevel((tShadow_level)arg);
            } else if (!strcmp(token, "DepthCueingOn")) {
                SetDepthCueingOn((int)arg);
            } else if (!strcmp(token, "Yon")) {
                SetYon(arg);
            } else if (!strcmp(token, "CarSimplificationLevel")) {
                SetCarSimplificationLevel((int)arg);
            } else if (!strcmp(token, "AccessoryRendering")) {
                SetAccessoryRendering((int)arg);
            } else if (!strcmp(token, "SmokeOn")) {
                SetSmokeOn((int)arg);
            } else if (!strcmp(token, "SoundDetailLevel")) {
                SetSoundDetailLevel((int)arg);
            } else if (!strcmp(token, "ScreenSize")) {
                SetScreenSize((int)arg);
            } else if (!strcmp(token, "MapRenderX")) {
                gMap_render_x = arg;
            } else if (!strcmp(token, "MapRenderY")) {
                gMap_render_y = arg;
            } else if (!strcmp(token, "MapRenderWidth")) {
                gMap_render_width = arg;
            } else if (!strcmp(token, "MapRenderHeight")) {
                gMap_render_height = arg;
            } else if (!strcmp(token, "PlayerName")) {
                fgets(line, 80, f);
                s = strtok(line, "\n\r");
                strcpy(gProgram_state.player_name[(int)arg], s);
            } else if (!strcmp(token, "EVolume")) {
                gProgram_state.effects_volume = (int)arg;
            } else if (!strcmp(token, "MVolume")) {
                gProgram_state.music_volume = (int)arg;
            } else if (!strcmp(token, "KeyMapIndex")) {
                gKey_map_index = (int)arg;
            } else if (!strcmp(token, "Joystick_min1x")) {
                gJoystick_min1x = (int)arg;
            } else if (!strcmp(token, "Joystick_min1y")) {
                gJoystick_min1y = (int)arg;
            } else if (!strcmp(token, "Joystick_min2x")) {
                gJoystick_min2x = (int)arg;
            } else if (!strcmp(token, "Joystick_min2y")) {
                gJoystick_min2y = (int)arg;
            } else if (!strcmp(token, "Joystick_range1x")) {
                gJoystick_range1x = (int)arg;
            } else if (!strcmp(token, "Joystick_range1y")) {
                gJoystick_range1y = (int)arg;
            } else if (!strcmp(token, "Joystick_range2x")) {
                gJoystick_range2x = (int)arg;
            } else if (!strcmp(token, "Joystick_range2y")) {
                gJoystick_range2y = (int)arg;
            } else if (!strcmp(token, "NetName")) {
                fgets(line, 80, f);
                s = strtok(line, "\n\r");
                strcpy(gNet_player_name, s);
            } else if (!strcmp(token, "NETGAMETYPE")) {
                gLast_game_type = (tNet_game_type)arg;
            } else if (!strcmp(token, "NETSETTINGS")) {
                ReadNetworkSettings(f, &gNet_settings[(int)arg]);
            }
        }
    }
    fclose(f);
    return 1;
}

// Offset: 42556
// Size: 75
void InitFunkGrooveFlags() {
    NOT_IMPLEMENTED();
}
