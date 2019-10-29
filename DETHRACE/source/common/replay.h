#include "dr_types.h"
#include "br_types.h"
// Offset: 0
// Size: 71
int ReplayIsPaused();

// Offset: 72
// Size: 47
float GetReplayRate();

// Offset: 120
// Size: 45
int GetReplayDirection();

// Offset: 168
// Size: 54
void StopSaving();

// Offset: 224
// Size: 1597
// EAX: pSpecial_zappy_bastard
void ActualActionReplayHeadups(int pSpecial_zappy_bastard);

// Offset: 1824
// Size: 44
void DoActionReplayPostSwap();

// Offset: 1868
// Size: 109
// EAX: pSpecial_zappy_bastard
void DoZappyActionReplayHeadups(int pSpecial_zappy_bastard);

// Offset: 1980
// Size: 41
void DoActionReplayHeadups();

// Offset: 2024
// Size: 318
// EAX: pMove_amount
void MoveReplayBuffer(tS32 pMove_amount);

// Offset: 2344
// Size: 118
void MoveToEndOfReplay();

// Offset: 2464
// Size: 118
void MoveToStartOfReplay();

// Offset: 2584
// Size: 395
void ToggleReplay();

// Offset: 2980
// Size: 49
// EAX: pEffect_index
// EDX: pSound_tag
void ReverseSound(tS3_effect_tag pEffect_index, tS3_sound_tag pSound_tag);

// Offset: 3032
// Size: 223
int FindUniqueFile();

// Offset: 3256
// Size: 1576
// EAX: pFrame_period
void PollActionReplayControls(tU32 pFrame_period);

// Offset: 4832
// Size: 94
void CheckReplayTurnOn();

// Offset: 4928
// Size: 98
void InitializeActionReplay();

// Offset: 5028
// Size: 71
// EAX: pFrame_period
void DoActionReplay(tU32 pFrame_period);

// Offset: 5100
// Size: 330
void SynchronizeActionReplay();

