#ifndef _REPLAY_H_
#define _REPLAY_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern char* gReplay_pixie_names[10];
extern int gSingle_frame_mode;
extern tU32 gCam_change_time;
extern int gSave_file;
extern int gProgress_line_left[2];
extern int gProgress_line_right[2];
extern int gProgress_line_top[2];
extern br_pixelmap* gReplay_pixies[10];
extern int gKey_down;
extern int gNo_cursor;
extern int gSave_frame_number;
extern int gCam_change_button_down;
extern tU32 gAction_replay_start_time;
extern tU32 gLast_replay_zappy_screen;
extern tS32 gStopped_time;
extern float gPending_replay_rate;
extern tU32 gAction_replay_end_time;
extern float gReplay_rate;
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
