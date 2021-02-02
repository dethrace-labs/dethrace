#include "loading.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "brender.h"
#include "brucetrk.h"
#include "car.h"
#include "controls.h"
#include "crush.h"
#include "depth.h"
#include "displays.h"
#include "drmem.h"
#include "errors.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "init.h"
#include "input.h"
#include "newgame.h"
#include "opponent.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "sound.h"
#include "spark.h"
#include "utility.h"
#include "world.h"

#define HITHER_MULTIPLIER 2.0f
#define AMBIENT_MULTIPLIER 0.0099999998f
#define NBR_FUNK_GROVE_FLAGS 30

char* gWheel_actor_names[] = {
    "FLWHEEL.ACT",
    "FRWHEEL.ACT",
    "RLWHEEL.ACT",
    "RRWHEEL.ACT",
    "IFLWHEEL.ACT",
    "IFRWHEEL.ACT"
};
int gFunk_groove_flags[30];
char* gNet_avail_names[] = { "never", "eagle", "hawk", "all" };
char* gDamage_names[] = {
    "engine",
    "transmission",
    "driver",
    "steering",
    "lf_brake",
    "rf_brake",
    "lr_brake",
    "rr_brake",
    "lf_wheel",
    "rf_wheel",
    "lr_wheel",
    "rr_wheel"
};
char* gDrivable_car_names[6];
char* gYour_car_names[2][6];
char gDef_def_water_screen_name[32];
tHeadup_info gHeadup_image_info[31] = {
    { "LADY.PIX", eNet_or_otherwise },
    { "GENT.PIX", eNet_or_otherwise },
    { "CODGER.PIX", eNet_or_otherwise },
    { "SPROG.PIX", eNet_or_otherwise },
    { "GO.PIX", eNet_or_otherwise },
    { "NUMBER1.PIX", eNet_or_otherwise },
    { "NUMBER2.PIX", eNet_or_otherwise },
    { "NUMBER3.PIX", eNet_or_otherwise },
    { "NUMBER4.PIX", eNet_or_otherwise },
    { "NUMBER5.PIX", eNet_or_otherwise },
    { "SPLATTER.PIX", eNet_or_otherwise },
    { "PILEDRIV.PIX", eNet_or_otherwise },
    { "EXTRASTY.PIX", eNet_or_otherwise },
    { "ARTISTIC.PIX", eNet_or_otherwise },
    { "2XCOMBO.PIX", eNet_or_otherwise },
    { "3XCOMBO.PIX", eNet_or_otherwise },
    { "4XCOMBO.PIX", eNet_or_otherwise },
    { "5XCOMBO.PIX", eNet_or_otherwise },
    { "BILLIARD.PIX", eNet_or_otherwise },
    { "POLITE.PIX", eNet_or_otherwise },
    { "HEADON.PIX", eNet_or_otherwise },
    { "DESTROY.PIX", eNet_or_otherwise },
    { "CHECKPNT.PIX", eNet_or_otherwise },
    { "TIMEUP.PIX", eNot_net },
    { "RACEOVER.PIX", eNet_or_otherwise },
    { "UWASTED.PIX", eNet_only },
    { "MAD.PIX", eNet_only },
    { "GAMEOVER.PIX", eNet_only },
    { "UBROKE.PIX", eNet_only },
    { "ULOST.PIX", eNet_only },
    { "UWON.PIX", eNet_only }
};
int gAllow_open_to_fail = 1;
br_material* gDestn_screen_mat;
br_material* gSource_screen_mat;
char* gRaces_file_names[] = {
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
char* gFloorpan_names[] = { "GBUNDER.MAT", "BGLUNDER.MAT", "GRIMBOT.MAT", "DDBASE.MAT", "HFUNDER.MAT" };
int gCurrent_race_file_index;
int gGroove_funk_offset;
int gDemo_armour;
int gDemo_rank;
int gDecode_thing = '@';
int gDemo_opponents[5];
int gDemo_power;
int gDemo_offensive;
char gDecode_string[] = { 0x9B, 0x52, 0x93, 0x9F, 0x52, 0x98, 0x9B, 0x96, 0x96, 0x9E, 0x9B, 0xA0, 0x99, 0x0 };

#define DOUBLESIDED_FLAG_COLOR_MAP (br_pixelmap*)12345

// IDA: tU32 __usercall ReadU32@<EAX>(FILE *pF@<EAX>)
tU32 ReadU32(FILE* pF) {
    tU32 raw_long;
    LOG_TRACE("(%p)", pF);
    NOT_IMPLEMENTED();
}

// IDA: tU16 __usercall ReadU16@<AX>(FILE *pF@<EAX>)
tU16 ReadU16(FILE* pF) {
    tU16 raw_short;
    LOG_TRACE("(%p)", pF);
    NOT_IMPLEMENTED();
}

// IDA: tU8 __usercall ReadU8@<AL>(FILE *pF@<EAX>)
tU8 ReadU8(FILE* pF) {
    tU8 raw_byte;
    LOG_TRACE("(%p)", pF);
    NOT_IMPLEMENTED();
}

// IDA: tS32 __usercall ReadS32@<EAX>(FILE *pF@<EAX>)
tS32 ReadS32(FILE* pF) {
    tS32 raw_long;
    LOG_TRACE("(%p)", pF);
    NOT_IMPLEMENTED();
}

// IDA: tS16 __usercall ReadS16@<AX>(FILE *pF@<EAX>)
tS16 ReadS16(FILE* pF) {
    tS16 raw_short;
    LOG_TRACE("(%p)", pF);
    NOT_IMPLEMENTED();
}

// IDA: tS8 __usercall ReadS8@<AL>(FILE *pF@<EAX>)
tS8 ReadS8(FILE* pF) {
    tS8 raw_byte;
    LOG_TRACE("(%p)", pF);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall WriteU32L(FILE *pF@<EAX>, tU32 pNumber@<EDX>)
void WriteU32L(FILE* pF, tU32 pNumber) {
    tU32 raw_long;
    LOG_TRACE("(%p, %d)", pF, pNumber);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall WriteU16L(FILE *pF@<EAX>, tU16 pNumber@<EDX>)
void WriteU16L(FILE* pF, tU16 pNumber) {
    tU16 raw_short;
    LOG_TRACE("(%p, %d)", pF, pNumber);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall WriteU8L(FILE *pF@<EAX>, tU8 pNumber@<EDX>)
void WriteU8L(FILE* pF, tU8 pNumber) {
    tU8 raw_byte;
    LOG_TRACE("(%p, %d)", pF, pNumber);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall WriteS32L(FILE *pF@<EAX>, tS32 pNumber@<EDX>)
void WriteS32L(FILE* pF, tS32 pNumber) {
    tS32 raw_long;
    LOG_TRACE("(%p, %d)", pF, pNumber);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall WriteS16L(FILE *pF@<EAX>, tS16 pNumber@<EDX>)
void WriteS16L(FILE* pF, tS16 pNumber) {
    tS16 raw_short;
    LOG_TRACE("(%p, %d)", pF, pNumber);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall WriteS8L(FILE *pF@<EAX>, tS8 pNumber@<EDX>)
void WriteS8L(FILE* pF, tS8 pNumber) {
    tS8 raw_byte;
    LOG_TRACE("(%p, %d)", pF, pNumber);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SkipBytes(FILE *pF@<EAX>, int pBytes_to_skip@<EDX>)
void SkipBytes(FILE* pF, int pBytes_to_skip) {
    LOG_TRACE("(%p, %d)", pF, pBytes_to_skip);
    NOT_IMPLEMENTED();
}

// IDA: tU32 __usercall MemReadU32@<EAX>(char **pPtr@<EAX>)
tU32 MemReadU32(char** pPtr) {
    tU32 raw_long;
    memcpy(&raw_long, *pPtr, 4);
    *pPtr += 4;
    return raw_long;
}

// IDA: tU16 __usercall MemReadU16@<AX>(char **pPtr@<EAX>)
tU16 MemReadU16(char** pPtr) {
    tU16 raw_short;
    memcpy(&raw_short, *pPtr, 2);
    *pPtr += 2;
    return raw_short;
}

// IDA: tU8 __usercall MemReadU8@<AL>(char **pPtr@<EAX>)
tU8 MemReadU8(char** pPtr) {
    return *(*pPtr)++;
}

// IDA: tS32 __usercall MemReadS32@<EAX>(char **pPtr@<EAX>)
tS32 MemReadS32(char** pPtr) {
    tS32 raw_long;
    LOG_TRACE("(%p)", pPtr);
    NOT_IMPLEMENTED();
}

// IDA: tS16 __usercall MemReadS16@<AX>(char **pPtr@<EAX>)
tS16 MemReadS16(char** pPtr) {
    tS16 raw_short;
    memcpy(&raw_short, *pPtr, 2);
    *pPtr += 2;
    return raw_short;
}

// IDA: tS8 __usercall MemReadS8@<AL>(char **pPtr@<EAX>)
tS8 MemReadS8(char** pPtr) {
    tS8 raw_byte;
    raw_byte = **pPtr;
    (*pPtr)++;
    return raw_byte;
}

// IDA: void __usercall MemSkipBytes(char **pPtr@<EAX>, int pBytes_to_skip@<EDX>)
void MemSkipBytes(char** pPtr, int pBytes_to_skip) {
    *pPtr += pBytes_to_skip;
}

// IDA: void __cdecl LoadGeneralParameters()
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

// IDA: void __cdecl FinishLoadingGeneral()
void FinishLoadingGeneral() {
    /*dword_1298C0 = */ BrMaterialFind(gDef_def_water_screen_name);
}

// IDA: br_pixelmap* __usercall LoadPixelmap@<EAX>(char *pName@<EAX>)
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
    }
    return pm;
}

// IDA: br_uint_32 __usercall LoadPixelmaps@<EAX>(char *pFile_name@<EAX>, br_pixelmap **pPixelmaps@<EDX>, br_uint_16 pNum@<EBX>)
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

// IDA: br_pixelmap* __usercall LoadShadeTable@<EAX>(char *pName@<EAX>)
br_pixelmap* LoadShadeTable(char* pName) {
    tPath_name the_path;
    LOG_TRACE("(\"%s\")", pName);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall LoadMaterial@<EAX>(char *pName@<EAX>)
br_material* LoadMaterial(char* pName) {
    tPath_name the_path;
    br_material* result;
    LOG_TRACE("(\"%s\")", pName);
    NOT_IMPLEMENTED();
}

// IDA: br_model* __usercall LoadModel@<EAX>(char *pName@<EAX>)
br_model* LoadModel(char* pName) {
    tPath_name the_path;
    br_model* model;
    LOG_TRACE("(\"%s\")", pName);

    PossibleService();
    PathCat(the_path, gApplication_path, "MODELS");
    PathCat(the_path, the_path, pName);
    return BrModelLoad(the_path);
}

// IDA: br_actor* __usercall LoadActor@<EAX>(char *pName@<EAX>)
br_actor* LoadActor(char* pName) {
    tPath_name the_path;
    LOG_TRACE("(\"%s\")", pName);

    PossibleService();
    PathCat(the_path, gApplication_path, "ACTORS");
    PathCat(the_path, the_path, pName);
    return BrActorLoad(the_path);
}

// IDA: void __usercall DRLoadPalette(char *pPath_name@<EAX>)
void DRLoadPalette(char* pPath_name) {
    br_pixelmap* palette_array[100];
    int number_of_palettes;

    number_of_palettes = DRPixelmapLoadMany(pPath_name, palette_array, 100);
    BrTableAddMany(palette_array, number_of_palettes);
}

// IDA: void __usercall DRLoadShadeTable(char *pPath_name@<EAX>)
void DRLoadShadeTable(char* pPath_name) {
    br_pixelmap* table_array[100];
    int number_of_tables;

    number_of_tables = DRPixelmapLoadMany(pPath_name, table_array, 100);
    BrTableAddMany(table_array, number_of_tables);
}

// IDA: void __usercall RezeroPixelmaps(br_pixelmap **pPixelmap_array@<EAX>, int pCount@<EDX>)
void RezeroPixelmaps(br_pixelmap** pPixelmap_array, int pCount) {
    LOG_TRACE("(%p, %d)", pPixelmap_array, pCount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRLoadPixelmaps(char *pPath_name@<EAX>)
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

// IDA: void __usercall DRLoadMaterials(char *pPath_name@<EAX>)
void DRLoadMaterials(char* pPath_name) {
    br_material* material_array[100];
    int number_of_materials;

    PossibleService();
    number_of_materials = BrMaterialLoadMany(pPath_name, material_array, 100);
    BrMaterialAddMany(material_array, number_of_materials);
}

// IDA: void __usercall DRLoadModels(char *pPath_name@<EAX>)
void DRLoadModels(char* pPath_name) {
    br_model* model_array[100];
    int number_of_models;
    LOG_TRACE("(\"%s\")", pPath_name);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRLoadActors(char *pPath_name@<EAX>)
void DRLoadActors(char* pPath_name) {
    br_actor* actor_array[100];
    int number_of_actors;
    int i;
    LOG_TRACE("(\"%s\")", pPath_name);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRLoadLights(char *pPath_name@<EAX>)
void DRLoadLights(char* pPath_name) {
    br_actor* light_array[100];
    int number_of_lights;
    int i;
    STUB();
}

// IDA: void __usercall LoadInFiles(char *pThe_base_path@<EAX>, char *pThe_dir_name@<EDX>, void (*pLoad_routine)(char*)@<EBX>)
void LoadInFiles(char* pThe_base_path, char* pThe_dir_name, void (*pLoad_routine)(char*)) {
    tPath_name the_path;
    LOG_TRACE("(\"%s\", \"%s\", %p)", pThe_base_path, pThe_dir_name, pLoad_routine);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LoadInRegisteeDir(char *pThe_dir_path@<EAX>)
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

// IDA: void __cdecl LoadInRegistees()
void LoadInRegistees() {
    LoadInRegisteeDir(gApplication_path);
}

// IDA: void __cdecl LoadKeyMapping()
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

// IDA: void __usercall LoadInterfaceStuff(int pWithin_race@<EAX>)
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

// IDA: void __cdecl UnlockInterfaceStuff()
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

// IDA: void __cdecl InitInterfaceLoadState()
void InitInterfaceLoadState() {
    LOG_TRACE("()");
    memset(gCursors, 0, sizeof(gCursors));
}

// IDA: tS8* __usercall ConvertPixTo16BitStripMap@<EAX>(br_pixelmap *pBr_map@<EAX>)
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
    LOG_TRACE("(%p)", pBr_map);
    NOT_IMPLEMENTED();
}

// IDA: tS8* __usercall ConvertPixToStripMap@<EAX>(br_pixelmap *pThe_br_map@<EAX>)
// Jeff: This appears to be used only for dashboard views, either to save memory storage or pixel copies.
// See also: `CopyStripImage`
// It is a simple RLE algorithm, but only targets runs of blank pixels.
// Format:
//  number_of_lines (1 byte)
//  for each line:
//    chunk_count (1 byte)
//    for each chunk:
//      chunk_length (1 signed byte). If positive, skip this number of blank pixels. If negative, copy the following `length` bytes
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
    LOG_TRACE("(%p)", pThe_br_map);
    int total;

    temp_strip_image = BrMemAllocate(pThe_br_map->row_bytes * pThe_br_map->height, kMem_strip_image);
    current_size = 2;

    *temp_strip_image = pThe_br_map->height;
    current_strip_pointer = temp_strip_image;

    for (i = 0; i < pThe_br_map->height; i++) {
        next_byte = (char*)pThe_br_map->pixels + i * pThe_br_map->row_bytes; //points to start of this line
        new_line_length = 2; // leave space at the start of the line to store number of chunks and first chunk length
        j = 0;
        counter = 0;
        total = 0;
        counting_blanks = *next_byte == 0;
        chunk_counter = 0;
        while (1) {
            while (counter <= 126) {
                if (j == pThe_br_map->width) {
                    break;
                }
                the_byte = *next_byte;
                if ((the_byte == 0 && !counting_blanks) || (the_byte != 0 && counting_blanks)) {
                    break;
                }
                if (!counting_blanks) {
                    new_line[new_line_length] = the_byte;
                    new_line_length++;
                }
                counter++;
                j++;
                next_byte++;
            }
            if (counting_blanks) {
                new_line[new_line_length - 1] = counter;
            } else {
                new_line[new_line_length - counter - 1] = -counter;
            }
            counting_blanks = the_byte == 0;
            chunk_counter++;
            total += counter;
            counter = 0;
            if (j == pThe_br_map->width) {
                break;
            }
            new_line_length++;
        }
        new_line[0] = chunk_counter;
        current_strip_pointer = &temp_strip_image[current_size];
        memcpy(current_strip_pointer, new_line, new_line_length);
        current_size += new_line_length;
    }
    the_strip_image = BrMemAllocate(current_size, kMem_strip_image_perm);
    memcpy(the_strip_image, temp_strip_image, current_size);
    BrMemFree(temp_strip_image);
    return (tS8*)the_strip_image;
}

// IDA: void __usercall KillWindscreen(br_model *pModel@<EAX>, br_material *pMaterial@<EDX>)
void KillWindscreen(br_model* pModel, br_material* pMaterial) {
    br_face* face;
    int i;
    LOG_TRACE("(%p, %p)", pModel, pMaterial);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DropOffDyingPeds(tCar_spec *pCar@<EAX>)
void DropOffDyingPeds(tCar_spec* pCar) {
    br_actor* child;
    br_actor* next;
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeCar(tCar_spec *pCar_spec@<EAX>, int pOwner@<EDX>)
void DisposeCar(tCar_spec* pCar_spec, int pOwner) {
    int i;
    int j;
    LOG_TRACE("(%p, %d)", pCar_spec, pOwner);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AdjustCarCoordinates(tCar_spec *pCar@<EAX>)
void AdjustCarCoordinates(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);

    for (i = 0; i < COUNT_OF(pCar->render_left); i++) {
        pCar->render_left[i] -= gCurrent_graf_data->cock_margin_x;

        pCar->render_top[i] -= gCurrent_graf_data->cock_margin_y;
        pCar->render_right[i] -= gCurrent_graf_data->cock_margin_x;
        pCar->render_bottom[i] -= gCurrent_graf_data->cock_margin_y;
    }
    pCar->mirror_left -= gCurrent_graf_data->cock_margin_x;
    pCar->mirror_top -= gCurrent_graf_data->cock_margin_y;
    pCar->mirror_right -= gCurrent_graf_data->cock_margin_x;
    pCar->mirror_bottom -= gCurrent_graf_data->cock_margin_y;
    pCar->speedo_centre_x[1] -= gCurrent_graf_data->cock_margin_x;
    pCar->speedo_centre_y[1] -= gCurrent_graf_data->cock_margin_y;
    pCar->tacho_centre_x[1] -= gCurrent_graf_data->cock_margin_x;
    pCar->tacho_centre_y[1] -= gCurrent_graf_data->cock_margin_y;
    pCar->speedo_x[1] -= gCurrent_graf_data->cock_margin_x;
    pCar->speedo_y[1] -= gCurrent_graf_data->cock_margin_y;
    pCar->tacho_x[1] -= gCurrent_graf_data->cock_margin_x;
    pCar->tacho_y[1] -= gCurrent_graf_data->cock_margin_y;
    for (i = 0; i < COUNT_OF(pCar->lhands_x); i++) {
        pCar->lhands_x[i] -= gCurrent_graf_data->cock_margin_x;
        pCar->lhands_y[i] -= gCurrent_graf_data->cock_margin_y;
        pCar->rhands_x[i] -= gCurrent_graf_data->cock_margin_x;
        pCar->rhands_y[i] -= gCurrent_graf_data->cock_margin_y;
    }
    for (i = 0; i < COUNT_OF(pCar->damage_units); i++) {
        pCar->damage_units[i].x_coord -= gCurrent_graf_data->cock_margin_x;
        pCar->damage_units[i].y_coord -= gCurrent_graf_data->cock_margin_y;
    }
}

// IDA: void __usercall LoadSpeedo(FILE *pF@<EAX>, int pIndex@<EDX>, tCar_spec *pCar_spec@<EBX>)
void LoadSpeedo(FILE* pF, int pIndex, tCar_spec* pCar_spec) {
    tPath_name the_path;
    char s[256];
    char* str;
    char the_char1;
    char the_char2;
    LOG_TRACE("(%p, %d, %p)", pF, pIndex, pCar_spec);

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%c", &the_char1);
    if (the_char1 == 'd') {
        pCar_spec->speedo_radius_2[pIndex] = -1;
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->speedo_x[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->speedo_y[pIndex]);
        str = strtok(NULL, "\t ,/");
        pCar_spec->speedo_image[pIndex] = LoadPixelmap(str);
        if (!pCar_spec->speedo_image[pIndex]) {
            FatalError(35);
        }
        pCar_spec->speedo_y_pitch[pIndex] = pCar_spec->speedo_image[pIndex]->height / 10;
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->speedo_x_pitch[pIndex]);
    } else {
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->speedo_x[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->speedo_y[pIndex]);
        str = strtok(NULL, "\t ,/");
        pCar_spec->speedo_image[pIndex] = LoadPixelmap(str);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->speedo_centre_x[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->speedo_centre_y[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->speedo_radius_1[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->speedo_radius_2[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->speedo_start_angle[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->speedo_end_angle[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->speedo_needle_colour[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->max_speed);
    }
}

// IDA: void __usercall LoadTacho(FILE *pF@<EAX>, int pIndex@<EDX>, tCar_spec *pCar_spec@<EBX>)
void LoadTacho(FILE* pF, int pIndex, tCar_spec* pCar_spec) {
    tPath_name the_path;
    char s[256];
    char* str;
    char the_char1;
    char the_char2;
    LOG_TRACE("(%p, %d, %p)", pF, pIndex, pCar_spec);

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%c", &the_char1);
    if (the_char1 == 'd') {
        pCar_spec->tacho_radius_2[pIndex] = -1;
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->tacho_x[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->tacho_y[pIndex]);
        str = strtok(NULL, "\t ,/");
        pCar_spec->tacho_image[pIndex] = LoadPixelmap(str);
    } else {
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->tacho_x[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->tacho_y[pIndex]);
        str = strtok(NULL, "\t ,/");
        pCar_spec->tacho_image[pIndex] = LoadPixelmap(str);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->tacho_centre_x[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->tacho_centre_y[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->tacho_radius_1[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->tacho_radius_2[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->tacho_start_angle[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->tacho_end_angle[pIndex]);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->tacho_needle_colour[pIndex]);
    }
}

// IDA: void __usercall LoadHeadups(FILE *pF@<EAX>, int pIndex@<EDX>, tCar_spec *pCar_spec@<EBX>)
void LoadHeadups(FILE* pF, int pIndex, tCar_spec* pCar_spec) {
    char s[256];
    char* str;
    int j;
    int number_of_slots;
    LOG_TRACE("(%p, %d, %p)", pF, pIndex, pCar_spec);

    number_of_slots = GetAnInt(pF);
    for (j = 0; j < number_of_slots; j++) {
        GetALineAndDontArgue(pF, s);
        str = strtok(s, "\t ,/");
        sscanf(str, "%d", &pCar_spec->headup_slots[pIndex][j].x);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->headup_slots[pIndex][j].y);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->headup_slots[pIndex][j].colour);
        str = strtok(NULL, "\t ,/");
        strcpy(s, str);
        switch (s[0]) {
        case 'c':
            pCar_spec->headup_slots[pIndex][j].justification = 2;
            break;
        case 'l':
            pCar_spec->headup_slots[pIndex][j].justification = 0;
            break;
        case 'r':
            pCar_spec->headup_slots[pIndex][j].justification = 1;
            break;
        }
        if (s[1] == 'c') {
            pCar_spec->headup_slots[pIndex][j].cockpit_anchored = 1;
        }
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->headup_slots[pIndex][j].dim_left);
        if (pCar_spec->headup_slots[pIndex][j].dim_left < 0) {
            pCar_spec->headup_slots[pIndex][j].dimmed_background = 0;
        } else {
            pCar_spec->headup_slots[pIndex][j].dimmed_background = 1;
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%d", &pCar_spec->headup_slots[pIndex][j].dim_top);
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%d", &pCar_spec->headup_slots[pIndex][j].dim_right);
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%d", &pCar_spec->headup_slots[pIndex][j].dim_bottom);
        }
    }
}

// IDA: void __usercall ReadNonCarMechanicsData(FILE *pF@<EAX>, tNon_car_spec *non_car@<EDX>)
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
    LOG_TRACE("(%p, %p)", pF, non_car);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReadMechanicsData(FILE *pF@<EAX>, tCar_spec *c@<EDX>)
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
    LOG_TRACE("(%p, %p)", pF, c);

    GetALineAndDontArgue(pF, s);
    for (i = strlen(s) - 1; s[i] == ' '; --i) {
        ;
    }
    version = s[i];
    for (i = 0; i < 4; ++i) {
        GetThreeFloats(pF, &c->wpos[i].v[0], &c->wpos[i].v[1], &c->wpos[i].v[2]);
    }
    actor_offset = &c->car_model_actors[c->principal_car_actor].actor->t.t.translate.t;
    GetThreeFloats(pF, &c->cmpos.v[0], &c->cmpos.v[1], &c->cmpos.v[2]);
    if (version < '3') {
        c->extra_point_num = 0;
        i = GetAnInt(pF);
    }
    GetThreeFloats(pF, &c->bounds[1].min.v[0], &c->bounds[1].min.v[1], &c->bounds[1].min.v[2]);
    GetThreeFloats(pF, &c->bounds[1].max.v[0], &c->bounds[1].max.v[1], &c->bounds[1].max.v[2]);
    c->bounds[1].min.v[0] = c->bounds[1].min.v[0] + actor_offset->v[0];
    c->bounds[1].min.v[1] = c->bounds[1].min.v[1] + actor_offset->v[1];
    c->bounds[1].min.v[2] = c->bounds[1].min.v[2] + actor_offset->v[2];
    c->bounds[1].max.v[0] = c->bounds[1].max.v[0] + actor_offset->v[0];
    c->bounds[1].max.v[1] = c->bounds[1].max.v[1] + actor_offset->v[1];
    c->bounds[1].max.v[2] = c->bounds[1].max.v[2] + actor_offset->v[2];
    if (version >= '3') {
        c->extra_point_num = GetAnInt(pF);
        if (c->extra_point_num > 6) {
            sprintf(s, "%d", c->index);
            FatalError(93, s);
        }
        for (i = 0; c->extra_point_num > i; ++i) {
            GetThreeFloats(pF, &c->extra_points[i].v[0], &c->extra_points[i].v[1], &c->extra_points[i].v[2]);
            c->extra_points[i].v[0] = c->extra_points[i].v[0] + actor_offset->v[0];
            c->extra_points[i].v[1] = c->extra_points[i].v[1] + actor_offset->v[1];
            c->extra_points[i].v[2] = c->extra_points[i].v[2] + actor_offset->v[2];
        }
    }
    c->maxcurve = 1.0 / GetAFloat(pF);
    GetPairOfFloats(pF, &c->susp_give[1], &c->susp_give[0]);
    c->ride_height = GetAFloat(pF);
    c->ride_height = c->bounds[1].min.v[1] + 0.01;
    c->damping = GetAFloat(pF);
    c->M = GetAFloat(pF);
    c->freduction = GetAFloat(pF);
    if (version >= '4') {
        GetThreeFloats(pF, &theta_front, &theta_back, &theta_comp);
    } else {
        GetPairOfFloats(pF, &theta_front, &theta_back);
        theta_comp = theta_back;
    }
    GetThreeFloats(pF, &wid, &het, &len);
    c->rolling_r_front = 0.050000001;
    c->rolling_r_back = 0.050000001;
    c->max_gear = 6;
    speed = 200.0;
    force = 4.0;
    c->friction_elipticity = 1.0;
    c->down_force_speed = 2000.0;
    c->initial_brake = c->M * 12.0;
    c->brake_increase = c->M * 12.0;
    if (version >= '2' && version <= '4') {
        c->friction_elipticity = GetAFloat(pF); // 2, 3, 4
        c->down_force_speed = GetAFloat(pF);
        c->initial_brake = GetAFloat(pF) * c->M * 12.0;
        c->brake_increase = GetAFloat(pF) * c->M * 12.0;
    }
    if (version >= '1' && version <= '4') {
        GetPairOfFloats(pF, &c->rolling_r_front, &c->rolling_r_back); // 1, 2, 3, 4
        c->max_gear = GetAnInt(pF);
        speed = GetAFloat(pF);
        force = GetAFloat(pF);
    }

    speed = speed * 4.0 / 9.0;
    c->speed_revs_ratio = speed / (double)c->max_gear / 6000.0;
    c->force_torque_ratio = (double)c->max_gear * c->M * force;
    c->mu[1] = tan(theta_front * 3.14 / 180.0) / 4.0;
    c->mu[0] = tan(theta_back * 3.14 / 180.0) / 4.0;
    c->mu[2] = tan(theta_comp * 3.14 / 180.0) / 4.0;

    c->mu[0] *= sqrt((c->wpos[2].v[2] - c->cmpos.v[2]) / (c->wpos[2].v[2] - c->wpos[0].v[2]) * (c->M * 5.0));
    c->mu[1] *= sqrt((c->wpos[0].v[2] - c->cmpos.v[2]) / (c->wpos[0].v[2] - c->wpos[2].v[2]) * (c->M * 5.0));
    c->mu[2] *= sqrt((c->wpos[2].v[2] - c->cmpos.v[2]) / (c->wpos[2].v[2] - c->wpos[0].v[2]) * (c->M * 5.0));
    for (i = 0; i < 4; ++i) {
        c->wpos[i].v[1] = c->ride_height;
    }
    c->bounds[0].min.v[0] = c->bounds[1].min.v[0];
    c->bounds[0].min.v[1] = c->bounds[1].min.v[1];
    c->bounds[0].min.v[2] = c->bounds[1].min.v[2];
    c->bounds[0].max.v[0] = c->bounds[1].max.v[0];
    c->bounds[0].max.v[1] = c->bounds[1].max.v[1];
    c->bounds[0].max.v[2] = c->bounds[1].max.v[2];
    for (i = 0; c->extra_point_num > i; ++i) {
        for (j = 0; j < 3; ++j) {
            if (c->extra_points[i].v[j] < c->bounds[0].min.v[j]) {
                c->bounds[0].min.v[j] = c->extra_points[i].v[j];
            }
            if (c->extra_points[i].v[j] > c->bounds[0].max.v[j]) {
                c->bounds[0].max.v[j] = c->extra_points[i].v[j];
            }
        }
    }
    memcpy(&c->bounds[2], &c->bounds[0], sizeof(br_bounds));
    c->I.v[2] = het * het + wid * wid * c->M / 12.0;
    c->I.v[1] = wid * wid + len * len * c->M / 12.0;
    c->I.v[0] = het * het + len * len * c->M / 12.0;
    for (i = 0; i < 4; ++i) {
        c->wpos[i].v[0] = c->wpos[i].v[0] * 6.9;
        c->wpos[i].v[1] = c->wpos[i].v[1] * 6.9;
        c->wpos[i].v[2] = c->wpos[i].v[2] * 6.9;
    }
    c->cmpos.v[0] = c->cmpos.v[0] * 6.9000001;
    c->cmpos.v[1] = c->cmpos.v[1] * 6.9000001;
    c->cmpos.v[2] = c->cmpos.v[2] * 6.9000001;
    c->I.v[0] = c->I.v[0] * 47.610001;
    c->I.v[1] = c->I.v[1] * 47.610001;
    c->I.v[2] = c->I.v[2] * 47.610001;
    c->bounds[1].min.v[0] = c->bounds[1].min.v[0] * 6.9000001;
    c->bounds[1].min.v[1] = c->bounds[1].min.v[1] * 6.9000001;
    c->bounds[1].min.v[2] = c->bounds[1].min.v[2] * 6.9000001;
    c->bounds[1].max.v[0] = c->bounds[1].max.v[0] * 6.9000001;
    c->bounds[1].max.v[1] = c->bounds[1].max.v[1] * 6.9000001;
    c->bounds[1].max.v[2] = c->bounds[1].max.v[2] * 6.9000001;
    for (i = 0; c->extra_point_num > i; ++i) {
        c->extra_points[i].v[0] = c->extra_points[i].v[0] * 6.9000001;
        c->extra_points[i].v[1] = c->extra_points[i].v[1] * 6.9000001;
        c->extra_points[i].v[2] = c->extra_points[i].v[2] * 6.9000001;
    }
    memcpy(c->max_bounds, c->bounds, sizeof(br_bounds));
    memcpy(&c->max_bounds[1], &c->bounds[1], sizeof(br_bounds));
    for (i = 0; c->extra_point_num > i; ++i) {
        c->original_extra_points_z[i] = c->extra_points[i].v[2];
    }
    c->maxcurve = c->maxcurve / 6.9;
    c->ride_height = c->ride_height * 6.9;

    // JeffH this seems to do nothing since these fields are not yet initialized
    for (i = 0; i < 2; ++i) {
        c->susp_height[i] = c->susp_height[i] * 6.9;
        c->sk[i] = c->sk[i] / 6.9;
        c->sb[i] = c->sb[i] / sqrt(6.9);
    }
    GetAString(pF, s);
    SetCarSuspGiveAndHeight(c, 1.0, 1.0, 1.0, 0.0, 0.0);
}

// IDA: void __usercall LoadGear(FILE *pF@<EAX>, int pIndex@<EDX>, tCar_spec *pCar_spec@<EBX>)
void LoadGear(FILE* pF, int pIndex, tCar_spec* pCar_spec) {
    tPath_name the_path;
    char s[256];
    char* str;
    LOG_TRACE("(%p, %d, %p)", pF, pIndex, pCar_spec);

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &pCar_spec->gear_x[pIndex]);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%d", &pCar_spec->gear_y[pIndex]);
    str = strtok(NULL, "\t ,/");
    if (!pIndex) {
        pCar_spec->gears_image = LoadPixelmap(str);
    }
}

// IDA: void __usercall AddRefOffset(int *pRef_holder@<EAX>)
void AddRefOffset(int* pRef_holder) {
    LOG_TRACE8("(%p)", pRef_holder);

    if (*pRef_holder >= 0) {
        *pRef_holder += gGroove_funk_offset;
    }
}

// IDA: void __usercall GetDamageProgram(FILE *pF@<EAX>, tCar_spec *pCar_spec@<EDX>, tImpact_location pImpact_location@<EBX>)
void GetDamageProgram(FILE* pF, tCar_spec* pCar_spec, tImpact_location pImpact_location) {
    tDamage_clause* the_clause;
    int i;
    int j;
    int k;
    int count;
    char s[256];
    char delim[64];
    char* str;
    LOG_TRACE("(%p, %p, %d)", pF, pCar_spec, pImpact_location);

    PossibleService();
    count = GetAnInt(pF);
    pCar_spec->damage_programs[pImpact_location].clause_count = count;
    pCar_spec->damage_programs[pImpact_location].clauses = (tDamage_clause*)BrMemAllocate(count * sizeof(tDamage_clause), kMem_damage_clauses);
    strcpy(delim, "\t ,/");
    strcat(delim, "&");

    for (i = 0; i < count; i++) {
        the_clause = &pCar_spec->damage_programs[pImpact_location].clauses[i];
        the_clause->condition_count = 0;
        GetALineAndDontArgue(pF, s);
        str = strtok(s, delim);
        do {
            switch (str[0]) {
            case 'x':
                the_clause->conditions[the_clause->condition_count].axis_comp = 0;
                break;
            case 'y':
                the_clause->conditions[the_clause->condition_count].axis_comp = 1;
                break;
            case 'z':
                the_clause->conditions[the_clause->condition_count].axis_comp = 2;
                break;
            default:
                goto LABEL_17;
            }
            if (str[1] == '>') {
                the_clause->conditions[the_clause->condition_count].condition_operator = 1;
            } else if (str[1] == '<') {
                the_clause->conditions[the_clause->condition_count].condition_operator = 0;
            } else {
                FatalError(92);
            }
            sscanf(str + 2, "%f", &the_clause->conditions[the_clause->condition_count].comparitor);
            the_clause->condition_count++;
            str = strtok(NULL, delim);
        } while (the_clause->condition_count < 2);
    LABEL_17:
        the_clause->effect_count = GetAnInt(pF);
        for (j = 0; j < the_clause->effect_count; j++) {
            the_clause->effects[j].type = -1;
            GetALineAndDontArgue(pF, s);
            str = strtok(s, "\t ,/");
            for (k = 0; k < COUNT_OF(gDamage_names); k++) {
                if (strcmp(str, gDamage_names[k]) == 0) {
                    the_clause->effects[j].type = k;
                    break;
                }
            }
            if (the_clause->effects[j].type < 0) {
                FatalError(91);
            }
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%f", &the_clause->effects[j].weakness_factor);
        }
        ++the_clause;
    }
}

// IDA: br_uint_32 __cdecl LinkModel(br_actor *pActor, tModel_pool *pModel_pool)
intptr_t LinkModel(br_actor* pActor, tModel_pool* pModel_pool) {
    int i;
    LOG_TRACE("(%p, %p)", pActor, pModel_pool);

    if (pActor->model && pActor->model->identifier) {
        for (i = 0; i < pModel_pool->model_count; i++) {
            if (pModel_pool->model_array[i]->identifier
                && !strcmp(pModel_pool->model_array[i]->identifier, pActor->model->identifier)) {
                pActor->model = pModel_pool->model_array[i];
                return 0;
            }
        }
    }
    //LOG_WARN("failed to link model %s", pActor->model->identifier);
    return 0;
}

// IDA: void __usercall FreeUpBonnetModels(br_model **pModel_array@<EAX>, int pModel_count@<EDX>)
void FreeUpBonnetModels(br_model** pModel_array, int pModel_count) {
    int i;
    LOG_TRACE("(%p, %d)", pModel_array, pModel_count);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LinkModelsToActor(br_actor *pActor@<EAX>, br_model **pModel_array@<EDX>, int pModel_count@<EBX>)
void LinkModelsToActor(br_actor* pActor, br_model** pModel_array, int pModel_count) {
    tModel_pool model_pool;
    LOG_TRACE("(%p, %p, %d)", pActor, pModel_array, pModel_count);

    DRActorEnumRecurse(pActor, LinkModel, &pModel_array);
}

// IDA: void __usercall ReadShrapnelMaterials(FILE *pF@<EAX>, tCollision_info *pCar_spec@<EDX>)
void ReadShrapnelMaterials(FILE* pF, tCollision_info* pCar_spec) {
    char s[256];
    char version;
    int i;
    LOG_TRACE("(%p, %p)", pF, pCar_spec);

    pCar_spec->max_shrapnel_material = GetAnInt(pF);
    for (i = 0; i < pCar_spec->max_shrapnel_material; i++) {
        GetAString(pF, s);
        pCar_spec->shrapnel_material[i] = BrMaterialFind(s);
    }
}

// IDA: void __usercall CloneCar(tCar_spec **pOutput_car@<EAX>, tCar_spec *pInput_car@<EDX>)
void CloneCar(tCar_spec** pOutput_car, tCar_spec* pInput_car) {
    int i;
    LOG_TRACE("(%p, %p)", pOutput_car, pInput_car);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeClonedCar(tCar_spec *pCar@<EAX>)
void DisposeClonedCar(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall RemoveDoubleSided@<EAX>(br_model *pModel@<EAX>)
int RemoveDoubleSided(br_model* pModel) {
    br_face* faces;
    br_face* face;
    int temp;
    int num_double_sided_faces;
    int i;
    int orig_nfaces;
    int result;
    LOG_TRACE("(%p)", pModel);

    result = 0;
    if (pModel && pModel->nfaces) {
        num_double_sided_faces = 0;

        for (i = 0; i < pModel->nfaces; i++) {
            face = &pModel->faces[i];
            if (face->material) {
                if (face->material->colour_map_1 == DOUBLESIDED_FLAG_COLOR_MAP) {
                    num_double_sided_faces++;
                }
            }
        }
        if (num_double_sided_faces > 0) {
            LOG_DEBUG("nbr dbl side %d, %p", num_double_sided_faces, pModel);
            faces = BrResAllocate(pModel, sizeof(br_face) * (num_double_sided_faces + pModel->nfaces), kMem_misc);
            memcpy(faces, pModel->faces, sizeof(br_face) * pModel->nfaces);
            orig_nfaces = pModel->nfaces;
            face = pModel->faces;
            for (i = 0; i < orig_nfaces; i++) {
                if (face->material && face->material->colour_map_1 == DOUBLESIDED_FLAG_COLOR_MAP) {
                    faces[pModel->nfaces].vertices[0] = face->vertices[1];
                    faces[pModel->nfaces].vertices[1] = face->vertices[0];
                    faces[pModel->nfaces].vertices[2] = face->vertices[2];
                    faces[pModel->nfaces].flags = face->flags;
                    faces[pModel->nfaces].material = face->material;
                    pModel->nfaces++;
                }
                ++face;
            }
            BrResFree(pModel->faces);
            pModel->faces = faces;
            result = 1;
        }
    }
    return result;
}

// IDA: void __usercall MungeWindscreen(br_model *pModel@<EAX>)
void MungeWindscreen(br_model* pModel) {
    br_face* face;
    int i;
    LOG_TRACE("(%p)", pModel);

    if (pModel && pModel->nfaces) {
        face = pModel->faces;
        for (i = 0; i < pModel->nfaces; i++) {
            if (!face->material
                || (face->material->identifier
                    && gSource_screen_mat && !strcmp(face->material->identifier, gSource_screen_mat->identifier))) {
                face->material = gDestn_screen_mat;
            }
            face++;
        }
        BrModelUpdate(pModel, 0x7FFFu);
    }
}

// IDA: void __usercall SetModelFlags(br_model *pModel@<EAX>, int pOwner@<EDX>)
void SetModelFlags(br_model* pModel, int pOwner) {
    LOG_TRACE("(%p, %d)", pModel, pOwner);

    if (pModel && pModel->nfaces) {
        if (pOwner == eDriver_net_human || gAusterity_mode) {
            if ((pModel->flags & 0x80u) != 0) {
                pModel->flags &= 0xFF7Du; // not BR_MODF_KEEP_ORIGINAL, not BR_MODF_UPDATEABLE
                BrModelUpdate(pModel, 0x7FFFu);
            }
        } else {
            pModel->flags |= (BR_MODF_DONT_WELD | BR_MODF_KEEP_ORIGINAL | BR_MODF_UPDATEABLE);
            BrModelUpdate(pModel, 0x7FFFu);
        }
    }
}

// IDA: void __usercall LoadCar(char *pCar_name@<EAX>, tDriver pDriver@<EDX>, tCar_spec *pCar_spec@<EBX>, int pOwner@<ECX>, char *pDriver_name, tBrender_storage *pStorage_space)
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
    LOG_TRACE("(\"%s\", %d, %p, %d, \"%s\", %p)", pCar_name, pDriver, pCar_spec, pOwner, pDriver_name, pStorage_space);

    if (pDriver == eDriver_local_human) {
        if (!strcmp(gProgram_state.car_name, pCar_name))
            return;
        if (gProgram_state.car_name[0]) {
            DisposeCar(&gProgram_state.current_car, gProgram_state.current_car.index);
            ClearOutStorageSpace(&gOur_car_storage_space);
        }
        strcpy(gProgram_state.car_name, pCar_name);
    }
    pCar_spec->driver = pDriver;
    pCar_spec->index = pOwner;
    if (pDriver == eDriver_local_human) {
        gProgram_state.current_car_index = pOwner;
        gFunk_groove_flags[0] = 1;
        gGroove_funk_offset = 0;
    } else {
        gGroove_funk_offset = -1;
        for (i = 1; i < COUNT_OF(gFunk_groove_flags); i++) {
            if (!gFunk_groove_flags[i]) {
                pCar_spec->fg_index = i;
                gFunk_groove_flags[i] = 1;
                gGroove_funk_offset = 24 * i;
                break;
            }
        }
    }

    if (gGroove_funk_offset < 0) {
        FatalError(113);
    }
    if (!strcmp(pCar_name, "STELLA.TXT")) {
        pCar_spec->proxy_ray_distance = 6.0;
    } else {
        pCar_spec->proxy_ray_distance = 0.0;
    }
    PathCat(the_path, gApplication_path, "CARS");
    PathCat(the_path, the_path, pCar_name);
    f = DRfopen(the_path, "rt");
    if (!f) {
        FatalError(25);
    }
    PathCat(the_path, gApplication_path, gGraf_specs[gGraf_spec_index].data_dir_name);
    PathCat(the_path, the_path, "CARS");
    PathCat(the_path, the_path, pCar_name);
    AllowOpenToFail();
    g = DRfopen(the_path, "rt");
    if (!g) {
        PathCat(the_path, gApplication_path, gGraf_specs[gGraf_spec_index].data_dir_name);
        PathCat(the_path, the_path, "CARS");
        PathCat(the_path, the_path, gBasic_car_names[0]);
        g = DRfopen(the_path, "rt");
        if (!g)
            FatalError(104);
    }
    GetAString(f, s);
    strcpy(pCar_spec->name, s);
    if (strcmp(s, pCar_name) != 0)
        FatalError(115);
    if (*pDriver_name) {
        memcpy(pCar_spec->driver_name, pDriver_name, sizeof(pCar_spec->driver_name));
        pCar_spec->driver_name[31] = 0;
    } else {
        pCar_spec->driver_name[0] = 'X';
        pCar_spec->driver_name[1] = 0;
    }
    pCar_spec->can_be_stolen = 0;
    pCar_spec->has_been_stolen = 0;
    pCar_spec->knackered = 0;
    pCar_spec->time_last_hit = 0;
    pCar_spec->time_last_victim = 0;
    pCar_spec->disabled = 0;
    pCar_spec->active = 1;
    for (i = 0; i < COUNT_OF(pCar_spec->power_up_levels); ++i) {
        pCar_spec->power_up_levels[i] = 0;
    }
    GetALineAndDontArgue(f, s);
    if (pDriver == eDriver_local_human) {
        for (j = 0; j < COUNT_OF(pCar_spec->cockpit_images); j++) {
            GetALineAndDontArgue(g, s);
            str = strtok(s, "\t ,/");
            if (gAusterity_mode) {
                pCar_spec->cockpit_images[j] = 0;
            } else {
                the_image = LoadPixelmap(str);
                if (!the_image)
                    FatalError(26);
                pCar_spec->cockpit_images[j] = ConvertPixToStripMap(the_image);
                BrPixelmapFree(the_image);
            }
            GetALineAndDontArgue(g, s);
            str = strtok(s, "\t ,/");
            sscanf(str, "%d", &pCar_spec->render_left[j]);
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%d", &pCar_spec->render_top[j]);
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%d", &pCar_spec->render_right[j]);
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%d", &pCar_spec->render_bottom[j]);
            PossibleService();
        }
        LoadSpeedo(g, 0, pCar_spec);
        if (gAusterity_mode)
            GetALineAndDontArgue(g, s);
        else
            LoadSpeedo(g, 1, pCar_spec);
        PossibleService();
        LoadTacho(g, 0, pCar_spec);
        if (gAusterity_mode)
            GetALineAndDontArgue(g, s);
        else
            LoadTacho(g, 1, pCar_spec);
        PossibleService();
        LoadGear(g, 0, pCar_spec);
        if (gAusterity_mode)
            GetALineAndDontArgue(g, s);
        else
            LoadGear(g, 1, pCar_spec);
        PossibleService();
        GetALineAndDontArgue(g, s);
        str = strtok(s, "\t ,/");
        sscanf(str, "%d", &pCar_spec->number_of_hands_images);
        for (j = 0; pCar_spec->number_of_hands_images > j; ++j) {
            GetALineAndDontArgue(g, s);
            str = strtok(s, "\t ,/");
            sscanf(str, "%d", &pCar_spec->lhands_x[j]);
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%d", &pCar_spec->lhands_y[j]);
            str = strtok(NULL, "\t ,/");
            pCar_spec->lhands_images[j] = LoadPixelmap(str);
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%d", &pCar_spec->rhands_x[j]);
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%d", &pCar_spec->rhands_y[j]);
            str = strtok(NULL, "\t ,/");
            if (!gAusterity_mode) {
                pCar_spec->rhands_images[j] = LoadPixelmap(str);
            }
            PossibleService();
        }
        pCar_spec->red_line = 8000;
        GetALineAndDontArgue(f, s);
        str = strtok(s, "\t ,/");
        sscanf(str, "%f", &pCar_spec->driver_x_offset);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%f", &pCar_spec->driver_y_offset);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%f", &pCar_spec->driver_z_offset);
        GetALineAndDontArgue(f, s);
        str = strtok(s, "\t ,/");
        sscanf(str, "%f", &pCar_spec->head_left_angle);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%f", &pCar_spec->head_right_angle);
        GetALineAndDontArgue(f, s);
        str = strtok(s, "\t ,/");
        sscanf(str, "%f", &pCar_spec->mirror_x_offset);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%f", &pCar_spec->mirror_y_offset);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%f", &pCar_spec->mirror_z_offset);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%f", &pCar_spec->rearview_camera_angle);
        GetALineAndDontArgue(g, s);
        str = strtok(s, "\t ,/");
        sscanf(str, "%d", &pCar_spec->mirror_left);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->mirror_top);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->mirror_right);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->mirror_bottom);
        GetALineAndDontArgue(g, s);
        str = strtok(s, "\t ,/");
        sscanf(str, "%d", &pCar_spec->prat_left);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->prat_top);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->prat_right);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->prat_bottom);
        GetALineAndDontArgue(f, s);
        PossibleService();
        str = strtok(s, "\t ,/");
        pCar_spec->prat_cam_left = LoadPixelmap(str);
        str = strtok(NULL, "\t ,/");
        pCar_spec->prat_cam_top = LoadPixelmap(str);
        str = strtok(NULL, "\t ,/");
        pCar_spec->prat_cam_right = LoadPixelmap(str);
        str = strtok(NULL, "\t ,/");
        pCar_spec->prat_cam_bottom = LoadPixelmap(str);
        PossibleService();
        for (j = 0; j < COUNT_OF(pCar_spec->damage_units); ++j) {
            if (j == 2) {
                pCar_spec->damage_units[2].images = 0;
            } else {
                GetALineAndDontArgue(g, s);
                str = strtok(s, "\t ,/");
                sscanf(str, "%d", &pCar_spec->damage_units[j].x_coord);
                str = strtok(NULL, "\t ,/");
                sscanf(str, "%d", &pCar_spec->damage_units[j].y_coord);
                for (k = 0; k < COUNT_OF(pCar_spec->damage_units[j].periods); k++) {
                    str = strtok(NULL, "\t ,/");
                    sscanf(str, "%f", &temp_float);
                    rate = 1000.0 / temp_float / 2.0;
                    pCar_spec->damage_units[j].periods[k] = rate;
                }
                str = strtok(NULL, "\t ,/");
                pCar_spec->damage_units[j].images = LoadPixelmap(str);
                if (!pCar_spec->damage_units[j].images)
                    FatalError(27);
            }
            pCar_spec->damage_units[j].damage_level = 0;
            PossibleService();
        }
        GetALineAndDontArgue(g, s);
        str = strtok(s, "\t ,/");
        sscanf(str, "%d", &pCar_spec->damage_x_offset);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->damage_y_offset);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->damage_background_x);
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &pCar_spec->damage_background_y);
        str = strtok(NULL, "\t ,/");
        pCar_spec->damage_background = LoadPixelmap(str);

        for (i = 0; i < COUNT_OF(pCar_spec->dim_count); i++) {
            pCar_spec->dim_count[i] = GetAnInt(g);
            for (j = 0; j < pCar_spec->dim_count[i]; j++)
                GetFourInts(
                    g,
                    &pCar_spec->dim_left[i][j],
                    &pCar_spec->dim_top[i][j],
                    &pCar_spec->dim_right[i][j],
                    &pCar_spec->dim_bottom[i][j]);
        }
        PathCat(the_path, gApplication_path, gGraf_specs[gGraf_spec_index].data_dir_name);
        PathCat(the_path, the_path, "HEADUP.TXT");
        h = DRfopen(the_path, "rt");
        if (!h) {
            FatalError(102);
        }
        PossibleService();
        LoadHeadups(h, 0, pCar_spec);
        LoadHeadups(h, 1, pCar_spec);
        PossibleService();
        fclose(h);
        PathCat(the_path, gApplication_path, "PARTSHOP.TXT");
        h = DRfopen(the_path, "rt");
        if (!h) {
            FatalError(103);
        }
        for (i = 0; i < COUNT_OF(pCar_spec->power_ups); ++i) {
            GetALineAndDontArgue(h, s);
            str = strtok(s, "\t ,/");
            sscanf(str, "%d", &pCar_spec->power_ups[i].number_of_parts);
            for (j = 0; pCar_spec->power_ups[i].number_of_parts > j; ++j) {
                GetALineAndDontArgue(h, s);
                str = strtok(s, "\t ,/");
                sscanf(str, "%d", &pCar_spec->power_ups[i].info[j].rank_required);
                str = strtok(NULL, "\t ,/");
                strcpy(pCar_spec->power_ups[i].info[j].part_name, str);
                pCar_spec->power_ups[i].info[j].data_ptr = NULL;
                for (k = 0; k < 3; ++k) {
                    str = strtok(NULL, "\t ,/");
                    sscanf(str, "%d", &pCar_spec->power_ups[i].info[j].prices[k]);
                }
            }
            PossibleService();
        }
        fclose(h);
        AdjustCarCoordinates(&gProgram_state.current_car);
        AdjustRenderScreenSize();
        LOG_DEBUG("out");
        //PossibleService();
        ReinitialiseRearviewCamera();
        GetALineAndDontArgue(f, s);
    } else {
        while (!feof(f)) {
            GetALineAndDontArgue(f, s);
            if (strcmp(s, "END OF DRIVABLE STUFF") == 0) {
                break;
            }
        }
        pCar_spec->red_line = 8000;
    }
    PossibleService();
    GetThreeInts(f, pCar_spec->engine_noises, &pCar_spec->engine_noises[1], &pCar_spec->engine_noises[2]);
    GetAString(f, s);
    pCar_spec->can_be_stolen = strcmp(s, "stealworthy") == 0;
    GetDamageProgram(f, pCar_spec, 0);
    GetDamageProgram(f, pCar_spec, 1);
    GetDamageProgram(f, pCar_spec, 2);
    GetDamageProgram(f, pCar_spec, 3);
    GetDamageProgram(f, pCar_spec, 4);
    GetDamageProgram(f, pCar_spec, 5);
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    strcpy(pCar_spec->grid_icon_names[0], str);
    str = strtok(NULL, "\t ,/");
    strcpy(pCar_spec->grid_icon_names[1], str);
    str = strtok(NULL, "\t ,/");
    strcpy(pCar_spec->grid_icon_names[2], str);
    pCar_spec->grid_icon_image = 0;
    if (gAusterity_mode) {
        LoadSomePixelmaps(pStorage_space, f);
        SkipNLines(f);
        SkipNLines(f);
    } else if (gGraf_data_index) {
        SkipNLines(f);
        SkipNLines(f);
        LoadSomePixelmaps(pStorage_space, f);
    } else {
        SkipNLines(f);
        LoadSomePixelmaps(pStorage_space, f);
        SkipNLines(f);
    }
    LoadSomeShadeTables(pStorage_space, f);
    old_material_count = pStorage_space->materials_count;
    if (gAusterity_mode) {
        LoadSomeMaterials(pStorage_space, f);
        SkipNLines(f);
        SkipNLines(f);
    } else if (gGraf_data_index) {
        SkipNLines(f);
        SkipNLines(f);
        LoadSomeMaterials(pStorage_space, f);
    } else {
        SkipNLines(f);
        LoadSomeMaterials(pStorage_space, f);
        SkipNLines(f);
    }

    number_of_floorpans = 5;
    for (i = old_material_count; i < pStorage_space->materials_count; i++) {
        if (pStorage_space->materials[i] && pStorage_space->materials[i]->colour_map) {
            pStorage_space->materials[i]->flags |= (BR_MATF_LIGHT | BR_MATF_PRELIT | BR_MATF_SMOOTH);
            if (pStorage_space->materials[i]->flags & BR_MATF_TWO_SIDED) {
                its_a_floorpan = 0;
                for (j = 0; j < number_of_floorpans; j++) {
                    if (!strcmp(gFloorpan_names[j], pStorage_space->materials[i]->identifier)) {
                        its_a_floorpan = 1;
                        break;
                    }
                }
                if (!its_a_floorpan) {
                    pStorage_space->materials[i]->colour_map_1 = DOUBLESIDED_FLAG_COLOR_MAP;
                }
                pStorage_space->materials[i]->flags &= 0xFFFFEFFF;
            }
            pStorage_space->materials[i]->index_shade = gShade_list[0];
            BrMaterialUpdate(pStorage_space->materials[i], 0x7FFF);
        }
        PossibleService();
    }
    old_model_count = pStorage_space->models_count;
    LoadSomeModels(pStorage_space, f);
    if (pDriver == eDriver_local_human) {
        pCar_spec->car_master_actor = gSelf;
    } else {
        pCar_spec->car_master_actor = BrActorAllocate(0, NULL);
        BrActorAdd(gNon_track_actor, pCar_spec->car_master_actor);
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &pCar_spec->car_actor_count);
    pCar_spec->principal_car_actor = 0;
    for (i = 0; pCar_spec->car_actor_count > i; ++i) {
        PossibleService();
        GetALineAndDontArgue(f, s);
        str = strtok(s, "\t ,/");
        sscanf(str, "%f", &temp_float);
        if (temp_float < 0.0 && pDriver != eDriver_local_human) {
            FreeUpBonnetModels(&pStorage_space->models[old_model_count], pStorage_space->models_count - old_model_count);
            --pCar_spec->car_actor_count;
            break;
        }
        if (temp_float >= 1.0)
            pCar_spec->car_model_actors[i].min_distance_squared = temp_float * temp_float;
        else
            pCar_spec->car_model_actors[i].min_distance_squared = temp_float;
        str = strtok(NULL, "\t ,/");
        PathCat(the_path, gApplication_path, "ACTORS");
        PathCat(the_path, the_path, str);
        pCar_spec->car_model_actors[i].actor = BrActorLoad(the_path);
        if (!pCar_spec->car_model_actors[i].actor) {
            FatalError(71);
        }
        LOG_DEBUG("actor %s", pCar_spec->car_model_actors[i].actor->identifier);
        LOG_DEBUG("actor %s, model %s", pCar_spec->car_model_actors[i].actor->identifier, pCar_spec->car_model_actors[i].actor->model->identifier);
        LinkModelsToActor(
            pCar_spec->car_model_actors[i].actor,
            &pStorage_space->models[old_model_count],
            pStorage_space->models_count - old_model_count);
        PossibleService();
        for (j = old_model_count; j < pStorage_space->models_count; j++) {
            SetModelFlags(pStorage_space->models[j], pOwner);
        }
        BrActorAdd(pCar_spec->car_master_actor, pCar_spec->car_model_actors[i].actor);
        if (pCar_spec->car_model_actors[i].min_distance_squared == 0.0)
            pCar_spec->principal_car_actor = i;
    }
    if (pDriver != eDriver_local_human && pCar_spec->car_model_actors[pCar_spec->car_actor_count - 1].min_distance_squared < 0.0)
        SwitchCarActor(pCar_spec, pCar_spec->car_actor_count - 2);
    else
        SwitchCarActor(pCar_spec, pCar_spec->car_actor_count - 1);
    GetAString(f, s);
    pCar_spec->screen_material = BrMaterialFind(s);
    if (pCar_spec->screen_material) {
        gSource_screen_mat = pCar_spec->screen_material;
        pCar_spec->screen_material = DRMaterialClone(pCar_spec->screen_material);
        gDestn_screen_mat = pCar_spec->screen_material;
    } else {
        gSource_screen_mat = NULL;
        gDestn_screen_mat = NULL;
    }
    pCar_spec->screen_material_source = NULL;
    if (gDestn_screen_mat)
        MungeWindscreen(pCar_spec->car_model_actors[pCar_spec->principal_car_actor].actor->model);
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &pCar_spec->number_of_steerable_wheels);
    for (i = 0; pCar_spec->number_of_steerable_wheels > i; ++i) {
        GetALineAndDontArgue(f, s);
        str = strtok(s, "\t ,/");
        sscanf(str, "%d", &pCar_spec->steering_ref[i]);
        AddRefOffset(&pCar_spec->steering_ref[i]);
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    for (i = 0; i < 4; ++i) {
        sscanf(str, "%d", &pCar_spec->lf_sus_ref[i]);
        AddRefOffset(&pCar_spec->lf_sus_ref[i]);
        str = strtok(NULL, "\t ,/");
    }
    PossibleService();
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    for (i = 0; i < 4; ++i) {
        sscanf(str, "%d", &pCar_spec->rf_sus_ref[i]);
        AddRefOffset(&pCar_spec->rf_sus_ref[i]);
        str = strtok(NULL, "\t ,/");
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    for (i = 0; i < 2; ++i) {
        sscanf(str, "%d", &pCar_spec->lr_sus_ref[i]);
        AddRefOffset(&pCar_spec->lr_sus_ref[i]);
        str = strtok(NULL, "\t ,/");
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    for (i = 0; i < 2; ++i) {
        sscanf(str, "%d", &pCar_spec->rr_sus_ref[i]);
        AddRefOffset(&pCar_spec->rr_sus_ref[i]);
        str = strtok(NULL, "\t ,/");
    }
    GetFourInts(
        f,
        &pCar_spec->driven_wheels_spin_ref_1,
        &pCar_spec->driven_wheels_spin_ref_2,
        &pCar_spec->driven_wheels_spin_ref_3,
        &pCar_spec->driven_wheels_spin_ref_4);
    AddRefOffset(&pCar_spec->driven_wheels_spin_ref_1);
    AddRefOffset(&pCar_spec->driven_wheels_spin_ref_2);
    AddRefOffset(&pCar_spec->driven_wheels_spin_ref_3);
    AddRefOffset(&pCar_spec->driven_wheels_spin_ref_4);
    PossibleService();
    GetFourInts(
        f,
        &pCar_spec->non_driven_wheels_spin_ref_1,
        &pCar_spec->non_driven_wheels_spin_ref_2,
        &pCar_spec->non_driven_wheels_spin_ref_3,
        &pCar_spec->non_driven_wheels_spin_ref_4);
    AddRefOffset(&pCar_spec->non_driven_wheels_spin_ref_1);
    AddRefOffset(&pCar_spec->non_driven_wheels_spin_ref_2);
    AddRefOffset(&pCar_spec->non_driven_wheels_spin_ref_3);
    AddRefOffset(&pCar_spec->non_driven_wheels_spin_ref_4);
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%f", &temp_float);
    pCar_spec->driven_wheels_circum = temp_float * 2.0 * 3.141592653589793;
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%f", &temp_float);
    pCar_spec->non_driven_wheels_circum = temp_float * 2.0 * 3.141592653589793;
    pCar_spec->car_model_variable = pDriver != 4;
    PossibleService();
    GetALineAndDontArgue(f, s);
    AddFunkotronics(f, pOwner, gGroove_funk_offset);
    GetALineAndDontArgue(f, s);
    AddGroovidelics(f, pOwner, pCar_spec->car_master_actor, gGroove_funk_offset, 1);
    for (i = 0; i < pCar_spec->car_actor_count; i++) {
        PossibleService();
        // Jeff: Bug? pOwner will never be 3. Probably this was supposed to be "pDriver == eDriver_net_human"?
        if (pOwner == 3 || gAusterity_mode) {
            temp_float = SkipCrushData(f);
            pCar_spec->car_model_actors[i].crush_data.softness_factor = temp_float;
            pCar_spec->car_model_actors[i].crush_data.crush_points = 0;
            pCar_spec->car_model_actors[i].crush_data.number_of_crush_points = 0;
        } else {
            ReadCrushData(f, &pCar_spec->car_model_actors[i].crush_data);
        }
        if (pCar_spec->driver < eDriver_net_human || gAusterity_mode) {
            pCar_spec->car_model_actors[i].undamaged_vertices = NULL;
        } else {
            PossibleService();
            vertex_array_size = sizeof(br_vertex) * pCar_spec->car_model_actors[i].actor->model->nvertices;
            pCar_spec->car_model_actors[i].undamaged_vertices = BrMemAllocate(vertex_array_size, kMem_undamaged_vertices);
            memcpy(
                pCar_spec->car_model_actors[i].undamaged_vertices,
                pCar_spec->car_model_actors[i].actor->model->vertices,
                vertex_array_size);
        }
    }
    if (pDriver != eDriver_local_human) {
        SkipCrushData(f);
    }
    PossibleService();
    for (i = 0; i < COUNT_OF(gWheel_actor_names); ++i) {
        pCar_spec->wheel_actors[i] = DRActorFindRecurse(pCar_spec->car_master_actor, gWheel_actor_names[i]);
    }
    PossibleService();
    ReadMechanicsData(f, pCar_spec);
    PossibleService();
    ReadShrapnelMaterials(f, (tCollision_info*)pCar_spec);
    vertex_total = 0;
    model = pCar_spec->car_model_actors[pCar_spec->principal_car_actor].actor->model;
    for (i = 0; i < V11MODEL(model)->ngroups; i++) {
        vertex_total += V11MODEL(model)->groups[i].nvertices;
    }
    for (i = 0; i < COUNT_OF(pCar_spec->fire_vertex); i++) {
        if (feof(f)) {
            initial_vertex = IRandomBetween(0, vertex_total - 1);
            pCar_spec->fire_vertex[i] = initial_vertex;
        } else {
            initial_vertex = GetAnInt(f);
            pCar_spec->fire_vertex[i] = initial_vertex;
            if (pCar_spec->fire_vertex[i] >= vertex_total)
                pCar_spec->fire_vertex[i] = 0;
            v_num = 0;
            for (group = 0; group < V11MODEL(model)->ngroups; group++) {
                for (v = 0; v < V11MODEL(model)->groups[group].nvertices; v++) {
                    if (V11MODEL(model)->groups[group].vertex_user[v] == pCar_spec->fire_vertex[i]) {
                        pCar_spec->fire_vertex[i] = v_num;
                        group = V11MODEL(model)->ngroups;
                        break;
                    }
                    v_num++;
                }
            }
        }
    }
    fclose(f);
    fclose(g);
}

// IDA: void __cdecl LoadHeadupImages()
void LoadHeadupImages() {
    int i;
    tPath_name the_path;
    LOG_TRACE("()");

    for (i = 0; i < 31; ++i) {
        PossibleService();
        if (gHeadup_image_info[i].avail && (gHeadup_image_info[i].avail != eNot_net || gNet_mode) && (gHeadup_image_info[i].avail != eNet_only || !gNet_mode)) {
            gHeadup_images[i] = NULL;
        } else {
            gHeadup_images[i] = LoadPixelmap(gHeadup_image_info[i].name);
        }
    }
}

// IDA: void __cdecl DisposeHeadupImages()
void DisposeHeadupImages() {
    int i;
    tPath_name the_path;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: FILE* __cdecl OpenRaceFile()
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

// IDA: void __usercall SkipRestOfRace(FILE *pF@<EAX>)
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

// IDA: void __usercall LoadRaces(tRace_list_spec *pRace_list@<EAX>, int *pCount@<EDX>, int pRace_type_index@<EBX>)
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

// IDA: void __usercall UnlockOpponentMugshot(int pIndex@<EAX>)
void UnlockOpponentMugshot(int pIndex) {
    LOG_TRACE("(%d)", pIndex);

    if (pIndex >= 0) {
        if (gOpponents[pIndex].mug_shot_image_data) {
            MAMSUnlock((void**)&gOpponents[pIndex].mug_shot_image_data);
        }
    }
}

// IDA: void __usercall LoadOpponentMugShot(int pIndex@<EAX>)
void LoadOpponentMugShot(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    PossibleService();
    if (pIndex >= 0 && !gOpponents[pIndex].mug_shot_image_data) {
        if (!LoadFlicData(
                gOpponents[pIndex].mug_shot_name,
                &gOpponents[pIndex].mug_shot_image_data,
                &gOpponents[pIndex].mug_shot_image_data_length)) {
            FatalError(56);
        }
    }
}

// IDA: void __usercall DisposeOpponentGridIcon(tRace_info *pRace_info@<EAX>, int pIndex@<EDX>)
void DisposeOpponentGridIcon(tRace_info* pRace_info, int pIndex) {
    LOG_TRACE("(%p, %d)", pRace_info, pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LoadOpponentGridIcon(tRace_info *pRace_info@<EAX>, int pIndex@<EDX>)
void LoadOpponentGridIcon(tRace_info* pRace_info, int pIndex) {
    LOG_TRACE("(%p, %d)", pRace_info, pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LoadRaceInfo(int pRace_index@<EAX>, tRace_info *pRace_info@<EDX>)
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
    LOG_TRACE("(%d, %p)", pRace_index, pRace_info);

    f = OpenRaceFile();
    temp_index = pRace_index;
    while (1) {
        if (!temp_index) {
            break;
        }
        PossibleService();
        GetALineAndDontArgue(f, s);
        SkipRestOfRace(f);
        temp_index--;
    }
    GetALineAndDontArgue(f, pRace_info->name);
    pRace_info->rank_required = gRace_list[pRace_index].rank_required;
    pRace_info->best_rank = gRace_list[pRace_index].best_rank;
    pRace_info->suggested_rank = gRace_list[pRace_index].suggested_rank;
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    pRace_info->scene_image_data = NULL;
    pRace_info->map_image_data = NULL;
    pRace_info->info_image_data = NULL;
    PossibleService();
    if (!gNet_mode) {
        if (!LoadFlicData(str, &pRace_info->scene_image_data, &pRace_info->scene_image_data_length)) {
            FatalError(51);
        }
        str = strtok(0, "\t ,/");
        if (!LoadFlicData(str, &pRace_info->map_image_data, &pRace_info->map_image_data_length)) {
            FatalError(52);
        }
        str = strtok(0, "\t ,/");
        if (!LoadFlicData(str, &pRace_info->info_image_data, &pRace_info->info_image_data_length)) {
            FatalError(53);
        }
        for (i = 0; i < pRace_info->number_of_racers; i++) {
            PossibleService();
            LoadOpponentMugShot(pRace_info->opponent_list[i].index);
        }
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    strcpy(pRace_info->track_file_name, str);
    pRace_info->text_chunk_count = GetAnInt(f);
    pRace_info->text_chunks = BrMemAllocate(sizeof(tText_chunk) * pRace_info->text_chunk_count, kMem_race_text_chunk);

    the_chunk = pRace_info->text_chunks;
    for (i = 0; i < pRace_info->text_chunk_count; i++) {
        //while (pRace_info->text_chunk_count > v5) {
        PossibleService();
        GetPairOfInts(f, &the_chunk->x_coord, &the_chunk->y_coord);
        GetPairOfInts(f, &the_chunk->frame_cue, &the_chunk->frame_end);
        the_chunk->line_count = GetAnInt(f);
        while (the_chunk->line_count > 8) {
            --the_chunk->line_count;
            GetALineAndDontArgue(f, s);
        }
        for (k = 0; k < the_chunk->line_count; k++) {
            GetALineAndDontArgue(f, s);
            the_chunk->text[k] = BrMemAllocate(strlen(s) + 1, kMem_race_text_str);
            strcpy(the_chunk->text[k], s);
        }
        the_chunk++;
    }
    fclose(f);
}

// IDA: void __usercall DisposeRaceInfo(tRace_info *pRace_info@<EAX>)
void DisposeRaceInfo(tRace_info* pRace_info) {
    int i;
    int j;
    int k;
    tText_chunk* the_chunk;
    LOG_TRACE("(%p)", pRace_info);

    if (!gNet_mode) {
        the_chunk = pRace_info->text_chunks;
        for (i = 0; i < pRace_info->text_chunk_count; i++) {
            PossibleService();
            for (j = 0; j < the_chunk->line_count; j++) {
                if (the_chunk->text[j]) {
                    BrMemFree(the_chunk->text[j]);
                }
            }
            the_chunk++;
        }
        if (pRace_info->text_chunks) {
            BrMemFree(pRace_info->text_chunks);
        }
        if (pRace_info->scene_image_data) {
            BrMemFree(pRace_info->scene_image_data);
        }
        if (pRace_info->map_image_data) {
            BrMemFree(pRace_info->map_image_data);
        }
        PossibleService();
        if (pRace_info->info_image_data) {
            BrMemFree(pRace_info->info_image_data);
        }
        for (k = 0; k < pRace_info->number_of_racers; k++) {
            UnlockOpponentMugshot(pRace_info->opponent_list[k].index);
        }
        PossibleService();
    }
}

// IDA: void __usercall LoadGridIcons(tRace_info *pRace_info@<EAX>)
void LoadGridIcons(tRace_info* pRace_info) {
    int i;
    LOG_TRACE("(%p)", pRace_info);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeGridIcons(tRace_info *pRace_info@<EAX>)
void DisposeGridIcons(tRace_info* pRace_info) {
    int i;
    LOG_TRACE("(%p)", pRace_info);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LoadOpponents()
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

// IDA: br_font* __usercall LoadBRFont@<EAX>(char *pName@<EAX>)
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

// IDA: void __cdecl LoadParts()
void LoadParts() {
    int i;
    int j;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl UnlockParts()
void UnlockParts() {
    int i;
    int j;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __cdecl LoadChromeFont()
br_pixelmap* LoadChromeFont() {
    br_pixelmap* result;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeChromeFont(br_pixelmap *pThe_font@<EAX>)
void DisposeChromeFont(br_pixelmap* pThe_font) {
    LOG_TRACE("(%p)", pThe_font);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GetALineAndInterpretCommand@<EAX>(FILE *pF@<EAX>, char **pString_list@<EDX>, int pCount@<EBX>)
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

// IDA: int __usercall GetAnInt@<EAX>(FILE *pF@<EAX>)
int GetAnInt(FILE* pF) {
    char s[256];
    char* str;
    int result;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &result);
    return result;
}

// IDA: float __usercall GetAFloat@<ST0>(FILE *pF@<EAX>)
float GetAFloat(FILE* pF) {
    char s[256];
    char* str;
    float result;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%f", &result);
    return result;
}

// IDA: float __usercall GetAFloatPercent@<ST0>(FILE *pF@<EAX>)
float GetAFloatPercent(FILE* pF) {
    char s[256];
    char* str;
    float result;
    LOG_TRACE("(%p)", pF);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetPairOfFloats(FILE *pF@<EAX>, float *pF1@<EDX>, float *pF2@<EBX>)
void GetPairOfFloats(FILE* pF, float* pF1, float* pF2) {
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%f", pF1);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%f", pF2);
}

// IDA: void __usercall GetThreeFloats(FILE *pF@<EAX>, float *pF1@<EDX>, float *pF2@<EBX>, float *pF3@<ECX>)
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

// IDA: void __usercall GetPairOfInts(FILE *pF@<EAX>, int *pF1@<EDX>, int *pF2@<EBX>)
void GetPairOfInts(FILE* pF, int* pF1, int* pF2) {
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", pF1);
    str = strtok(NULL, "\t ,/");
    sscanf(str, "%d", pF2);
}

// IDA: void __usercall GetThreeInts(FILE *pF@<EAX>, int *pF1@<EDX>, int *pF2@<EBX>, int *pF3@<ECX>)
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

// IDA: void __usercall GetThreeIntsAndAString(FILE *pF@<EAX>, int *pF1@<EDX>, int *pF2@<EBX>, int *pF3@<ECX>, char *pS)
void GetThreeIntsAndAString(FILE* pF, int* pF1, int* pF2, int* pF3, char* pS) {
    char s[256];
    char* str;
    LOG_TRACE("(%p, %p, %p, %p, \"%s\")", pF, pF1, pF2, pF3, pS);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetFourInts(FILE *pF@<EAX>, int *pF1@<EDX>, int *pF2@<EBX>, int *pF3@<ECX>, int *pF4)
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

// IDA: br_scalar __usercall GetAScalar@<ST0>(FILE *pF@<EAX>)
br_scalar GetAScalar(FILE* pF) {
    LOG_TRACE("(%p)", pF);
    return GetAFloat(pF);
}

// IDA: void __usercall GetPairOfScalars(FILE *pF@<EAX>, br_scalar *pS1@<EDX>, br_scalar *pS2@<EBX>)
void GetPairOfScalars(FILE* pF, br_scalar* pS1, br_scalar* pS2) {
    LOG_TRACE("(%p, %p, %p)", pF, pS1, pS2);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetThreeScalars(FILE *pF@<EAX>, br_scalar *pS1@<EDX>, br_scalar *pS2@<EBX>, br_scalar *pS3@<ECX>)
void GetThreeScalars(FILE* pF, br_scalar* pS1, br_scalar* pS2, br_scalar* pS3) {
    LOG_TRACE("(%p, %p, %p, %p)", pF, pS1, pS2, pS3);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetFourScalars(FILE *pF@<EAX>, br_scalar *pF1@<EDX>, br_scalar *pF2@<EBX>, br_scalar *pF3@<ECX>, br_scalar *pF4)
void GetFourScalars(FILE* pF, br_scalar* pF1, br_scalar* pF2, br_scalar* pF3, br_scalar* pF4) {
    char s[256];
    char* str;
    float f1;
    float f2;
    float f3;
    float f4;
    LOG_TRACE("(%p, %p, %p, %p, %p)", pF, pF1, pF2, pF3, pF4);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetFiveScalars(FILE *pF@<EAX>, br_scalar *pF1@<EDX>, br_scalar *pF2@<EBX>, br_scalar *pF3@<ECX>, br_scalar *pF4, br_scalar *pF5)
void GetFiveScalars(FILE* pF, br_scalar* pF1, br_scalar* pF2, br_scalar* pF3, br_scalar* pF4, br_scalar* pF5) {
    char s[256];
    char* str;
    float f1;
    float f2;
    float f3;
    float f4;
    float f5;
    LOG_TRACE("(%p, %p, %p, %p, %p, %p)", pF, pF1, pF2, pF3, pF4, pF5);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetNScalars(FILE *pF@<EAX>, int pNumber@<EDX>, br_scalar *pScalars@<EBX>)
void GetNScalars(FILE* pF, int pNumber, br_scalar* pScalars) {
    char s[256];
    char* str;
    float fleurting_point_numero;
    int i;
    LOG_TRACE("(%p, %d, %p)", pF, pNumber, pScalars);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetPairOfFloatPercents(FILE *pF@<EAX>, float *pF1@<EDX>, float *pF2@<EBX>)
void GetPairOfFloatPercents(FILE* pF, float* pF1, float* pF2) {
    char s[256];
    char* str;
    LOG_TRACE("(%p, %p, %p)", pF, pF1, pF2);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetThreeFloatPercents(FILE *pF@<EAX>, float *pF1@<EDX>, float *pF2@<EBX>, float *pF3@<ECX>)
void GetThreeFloatPercents(FILE* pF, float* pF1, float* pF2, float* pF3) {
    char s[256];
    char* str;
    LOG_TRACE("(%p, %p, %p, %p)", pF, pF1, pF2, pF3);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetAString(FILE *pF@<EAX>, char *pString@<EDX>)
void GetAString(FILE* pF, char* pString) {
    char s[256];
    char* str;

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    strcpy(pString, str);
}

// IDA: void __cdecl AboutToLoadFirstCar()
void AboutToLoadFirstCar() {
    LOG_TRACE("()");
    memset(gFunk_groove_flags, 0, sizeof(gFunk_groove_flags));
}

// IDA: void __usercall LoadOpponentsCars(tRace_info *pRace_info@<EAX>)
void LoadOpponentsCars(tRace_info* pRace_info) {
    int i;
    LOG_TRACE("(%p)", pRace_info);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeOpponentsCars(tRace_info *pRace_info@<EAX>)
void DisposeOpponentsCars(tRace_info* pRace_info) {
    int i;
    LOG_TRACE("(%p)", pRace_info);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LoadMiscStrings()
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

// IDA: void __usercall FillInRaceInfo(tRace_info *pThe_race@<EAX>)
void FillInRaceInfo(tRace_info* pThe_race) {
    LOG_TRACE("(%p)", pThe_race);

    strcpy(gProgram_state.track_file_name, pThe_race->track_file_name);
}

// IDA: FILE* __usercall OldDRfopen@<EAX>(char *pFilename@<EAX>, char *pMode@<EDX>)
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
    char ch;

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
                ch = fgetc(fp);
                if (ch != gDecode_thing) {
                    fclose(fp);
                    LOG_WARN("Unexpected encoding character");
                    return NULL;
                }
                ungetc(ch, fp);
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

// IDA: void __cdecl AllowOpenToFail()
void AllowOpenToFail() {
    LOG_TRACE("()");
    gAllow_open_to_fail = 1;
}

// IDA: void __cdecl DoNotAllowOpenToFail()
void DoNotAllowOpenToFail() {
    LOG_TRACE("()");
    gAllow_open_to_fail = 0;
}

// IDA: FILE* __usercall DRfopen@<EAX>(char *pFilename@<EAX>, char *pMode@<EDX>)
FILE* DRfopen(char* pFilename, char* pMode) {
    FILE* result;
    tPath_name CD_dir;
    char msg[336];
    LOG_TRACE("(\"%s\", \"%s\")", pFilename, pMode);

    result = OldDRfopen(pFilename, pMode);

    if (!result && !gAllow_open_to_fail) {
        LOG_DEBUG("allow to fail %d", gAllow_open_to_fail);
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

// IDA: int __usercall GetCDPathFromPathsTxtFile@<EAX>(char *pPath_name@<EAX>)
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

// IDA: int __cdecl TestForOriginalCarmaCDinDrive()
int TestForOriginalCarmaCDinDrive() {
    LOG_TRACE("()");

    // Jeff: the symbol dump didn't include any local variable information.
    // These names are not necessarily the original names.
    tPath_name cd_pathname;
    tPath_name cd_data_pathname;
    tPath_name cutscene_pathname;
    FILE* paths_txt_fp;
    tPath_name paths_txt;
    int paths_txt_first_char;

    // Jeff: Added to optionally bypass this check
    if (harness_disable_cd_check) {
        return 1;
    }

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

    strcpy(cutscene_pathname, cd_data_pathname);
    strcat(cutscene_pathname, gDir_separator);
    strcat(cutscene_pathname, "CUTSCENE");

    if (!PDCheckDriveExists2(cd_data_pathname, "GENERAL.TXT", 100)) {
        return 0;
    }
    if (!PDCheckDriveExists2(cd_pathname, "CARMA.EXE", 1000000)
        && !PDCheckDriveExists2(cd_pathname, "CARMAG.EXE", 1000000)
        && !PDCheckDriveExists2(cd_pathname, "MAINPROG.EXE", 1000000)
        && !PDCheckDriveExists2(cd_pathname, "CARMSPLT.EXE", 1000000)
        && !PDCheckDriveExists2(cd_pathname, "CARMGSPL.EXE", 1000000)) {
        return 0;
    }

    if (!PDCheckDriveExists2(cutscene_pathname, harness_game_mode.intro_smk_file, 2000000)) {
        return 0;
    }

    if (paths_txt_first_char != '@') {
        EncodeFile(paths_txt);
    }
    return 1;
}

// IDA: int __cdecl OriginalCarmaCDinDrive()
int OriginalCarmaCDinDrive() {
    return gCD_is_in_drive;
}

// IDA: int __cdecl CarmaCDinDriveOrFullGameInstalled()
int CarmaCDinDriveOrFullGameInstalled() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReadNetworkSettings(FILE *pF@<EAX>, tNet_game_options *pOptions@<EDX>)
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

// IDA: int __usercall PrintNetOptions@<EAX>(FILE *pF@<EAX>, int pIndex@<EDX>)
int PrintNetOptions(FILE* pF, int pIndex) {
    LOG_TRACE("(%p, %d)", pF, pIndex);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl SaveOptions()
int SaveOptions() {
    tPath_name the_path;
    FILE* f;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl RestoreOptions()
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

// IDA: void __cdecl InitFunkGrooveFlags()
void InitFunkGrooveFlags() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
