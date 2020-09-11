#include "pratcam.h"
#include <stdlib.h>

tFlic_descriptor gPrat_flic;
tS3_sound_tag gWhirr_noise;
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

// Offset: 0
// Size: 45
int PratcamGetCurrent() {
    NOT_IMPLEMENTED();
}

// Offset: 48
// Size: 45
int PratcamGetAmbient() {
    NOT_IMPLEMENTED();
}

// Offset: 96
// Size: 45
int PratcamGetPending() {
    NOT_IMPLEMENTED();
}

// Offset: 144
// Size: 240
void TogglePratcam() {
    tU32 time_diff;
    NOT_IMPLEMENTED();
}

// Offset: 384
// Size: 1836
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
    NOT_IMPLEMENTED();
}

// Offset: 2220
// Size: 495
void NextPratcamChunk() {
    int random_number;
    int count;
    tPrat_alternative* current_alternative;
    NOT_IMPLEMENTED();
}

// Offset: 2716
// Size: 83
void NewPratcamSequence(int pSequence_index, int pStart_chunk) {
    NOT_IMPLEMENTED();
}

// Offset: 2800
// Size: 67
void ChangeAmbientPratcamNow(int pIndex, int pStart_chunk) {
    NOT_IMPLEMENTED();
}

// Offset: 2868
// Size: 118
void ChangeAmbientPratcam(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 2988
// Size: 57
void PratcamEventNow(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 3048
// Size: 113
void PratcamEvent(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 3164
// Size: 94
int HighResPratBufferWidth() {
    NOT_IMPLEMENTED();
}

// Offset: 3260
// Size: 94
int HighResPratBufferHeight() {
    NOT_IMPLEMENTED();
}

// Offset: 3356
// Size: 419
void InitPratcam() {
    NOT_IMPLEMENTED();
}

// Offset: 3776
// Size: 447
void DisposePratcam() {
    int j;
    int k;
    int l;
    NOT_IMPLEMENTED();
}

// Offset: 4224
// Size: 1252
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
    NOT_IMPLEMENTED();
}

// Offset: 5476
// Size: 96
void TestPratCam(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 5572
// Size: 41
void PratCam0() {
    NOT_IMPLEMENTED();
}

// Offset: 5616
// Size: 44
void PratCam1() {
    NOT_IMPLEMENTED();
}

// Offset: 5660
// Size: 44
void PratCam2() {
    NOT_IMPLEMENTED();
}

// Offset: 5704
// Size: 44
void PratCam3() {
    NOT_IMPLEMENTED();
}

// Offset: 5748
// Size: 44
void PratCam4() {
    NOT_IMPLEMENTED();
}

// Offset: 5792
// Size: 44
void PratCam5() {
    NOT_IMPLEMENTED();
}

// Offset: 5836
// Size: 44
void PratCam6() {
    NOT_IMPLEMENTED();
}

// Offset: 5880
// Size: 44
void PratCam7() {
    NOT_IMPLEMENTED();
}

// Offset: 5924
// Size: 44
void PratCam8() {
    NOT_IMPLEMENTED();
}

// Offset: 5968
// Size: 44
void PratCam9() {
    NOT_IMPLEMENTED();
}
