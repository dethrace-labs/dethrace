#include "pratcam.h"
#include "brender.h"
#include "displays.h"
#include "errors.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrbm.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/trace.h"
#include "input.h"
#include "loading.h"
#include "pd/sys.h"
#include "sound.h"
#include "structur.h"
#include "utility.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x0050f064
tS3_sound_tag gWhirr_noise = 0;

// GLOBAL: CARM95 0x005362b0
tFlic_descriptor gPrat_flic;

// GLOBAL: CARM95 0x00536334
tPrat_sequence* gPratcam_sequences;

// GLOBAL: CARM95 0x00536324
tPrat_flic_spec* gPratcam_flics;

// GLOBAL: CARM95 0x00536320
int gCurrent_pratcam_chunk;

// GLOBAL: CARM95 0x0053632c
int gPending_ambient_prat;

// GLOBAL: CARM95 0x005362a0
int gCurrent_pratcam_index;

// GLOBAL: CARM95 0x00536340
br_pixelmap* gPrat_buffer;

// GLOBAL: CARM95 0x00536330
int gNumber_of_prat_sequences;

// GLOBAL: CARM95 0x005362a4
int gNumber_of_prat_flics;

// GLOBAL: CARM95 0x0053633c
tU32 gLast_pratcam_frame_time;

// GLOBAL: CARM95 0x00536328
int gCurrent_pratcam_precedence;

// GLOBAL: CARM95 0x00536338
int gCurrent_ambient_prat_sequence;

// GLOBAL: CARM95 0x005362a8
int gCurrent_pratcam_alternative;

// IDA: int __cdecl PratcamGetCurrent()
// FUNCTION: CARM95 0x0044d0b0
int PratcamGetCurrent(void) {

    return gCurrent_pratcam_index;
}

// IDA: int __cdecl PratcamGetAmbient()
// FUNCTION: CARM95 0x0044d0c5
int PratcamGetAmbient(void) {

    return gCurrent_ambient_prat_sequence;
}

// IDA: int __cdecl PratcamGetPending()
// FUNCTION: CARM95 0x0044d0da
int PratcamGetPending(void) {

    return gPending_ambient_prat;
}

// IDA: void __cdecl TogglePratcam()
// FUNCTION: CARM95 0x0044d0ef
void TogglePratcam(void) {
    tU32 the_time;
    tU32 time_diff;

    if (gAusterity_mode) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_NOT_ENOUGH_MEMORY));
    } else {
        if (gWhirr_noise == 0 || !DRS3SoundStillPlaying(gWhirr_noise)) {
            DRS3StopSound(gWhirr_noise);
            gWhirr_noise = DRS3StartSoundNoPiping(gCar_outlet, 1000);
            DRS3ChangePitchSpeed(gWhirr_noise, 19660);
        }
        the_time = GetTotalTime();
        gProgram_state.prat_cam_on = !gProgram_state.prat_cam_on;
        time_diff = the_time - gProgram_state.pratcam_move_start;
        gProgram_state.pratcam_move_start = the_time;
        if (time_diff <= 400) {
            gProgram_state.pratcam_move_start = the_time - 400 + time_diff;
        }
    }
}

