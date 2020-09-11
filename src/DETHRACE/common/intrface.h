#ifndef _INTRFACE_H_
#define _INTRFACE_H_

#include "br_types.h"
#include "dr_types.h"

void SetAlwaysTyping();

void ClearAlwaysTyping();

int ChoiceDisabled(int pChoice);

void ResetInterfaceTimeout();

void ChangeSelection(tInterface_spec* pSpec, int* pOld_selection, int* pNew_selection, int pMode, int pSkip_disabled);

void RecopyAreas(tInterface_spec* pSpec, br_pixelmap** pCopy_areas);

void DisableChoice(int pChoice);

void EnableChoice(int pChoice);

int DoInterfaceScreen(tInterface_spec* pSpec, int pOptions, int pCurrent_choice);

void ChangeSelectionTo(int pNew_choice, int pNew_mode);

#endif
