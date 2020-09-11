#ifndef _REPLAY_H_
#define _REPLAY_H_

#include "br_types.h"
#include "dr_types.h"

int ReplayIsPaused();

float GetReplayRate();

int GetReplayDirection();

void StopSaving();

void ActualActionReplayHeadups(int pSpecial_zappy_bastard);

void DoActionReplayPostSwap();

void DoZappyActionReplayHeadups(int pSpecial_zappy_bastard);

void DoActionReplayHeadups();

void MoveReplayBuffer(tS32 pMove_amount);

void MoveToEndOfReplay();

void MoveToStartOfReplay();

void ToggleReplay();

void ReverseSound(tS3_effect_tag pEffect_index, tS3_sound_tag pSound_tag);

int FindUniqueFile();

void PollActionReplayControls(tU32 pFrame_period);

void CheckReplayTurnOn();

void InitializeActionReplay();

void DoActionReplay(tU32 pFrame_period);

void SynchronizeActionReplay();

#endif