// IDA: void __usercall LoadPratcam(char *pFolder_name@<EAX>)
// FUNCTION: CARM95 0x0044d745
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

    if (gAusterity_mode) {
        return;
    }
    PathCat(folder_path, gApplication_path, gGraf_specs[gGraf_spec_index].data_dir_name);
    PathCat(folder_path, folder_path, "PRATCAM");
    PathCat(folder_path, folder_path, pFolder_name);
    PathCat(the_path, folder_path, "PRATCAM.TXT");
    f = DRfopen(the_path, "rt");
    if (f == NULL) {
        FatalError(kFatalError_OpenPratCamTextFile);
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &gNumber_of_prat_flics);
    gPratcam_flics = BrMemAllocate(gNumber_of_prat_flics * sizeof(tPrat_flic_spec), kMem_pratcam_flic_array);
    for (i = 0; i < gNumber_of_prat_flics; i++) {
        PossibleService();
        GetALineAndDontArgue(f, s);
        if (strcmp(s, "END") == 0) {
            FatalError(kFatalError_TooFewFlicSpecsInPratCamFile);
        }
        str = strtok(s, "\t ,/");
        PathCat(the_path, folder_path, str);
        g = DRfopen(the_path, "rb");
        if (g == NULL) {
            FatalError(kFatalError_OpenPratCamFlicFile_S, str);
        }
        gPratcam_flics[i].data_length = GetFileLength(g);
        gPratcam_flics[i].data = BrMemAllocate(gPratcam_flics[i].data_length, kMem_pratcam_flic_data);
        if (gPratcam_flics[i].data == NULL) {
            FatalError(kFatalError_AllocateMemoryPratCamFlicFile);
        }
        fread(gPratcam_flics[i].data, 1, gPratcam_flics[i].data_length, g);
        fclose(g);
    }
    GetALineAndDontArgue(f, s);
    if (strcmp(s, "END") != 0) {
        FatalError(kFatalError_TooManyFlicSpecsInPratCamTextFIle);
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
            FatalError(kFatalError_TooFewSequencesInPratCamTextFile);
        }
        str = strtok(s, "\t ,/");
        sscanf(str, "%d", &gPratcam_sequences[i].precedence);
        gPratcam_sequences[i].repeat_chunk = GetAnInt(f);
        gPratcam_sequences[i].number_of_chunks = GetAnInt(f);
        if (gPratcam_sequences[i].number_of_chunks > COUNT_OF(gPratcam_sequences[i].chunks)) {
            FatalError(kFatalError_PratCamSequenceTooManyChunks_S, num_str);
        }
        for (j = 0; j < gPratcam_sequences[i].number_of_chunks; j++) {
            GetALineAndDontArgue(f, s);
            str = strtok(s, "\t ,/");
            sscanf(str, "%d", &gPratcam_sequences[i].chunks[j].number_of_alternatives);
            if (gPratcam_sequences[i].chunks[j].number_of_alternatives > COUNT_OF(gPratcam_sequences[i].chunks[j].alternatives)) {
                FatalError(kFatalError_PratCamSequenceTooManyAlternatives_S, num_str);
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
                        FatalError(kFatalError_PratCamSequenceTooManySounds_S, num_str);
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
        FatalError(kFatalError_TooManySequencesInPratCamTextFile);
    }
    fclose(f);
}

// IDA: void __cdecl NextPratcamChunk()
// FUNCTION: CARM95 0x0044d2d8
void NextPratcamChunk(void) {
    int i;
    int random_number;
    int count;
    tPrat_alternative* current_alternative;

    if (gCurrent_pratcam_index == -1) {
        gCurrent_pratcam_index = gCurrent_ambient_prat_sequence;
    } else {
        EndFlic(&gPrat_flic);
    }
    count = gCurrent_pratcam_chunk;
    gCurrent_pratcam_chunk++;
    if (gPratcam_sequences[gCurrent_pratcam_index].number_of_chunks <= count) {
        if (gPending_ambient_prat == -1) {
            ChangeAmbientPratcamNow(gCurrent_ambient_prat_sequence, gPratcam_sequences[gCurrent_pratcam_index].repeat_chunk);
        } else {
            ChangeAmbientPratcamNow(gPending_ambient_prat, 0);
        }
    } else {
        gLast_pratcam_frame_time = 0;
        random_number = IRandomBetween(0, 99);
        for (i = 0; i < gPratcam_sequences[gCurrent_pratcam_index].chunks[gCurrent_pratcam_chunk].number_of_alternatives; i++) {
            current_alternative = &gPratcam_sequences[gCurrent_pratcam_index].chunks[gCurrent_pratcam_chunk].alternatives[i];
            random_number -= current_alternative->chance;
            if (random_number <= 0) {
                gCurrent_pratcam_alternative = i;
                gPrat_flic.data_start = NULL;
                StartFlic(NULL, -1, &gPrat_flic, gPratcam_flics[current_alternative->ref].data_length,
                    (tS8*)gPratcam_flics[current_alternative->ref].data, gPrat_buffer, 0, 0, 0);
                if (current_alternative->sound_chance == 0) {
                    return;
                }
                if (!PercentageChance(current_alternative->sound_chance)) {
                    return;
                }
                if (gCurrent_pratcam_precedence == 0 && DRS3OutletSoundsPlaying(gDriver_outlet)) {
                    return;
                }
                DRS3StartSound(gDriver_outlet, current_alternative->sound_ids[IRandomBetween(0, current_alternative->number_of_sounds - 1)]);
                return;
            }
        }
        NextPratcamChunk();
    }
}

// IDA: void __usercall NewPratcamSequence(int pSequence_index@<EAX>, int pStart_chunk@<EDX>)
// FUNCTION: CARM95 0x0044d297
void NewPratcamSequence(int pSequence_index, int pStart_chunk) {

    gCurrent_pratcam_precedence = gPratcam_sequences[pSequence_index].precedence;
    gCurrent_pratcam_index = pSequence_index;
    gCurrent_pratcam_chunk = pStart_chunk - 1;
    NextPratcamChunk();
}

