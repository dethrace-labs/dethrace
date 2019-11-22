#include "pratcam.h"

tFlic_descriptor gPrat_flic;
tS3_sound_tag gWhirr_noise;
tPrat_sequence *gPratcam_sequences;
tPrat_flic_spec *gPratcam_flics;
int gCurrent_pratcam_chunk;
int gPending_ambient_prat;
int gCurrent_pratcam_index;
br_pixelmap *gPrat_buffer;
int gNumber_of_prat_sequences;
int gNumber_of_prat_flics;
tU32 gLast_pratcam_frame_time;
int gCurrent_pratcam_precedence;
int gCurrent_ambient_prat_sequence;
int gCurrent_pratcam_alternative;

// Offset: 0
// Size: 45
int PratcamGetCurrent() {
}

// Offset: 48
// Size: 45
int PratcamGetAmbient() {
}

// Offset: 96
// Size: 45
int PratcamGetPending() {
}

// Offset: 144
// Size: 240
void TogglePratcam() {
    tU32 time_diff;
}

// Offset: 384
// Size: 1836
// EAX: pFolder_name
void LoadPratcam(char *pFolder_name) {
    FILE *f;
    FILE *g;
    tPath_name the_path;
    int i;
    int j;
    int k;
    int l;
    char s[256];
    char folder_path[256];
    char *str;
    char num_str[256];
}

// Offset: 2220
// Size: 495
void NextPratcamChunk() {
    int random_number;
    int count;
    tPrat_alternative *current_alternative;
}

// Offset: 2716
// Size: 83
// EAX: pSequence_index
// EDX: pStart_chunk
void NewPratcamSequence(int pSequence_index, int pStart_chunk) {
}

// Offset: 2800
// Size: 67
// EAX: pIndex
// EDX: pStart_chunk
void ChangeAmbientPratcamNow(int pIndex, int pStart_chunk) {
}

// Offset: 2868
// Size: 118
// EAX: pIndex
void ChangeAmbientPratcam(int pIndex) {
}

// Offset: 2988
// Size: 57
// EAX: pIndex
void PratcamEventNow(int pIndex) {
}

// Offset: 3048
// Size: 113
// EAX: pIndex
void PratcamEvent(int pIndex) {
}

// Offset: 3164
// Size: 94
int HighResPratBufferWidth() {
}

// Offset: 3260
// Size: 94
int HighResPratBufferHeight() {
}

// Offset: 3356
// Size: 419
void InitPratcam() {
}

// Offset: 3776
// Size: 447
void DisposePratcam() {
    int j;
    int k;
    int l;
}

// Offset: 4224
// Size: 1252
// EAX: pThe_time
void DoPratcam(tU32 pThe_time) {
    int i;
    int offset;
    int y_offset;
    int top_border_height;
    int prat_cam_move_width;
    int right_hand;
    tU32 time_diff;
    tU32 old_last_time;
    br_pixelmap *the_image;
    br_pixelmap *left_image;
    br_pixelmap *right_image;
}

// Offset: 5476
// Size: 96
// EAX: pIndex
void TestPratCam(int pIndex) {
}

// Offset: 5572
// Size: 41
void PratCam0() {
}

// Offset: 5616
// Size: 44
void PratCam1() {
}

// Offset: 5660
// Size: 44
void PratCam2() {
}

// Offset: 5704
// Size: 44
void PratCam3() {
}

// Offset: 5748
// Size: 44
void PratCam4() {
}

// Offset: 5792
// Size: 44
void PratCam5() {
}

// Offset: 5836
// Size: 44
void PratCam6() {
}

// Offset: 5880
// Size: 44
void PratCam7() {
}

// Offset: 5924
// Size: 44
void PratCam8() {
}

// Offset: 5968
// Size: 44
void PratCam9() {
}

