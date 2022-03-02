#include "pratcam.h"
#include "displays.h"
#include "errors.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "loading.h"
#include "sound.h"
#include "utility.h"
#include "brender/brender.h"
#include "pd/sys.h"
#include "harness/trace.h"
#include <stdlib.h>

tS3_sound_tag gWhirr_noise;
tFlic_descriptor gPrat_flic;
tPrat_sequence* gPratcam_sequences;
tPrat_flic_spec* gPratcam_flics;
int gCurrent_pratcam_chunk;
int gPending_ambient_prat;
int gCurrent_pratcam_index;
br_pixelmap* gPrat_buffer;
int gNumber_of_prat_sequences;
int gNumber_of_prat_flics;
tU32 gLast_pratcam_frame_time;
int gCurrent_pratcam_precedence;
int gCurrent_ambient_prat_sequence;
int gCurrent_pratcam_alternative;

// IDA: int __cdecl PratcamGetCurrent()
int PratcamGetCurrent() {
    LOG_TRACE("()");

    return gCurrent_pratcam_index;
}

// IDA: int __cdecl PratcamGetAmbient()
int PratcamGetAmbient() {
    LOG_TRACE("()");

    return gCurrent_ambient_prat_sequence;
}

// IDA: int __cdecl PratcamGetPending()
int PratcamGetPending() {
    LOG_TRACE("()");

    return gPending_ambient_prat;
}

// IDA: void __cdecl TogglePratcam()
void TogglePratcam() {
    tU32 the_time;
    tU32 time_diff;
    LOG_TRACE("()");

    if (gAusterity_mode) {
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(192));
    } else {
        the_time = GetTotalTime();
        gProgram_state.prat_cam_on = !gProgram_state.prat_cam_on;
        time_diff = the_time = gProgram_state.pratcam_move_start;
        if (time_diff <= 400) {
            gProgram_state.pratcam_move_start = the_time - 400 + time_diff;
        }
    }
}

// IDA: void __usercall LoadPratcam(char *pFolder_name@<EAX>)
void LoadPratcam(char* pFolder_name) {
    FILE* f;
    FILE* g;
    tPath_name the_path;
    int i;
    int j;
    int k;
    int l;
    char s[256];
    char folder_path[256];
    char* str;
    char num_str[256];
    LOG_TRACE("(\"%s\")", pFolder_name);

    if (gAusterity_mode) {
        return;
    }
    PathCat(folder_path, gApplication_path, gGraf_specs[gGraf_spec_index].data_dir_name);
    PathCat(folder_path, folder_path, "PRATCAM");
    PathCat(folder_path, folder_path, pFolder_name);
    PathCat(the_path, folder_path, "PRATCAM.TXT");
    f = DRfopen(the_path, "rt");
    if (f == NULL) {
        FatalError(28);
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &gNumber_of_prat_flics);
    gPratcam_flics = BrMemAllocate(gNumber_of_prat_flics * sizeof(tPrat_flic_spec), kMem_pratcam_flic_array);
    for (i = 0; i < gNumber_of_prat_flics; i++) {
        PossibleService();
        GetALineAndDontArgue(f, s);
        if (strcmp(s, "END") == 0) {
            FatalError(29);
        }
        str = strtok(s, "\t ,/");
        PathCat(the_path, folder_path, str);
        g = DRfopen(the_path, "rb");
        if (g == NULL) {
            FatalError(30);
        }
        gPratcam_flics[i].data_length = GetFileLength(g);
        gPratcam_flics[i].data = BrMemAllocate(gPratcam_flics[i].data_length, kMem_pratcam_flic_data);
        if (gPratcam_flics[i].data == NULL) {
            FatalError(31);
        }
        fread(gPratcam_flics[i].data, 1, gPratcam_flics[i].data_length, g);
        fclose(g);
    }
    GetALineAndDontArgue(f, s);
    if (strcmp(s, "END") != 0) {
        FatalError(32);
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &gNumber_of_prat_sequences);
    gPratcam_sequences = BrMemAllocate(gNumber_of_prat_sequences * sizeof(tPrat_sequence), kMem_pratcam_sequence_array);
    for (i = 0; i < gNumber_of_prat_sequences; i++) {
        PossibleService();
        sprintf(num_str, "%d", i);
        GetALineAndDontArgue(f, s);
        if (strcmp(s, "END") == 0) {
            FatalError(33);
        }
        str = strtok(s, "\t ,/");
        sscanf(str, "%d", &gPratcam_sequences[i].precedence);
        gPratcam_sequences[i].repeat_chunk = GetAnInt(f);
        gPratcam_sequences[i].number_of_chunks = GetAnInt(f);
        if (gPratcam_sequences[i].number_of_chunks > COUNT_OF(gPratcam_sequences[i].chunks)) {
            FatalError(37, num_str);
        }
        for (j = 0; j < gPratcam_sequences[i].number_of_chunks; j++) {
            GetALineAndDontArgue(f, s);
            str = strtok(s, "\t ,/");
            sscanf(str, "%d", &gPratcam_sequences[i].chunks[j].number_of_alternatives);
            if (gPratcam_sequences[i].chunks[j].number_of_alternatives > COUNT_OF(gPratcam_sequences[i].chunks[j].alternatives)) {
                FatalError(38, num_str);
            }
            for (k = 0; k < gPratcam_sequences[i].chunks[j].number_of_alternatives; k++) {
                GetALineAndDontArgue(f, s);
                str = strtok(s, "\t ,/");
                sscanf(str, "%d", &gPratcam_sequences[i].chunks[j].alternatives[k].ref);
                str = strtok(NULL, "\t ,/");
                sscanf(str, "%d", &gPratcam_sequences[i].chunks[j].alternatives[k].chance);
                str = strtok(NULL, "\t ,/");
                sscanf(str, "%d", &gPratcam_sequences[i].chunks[j].alternatives[k].sound_chance);
                if (gPratcam_sequences[i].chunks[j].alternatives[k].sound_chance > 0) {
                    str = strtok(NULL, "\t ,/");
                    sscanf(str, "%d", &gPratcam_sequences[i].chunks[j].alternatives[k].number_of_sounds);
                    if (gPratcam_sequences[i].chunks[j].alternatives[k].number_of_sounds > COUNT_OF(gPratcam_sequences[i].chunks[j].alternatives[k].sound_ids)) {
                        FatalError(39, num_str);
                    }
                    for (l = 0; l < gPratcam_sequences[i].chunks[j].alternatives[k].number_of_sounds; l++) {
                        str = strtok(NULL, "\t ,/");
                        sscanf(str, "%d", &gPratcam_sequences[i].chunks[j].alternatives[k].sound_ids[l]);
                        DRS3LoadSound(gPratcam_sequences[i].chunks[j].alternatives[k].sound_ids[l]);
                    }
                }
            }
        }
    }
    GetALineAndDontArgue(f, s);
    if (strcmp(s, "END") != 0) {
        FatalError(34);
    }
    fclose(f);
}