// IDA: void __usercall ChangeAmbientPratcamNow(int pIndex@<EAX>, int pStart_chunk@<EDX>)
// FUNCTION: CARM95 0x0044d26a
void ChangeAmbientPratcamNow(int pIndex, int pStart_chunk) {

    gCurrent_ambient_prat_sequence = pIndex;
    gPending_ambient_prat = -1;
    NewPratcamSequence(pIndex, pStart_chunk);
}

// IDA: void __usercall ChangeAmbientPratcam(int pIndex@<EAX>)
// FUNCTION: CARM95 0x0044d1f0
void ChangeAmbientPratcam(int pIndex) {

    if (gRace_finished) {
        return;
    }
    if (gInterface_within_race_mode) {
        return;
    }
    if (pIndex == gCurrent_ambient_prat_sequence) {
        return;
    }
    if (!gProgram_state.prat_cam_on) {
        return;
    }

    if (gCurrent_pratcam_index == -1) {
        ChangeAmbientPratcamNow(pIndex, 0);
    } else {
        gPending_ambient_prat = pIndex;
    }
}

// IDA: void __usercall PratcamEventNow(int pIndex@<EAX>)
// FUNCTION: CARM95 0x0044d58e
void PratcamEventNow(int pIndex) {

    NewPratcamSequence(pIndex, 0);
    gPending_ambient_prat = -1;
}

// IDA: void __usercall PratcamEvent(int pIndex@<EAX>)
// FUNCTION: CARM95 0x0044d517
void PratcamEvent(int pIndex) {

    if (gRace_finished) {
        return;
    }
    if (gInterface_within_race_mode) {
        return;
    }
#if defined(DETHRACE_FIX_BUGS)
    // In low memory mode, `gPratcam_sequences`is not allocated and thus unsafe to access
    if (gAusterity_mode) {
        return;
    }
#endif
    if (gPratcam_sequences[pIndex].precedence <= gCurrent_pratcam_precedence) {
        return;
    }
    if (!gProgram_state.prat_cam_on) {
        return;
    }
    PratcamEventNow(pIndex);
}

// IDA: int __cdecl HighResPratBufferWidth()
int HighResPratBufferWidth(void) {
    int prat_width;

    if (!gDevious_2d || !gTextures_need_powers_of_2) {
        return 104;
    }
    for (prat_width = 1; prat_width < 104; prat_width *= 2) {
        ;
    }
    return prat_width;
}

// IDA: int __cdecl HighResPratBufferHeight()
int HighResPratBufferHeight(void) {
    int prat_height;

    if (!gDevious_2d || !gTextures_need_powers_of_2) {
        return 110;
    }
    for (prat_height = 1; prat_height < 110; prat_height *= 2) {
        ;
    }
    return prat_height;
}

