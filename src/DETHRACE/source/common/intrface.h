#ifndef _INTRFACE_H_
#define _INTRFACE_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 44
void SetAlwaysTyping();

// Offset: 44
// Size: 44
void ClearAlwaysTyping();

// Offset: 88
// Size: 103
// EAX: pChoice
int ChoiceDisabled(int pChoice);

// Offset: 192
// Size: 44
void ResetInterfaceTimeout();

// Offset: 236
// Size: 627
// EAX: pSpec
// EDX: pOld_selection
// EBX: pNew_selection
// ECX: pMode
void ChangeSelection(tInterface_spec *pSpec, int *pOld_selection, int *pNew_selection, int pMode, int pSkip_disabled);

// Offset: 864
// Size: 292
// EAX: pSpec
// EDX: pCopy_areas
void RecopyAreas(tInterface_spec *pSpec, br_pixelmap **pCopy_areas);

// Offset: 1156
// Size: 111
// EAX: pChoice
void DisableChoice(int pChoice);

// Offset: 1268
// Size: 140
// EAX: pChoice
void EnableChoice(int pChoice);

// Offset: 1408
// Size: 5354
// EAX: pSpec
// EDX: pOptions
// EBX: pCurrent_choice
int DoInterfaceScreen(tInterface_spec *pSpec, int pOptions, int pCurrent_choice);

// Offset: 6764
// Size: 88
// EAX: pNew_choice
// EDX: pNew_mode
void ChangeSelectionTo(int pNew_choice, int pNew_mode);

#endif
