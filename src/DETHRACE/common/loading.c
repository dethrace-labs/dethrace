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
char* gNet_avail_names[4] = { "never", "eagle", "hawk", "all" };
char* gDamage_names[12];
char* gDrivable_car_names[6];
char* gYour_car_names[2][6];
char gDef_def_water_screen_name[32];
tHeadup_info gHeadup_image_info[31];
int gAllow_open_to_fail = 1;
br_material* gDestn_screen_mat;
br_material* gSource_screen_mat;
char* gRaces_file_names[9] = {
    "RACES.TXT",
    "NETRACES.TXT",
    "NETRACES.TXT",
    "PEDRACES.TXT",
    "RACES.TXT",
    "RACES.TXT",
    "NETRACES.TXT",
    "NETRACES.TXT",
    "NETRACES.TXT"
};
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

tU32 ReadU32(FILE* pF) {
    tU32 raw_long;
    NOT_IMPLEMENTED();
}

tU16 ReadU16(FILE* pF) {
    tU16 raw_short;
    NOT_IMPLEMENTED();
}

tU8 ReadU8(FILE* pF) {
    tU8 raw_byte;
    NOT_IMPLEMENTED();
}

tS32 ReadS32(FILE* pF) {
    tS32 raw_long;
    NOT_IMPLEMENTED();
}

tS16 ReadS16(FILE* pF) {
    tS16 raw_short;
    NOT_IMPLEMENTED();
}

tS8 ReadS8(FILE* pF) {
    tS8 raw_byte;
    NOT_IMPLEMENTED();
}

void WriteU32L(FILE* pF, tU32 pNumber) {
    tU32 raw_long;
    NOT_IMPLEMENTED();
}

void WriteU16L(FILE* pF, tU16 pNumber) {
    tU16 raw_short;
    NOT_IMPLEMENTED();
}

void WriteU8L(FILE* pF, tU8 pNumber) {
    tU8 raw_byte;
    NOT_IMPLEMENTED();
}

void WriteS32L(FILE* pF, tS32 pNumber) {
    tS32 raw_long;
    NOT_IMPLEMENTED();
}

void WriteS16L(FILE* pF, tS16 pNumber) {
    tS16 raw_short;
    NOT_IMPLEMENTED();
}

void WriteS8L(FILE* pF, tS8 pNumber) {
    tS8 raw_byte;
    NOT_IMPLEMENTED();
}

void SkipBytes(FILE* pF, int pBytes_to_skip) {
    NOT_IMPLEMENTED();
}

tU32 MemReadU32(char** pPtr) {
    tU32 raw_long;
    memcpy(&raw_long, *pPtr, 4);
    *pPtr += 4;
    return raw_long;
}

tU16 MemReadU16(char** pPtr) {
    tU16 raw_short;
    memcpy(&raw_short, *pPtr, 2);
    *pPtr += 2;
    return raw_short;
}

tU8 MemReadU8(char** pPtr) {
    return *(*pPtr)++;
}

tS32 MemReadS32(char** pPtr) {
    tS32 raw_long;
    NOT_IMPLEMENTED();
}

tS16 MemReadS16(char** pPtr) {
    tS16 raw_short;
    memcpy(&raw_short, *pPtr, 2);
    *pPtr += 2;
    return raw_short;
}

tS8 MemReadS8(char** pPtr) {
    tS8 raw_byte;
    raw_byte = **pPtr;
    (*pPtr)++;
    return raw_byte;
}

void MemSkipBytes(char** pPtr, int pBytes_to_skip) {
    *pPtr += pBytes_to_skip;
}

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

void FinishLoadingGeneral() {
    /*dword_1298C0 = */ BrMaterialFind(gDef_def_water_screen_name);
}

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

br_uint_32 LoadPixelmaps(char* pFile_name, br_pixelmap** pPixelmaps, br_uint_16 pNum) {
    tPath_name path;
    int count;

    PathCat(path, gApplication_path, gGraf_specs[gGraf_spec_index].data_dir_name);
    PathCat(path, path, "PIXELMAP");

    PathCat(path, path, pFile_name);
    gAllow_open_to_fail = 1;
    count = DRPixelmapLoadMany(path, pPixelmaps, pNum);
    if (!count) {
        PathCat(path, gApplication_path, "PIXELMAP");
        PathCat(path, path, pFile_name);
        count = DRPixelmapLoadMany(path, pPixelmaps, pNum);
    }
    return count;
}