// IDA: void __cdecl InitPratcam()
// FUNCTION: CARM95 0x0044d5b1
void InitPratcam(void) {
    void* the_pixels;

    if (gAusterity_mode) {
        return;
    }
    gWhirr_noise = 0;
    gPrat_flic.data = NULL;
    gCurrent_ambient_prat_sequence = -1;
    switch (gGraf_data_index) {
    case 0:
        the_pixels = BrMemAllocate(52 * 46, kMem_pratcam_pixelmap);
        break;
    case 1:
#ifdef DETHRACE_3DFX_PATCH
        the_pixels = BrMemAllocate(HighResPratBufferWidth() * HighResPratBufferHeight(), kMem_pratcam_pixelmap);
#else
        the_pixels = BrMemAllocate(104 * 110, kMem_pratcam_pixelmap);
#endif
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
#ifdef DETHRACE_3DFX_PATCH
        gPrat_buffer = DRPixelmapAllocate(BR_PMT_INDEX_8, HighResPratBufferWidth(), HighResPratBufferHeight(), the_pixels, 0);
#else
        gPrat_buffer = DRPixelmapAllocate(gScreen->type, 104, 110, the_pixels, 0);
#endif
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
// FUNCTION: CARM95 0x0044e0dd
void DisposePratcam(void) {
    int i;
    int j;
    int k;
    int l;

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
// FUNCTION: CARM95 0x0044e2e6
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
    br_pixelmap* right_image;
    br_pixelmap* left_image;

    if (gAusterity_mode) {
        return;
    }
    right_image = gProgram_state.current_car.prat_cam_right;
    left_image = gProgram_state.current_car.prat_cam_left;
    y_offset = (gNet_mode == eNet_mode_none) ? 0 : gCurrent_graf_data->net_head_box_bot + 1;

    right_hand = gProgram_state.current_car.prat_left <= gBack_screen->width / 2;
    if (right_hand) {
        prat_cam_move_width = gProgram_state.current_car.prat_right + (right_image != NULL ? right_image->width : 0);
    } else {
        prat_cam_move_width = gBack_screen->width - gProgram_state.current_car.prat_left + (left_image != NULL ? left_image->width : 0);
    }
    time_diff = pThe_time - gProgram_state.pratcam_move_start;
    if (time_diff <= 400) {
        offset = prat_cam_move_width * time_diff / (float)400;
    } else {
        if (gWhirr_noise) {
            DRS3StopSound(gWhirr_noise);
            gWhirr_noise = 0;
        }
        if (!gProgram_state.prat_cam_on) {
            gCurrent_pratcam_index = -1;
            gCurrent_pratcam_precedence = 0;
            gPending_ambient_prat = -1;
            return;
        }
        offset = prat_cam_move_width;
    }

    old_last_time = gLast_pratcam_frame_time;
    if (gProgram_state.prat_cam_on) {
        offset = prat_cam_move_width - offset;
    }
    if (right_hand) {
        offset = -offset;
    }

    DontLetFlicFuckWithPalettes();
    DisableTranslationText();
    for (i = 0; i < (old_last_time != 0 ? ((pThe_time - old_last_time) / gPrat_flic.frame_period) : 1); i++) {
#ifdef DETHRACE_FIX_BUGS
        if (gPrat_flic.data == NULL) {
            switch (gRace_over_reason) {
            case eRace_over_laps:
            case eRace_over_peds:
            case eRace_over_opponents:
                ChangeAmbientPratcamNow(kPratcam_race_complete, 0);
                break;
            case eRace_over_demo:
            case eRace_over_out_of_time:
                ChangeAmbientPratcamNow(kPratcam_out_of_time, 0);
                break;
            case eRace_over_abandoned:
            case eRace_over_network_victory:
            case eRace_over_network_loss:
                ChangeAmbientPratcamNow(kPratcam_network_timeout, 0);
                break;
            default:
            case eRace_not_over_yet:
                ChangeAmbientPratcamNow(kPratcam_stationary_or_below_25mph, 0);
                break;
            }
        }
#endif
        if (PlayNextFlicFrame(&gPrat_flic)) {
            NextPratcamChunk();
            break;
        }
        gLast_pratcam_frame_time = pThe_time;
    }
    EnableTranslationText();
    LetFlicFuckWithPalettes();

#ifdef DETHRACE_3DFX_PATCH
    PDUnlockRealBackScreen(1);
    if (gDevious_2d) {
        gPrat_model->vertices[1].p.v[0] = gProgram_state.current_car.prat_left + offset;
        gPrat_model->vertices[0].p.v[0] = gProgram_state.current_car.prat_left + offset;
        gPrat_model->vertices[3].p.v[1] = -(y_offset + gProgram_state.current_car.prat_top);
        gPrat_model->vertices[0].p.v[1] = -(y_offset + gProgram_state.current_car.prat_top);

        gPrat_model->vertices[3].p.v[0] = gPrat_model->vertices[1].p.v[0] + 104.0f;
        gPrat_model->vertices[2].p.v[0] = gPrat_model->vertices[1].p.v[0] + 104.0f;

        gPrat_model->vertices[2].p.v[1] = gPrat_model->vertices[3].p.v[1] - 110.0f;
        gPrat_model->vertices[1].p.v[1] = gPrat_model->vertices[3].p.v[1] - 110.0f;
        BrModelUpdate(gPrat_model, BR_MODU_VERTEX_POSITIONS);
        gPrat_actor->render_style = BR_RSTYLE_FACES;
        gPrat_material->colour_map = gPrat_buffer;
        gPrat_buffer->map = gRender_palette;
        BrMapAdd(gPrat_buffer);
        BrMaterialUpdate(gPrat_material, BR_MATU_ALL);
        BrZbSceneRender(g2d_camera, g2d_camera, gBack_screen, gDepth_buffer);
        BrMapRemove(gPrat_buffer);
        gPrat_actor->render_style = BR_RSTYLE_NONE;
    } else {
        DRPixelmapRectangleCopy(
            gBack_screen,
            gProgram_state.current_car.prat_left + offset,
            gProgram_state.current_car.prat_top + y_offset,
            gPrat_buffer,
            0, 0,
            gPrat_buffer->width, gPrat_buffer->height);
    }
    PDLockRealBackScreen(1);
#else
    BrPixelmapRectangleCopy(
        gBack_screen,
        gProgram_state.current_car.prat_left + offset,
        gProgram_state.current_car.prat_top + y_offset,
        gPrat_buffer,
        0, 0,
        gPrat_buffer->width, gPrat_buffer->height);
#endif

    if (gProgram_state.current_car.prat_cam_top != NULL) {
        top_border_height = gProgram_state.current_car.prat_cam_top->height;
        DRPixelmapRectangleMaskedCopy(
            gBack_screen,
            gProgram_state.current_car.prat_left + offset,
            gProgram_state.current_car.prat_top - top_border_height + y_offset,
            gProgram_state.current_car.prat_cam_top,
            0, 0,
            gProgram_state.current_car.prat_cam_top->width,
            gProgram_state.current_car.prat_cam_top->height);
    } else {
        top_border_height = 0;
    }
    if (left_image != NULL) {
        DRPixelmapRectangleMaskedCopy(gBack_screen,
            gProgram_state.current_car.prat_left - left_image->width + offset,
            gProgram_state.current_car.prat_top - top_border_height + y_offset,
            left_image,
            0, 0,
            left_image->width, left_image->height);
    }
    if (right_image != NULL) {
        DRPixelmapRectangleMaskedCopy(
            gBack_screen,
            gProgram_state.current_car.prat_right + offset - 1,
            gProgram_state.current_car.prat_top - top_border_height - 1 + y_offset,
            right_image,
            0, 0,
            right_image->width, right_image->height);
    }
    if (gProgram_state.current_car.prat_cam_bottom != NULL) {
        DRPixelmapRectangleMaskedCopy(
            gBack_screen,
            gProgram_state.current_car.prat_left + offset,
            gProgram_state.current_car.prat_bottom + y_offset,
            gProgram_state.current_car.prat_cam_bottom,
            0, 0,
            gProgram_state.current_car.prat_cam_bottom->width,
            gProgram_state.current_car.prat_cam_bottom->height);
    }
}

// IDA: void __usercall TestPratCam(int pIndex@<EAX>)
// FUNCTION: CARM95 0x0044e6d5
void TestPratCam(int pIndex) {

    if (PDKeyDown(KEY_CTRL_ANY)) {
        pIndex += 40;
    }
    if (PDKeyDown(KEY_ALT_ANY)) {
        pIndex += 20;
    }
    if (PDKeyDown(KEY_SHIFT_ANY)) {
        pIndex += 10;
    }
    PratcamEventNow(pIndex);
}

// IDA: void __cdecl PratCam0()
// FUNCTION: CARM95 0x0044e6c0
void PratCam0(void) {

    TestPratCam(kPratcam_stationary_or_below_25mph);
}

// IDA: void __cdecl PratCam1()
// FUNCTION: CARM95 0x0044e72e
void PratCam1(void) {

    TestPratCam(kPratcam_between_25_and_67mph);
}

// IDA: void __cdecl PratCam2()
// FUNCTION: CARM95 0x0044e743
void PratCam2(void) {

    TestPratCam(kPratcam_between_67_and_167mph);
}

// IDA: void __cdecl PratCam3()
// FUNCTION: CARM95 0x0044e758
void PratCam3(void) {

    TestPratCam(kPratcam_over_137mph);
}

// IDA: void __cdecl PratCam4()
// FUNCTION: CARM95 0x0044e76d
void PratCam4(void) {

    TestPratCam(kPratcam_accelerating_unused_4);
}

// IDA: void __cdecl PratCam5()
// FUNCTION: CARM95 0x0044e782
void PratCam5(void) {

    TestPratCam(kPratcam_flying);
}

// IDA: void __cdecl PratCam6()
// FUNCTION: CARM95 0x0044e797
void PratCam6(void) {

    TestPratCam(kPratcam_falling);
}

// IDA: void __cdecl PratCam7()
// FUNCTION: CARM95 0x0044e7ac
void PratCam7(void) {

    TestPratCam(kPratcam_rolling_gently);
}

// IDA: void __cdecl PratCam8()
// FUNCTION: CARM95 0x0044e7c1
void PratCam8(void) {

    TestPratCam(kPratcam_rolling_medium);
}

// IDA: void __cdecl PratCam9()
// FUNCTION: CARM95 0x0044e7d6
void PratCam9(void) {

    TestPratCam(kPratcam_rolling_violent);
}
