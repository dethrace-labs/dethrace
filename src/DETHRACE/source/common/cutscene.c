#include "cutscene.h"

// Global variables
tS32 gLast_demo_end_anim;

// Offset: 0
// Size: 118
void* radmalloc(unsigned long numbytes) {
  unsigned char *temp;
  unsigned char i;
}

// Offset: 120
// Size: 59
void radfree(void *ptr) {
}

// Offset: 180
// Size: 179
// EAX: pIndex
// EDX: pWait_end
// EBX: pSound_ID
void ShowCutScene(int pIndex, int pWait_end, int pSound_ID, br_scalar pDelay, signed char __unk4__) {
}

// Offset: 360
// Size: 34
void DoSCILogo() {
}

// Offset: 396
// Size: 34
void DoStainlessLogo() {
}

// Offset: 432
// Size: 844
// EAX: pSmack_name
void PlaySmackerFile(char *pSmack_name) {
  tPath_name the_path;
  br_colour *br_colours_ptr;
  tU8 *smack_colours_ptr;
  Smack *smk;
  int i;
  int j;
  int len;
  int fuck_off;
}

// Offset: 1276
// Size: 59
void DoOpeningAnimation() {
}

// Offset: 1336
// Size: 79
void DoNewGameAnimation() {
}

// Offset: 1416
// Size: 34
void DoGoToRaceAnimation() {
}

// Offset: 1452
// Size: 215
void DoEndRaceAnimation() {
  int went_up_a_rank;
}

// Offset: 1668
// Size: 54
void DoGameOverAnimation() {
}

// Offset: 1724
// Size: 54
void DoGameCompletedAnimation() {
}

// Offset: 1780
// Size: 70
void StartLoadingScreen() {
}

