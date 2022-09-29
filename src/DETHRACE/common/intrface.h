#ifndef _INTRFACE_H_
#define _INTRFACE_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern int gDisabled_choices[10];
extern int gCurrent_mode;
extern tU32 gStart_time;
extern int gCurrent_choice;
extern tInterface_spec* gSpec;
extern int gAlways_typing;
extern int gDisabled_count;

void SetAlwaysTyping(void);

void ClearAlwaysTyping(void);

int ChoiceDisabled(int pChoice);

void ResetInterfaceTimeout(void);

void ChangeSelection(tInterface_spec* pSpec, int* pOld_selection, int* pNew_selection, int pMode, int pSkip_disabled);

void RecopyAreas(tInterface_spec* pSpec, br_pixelmap** pCopy_areas);

void DisableChoice(int pChoice);

void EnableChoice(int pChoice);

int DoInterfaceScreen(tInterface_spec* pSpec, int pOptions, int pCurrent_choice);

void ChangeSelectionTo(int pNew_choice, int pNew_mode);

#endif
