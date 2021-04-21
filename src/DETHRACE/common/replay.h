#ifndef _REPLAY_H_
#define _REPLAY_H_

#include "br_types.h"
#include "dr_types.h"

extern int gKey_down;
extern int gNo_cursor;
extern int gSingle_frame_mode;
extern int gSave_frame_number;
extern int gCam_change_button_down;
extern tU32 gAction_replay_start_time;
extern tU32 gLast_replay_zappy_screen;
extern tS32 gStopped_time;
extern tU32 gCam_change_time;
extern float gPending_replay_rate;
extern tU32 gAction_replay_end_time;
extern float gReplay_rate;
extern int gSave_file;
extern int gSave_bunch_ID;
extern int gPlay_direction;
extern int gPaused;
extern tAction_replay_camera_type gAction_replay_camera_mode;

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