// IDA: void __cdecl NextPratcamChunk()
void NextPratcamChunk() {
    int i;
    int random_number;
    int count;
    tPrat_alternative* current_alternative;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NewPratcamSequence(int pSequence_index@<EAX>, int pStart_chunk@<EDX>)
void NewPratcamSequence(int pSequence_index, int pStart_chunk) {
    LOG_TRACE("(%d, %d)", pSequence_index, pStart_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ChangeAmbientPratcamNow(int pIndex@<EAX>, int pStart_chunk@<EDX>)
void ChangeAmbientPratcamNow(int pIndex, int pStart_chunk) {
    LOG_TRACE("(%d, %d)", pIndex, pStart_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ChangeAmbientPratcam(int pIndex@<EAX>)
void ChangeAmbientPratcam(int pIndex) {
    LOG_TRACE("(%d)", pIndex);

    if (!gRace_finished && !gInterface_within_race_mode && pIndex != gCurrent_ambient_prat_sequence && gProgram_state.prat_cam_on) {
        if (gCurrent_pratcam_index == -1) {
            ChangeAmbientPratcamNow(pIndex, 0);
        } else {
            gPending_ambient_prat = pIndex;
        }
    }
}

// IDA: void __usercall PratcamEventNow(int pIndex@<EAX>)
void PratcamEventNow(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PratcamEvent(int pIndex@<EAX>)
void PratcamEvent(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl HighResPratBufferWidth()
int HighResPratBufferWidth() {
    int prat_width;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl HighResPratBufferHeight()
int HighResPratBufferHeight() {
    int prat_height;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitPratcam()
void InitPratcam() {
    void* the_pixels;
    LOG_TRACE("()");

    if (gAusterity_mode) {
        return;
    }
    gWhirr_noise = 0;
    gPrat_flic.data = NULL;
    gCurrent_ambient_prat_sequence = -1;
    switch (gGraf_data_index) {
    case 0:
        the_pixels = BrMemAllocate(2392, kMem_pratcam_pixelmap);
        break;
    case 1:
        the_pixels = BrMemAllocate(1140, kMem_pratcam_pixelmap);
        break;
    default:
        TELL_ME_IF_WE_PASS_THIS_WAY();
    }
    if (gScreen->row_bytes < 0) {
        BrFatal("C:\\Msdev\\Projects\\DethRace\\Pratcam.c", 409, "Bruce bug at line %d, file C:\\Msdev\\Projects\\DethRace\\Pratcam.c", 409);
    }
    switch (gGraf_data_index) {
    case 0:
        gPrat_buffer = DRPixelmapAllocate(gScreen->type, 52, 46, the_pixels, 0);
        break;
    case 1:
        gPrat_buffer = DRPixelmapAllocate(gScreen->type, 104, 110, the_pixels, 0);
        break;
    default:
        TELL_ME_IF_WE_PASS_THIS_WAY();
    }
    gCurrent_pratcam_index = -1;
    gPending_ambient_prat = -1;
    gCurrent_pratcam_precedence = 0;
    if (gNet_mode == eNet_mode_none) {
        if (gProgram_state.frank_or_anniness == eFrankie) {
            LoadPratcam("FRANK");
        } else {
            LoadPratcam("ANNIE");
        }
    } else if (gNet_players[gThis_net_player_index].car_index == 1 || gNet_players[gThis_net_player_index].car_index > 34) {
        LoadPratcam("ANNIE");
    } else {
        LoadPratcam("FRANK");
    }
}

// IDA: void __cdecl DisposePratcam()
void DisposePratcam() {
    int i;
    int j;
    int k;
    int l;
    LOG_TRACE("()");

    if (gAusterity_mode) {
        return;
    }
    if (gWhirr_noise != 0) {
        DRS3StopSound(gWhirr_noise);
        gWhirr_noise = 0;
    }
    if (gCurrent_pratcam_index != -1) {
        EndFlic(&gPrat_flic);
    }
    BrMemFree(gPrat_buffer->pixels);
    BrPixelmapFree(gPrat_buffer);
    for (i = 0; i < gNumber_of_prat_flics; i++) {
        BrMemFree(gPratcam_flics[i].data);
    }
    for (i = 0; i < gNumber_of_prat_sequences; i++) {
        for (j = 0; j < gPratcam_sequences[i].number_of_chunks; j++) {
            for (k = 0; k < gPratcam_sequences[i].chunks[j].number_of_alternatives; k++) {
                for (l = 0; l < gPratcam_sequences[i].chunks[j].alternatives[k].number_of_sounds; l++) {
                    DRS3ReleaseSound(gPratcam_sequences[i].chunks[j].alternatives[k].sound_ids[l]);
                }
            }
        }
    }
    BrMemFree(gPratcam_flics);
    BrMemFree(gPratcam_sequences);
}

// IDA: void __usercall DoPratcam(tU32 pThe_time@<EAX>)
void DoPratcam(tU32 pThe_time) {
    int i;
    int offset;
    int y_offset;
    int top_border_height;
    int prat_cam_move_width;
    int right_hand;
    tU32 time_diff;
    tU32 old_last_time;
    br_pixelmap* the_image;
    br_pixelmap* left_image;
    br_pixelmap* right_image;
    LOG_TRACE("(%d)", pThe_time);
    STUB_ONCE();
}

// IDA: void __usercall TestPratCam(int pIndex@<EAX>)
void TestPratCam(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PratCam0()
void PratCam0() {
    LOG_TRACE("()");

    TestPratCam(0);
}

// IDA: void __cdecl PratCam1()
void PratCam1() {
    LOG_TRACE("()");

    TestPratCam(1);
}

// IDA: void __cdecl PratCam2()
void PratCam2() {
    LOG_TRACE("()");

    TestPratCam(2);
}

// IDA: void __cdecl PratCam3()
void PratCam3() {
    LOG_TRACE("()");

    TestPratCam(3);
}

// IDA: void __cdecl PratCam4()
void PratCam4() {
    LOG_TRACE("()");

    TestPratCam(4);
}

// IDA: void __cdecl PratCam5()
void PratCam5() {
    LOG_TRACE("()");

    TestPratCam(5);
}

// IDA: void __cdecl PratCam6()
void PratCam6() {
    LOG_TRACE("()");

    TestPratCam(6);
}

// IDA: void __cdecl PratCam7()
void PratCam7() {
    LOG_TRACE("()");

    TestPratCam(7);
}

// IDA: void __cdecl PratCam8()
void PratCam8() {
    LOG_TRACE("()");

    TestPratCam(8);
}

// IDA: void __cdecl PratCam9()
void PratCam9() {
    LOG_TRACE("()");

    TestPratCam(9);
}
