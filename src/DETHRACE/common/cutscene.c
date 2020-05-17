#include "cutscene.h"
#include <stdlib.h>

tS32 gLast_demo_end_anim;

// Offset: 0
// Size: 118
void* radmalloc(unsigned long numbytes) {
    unsigned char* temp;
    unsigned char i;
    NOT_IMPLEMENTED();
}

// Offset: 120
// Size: 59
void radfree(void* ptr) {
    NOT_IMPLEMENTED();
}

// Offset: 180
// Size: 179
// EAX: pIndex
// EDX: pWait_end
// EBX: pSound_ID
void ShowCutScene(int pIndex, int pWait_end, int pSound_ID, br_scalar pDelay) {
    NOT_IMPLEMENTED();
}

// Offset: 360
// Size: 34
void DoSCILogo() {
    NOT_IMPLEMENTED();
}

// Offset: 396
// Size: 34
void DoStainlessLogo() {
    NOT_IMPLEMENTED();
}

// Offset: 432
// Size: 844
// EAX: pSmack_name
void PlaySmackerFile(char* pSmack_name) {
    tPath_name the_path;
    br_colour* br_colours_ptr;
    tU8* smack_colours_ptr;
    Smack* smk;
    int i;
    int j;
    int len;
    int fuck_off;
    NOT_IMPLEMENTED();
}

// Offset: 1276
// Size: 59
void DoOpeningAnimation() {
    NOT_IMPLEMENTED();
}

// Offset: 1336
// Size: 79
void DoNewGameAnimation() {
    NOT_IMPLEMENTED();
}

// Offset: 1416
// Size: 34
void DoGoToRaceAnimation() {
    NOT_IMPLEMENTED();
}

// Offset: 1452
// Size: 215
void DoEndRaceAnimation() {
    int went_up_a_rank;
    NOT_IMPLEMENTED();
}

// Offset: 1668
// Size: 54
void DoGameOverAnimation() {
    NOT_IMPLEMENTED();
}

// Offset: 1724
// Size: 54
void DoGameCompletedAnimation() {
    NOT_IMPLEMENTED();
}

// Offset: 1780
// Size: 70
void StartLoadingScreen() {
    NOT_IMPLEMENTED();
}
