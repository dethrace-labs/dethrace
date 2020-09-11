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

int PratcamGetCurrent() {
    NOT_IMPLEMENTED();
}

int PratcamGetAmbient() {
    NOT_IMPLEMENTED();
}

int PratcamGetPending() {
    NOT_IMPLEMENTED();
}

void TogglePratcam() {
    tU32 time_diff;
    NOT_IMPLEMENTED();
}

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

void NextPratcamChunk() {
    int random_number;
    int count;
    tPrat_alternative* current_alternative;
    NOT_IMPLEMENTED();
}

void NewPratcamSequence(int pSequence_index, int pStart_chunk) {
    NOT_IMPLEMENTED();
}

void ChangeAmbientPratcamNow(int pIndex, int pStart_chunk) {
    NOT_IMPLEMENTED();
}

void ChangeAmbientPratcam(int pIndex) {
    NOT_IMPLEMENTED();
}

void PratcamEventNow(int pIndex) {
    NOT_IMPLEMENTED();
}

void PratcamEvent(int pIndex) {
    NOT_IMPLEMENTED();
}

int HighResPratBufferWidth() {
    NOT_IMPLEMENTED();
}

int HighResPratBufferHeight() {
    NOT_IMPLEMENTED();
}

void InitPratcam() {
    NOT_IMPLEMENTED();
}

void DisposePratcam() {
    int j;
    int k;
    int l;
    NOT_IMPLEMENTED();
}

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

void TestPratCam(int pIndex) {
    NOT_IMPLEMENTED();
}

void PratCam0() {
    NOT_IMPLEMENTED();
}

void PratCam1() {
    NOT_IMPLEMENTED();
}

void PratCam2() {
    NOT_IMPLEMENTED();
}

void PratCam3() {
    NOT_IMPLEMENTED();
}

void PratCam4() {
    NOT_IMPLEMENTED();
}

void PratCam5() {
    NOT_IMPLEMENTED();
}

void PratCam6() {
    NOT_IMPLEMENTED();
}

void PratCam7() {
    NOT_IMPLEMENTED();
}

void PratCam8() {
    NOT_IMPLEMENTED();
}

void PratCam9() {
    NOT_IMPLEMENTED();
}
