#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 161
// EAX: pWhich_one
// EDX: pWhich_stage
void DrawDial(int pWhich_one, int pWhich_stage);

// Offset: 164
// Size: 386
// EAX: pWhich_one
// EDX: pOld_stage
// EBX: pNew_stage
void MoveDialFromTo(int pWhich_one, int pOld_stage, int pNew_stage);

// Offset: 552
// Size: 94
void SoundOptionsStart();

// Offset: 648
// Size: 90
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int SoundOptionsDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

// Offset: 740
// Size: 154
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int SoundOptionsLeft(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 896
// Size: 154
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int SoundOptionsRight(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 1052
// Size: 384
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int SoundClick(int *pCurrent_choice, int *pCurrent_mode, int pX_offset, int pY_offset);

// Offset: 1436
// Size: 77
void DoSoundOptions();

// Offset: 1516
// Size: 586
void GetGraphicsOptions();

// Offset: 2104
// Size: 513
void SetGraphicsOptions();

// Offset: 2620
// Size: 80
// EAX: pIndex
// EDX: pValue
void PlayRadioOn2(int pIndex, int pValue);

// Offset: 2700
// Size: 80
// EAX: pIndex
// EDX: pValue
void PlayRadioOff2(int pIndex, int pValue);

// Offset: 2780
// Size: 79
// EAX: pIndex
// EDX: pValue
void PlayRadioOn(int pIndex, int pValue);

// Offset: 2860
// Size: 79
// EAX: pIndex
// EDX: pValue
void PlayRadioOff(int pIndex, int pValue);

// Offset: 2940
// Size: 109
void DrawInitialRadios();

// Offset: 3052
// Size: 84
// EAX: pIndex
// EDX: pNew_value
void RadioChanged(int pIndex, int pNew_value);

// Offset: 3136
// Size: 121
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int GraphOptLeft(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 3260
// Size: 125
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int GraphOptRight(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 3388
// Size: 148
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int GraphOptUp(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 3536
// Size: 148
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int GraphOptDown(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 3684
// Size: 175
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int RadioClick(int *pCurrent_choice, int *pCurrent_mode, int pX_offset, int pY_offset);

// Offset: 3860
// Size: 59
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int GraphOptGoAhead(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 3920
// Size: 195
// EAX: pIndex
// EDX: pColour_value
void PlotAGraphBox(int pIndex, int pColour_value);

// Offset: 4116
// Size: 50
// EAX: pIndex
void DrawAGraphBox(int pIndex);

// Offset: 4168
// Size: 47
// EAX: pIndex
void EraseAGraphBox(int pIndex);

// Offset: 4216
// Size: 80
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void DrawGraphBox(int pCurrent_choice, int pCurrent_mode);

// Offset: 4296
// Size: 128
void DoGraphicsOptions();

// Offset: 4424
// Size: 1526
void CalibrateJoysticks();

// Offset: 5952
// Size: 118
// EAX: pStr
void StripControls(unsigned char *pStr);

// Offset: 6072
// Size: 215
void LoadKeyNames();

// Offset: 6288
// Size: 79
void DisposeKeyNames();

// Offset: 6368
// Size: 54
void SaveOrigKeyMapping();

// Offset: 6424
// Size: 384
// EAX: pIndex
// EDX: pY
// EBX: pName_x
// ECX: pKey_x
void GetKeyCoords(int pIndex, int *pY, int *pName_x, int *pKey_x, int *pEnd_box);

// Offset: 6808
// Size: 182
void SetKeysToDefault();

// Offset: 6992
// Size: 217
void SaveKeyMapping();

// Offset: 7212
// Size: 60
// EAX: pNew_one
void ChangeKeyMapIndex(int pNew_one);

// Offset: 7272
// Size: 1178
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void DrawKeyAssignments(int pCurrent_choice, int pCurrent_mode);

// Offset: 8452
// Size: 252
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int KeyAssignLeft(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 8704
// Size: 252
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int KeyAssignRight(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 8956
// Size: 299
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int KeyAssignUp(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 9256
// Size: 265
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int KeyAssignDown(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 9524
// Size: 1009
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int KeyAssignGoAhead(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 10536
// Size: 168
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int MouseyClickBastard(int *pCurrent_choice, int *pCurrent_mode, int pX_offset, int pY_offset);

// Offset: 10704
// Size: 90
void DrawInitialKMRadios();

// Offset: 10796
// Size: 366
void DoControlOptions();

// Offset: 11164
// Size: 68
void LoadSoundOptionsData();

// Offset: 11232
// Size: 48
void FreeSoundOptionsData();

// Offset: 11280
// Size: 165
void DrawDisabledOptions();

// Offset: 11448
// Size: 146
void DoOptions();

#endif
