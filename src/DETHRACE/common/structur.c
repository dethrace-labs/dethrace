#include "structur.h"

int gOpponent_mix[10][5];
int gPratcam_on;
tRace_over_reason gRace_over_reason;
int gCockpit_on;
tU32 gLast_checkpoint_time;
int gLast_wrong_checkpoint;
int gMirror_on;

// Offset: 0
// Size: 124
int NumberOfOpponentsLeft() {
    int car_count;
    int result;
    tCar_spec* the_car;
}

// Offset: 124
// Size: 419
// EAX: pReason
void RaceCompleted(tRace_over_reason pReason) {
}

// Offset: 544
// Size: 79
// EAX: pCheckpoint_index
// EDX: pDo_sound
void Checkpoint(int pCheckpoint_index, int pDo_sound) {
}

// Offset: 624
// Size: 305
void IncrementCheckpoint() {
}

// Offset: 932
// Size: 70
void IncrementLap() {
}

// Offset: 1004
// Size: 152
// EAX: pV0
// EDX: pV1
// EBX: pV2
// ECX: pNormal
int RayHitFace(br_vector3* pV0, br_vector3* pV1, br_vector3* pV2, br_vector3* pNormal, br_vector3* pStart, br_vector3* pDir) {
    tFace_ref the_face;
    br_scalar rt;
}

// Offset: 1156
// Size: 241
// EAX: pCheckpoint_index
void WrongCheckpoint(int pCheckpoint_index) {
}

// Offset: 1400
// Size: 870
void CheckCheckpoints() {
    tCar_spec* car;
    br_vector3 orig;
    br_vector3 dir;
    int i;
    int j;
    int cat;
    int car_count;
    int car_index;
    tNet_game_player_info* net_player;
}

// Offset: 2272
// Size: 72
void TotalRepair() {
}

// Offset: 2344
// Size: 64
void DoLogos() {
}

// Offset: 2408
// Size: 54
void DoProgOpeningAnimation() {
}

// Offset: 2464
// Size: 59
void DoProgramDemo() {
}

// Offset: 2524
// Size: 308
// EAX: pNastiness
// EDX: pHad_scum
int ChooseOpponent(int pNastiness, int* pHad_scum) {
    int i;
    int count;
    int temp_array[40];
}

// Offset: 2832
// Size: 294
// EAX: pRace_info
void SelectOpponents(tRace_info* pRace_info) {
    int i;
    int rank_band;
    int nastiness;
    int had_scum;
}

// Offset: 3128
// Size: 269
// EAX: pCurrent_race
// EDX: pNet_race_sequence
int PickNetRace(int pCurrent_race, tNet_sequence_type pNet_race_sequence) {
    int i;
    int new_index;
    int races_count;
    int most_seldom_seen;
    int races_to_pick_from[50];
}

// Offset: 3400
// Size: 288
void SwapNetCarsLoad() {
    int switched_res;
}

// Offset: 3688
// Size: 165
void SwapNetCarsDispose() {
}

// Offset: 3856
// Size: 1161
void DoGame() {
    tRace_result race_result;
    int second_select_race;
    int first_summary_done;
    int i;
}

// Offset: 5020
// Size: 242
void InitialiseProgramState() {
}

// Offset: 5264
// Size: 171
void DoProgram() {
}

// Offset: 5436
// Size: 270
void JumpTheStart() {
}

// Offset: 5708
// Size: 81
void GoingToInterfaceFromRace() {
}

// Offset: 5792
// Size: 81
void GoingBackToRaceFromInterface() {
}
