#include "intrface.h"

// Global variables
int gDisabled_choices[10];
int gCurrent_mode;
tU32 gStart_time;
int gCurrent_choice;
tInterface_spec *gSpec;
int gAlways_typing;
int gDisabled_count;

// Offset: 0
// Size: 44
void SetAlwaysTyping() {
}

// Offset: 44
// Size: 44
void ClearAlwaysTyping() {
}

// Offset: 88
// Size: 103
// EAX: pChoice
int ChoiceDisabled(int pChoice) {
  int i;
}

// Offset: 192
// Size: 44
void ResetInterfaceTimeout() {
}

// Offset: 236
// Size: 627
// EAX: pSpec
// EDX: pOld_selection
// EBX: pNew_selection
// ECX: pMode
void ChangeSelection(tInterface_spec *pSpec, int *pOld_selection, int *pNew_selection) {
  int pMode;
  int pSkip_disabled;
  int i;
}

// Offset: 864
// Size: 292
// EAX: pSpec
// EDX: pCopy_areas
void RecopyAreas(tInterface_spec *pSpec, br_pixelmap **pCopy_areas) {
  int i;
}

// Offset: 1156
// Size: 111
// EAX: pChoice
void DisableChoice(int pChoice) {
  int i;
}

// Offset: 1268
// Size: 140
// EAX: pChoice
void EnableChoice(int pChoice) {
  int i;
}

// Offset: 1408
// Size: 5354
// EAX: pSpec
// EDX: pOptions
// EBX: pCurrent_choice
int DoInterfaceScreen(tInterface_spec *pSpec, int pOptions) {
  int pCurrent_choice;
  tProg_status entry_status;
  int x_coord;
  int y_coord;
  int mouse_in_somewhere;
  int i;
  int key2;
  int mouse_was_started;
  int last_choice;
  int escaped;
  int timed_out;
  int go_ahead;
  int last_mode;
  int result;
  int the_key;
  int the_max;
  int mouse_down;
  int new_mouse_down;
  int last_mouse_down;
  int defeat_mode_change;
  int selection_changed;
  char the_str[256];
  tU32 last_press;
  tU32 last_left_press;
  tU32 last_right_press;
  tU32 last_up_press;
  tU32 last_down_press;
  br_pixelmap **copy_areas;
  br_pixelmap *old_current_splash;
  void *pixels_copy;
  void *palette_copy;
}

// Offset: 6764
// Size: 88
// EAX: pNew_choice
// EDX: pNew_mode
void ChangeSelectionTo(int pNew_choice, int pNew_mode) {
  int last_choice;
}