br_pixelmap* LoadShadeTable(char* pName) {
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

br_material* LoadMaterial(char* pName) {
    tPath_name the_path;
    br_material* result;
    NOT_IMPLEMENTED();
}

br_model* LoadModel(char* pName) {
    tPath_name the_path;
    br_model* model;
    LOG_TRACE("(\"%s\")", pName);

    PossibleService();
    PathCat(the_path, gApplication_path, "MODELS");
    PathCat(the_path, the_path, pName);
    return BrModelLoad(the_path);
}

br_actor* LoadActor(char* pName) {
    tPath_name the_path;
    LOG_TRACE("(\"%s\")", pName);

    PossibleService();
    PathCat(the_path, gApplication_path, "ACTORS");
    PathCat(the_path, the_path, pName);
    return BrActorLoad(the_path);
}

void DRLoadPalette(char* pPath_name) {
    br_pixelmap* palette_array[100];
    int number_of_palettes;

    number_of_palettes = DRPixelmapLoadMany(pPath_name, palette_array, 100);
    BrTableAddMany(palette_array, number_of_palettes);
}

void DRLoadShadeTable(char* pPath_name) {
    br_pixelmap* table_array[100];
    int number_of_tables;

    number_of_tables = DRPixelmapLoadMany(pPath_name, table_array, 100);
    BrTableAddMany(table_array, number_of_tables);
}

void RezeroPixelmaps(br_pixelmap** pPixelmap_array, int pCount) {
    NOT_IMPLEMENTED();
}

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

void DRLoadMaterials(char* pPath_name) {
    br_material* material_array[100];
    int number_of_materials;

    PossibleService();
    number_of_materials = BrMaterialLoadMany(pPath_name, material_array, 100);
    BrMaterialAddMany(material_array, number_of_materials);
}

void DRLoadModels(char* pPath_name) {
    br_model* model_array[100];
    int number_of_models;
    NOT_IMPLEMENTED();
}

void DRLoadActors(char* pPath_name) {
    br_actor* actor_array[100];
    int number_of_actors;
    int i;
    NOT_IMPLEMENTED();
}

void DRLoadLights(char* pPath_name) {
    br_actor* light_array[100];
    int number_of_lights;
    int i;
    STUB();
}

void LoadInFiles(char* pThe_base_path, char* pThe_dir_name, void (*pLoad_routine)(char*)) {
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

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

void LoadInRegistees() {
    LoadInRegisteeDir(gApplication_path);
}

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

void LoadInterfaceStuff(int pWithin_race) {
    tPath_name path;
    int i;
    LOG_TRACE("(%d)", pWithin_race);

    if (gProgram_state.sausage_eater_mode) {
        strcpy(path, "GHANDX.PIX");
    } else {
        strcpy(path, "HANDX.PIX");
    }
    PossibleService();
    if (!gCursors[0] && !LoadPixelmaps(path, gCursors, 4)) {
        FatalError(22);
    }
    if (gProgram_state.sausage_eater_mode) {
        strcpy(path, "GHANDPX.PIX");
    } else {
        strcpy(path, "HANDPX.PIX");
    }
    PossibleService();
    if (!gCursors[4] && !LoadPixelmaps(path, &gCursors[4], 4)) {
        FatalError(22);
    }
    PossibleService();
    if (!gCursor_giblet_images[0] && !LoadPixelmaps("CURSGIBX.PIX", gCursor_giblet_images, 18u)) {
        FatalError(23);
    }
}

void UnlockInterfaceStuff() {
    int i;
    LOG_TRACE("()");
    for (i = 0; i < 4; i++) {
        if (gCursors[i]) {
            BrPixelmapFree(gCursors[i]);
            gCursors[i] = NULL;
        }
    }
    PossibleService();
    for (i = 0; i < 4; i++) {
        if (gCursors[i + 4]) {
            BrPixelmapFree(gCursors[i + 4]);
            gCursors[i + 4] = NULL;
        }
    }
    PossibleService();
    for (i = 0; i < 18; i++) {
        if (gCursor_giblet_images[i]) {
            BrPixelmapFree(gCursor_giblet_images[i]);
            gCursor_giblet_images[i] = NULL;
        }
    }
}

void InitInterfaceLoadState() {
    LOG_TRACE("()");
    memset(gCursors, 0, sizeof(gCursors));
}

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

void KillWindscreen(br_model* pModel, br_material* pMaterial) {
    br_face* face;
    int i;
    NOT_IMPLEMENTED();
}

void DropOffDyingPeds(tCar_spec* pCar) {
    br_actor* child;
    br_actor* next;
    NOT_IMPLEMENTED();
}

void DisposeCar(tCar_spec* pCar_spec, int pOwner) {
    int i;
    int j;
    NOT_IMPLEMENTED();
}

void AdjustCarCoordinates(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

void LoadSpeedo(FILE* pF, int pIndex, tCar_spec* pCar_spec) {
    tPath_name the_path;
    char s[256];
    char* str;
    char the_char1;
    char the_char2;
    NOT_IMPLEMENTED();
}

void LoadTacho(FILE* pF, int pIndex, tCar_spec* pCar_spec) {
    tPath_name the_path;
    char s[256];
    char* str;
    char the_char1;
    char the_char2;
    NOT_IMPLEMENTED();
}

void LoadHeadups(FILE* pF, int pIndex, tCar_spec* pCar_spec) {
    char s[256];
    char* str;
    int j;
    int number_of_slots;
    NOT_IMPLEMENTED();
}

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

void LoadGear(FILE* pF, int pIndex, tCar_spec* pCar_spec) {
    tPath_name the_path;
    char s[256];
    char* str;
    NOT_IMPLEMENTED();
}

void AddRefOffset(int* pRef_holder) {
    NOT_IMPLEMENTED();
}

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

br_uint_32 LinkModel(br_actor* pActor, tModel_pool* pModel_pool) {
    int i;
    NOT_IMPLEMENTED();
}

void FreeUpBonnetModels(br_model** pModel_array, int pModel_count) {
    int i;
    NOT_IMPLEMENTED();
}

void LinkModelsToActor(br_actor* pActor, br_model** pModel_array, int pModel_count) {
    tModel_pool model_pool;
    NOT_IMPLEMENTED();
}

void ReadShrapnelMaterials(FILE* pF, tCollision_info* pCar_spec) {
    char s[256];
    char version;
    int i;
    NOT_IMPLEMENTED();
}

void CloneCar(tCar_spec** pOutput_car, tCar_spec* pInput_car) {
    int i;
    NOT_IMPLEMENTED();
}

void DisposeClonedCar(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

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

void MungeWindscreen(br_model* pModel) {
    br_face* face;
    int i;
    NOT_IMPLEMENTED();
}

void SetModelFlags(br_model* pModel, int pOwner) {
    NOT_IMPLEMENTED();
}

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

void LoadHeadupImages() {
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

void DisposeHeadupImages() {
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

FILE* OpenRaceFile() {
    FILE* f;
    tPath_name the_path;

    PathCat(the_path, gApplication_path, gRaces_file_names[gCurrent_race_file_index]);
    f = DRfopen(the_path, "rt");
    if (!f) {
        FatalError(50);
    }
    return f;
}

void SkipRestOfRace(FILE* pF) {
    int j;
    int k;
    int text_chunk_count;
    int line_count;
    char s[256];

    GetALineAndDontArgue(pF, s);
    GetALineAndDontArgue(pF, s);

    text_chunk_count = GetAnInt(pF);
    for (j = 0; j < text_chunk_count; j++) {

        PossibleService();
        GetALineAndDontArgue(pF, s);
        GetALineAndDontArgue(pF, s);
        line_count = GetAnInt(pF);
        while (line_count > 8) {
            GetALineAndDontArgue(pF, s);
            line_count--;
        }
        for (k = 0; k < line_count; k++) {
            GetALineAndDontArgue(pF, s);
        }
    }
}

void LoadRaces(tRace_list_spec* pRace_list, int* pCount, int pRace_type_index) {
    FILE* f;
    int i;
    int j;
    int k;
    int number_of_racers;
    int last_race = 0;
    char s[256];
    char* str;
    LOG_TRACE("(%p, %p, %d)", pRace_list, pCount, pRace_type_index);

    gCurrent_race_file_index = pRace_type_index + 1;
    f = OpenRaceFile();
    number_of_racers = 0;
    while (!last_race) {
        GetALineAndDontArgue(f, s);
        if (strcmp(s, "END") == 0) {
            last_race = 1;
        } else {
            SkipRestOfRace(f);
            //s = (s + 48);
            number_of_racers++;
        }
    }

    LOG_DEBUG("race count %d", number_of_racers);

    *pCount = number_of_racers;
    fclose(f);
    j = 0;
    for (i = 0; i < number_of_racers; i++) {
        pRace_list[i].suggested_rank = 99 - j / (number_of_racers - 3);
        if (i >= 3) {
            pRace_list[i].rank_required = pRace_list[i - 2].suggested_rank;
        } else {
            pRace_list[i].rank_required = 99;
        }
        j += 100;
    }

    pRace_list[number_of_racers - 1].rank_required = 1;
    if (pRace_list[number_of_racers - 2].rank_required == 1) {
        --*pCount;
    }

    for (i = 0; i < number_of_racers; i++) {
        if (i < *pCount - 3) {
            pRace_list[i].best_rank = pRace_list[i + 3].suggested_rank;
        } else {
            pRace_list[i].best_rank = 1;
        }
    }
}

void UnlockOpponentMugshot(int pIndex) {
    NOT_IMPLEMENTED();
}

void LoadOpponentMugShot(int pIndex) {
    NOT_IMPLEMENTED();
}

void DisposeOpponentGridIcon(tRace_info* pRace_info, int pIndex) {
    NOT_IMPLEMENTED();
}

void LoadOpponentGridIcon(tRace_info* pRace_info, int pIndex) {
    NOT_IMPLEMENTED();
}

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

void DisposeRaceInfo(tRace_info* pRace_info) {
    int i;
    int j;
    int k;
    tText_chunk* the_chunk;
    NOT_IMPLEMENTED();
}

void LoadGridIcons(tRace_info* pRace_info) {
    int i;
    NOT_IMPLEMENTED();
}

void DisposeGridIcons(tRace_info* pRace_info) {
    int i;
    NOT_IMPLEMENTED();
}

void LoadOpponents() {
    FILE* f;
    tPath_name the_path;
    int i;
    int j;
    int k;
    char s[256];
    char* str;
    tText_chunk* the_chunk;
    LOG_TRACE("()");

    PathCat(the_path, gApplication_path, "OPPONENT.TXT");
    f = DRfopen(the_path, "rt");
    if (!f) {
        FatalError(54);
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &gNumber_of_racers);
    gOpponents = BrMemAllocate(sizeof(tOpponent) * gNumber_of_racers, kMem_oppo_array);

    for (i = 0; i < gNumber_of_racers; i++) {
        PossibleService();
        GetALineAndDontArgue(f, gOpponents[i].name);
        if (!strcmp(gOpponents[i].name, "END")) {
            FatalError(55);
        }

        GetALineAndDontArgue(f, gOpponents[i].abbrev_name);
        gOpponents[i].car_number = GetAnInt(f);
        gOpponents[i].strength_rating = GetAnInt(f);
        gOpponents[i].network_availability = GetALineAndInterpretCommand(f, gNet_avail_names, 4);

        GetALineAndDontArgue(f, s);
        str = strtok(s, "\t ,/");
        strcpy(gOpponents[i].mug_shot_name, str);

        gOpponents[i].mug_shot_image_data = NULL;
        gOpponents[i].grid_icon_image = NULL;
        gOpponents[i].stolen_car_image_data = NULL;

        GetALineAndDontArgue(f, s);
        str = strtok(s, "\t ,/");
        strcpy(gOpponents[i].car_file_name, str);
        GetALineAndDontArgue(f, s);
        str = strtok(s, "\t ,/");
        strcpy(gOpponents[i].stolen_car_flic_name, str);

        gOpponents[i].text_chunk_count = GetAnInt(f);
        gOpponents[i].text_chunks = BrMemAllocate(sizeof(tText_chunk) * gOpponents[i].text_chunk_count, kMem_oppo_text_chunk);

        for (j = 0; j < gOpponents[i].text_chunk_count; j++) {
            the_chunk = &gOpponents[i].text_chunks[j];
            PossibleService();
            GetPairOfInts(f, &the_chunk->x_coord, &the_chunk->y_coord);
            GetPairOfInts(f, &the_chunk->frame_cue, &the_chunk->frame_end);
            the_chunk->line_count = GetAnInt(f);
            while (the_chunk->line_count > 8) {
                the_chunk->line_count--;
                GetALineAndDontArgue(f, s);
            }

            for (k = 0; k < the_chunk->line_count; k++) {
                GetALineAndDontArgue(f, s);
                the_chunk->text[k] = BrMemAllocate(strlen(s) + 1, kMem_oppo_text_str);
                strcpy(the_chunk->text[k], s);
            }
        }

        gOpponents[i].dead = 0;
        InitOpponentPsyche(i);
    }
    GetALineAndDontArgue(f, s);
    if (strcmp(s, "END")) {
        FatalError(55);
    }
    return fclose(f);
}

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
    the_font = BrMemAllocate(sizeof(br_font), kMem_br_font);

    // we read 0x18 bytes as that is the size of the struct in 32 bit code.
    fread(the_font, 0x18, 1u, f);
    the_font->flags = BrSwap32(the_font->flags);

    // swap endianness
    the_font->glyph_x = the_font->glyph_x >> 8 | the_font->glyph_x << 8;
    the_font->glyph_y = the_font->glyph_y >> 8 | the_font->glyph_y << 8;
    the_font->spacing_x = the_font->spacing_x >> 8 | the_font->spacing_x << 8;
    the_font->spacing_y = the_font->spacing_y >> 8 | the_font->spacing_y << 8;

    data_size = sizeof(br_int_8) * 256;
    the_font->width = BrMemAllocate(data_size, kMem_br_font_wid);
    fread(the_font->width, data_size, 1, f);
    data_size = sizeof(br_uint_16) * 256;
    the_font->encoding = BrMemAllocate(data_size, kMem_br_font_enc);
    fread(the_font->encoding, data_size, 1u, f);
    for (i = 0; i < 256; i++) {
        the_font->encoding[i] = the_font->encoding[i] >> 8 | the_font->encoding[i] << 8;
    }
    PossibleService();
    fread(&data_size, sizeof(tU32), 1u, f);
    data_size = BrSwap32(data_size);
    PossibleService();
    the_font->glyphs = BrMemAllocate(data_size, kMem_br_font_glyphs);
    fread(the_font->glyphs, data_size, 1u, f);
    fclose(f);
    return the_font;
}

void LoadParts() {
    int j;
    NOT_IMPLEMENTED();
}

void UnlockParts() {
    int j;
    NOT_IMPLEMENTED();
}

br_pixelmap* LoadChromeFont() {
    NOT_IMPLEMENTED();
}

void DisposeChromeFont(br_pixelmap* pThe_font) {
    NOT_IMPLEMENTED();
}

int GetALineAndInterpretCommand(FILE* pF, char** pString_list, int pCount) {
    int i;
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);

    str = strtok(s, "\t ,/");
    if (pCount <= 0) {
        return -1;
    }
    for (i = 0; i < pCount; i++) {
        if (strcmp(s, pString_list[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int GetAnInt(FILE* pF) {
    char s[256];
    char* str;
    int result;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &result);
    return result;
}

float GetAFloat(FILE* pF) {
    char s[256];
    char* str;
    float result;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%f", &result);
    return result;
}

float GetAFloatPercent(FILE* pF) {
    char s[256];
    char* str;
    float result;
    NOT_IMPLEMENTED();
}

void GetPairOfFloats(FILE* pF, float* pF1, float* pF2) {
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%f", pF1);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%f", pF2);
}

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

void GetPairOfInts(FILE* pF, int* pF1, int* pF2) {
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", pF1);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%d", pF2);
}

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

void GetThreeIntsAndAString(FILE* pF, int* pF1, int* pF2, int* pF3, char* pS) {
    char s[256];
    char* str;
    NOT_IMPLEMENTED();
}

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

br_scalar GetAScalar(FILE* pF) {
    return GetAFloat(pF);
    NOT_IMPLEMENTED();
}

void GetPairOfScalars(FILE* pF, br_scalar* pS1, br_scalar* pS2) {
    NOT_IMPLEMENTED();
}

void GetThreeScalars(FILE* pF, br_scalar* pS1, br_scalar* pS2, br_scalar* pS3) {
    NOT_IMPLEMENTED();
}

void GetFourScalars(FILE* pF, br_scalar* pF1, br_scalar* pF2, br_scalar* pF3, br_scalar* pF4) {
    char s[256];
    char* str;
    float f1;
    float f2;
    float f3;
    float f4;
    NOT_IMPLEMENTED();
}

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

void GetNScalars(FILE* pF, int pNumber, br_scalar* pScalars) {
    char s[256];
    char* str;
    float fleurting_point_numero;
    int i;
    NOT_IMPLEMENTED();
}

void GetPairOfFloatPercents(FILE* pF, float* pF1, float* pF2) {
    char s[256];
    char* str;
    NOT_IMPLEMENTED();
}

void GetThreeFloatPercents(FILE* pF, float* pF1, float* pF2, float* pF3) {
    char s[256];
    char* str;
    NOT_IMPLEMENTED();
}

void GetAString(FILE* pF, char* pString) {
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    strcpy(pString, str);
}

void AboutToLoadFirstCar() {
    NOT_IMPLEMENTED();
}

void LoadOpponentsCars(tRace_info* pRace_info) {
    int i;
    NOT_IMPLEMENTED();
}

void DisposeOpponentsCars(tRace_info* pRace_info) {
    int i;
    NOT_IMPLEMENTED();
}

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
        gMisc_strings[i] = BrMemAllocate(strlen(s) + 1, kMem_misc_string);
        strcpy(gMisc_strings[i], s);
        if (feof(f)) {
            break;
        }
    }
    fclose(f);
}

void FillInRaceInfo(tRace_info* pThe_race) {
    NOT_IMPLEMENTED();
}

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

void AllowOpenToFail() {
    NOT_IMPLEMENTED();
}

void DoNotAllowOpenToFail() {
    NOT_IMPLEMENTED();
}

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
        LOG_WARN("failed");
    }
    return result;
}

int GetCDPathFromPathsTxtFile(char* pPath_name) {
    static int got_it_already = 0;
    static tPath_name cd_pathname;
    FILE* paths_txt_fp;
    tPath_name paths_txt;
    LOG_TRACE9("()");

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

int TestForOriginalCarmaCDinDrive() {
    LOG_TRACE("()");

    // JeffH the symbol dump didn't include any local variable information.
    // These names are not necessarily the original names.
    tPath_name cd_pathname;
    tPath_name cd_data_pathname;
    tPath_name cutscene_pathname;
    FILE* paths_txt_fp;
    tPath_name paths_txt;
    int paths_txt_first_char;

    paths_txt[0] = 0;
    strcat(paths_txt, gApplication_path);
    strcat(paths_txt, gDir_separator);
    strcat(paths_txt, "PATHS.TXT");

    if (!PDCheckDriveExists(paths_txt)) {
        return 0;
    }

    paths_txt_fp = fopen(paths_txt, "rt");
    if (!paths_txt_fp) {
        return 0;
    }
    paths_txt_first_char = fgetc(paths_txt_fp);
    ungetc(paths_txt_first_char, paths_txt_fp);
    GetALineAndDontArgue(paths_txt_fp, cd_pathname);
    fclose(paths_txt_fp);
    strcpy(cd_data_pathname, cd_pathname);
    strcat(cd_data_pathname, gDir_separator);
    strcat(cd_data_pathname, "DATA");

    if (DRStricmp(cd_pathname, gApplication_path) == 0) {
        return 0;
    }

    strcpy(cutscene_pathname, cd_pathname);
    strcat(cutscene_pathname, gDir_separator);
    strcat(cutscene_pathname, "CUTSCENE");

    if (PDCheckDriveExists2(cd_data_pathname, "GENERAL.TXT", 100)
        && (PDCheckDriveExists2(cd_pathname, "CARMA.EXE", 1000000)
            || PDCheckDriveExists2(cd_pathname, "CARMAG.EXE", 1000000)
            || PDCheckDriveExists2(cd_pathname, "MAINPROG.EXE", 1000000)
            || PDCheckDriveExists2(cd_pathname, "CARMSPLT.EXE", 1000000)
            || PDCheckDriveExists2(cd_pathname, "CARMGSPL.EXE", 1000000))
        && PDCheckDriveExists2(cutscene_pathname, "SPLINTRO.SMK", 2000000)) {
        if (paths_txt_first_char != '@') {
            EncodeFile(paths_txt);
        }
        return 1;
    }
    return 0;
}

int OriginalCarmaCDinDrive() {
    return gCD_is_in_drive;
}

int CarmaCDinDriveOrFullGameInstalled() {
    NOT_IMPLEMENTED();
}

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

int PrintNetOptions(FILE* pF, int pIndex) {
    NOT_IMPLEMENTED();
}

int SaveOptions() {
    FILE* f;
    NOT_IMPLEMENTED();
}

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

void InitFunkGrooveFlags() {
    NOT_IMPLEMENTED();
}
