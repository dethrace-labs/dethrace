#include "controls.h"

#include "brender.h"
#include "brucetrk.h"
#include "car.h"
#include "constants.h"
#include "crush.h"
#include "depth.h"
#include "displays.h"
#include "finteray.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/trace.h"
#include "init.h"
#include "input.h"
#include "loadsave.h"
#include "mainloop.h"
#include "netgame.h"
#include "network.h"
#include "opponent.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "piping.h"
#include "powerup.h"
#include "pratcam.h"
#include "raycast.h"
#include "replay.h"
#include "s3/s3.h"
#include "sound.h"
#include "spark.h"
#include "structur.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x0051b1c0
tToggle_element gToggle_array[] = {
    { KEYMAP_MIRROR, -2, 1, 1, 0, ToggleMirror },
    { KEYMAP_MAP, -2, 1, 1, 0, ToggleMap },
    { KEYMAP_PRATCAM, -2, 1, 1, 0, TogglePratcam },
    { KEYMAP_RECOVERY, -2, 1, 1, 0, SetRecovery },
    { KEYMAP_CTRL_ABORT_RACE, 7, 1, 1, 0, AbortRace },
    { KEYMAP_COCKPIT, -2, 1, 1, 0, ToggleCockpit },
    { KEYMAP_LOOK_LEFT, -2, 1, 1, 0, LookLeft },
    { KEYMAP_LOOK_FORWARD, -2, 1, 1, 0, LookForward },
    { KEYMAP_LOOK_RIGHT, -2, 1, 1, 0, LookRight },
    { KEYMAP_SOUND, -2, 0, 1, 0, ToggleSoundEnable },
    { KEYMAP_SHIFT_DUMPSCREEN, KEYMAP_SHIFT_ANY, 0, 1, 0, PrintScreen },
    { KEYMAP_CTRL_DRAW_SOME_TEXT, KEYMAP_CONTROL_ANY, 1, 1, 0, DrawSomeText }, // commented out in original executable
    { KEYMAP_CTRL_FLY, KEYMAP_CONTROL_ANY, 1, 1, 0, ToggleFlying },
    { KEYMAP_PEDESTRIANS, -2, 1, 1, 0, TogglePedestrians },
    { KEYMAP_F4, -2, 0, 0, 0, F4Key },
    { KEYMAP_F5, -2, 1, 0, 0, F5Key },
    { KEYMAP_F6, -2, 1, 0, 0, F6Key },
    { KEYMAP_F7, -2, 1, 0, 0, F7Key },
    { KEYMAP_F8, -2, 1, 0, 0, F8Key },
    { KEYMAP_F10, -2, 1, 0, 0, F10Key },
    { KEYMAP_F11, -2, 1, 0, 0, F11Key },
    { KEYMAP_F12, -2, 1, 0, 0, F12Key },
    { KEYMAP_EDITMODE_0, -2, 1, 0, 0, NumberKey0 },
    { KEYMAP_EDITMODE_1, -2, 1, 0, 0, NumberKey1 },
    { KEYMAP_EDITMODE_2, -2, 1, 0, 0, NumberKey2 },
    { KEYMAP_EDITMODE_3, -2, 1, 0, 0, NumberKey3 },
    { KEYMAP_EDITMODE_4, -2, 1, 0, 0, NumberKey4 },
    { KEYMAP_EDITMODE_5, -2, 1, 0, 0, NumberKey5 },
    { KEYMAP_EDITMODE_6, -2, 1, 0, 0, NumberKey6 },
    { KEYMAP_EDITMODE_7, -2, 1, 0, 0, NumberKey7 },
    { KEYMAP_EDITMODE_8, -2, 1, 0, 0, NumberKey8 },
    { KEYMAP_EDITMODE_9, -2, 1, 0, 0, NumberKey9 },
    { KEYMAP_SCREEN_SMALLER, -2, 1, 0, 0, ScreenSmaller },
    { KEYMAP_SCREEN_LARGER, -2, 1, 0, 0, ScreenLarger },
    { KEYMAP_BUY_ARMOUR, -2, 1, 0, 0, BuyArmour },
    { KEYMAP_BUY_POWER, -2, 1, 0, 0, BuyPower },
    { KEYMAP_BUY_OFFENSE, -2, 1, 0, 0, BuyOffense },
    { KEYMAP_VIEW_NETPLAYER, -2, 1, 0, 0, ViewNetPlayer },
    { KEYMAP_SEND_MESSAGE, -2, 1, 0, 0, UserSendMessage },
    { KEYMAP_ARROW, -2, 1, 1, 0, ToggleArrow }, // commented out in original executable
    { KEYMAP_INFO, -2, 1, 1, 0, ToggleInfo },
    { KEYMAP_INFO, KEYMAP_SHIFT_ANY, 1, 1, 0, ToggleInfo },
    { KEYMAP_INFO, KEYMAP_CONTROL_ANY, 1, 1, 0, ToggleInfo }
};

// GLOBAL: CARM95 0x0051b5c8
int gRepair_last_time;

// GLOBAL: CARM95 0x0051b5cc
int gHad_auto_recover;

// GLOBAL: CARM95 0x0051b5d0
tU32 gLast_repair_time;

// GLOBAL: CARM95 0x0051b5d4
tEdit_mode gWhich_edit_mode = eEdit_mode_options;
// GLOBAL: CARM95 0x0051b5d8
char* gEdit_mode_names[10] = {
    "Cheat",
    "Accessories",
    "Special Volumes",
    "Pedestrians",
    "Opponents",
    "Prat-cam",
    "Depth effects",
    "Damage",
    "Bonnet",
    "Options",
};

// order is: { None, CTRL, ALT, CTRL+ALT, SHIFT, CTRL+SHIFT, ALT+SHIFT, CTRL+ALT+SHIFT }

// GLOBAL: CARM95 0x0051b600
tEdit_func* gEdit_funcs[10][18][8] = {
    { // CHEAT EDIT MODE

        // F5
        { TotalRepair, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F6
        { ToggleInvulnerability, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F7
        { MoreTime, NULL, NULL, NULL, MuchMoreTime, NULL, NULL, NULL },
        // F8
        { ToggleTimerFreeze, ShadowMode, NULL, NULL, ToggleShadow, NULL, NULL, NULL },
        // F10
        { IncrementLap, NULL, NULL, NULL, IncrementCheckpoint, NULL, NULL, NULL },
        // F11
        { EarnDosh, NULL, NULL, NULL, LoseDosh, NULL, NULL, NULL },
        // F12
        { ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent },
        // 0
        { GotPowerup0, GotPowerup0, GotPowerup0, GotPowerup0, GotPowerup0, GotPowerup0, GotPowerup0, GotPowerup0 },
        // 1
        { GotPowerup1, GotPowerup1, GotPowerup1, GotPowerup1, GotPowerup1, GotPowerup1, GotPowerup1, GotPowerup1 },
        // 2
        { GotPowerup2, GotPowerup2, GotPowerup2, GotPowerup2, GotPowerup2, GotPowerup2, GotPowerup2, GotPowerup2 },
        // 3
        { GotPowerup3, GotPowerup3, GotPowerup3, GotPowerup3, GotPowerup3, GotPowerup3, GotPowerup3, GotPowerup3 },
        // 4
        { GotPowerup4, GotPowerup4, GotPowerup4, GotPowerup4, GotPowerup4, GotPowerup4, GotPowerup4, GotPowerup4 },
        // 5
        { GotPowerup5, GotPowerup5, GotPowerup5, GotPowerup5, GotPowerup5, GotPowerup5, GotPowerup5, GotPowerup5 },
        // 6
        { GotPowerup6, GotPowerup6, GotPowerup6, GotPowerup6, GotPowerup6, GotPowerup6, GotPowerup6, GotPowerup6 },
        // 7
        { GotPowerup7, GotPowerup7, GotPowerup7, GotPowerup7, GotPowerup7, GotPowerup7, GotPowerup7, GotPowerup7 },
        // 8
        { GotPowerup8, GotPowerup8, GotPowerup8, GotPowerup8, GotPowerup8, GotPowerup8, GotPowerup8, GotPowerup8 },
        // 9
        { GotPowerup9, GotPowerup9, GotPowerup9, GotPowerup9, GotPowerup9, GotPowerup9, GotPowerup9, GotPowerup9 },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },

    { // ACCESSORIES EDIT MODE

        // F5
        { CycleAccRotate, NULL, NULL, NULL, CycleAccScale, NULL, NULL, NULL },
        // F6
        { IdentifyAcc, ScaleAccUp2, ScaleAccUp3, ScaleAccUp4, IdentifyAcc, ScaleAccDown2, ScaleAccDown3, ScaleAccDown4 },
        // F7
        { RotateAccL, RotateAccL2, RotateAccL3, RotateAccL4, RotateAccR, RotateAccR2, RotateAccR3, RotateAccR4 },
        // F8
        { DeleteAcc, NULL, NULL, NULL, SnapAccToVertical, NULL, NULL, NULL },
        // F10
        { MoveXAccL, MoveXAccL2, MoveXAccL3, MoveXAccL4, MoveXAccR, MoveXAccR2, MoveXAccR3, MoveXAccR4 },
        // F11
        { MoveYAccL, MoveYAccL2, MoveYAccL3, MoveYAccL4, MoveYAccR, MoveYAccR2, MoveYAccR3, MoveYAccR4 },
        // F12
        { MoveZAccL, MoveZAccL2, MoveZAccL3, MoveZAccL4, MoveZAccR, MoveZAccR2, MoveZAccR3, MoveZAccR4 },
        // 0
        { DropActor0, DropActor0, DropActor0, DropActor0, DropActor0, DropActor0, DropActor0, DropActor0 },
        // 1
        { DropActor1, DropActor1, DropActor1, DropActor1, DropActor1, DropActor1, DropActor1, DropActor1 },
        // 2
        { DropActor2, DropActor2, DropActor2, DropActor2, DropActor2, DropActor2, DropActor2, DropActor2 },
        // 3
        { DropActor3, DropActor3, DropActor3, DropActor3, DropActor3, DropActor3, DropActor3, DropActor3 },
        // 4
        { DropActor4, DropActor4, DropActor4, DropActor4, DropActor4, DropActor4, DropActor4, DropActor4 },
        // 5
        { DropActor5, DropActor5, DropActor5, DropActor5, DropActor5, DropActor5, DropActor5, DropActor5 },
        // 6
        { DropActor6, DropActor6, DropActor6, DropActor6, DropActor6, DropActor6, DropActor6, DropActor6 },
        // 7
        { DropActor7, DropActor7, DropActor7, DropActor7, DropActor7, DropActor7, DropActor7, DropActor7 },
        // 8
        { DropActor8, DropActor8, DropActor8, DropActor8, DropActor8, DropActor8, DropActor8, DropActor8 },
        // 9
        { DropActor9, DropActor9, DropActor9, DropActor9, DropActor9, DropActor9, DropActor9, DropActor9 },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },

    { // SPECVOL EDIT MODE

        // F5
        { CycleSpecVolRotate, NULL, NULL, NULL, CycleSpecVolScale, NULL, NULL, NULL },
        // F6
        { IdentifySpecVol, ScaleSpecVolUp2, ScaleSpecVolUp3, ScaleSpecVolUp4, IdentifySpecVol, ScaleSpecVolDown2, ScaleSpecVolDown3, ScaleSpecVolDown4 },
        // F7
        { RotateSpecVolL, RotateSpecVolL2, RotateSpecVolL3, RotateSpecVolL4, RotateSpecVolR, RotateSpecVolR2, RotateSpecVolR3, RotateSpecVolR4 },
        // F8
        { DeleteSpecVol, NULL, NULL, NULL, SnapSpecVolToVertical, NULL, NULL, NULL },
        // F10
        { MoveXSpecVolL, MoveXSpecVolL2, MoveXSpecVolL3, MoveXSpecVolL4, MoveXSpecVolR, MoveXSpecVolR2, MoveXSpecVolR3, MoveXSpecVolR4 },
        // F11
        { MoveYSpecVolL, MoveYSpecVolL2, MoveYSpecVolL3, MoveYSpecVolL4, MoveYSpecVolR, MoveYSpecVolR2, MoveYSpecVolR3, MoveYSpecVolR4 },
        // F12
        { MoveZSpecVolL, MoveZSpecVolL2, MoveZSpecVolL3, MoveZSpecVolL4, MoveZSpecVolR, MoveZSpecVolR2, MoveZSpecVolR3, MoveZSpecVolR4 },
        // 0
        { DropSpecVol0, DropSpecVol0, DropSpecVol0, DropSpecVol0, DropSpecVol0, DropSpecVol0, DropSpecVol0, DropSpecVol0 },
        // 1
        { DropSpecVol1, DropSpecVol1, DropSpecVol1, DropSpecVol1, DropSpecVol1, DropSpecVol1, DropSpecVol1, DropSpecVol1 },
        // 2
        { DropSpecVol2, DropSpecVol2, DropSpecVol2, DropSpecVol2, DropSpecVol2, DropSpecVol2, DropSpecVol2, DropSpecVol2 },
        // 3
        { DropSpecVol3, DropSpecVol3, DropSpecVol3, DropSpecVol3, DropSpecVol3, DropSpecVol3, DropSpecVol3, DropSpecVol3 },
        // 4
        { DropSpecVol4, DropSpecVol4, DropSpecVol4, DropSpecVol4, DropSpecVol4, DropSpecVol4, DropSpecVol4, DropSpecVol4 },
        // 5
        { DropSpecVol5, DropSpecVol5, DropSpecVol5, DropSpecVol5, DropSpecVol5, DropSpecVol5, DropSpecVol5, DropSpecVol5 },
        // 6
        { DropSpecVol6, DropSpecVol6, DropSpecVol6, DropSpecVol6, DropSpecVol6, DropSpecVol6, DropSpecVol6, DropSpecVol6 },
        // 7
        { DropSpecVol7, DropSpecVol7, DropSpecVol7, DropSpecVol7, DropSpecVol7, DropSpecVol7, DropSpecVol7, DropSpecVol7 },
        // 8
        { DropSpecVol8, DropSpecVol8, DropSpecVol8, DropSpecVol8, DropSpecVol8, DropSpecVol8, DropSpecVol8, DropSpecVol8 },
        // 09
        { DropSpecVol9, DropSpecVol9, DropSpecVol9, DropSpecVol9, DropSpecVol9, DropSpecVol9, DropSpecVol9, DropSpecVol9 },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },

    { // PEDESTRIAN EDIT MODE

        // F5
        { TogglePedDetect, NULL, NULL, NULL, DoPedReport, NULL, NULL, NULL },
        // F6
        { ShowPedPaths, NULL, NULL, NULL, ShowPedPos, NULL, NULL, NULL },
        // F7
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F8
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F10
        { DropPedPoint, DropPedPointAir, NULL, NULL, PullPedPoint, PullPedPointAir, NULL, NULL },
        // F11
        { DropInitPedPoint, DropInitPedPointAir, NULL, NULL, NULL, NULL, NULL, NULL },
        // F12
        { ScrubPedestrian, DeletePedPoint, NULL, NULL, DeletePedPath, NULL, NULL, NULL },
        // 0
        { NewPed0, NewPed0, NewPed0, NewPed0, NewPed0, NewPed0, NewPed0, NewPed0 },
        // 1
        { NewPed1, NewPed1, NewPed1, NewPed1, NewPed1, NewPed1, NewPed1, NewPed1 },
        // 2
        { NewPed2, NewPed2, NewPed2, NewPed2, NewPed2, NewPed2, NewPed2, NewPed2 },
        // 3
        { NewPed3, NewPed3, NewPed3, NewPed3, NewPed3, NewPed3, NewPed3, NewPed3 },
        // 4
        { NewPed4, NewPed4, NewPed4, NewPed4, NewPed4, NewPed4, NewPed4, NewPed4 },
        // 5
        { NewPed5, NewPed5, NewPed5, NewPed5, NewPed5, NewPed5, NewPed5, NewPed5 },
        // 6
        { NewPed6, NewPed6, NewPed6, NewPed6, NewPed6, NewPed6, NewPed6, NewPed6 },
        // 7
        { NewPed7, NewPed7, NewPed7, NewPed7, NewPed7, NewPed7, NewPed7, NewPed7 },
        // 8
        { NewPed8, NewPed8, NewPed8, NewPed8, NewPed8, NewPed8, NewPed8, NewPed8 },
        // 9
        { NewPed9, NewPed9, NewPed9, NewPed9, NewPed9, NewPed9, NewPed9, NewPed9 },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },

    { // OPPONENTS EDIT MODE

        // F5
        { ShowHideOppoPaths, ShowSectionInfo1, NULL, NULL, ShowNodeInfo, ShowSectionInfo2, NULL, NULL },
        // F6
        { ReverseSectionDirection, ToggleOneWayNess, NULL, NULL, CycleSectionType, NULL, NULL, NULL },
        // F7
        { DropElasticateyNode, NULL, PullOppoPoint, NULL, InsertAndElasticate, NULL, NULL, NULL },
        // F8
        { DropNodeOnNodeAndStopElasticating, DropDeadEndNode, NULL, NULL, InsertAndDontElasticate, NULL, NULL, NULL },
        // F10
        { WidenOppoPathSection, IncreaseSectionMinSpeed, IncreaseSectionMaxSpeed, NULL, NarrowOppoPathSection, DecreaseSectionMinSpeed, DecreaseSectionMaxSpeed, NULL },
        // F11
        { NULL, DeleteOppoPathNodeAndJoin, NULL, NULL, DeleteOppoPathSection, DeleteOppoPathNodeAndSections, NULL, NULL },
        // F12
        { ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent },
        // 0
        { ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent, ViewOpponent },
        // 1
        { ToggleOpponentProcessing, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 2
        { CopStartPointInfo, DeleteCopStartPoint, DropCopStartPoint, NULL, CycleCopStartPointType, NULL, NULL, NULL },
        // 3
        { ToggleMellowOpponents, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 4
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 5
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 6
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 7
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 8
        { AdjustDownForce, NULL, NULL, NULL, PutOpponentsInNeutral, NULL, NULL, NULL },
        // 9
        { SwapCar, FreezeMechanics, NULL, NULL, ToggleCarToCarCollisions, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },

    { // PRAT-CAM EDIT MODE

        // F5
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F6
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F7
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F8
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F10
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F11
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F12
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 0
        { PratCam0, PratCam0, PratCam0, PratCam0, PratCam0, PratCam0, PratCam0, PratCam0 },
        // 1
        { PratCam1, PratCam1, PratCam1, PratCam1, PratCam1, PratCam1, PratCam1, PratCam1 },
        // 2
        { PratCam2, PratCam2, PratCam2, PratCam2, PratCam2, PratCam2, PratCam2, PratCam2 },
        // 3
        { PratCam3, PratCam3, PratCam3, PratCam3, PratCam3, PratCam3, PratCam3, PratCam3 },
        // 4
        { PratCam4, PratCam4, PratCam4, PratCam4, PratCam4, PratCam4, PratCam4, PratCam4 },
        // 5
        { PratCam5, PratCam5, PratCam5, PratCam5, PratCam5, PratCam5, PratCam5, PratCam5 },
        // 6
        { PratCam6, PratCam6, PratCam6, PratCam6, PratCam6, PratCam6, PratCam6, PratCam6 },
        // 7
        { PratCam7, PratCam7, PratCam7, PratCam7, PratCam7, PratCam7, PratCam7, PratCam7 },
        // 8
        { PratCam8, PratCam8, PratCam8, PratCam8, PratCam8, PratCam8, PratCam8, PratCam8 },
        // 9
        { PratCam9, PratCam9, PratCam9, PratCam9, PratCam9, PratCam9, PratCam9, PratCam9 },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },

    { // DEPTH EFFECTS EDIT MODE

        // F5
        { ToggleDepthMode, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F6
        { ToggleSky, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F7
        { LessDepthFactor, NULL, NULL, NULL, LessDepthFactor2, NULL, NULL, NULL },
        // F8
        { MoreDepthFactor, NULL, NULL, NULL, MoreDepthFactor2, NULL, NULL, NULL },
        // F10
        { IncreaseYon, NULL, NULL, NULL, DecreaseYon, NULL, NULL, NULL },
        // F11
        { IncreaseAngle, NULL, NULL, NULL, DecreaseAngle, NULL, NULL, NULL },
        // F12
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 0
        { ToggleShadow, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 1
        { ShadowMode, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 2
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 3
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 4
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 5
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 6
        { SelectFaceDown, NULL, NULL, NULL, SelectFaceForward, NULL, NULL, NULL },
        // 7
        { ScaleUpX, NULL, NULL, NULL, ScaleDnX, NULL, NULL, NULL },
        // 8
        { ScaleUpY, NULL, NULL, NULL, ScaleDnY, NULL, NULL, NULL },
        // 9
        { DustRotate, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },

    { // DAMAGE EDIT MODE

        // F5
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F6
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F7
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F8
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F10
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F11
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F12
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 0
        { TDamageEngine, NULL, NULL, NULL, TDamageEngine, NULL, NULL, NULL },
        // 1
        { TDamageTrans, NULL, NULL, NULL, TDamageTrans, NULL, NULL, NULL },
        // 2
        { TDamageSteering, NULL, NULL, NULL, TDamageSteering, NULL, NULL, NULL },
        // 3
        { TDamageLFWheel, NULL, NULL, NULL, TDamageLFBrake, NULL, NULL, NULL },
        // 4
        { TDamageLRWheel, NULL, NULL, NULL, TDamageLRBrake, NULL, NULL, NULL },
        // 5
        { TDamageRFWheel, NULL, NULL, NULL, TDamageRFBrake, NULL, NULL, NULL },
        // 6
        { TDamageRRWheel, NULL, NULL, NULL, TDamageRRBrake, NULL, NULL, NULL },
        // 7
        { TDamageDriver, NULL, NULL, NULL, TDamageDriver, NULL, NULL, NULL },
        // 8
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 9
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },

    { // BONNET EDIT MODE

        // F5
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F6
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F7
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F8
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F10
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F11
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F12
        { SaveBonnet, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 0
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 1
        { MoveBonnetForward, NULL, NULL, NULL, MoveBonnetBackward, NULL, NULL, NULL },
        // 2
        { MoveBonnetLeft, NULL, NULL, NULL, MoveBonnetRight, NULL, NULL, NULL },
        // 3
        { MoveBonnetUp, NULL, NULL, NULL, MoveBonnetDown, NULL, NULL, NULL },
        // 4
        { TiltBonnetUpX, NULL, NULL, NULL, TiltBonnetDownX, NULL, NULL, NULL },
        // 5
        { TiltBonnetUpY, NULL, NULL, NULL, TiltBonnetDownY, NULL, NULL, NULL },
        // 6
        { TiltBonnetUpZ, NULL, NULL, NULL, TiltBonnetDownZ, NULL, NULL, NULL },
        // 7
        { ShrinkBonnetX, NULL, NULL, NULL, SwellBonnetX, NULL, NULL, NULL },
        // 8
        { ShrinkBonnetY, NULL, NULL, NULL, SwellBonnetY, NULL, NULL, NULL },
        // 9
        { ShrinkBonnetZ, NULL, NULL, NULL, SwellBonnetZ, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } },

    { // OPTIONS EDIT MODE

        // F5
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F6
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F7
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F8
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F10
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F11
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // F12
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 0
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 1
        { CycleCarSimplificationLevel, NULL, NULL, NULL, CycleCarTexturingLevel, NULL, NULL, NULL },
        // 2
        { ToggleShadow, NULL, NULL, NULL, ToggleSmoke, NULL, NULL, NULL },
        // 3
        { CycleWallTexturingLevel, NULL, NULL, NULL, CycleRoadTexturingLevel, NULL, NULL, NULL },
        // 4
        { ToggleSky, NULL, NULL, NULL, ToggleDepthCueing, NULL, NULL, NULL },
        // 5
        { CycleYonFactor, NULL, NULL, NULL, ToggleAccessoryRendering, NULL, NULL, NULL },
        // 6
        { DecreaseYon, NULL, NULL, NULL, IncreaseYon, NULL, NULL, NULL },
        // 7
        { CycleSoundDetailLevel, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 8
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        // 9
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }

    }
};

// GLOBAL: CARM95 0x0051cc80
tCheat gKev_keys[43] = {
    { 0xa11ee75d, 0xf805eddd, SetFlag, 0x0a11ee75d },
    { 0x564e78b9, 0x99155115, SetFlag, 0x0564e78b9 },
    { 0x1f47e5e8, 0xa715222c, SetFlag2, 1 },
    { 0x39d4c4c4, 0xf0a2c5b0, FinishLap, 0 },
    { 0x2654216c, 0xf8256d15, GetPowerup, 1 },
    { 0x4294ec89, 0xc38ad60e, GetPowerup, 2 },
    { 0x2ceb2850, 0xa0c2d27e, GetPowerup, 3 },
    { 0x2d5f3125, 0x9ce8a541, GetPowerup, 4 },
    { 0x2169c78b, 0x7f3c9229, GetPowerup, 5 },
    { 0x2203c0cb, 0x79729be4, GetPowerup, 6 },
    { 0x34f4e3ec, 0x868c534d, GetPowerup, 7 },
    { 0x34010316, 0x591d1eb2, GetPowerup, 8 },
    { 0x214fe3bc, 0x87285111, GetPowerup, 9 },
    { 0x2fabc390, 0x0c93d9f7, GetPowerup, 10 },
    { 0x2902e890, 0x40969f67, GetPowerup, 11 },
    { 0x2f458288, 0x058074e2, GetPowerup, 12 },
    { 0x249da152, 0x8f287346, GetPowerup, 13 },
    { 0x23aae08b, 0xa668103d, GetPowerup, 14 },
    { 0x32130661, 0x56f03459, GetPowerup, 15 },
    { 0x2734e944, 0xfe1e4639, GetPowerup, 16 },
    { 0x28341139, 0x355f6d02, GetPowerup, 17 },
    { 0x20508831, 0x123d1961, GetPowerup, 18 },
    { 0x346b8bcb, 0x4aba696c, GetPowerup, 19 },
    { 0x3fc93df0, 0x29fa9efb, GetPowerup, 20 },
    { 0x2a80b09b, 0x058516f5, GetPowerup, 21 },
    { 0x2f548fd1, 0x696744da, GetPowerup, 22 },
    { 0x3cb74f32, 0xb915d88d, GetPowerup, 23 },
    { 0x297b53ba, 0x218d4d2d, GetPowerup, 24 },
    { 0x351bc37d, 0xb2a63343, GetPowerup, 25 },
    { 0x2b624386, 0x9ba6260e, GetPowerup, 26 },
    { 0x2ba4ae23, 0xc163a76c, GetPowerup, 27 },
    { 0x2fb92dca, 0x4ad7d54e, GetPowerup, 32 },
    { 0x3a42191b, 0xeff70f4c, GetPowerup, 35 },
    { 0x2aca3190, 0xd9004f25, GetPowerup, 36 },
    { 0x37c1f613, 0xb7faf351, GetPowerup, 37 },
    { 0x2ba3f603, 0x29f2425c, GetPowerup, 38 },
    { 0x416eff61, 0x2667df4b, GetPowerup, 39 },
    { 0x2554125c, 0x393ca35d, GetPowerup, 41 },
    { 0x3fff84d5, 0x84a42df4, GetPowerup, 42 },
    { 0x37e83018, 0xb609aee6, GetPowerup, 43 },
    { 0x2db03b19, 0x924a84b7, GetPowerup, 44 },
    { 0x30a19fab, 0x2b0c2782, GetPowerup, 45 },
    { 0x0, 0x0, 0x0, 0x0 },
};

// GLOBAL: CARM95 0x0051cf30
int gAllow_car_flying;

// GLOBAL: CARM95 0x0051cf34
int gEntering_message;

// GLOBAL: CARM95 0x0051cf38
tU32 gPalette_fade_time; // was gRecover_timer

// GLOBAL: CARM95 0x0053d608
char* gAbuse_text[10];

// GLOBAL: CARM95 0x0053d648
char gString[84];

// GLOBAL: CARM95 0x0053d5f8
int gToo_late;

// GLOBAL: CARM95 0x0053d5fc
int gRecover_timer;

// GLOBAL: CARM95 0x0053d63c
int gRecovery_voucher_count;

// GLOBAL: CARM95 0x00550550
int gInstant_handbrake;

// GLOBAL: CARM95 0x00550554
int gAuto_repair;

int _unittest_controls_lastGetPowerup = 0;

// IDA: void __cdecl AbortRace()
// FUNCTION: CARM95 0x004a0c70
void AbortRace(void) {

    if (!gRace_finished) {
        gAbandon_game = 1;
    }
}

// IDA: void __cdecl F4Key()
// FUNCTION: CARM95 0x004a0c92
void F4Key(void) {
    char s[256];
    tEdit_mode old_edit_mode;

    old_edit_mode = gWhich_edit_mode;
    if (gI_am_cheating == 0xa11ee75d || (gI_am_cheating == 0x564e78b9 && gNet_mode == eNet_mode_none)) {
        if (PDKeyDown(KEY_SHIFT_ANY)) {
            gWhich_edit_mode--;
            if ((int)gWhich_edit_mode < 0) {
                gWhich_edit_mode = COUNT_OF(gEdit_funcs) - 1;
            }
        } else {
            gWhich_edit_mode++;
            if (gWhich_edit_mode >= COUNT_OF(gEdit_funcs)) {
                gWhich_edit_mode = 0;
            }
        }
        sprintf(s, "Edit mode: %s", gEdit_mode_names[gWhich_edit_mode]);
        NewTextHeadupSlot2(eHeadupSlot_misc, 0, 2000, -4, s, 0);
        if (gWhich_edit_mode == eEdit_mode_spec_vol && old_edit_mode != eEdit_mode_spec_vol) {
            ShowSpecialVolumes();
        } else if (gWhich_edit_mode != eEdit_mode_spec_vol && old_edit_mode == eEdit_mode_spec_vol) {
            HideSpecialVolumes();
        }
    } else {
        gWhich_edit_mode = eEdit_mode_options;
    }
}

// IDA: void __usercall SetFlag(int i@<EAX>)
// FUNCTION: CARM95 0x004a0db7
void SetFlag(int i) {

    if (gNet_mode == eNet_mode_none) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, "You Cheat!");
    }
    gI_am_cheating = i;
    F4Key();
}

// IDA: void __usercall FinishLap(int i@<EAX>)
// FUNCTION: CARM95 0x004a0df4
void FinishLap(int i) {

    IncrementLap();
}

// IDA: void __cdecl EnsureSpecialVolumesHidden()
// FUNCTION: CARM95 0x004a0e04
void EnsureSpecialVolumesHidden(void) {

    if (gWhich_edit_mode == eEdit_mode_spec_vol) {
        HideSpecialVolumes();
    }
}

// IDA: void __cdecl ShowSpecialVolumesIfRequ()
// FUNCTION: CARM95 0x004a0e21
void ShowSpecialVolumesIfRequ(void) {

    if (gWhich_edit_mode == eEdit_mode_spec_vol) {
        ShowSpecialVolumes();
    }
}

// IDA: void __usercall DoEditModeKey(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004a0e53
void DoEditModeKey(int pIndex) {
    int modifiers;

    if (gI_am_cheating == 0xa11ee75d || (gI_am_cheating == 0x564e78b9 && gNet_mode == eNet_mode_none)) {
        modifiers = 0;
        if (PDKeyDown(KEY_SHIFT_ANY)) {
            modifiers |= 4;
        }
        if (PDKeyDown(KEY_ALT_ANY)) {
            modifiers |= 2;
        }
        if (PDKeyDown(KEY_CTRL_ANY)) {
            modifiers |= 1;
        }
        if (gEdit_funcs[gWhich_edit_mode][pIndex][modifiers] != NULL) {
            gEdit_funcs[gWhich_edit_mode][pIndex][modifiers]();
        }
    } else {
        gWhich_edit_mode = eEdit_mode_options;
    }
}

// IDA: void __cdecl F5Key()
// FUNCTION: CARM95 0x004a0e3e
void F5Key(void) {

    DoEditModeKey(0);
}

// IDA: void __cdecl F6Key()
// FUNCTION: CARM95 0x004a0f3d
void F6Key(void) {

    DoEditModeKey(1);
}

// IDA: void __cdecl F7Key()
// FUNCTION: CARM95 0x004a0f52
void F7Key(void) {

    DoEditModeKey(2);
}

// IDA: void __cdecl F8Key()
// FUNCTION: CARM95 0x004a0f67
void F8Key(void) {

    DoEditModeKey(3);
}

// IDA: void __cdecl F10Key()
// FUNCTION: CARM95 0x004a0f7c
void F10Key(void) {

    DoEditModeKey(4);
}

// IDA: void __cdecl F11Key()
// FUNCTION: CARM95 0x004a0f91
void F11Key(void) {

    DoEditModeKey(5);
}

// IDA: void __cdecl F12Key()
// FUNCTION: CARM95 0x004a0fa6
void F12Key(void) {

    DoEditModeKey(6);
}

// IDA: void __cdecl NumberKey0()
// FUNCTION: CARM95 0x004a0fbb
void NumberKey0(void) {

    DoEditModeKey(7);
}

// IDA: void __cdecl NumberKey1()
// FUNCTION: CARM95 0x004a0fd0
void NumberKey1(void) {

    DoEditModeKey(8);
}

// IDA: void __cdecl NumberKey2()
// FUNCTION: CARM95 0x004a0fe5
void NumberKey2(void) {

    DoEditModeKey(9);
}

// IDA: void __cdecl NumberKey3()
// FUNCTION: CARM95 0x004a0ffa
void NumberKey3(void) {

    DoEditModeKey(10);
}

// IDA: void __cdecl NumberKey4()
// FUNCTION: CARM95 0x004a100f
void NumberKey4(void) {

    DoEditModeKey(11);
}

// IDA: void __cdecl NumberKey5()
// FUNCTION: CARM95 0x004a1024
void NumberKey5(void) {

    DoEditModeKey(12);
}

// IDA: void __cdecl NumberKey6()
// FUNCTION: CARM95 0x004a1039
void NumberKey6(void) {

    DoEditModeKey(13);
}

// IDA: void __cdecl NumberKey7()
// FUNCTION: CARM95 0x004a104e
void NumberKey7(void) {

    DoEditModeKey(14);
}

// IDA: void __cdecl NumberKey8()
// FUNCTION: CARM95 0x004a1063
void NumberKey8(void) {

    DoEditModeKey(15);
}

// IDA: void __cdecl NumberKey9()
// FUNCTION: CARM95 0x004a1078
void NumberKey9(void) {

    DoEditModeKey(16);
}

// IDA: void __cdecl LookLeft()
// FUNCTION: CARM95 0x004a108d
void LookLeft(void) {

    if (gAusterity_mode) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_NOT_ENOUGH_MEMORY));
    } else {
        PratcamEvent(kPratcam_cockpit_head_left);
        gProgram_state.old_view = gProgram_state.which_view;
        if (gProgram_state.which_view == eView_left) {
            LookForward();
        } else if (gProgram_state.which_view == eView_right) {
            LookForward();
            gProgram_state.pending_view = eView_left;
        } else {
            ClearWobbles();
            gProgram_state.new_view = eView_left;
            gProgram_state.view_change_start = PDGetTotalTime();
            gProgram_state.pending_view = eView_undefined;
        }
    }
}

// IDA: void __cdecl LookForward()
// FUNCTION: CARM95 0x004a113a
void LookForward(void) {

    if (gProgram_state.which_view == eView_right) {
        PratcamEvent(kPratcam_cockpit_head_left);
    } else if (gProgram_state.which_view == eView_left) {
        PratcamEvent(kPratcam_cockpit_head_right);
    }
    if (gProgram_state.which_view != eView_forward) {
        gProgram_state.old_view = gProgram_state.which_view;
        ClearWobbles();
        gProgram_state.new_view = eView_forward;
        gProgram_state.view_change_start = PDGetTotalTime();
        gProgram_state.pending_view = eView_undefined;
    }
}

// IDA: void __cdecl LookRight()
// FUNCTION: CARM95 0x004a11b2
void LookRight(void) {

    if (gAusterity_mode) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_NOT_ENOUGH_MEMORY));
    } else {
        PratcamEvent(kPratcam_cockpit_head_right);
        gProgram_state.old_view = gProgram_state.which_view;
        if (gProgram_state.which_view == eView_right) {
            LookForward();
        } else if (gProgram_state.which_view == eView_left) {
            LookForward();
            gProgram_state.pending_view = eView_right;
        } else {
            ClearWobbles();
            gProgram_state.new_view = eView_right;
            gProgram_state.view_change_start = PDGetTotalTime();
            gProgram_state.pending_view = eView_undefined;
        }
    }
}

// IDA: void __cdecl DamageTest()
// FUNCTION: CARM95 0x004a125f
void DamageTest(void) {
}

// IDA: void __cdecl TDamageEngine()
// FUNCTION: CARM95 0x004a126a
void TDamageEngine(void) {

    DamageEngine(21);
}

// IDA: void __cdecl TDamageDriver()
// FUNCTION: CARM95 0x004a127f
void TDamageDriver(void) {

    if (gProgram_state.current_car.damage_units[eDamage_driver].damage_level >= 80) {
        DamageUnit(&gProgram_state.current_car, eDamage_driver, 2);
    } else {
        DamageUnit(&gProgram_state.current_car, eDamage_driver, 80 - gProgram_state.current_car.damage_units[2].damage_level);
    }
}

// IDA: void __cdecl TDamageTrans()
// FUNCTION: CARM95 0x004a12d4
void TDamageTrans(void) {

    DamageTrans(21);
}

// IDA: void __cdecl TDamageSteering()
// FUNCTION: CARM95 0x004a12e9
void TDamageSteering(void) {

    DamageSteering(21);
}

// IDA: void __cdecl TDamageLFWheel()
// FUNCTION: CARM95 0x004a12fe
void TDamageLFWheel(void) {

    DamageLFWheel(21);
}

// IDA: void __cdecl TDamageLFBrake()
// FUNCTION: CARM95 0x004a1313
void TDamageLFBrake(void) {

    DamageLFBrake(21);
}

// IDA: void __cdecl TDamageLRBrake()
// FUNCTION: CARM95 0x004a1328
void TDamageLRBrake(void) {

    DamageLRBrake(21);
}

// IDA: void __cdecl TDamageLRWheel()
// FUNCTION: CARM95 0x004a133d
void TDamageLRWheel(void) {

    DamageLRWheel(21);
}

// IDA: void __cdecl TDamageRFWheel()
// FUNCTION: CARM95 0x004a1352
void TDamageRFWheel(void) {

    DamageRFWheel(21);
}

// IDA: void __cdecl TDamageRFBrake()
// FUNCTION: CARM95 0x004a1367
void TDamageRFBrake(void) {

    DamageRFBrake(21);
}

// IDA: void __cdecl TDamageRRBrake()
// FUNCTION: CARM95 0x004a137c
void TDamageRRBrake(void) {

    DamageRRBrake(21);
}

// IDA: void __cdecl TDamageRRWheel()
// FUNCTION: CARM95 0x004a1391
void TDamageRRWheel(void) {

    DamageRRWheel(21);
}

// IDA: void __cdecl MoveBonnetForward()
// FUNCTION: CARM95 0x004a13a6
void MoveBonnetForward(void) {

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.translate.t.v[2] -= .005f;
}

// IDA: void __cdecl SaveBonnet()
// FUNCTION: CARM95 0x004a13dd
void SaveBonnet(void) {
    br_actor* bonny;
    tPath_name the_path;

    bonny = gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor;
    PathCat(the_path, gApplication_path, bonny->identifier);
    BrActorSave(the_path, bonny);
}

// IDA: void __cdecl MoveBonnetBackward()
// FUNCTION: CARM95 0x004a142f
void MoveBonnetBackward(void) {

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.translate.t.v[2] += .005f;
}

// IDA: void __cdecl MoveBonnetLeft()
// FUNCTION: CARM95 0x004a1466
void MoveBonnetLeft(void) {

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.translate.t.v[0] -= .005f;
}

// IDA: void __cdecl ShrinkBonnetX()
// FUNCTION: CARM95 0x004a149d
void ShrinkBonnetX(void) {

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat.m[0][0] *= .98f;
}

// IDA: void __cdecl SwellBonnetX()
// FUNCTION: CARM95 0x004a14d4
void SwellBonnetX(void) {

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat.m[0][0] *= 1.02f;
}

// IDA: void __cdecl ShrinkBonnetY()
// FUNCTION: CARM95 0x004a150b
void ShrinkBonnetY(void) {

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat.m[1][1] *= .98f;
}

// IDA: void __cdecl SwellBonnetY()
// FUNCTION: CARM95 0x004a1542
void SwellBonnetY(void) {

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat.m[1][1] *= 1.02f;
}

// IDA: void __cdecl ShrinkBonnetZ()
// FUNCTION: CARM95 0x004a1579
void ShrinkBonnetZ(void) {

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat.m[2][2] *= .98f;
}

// IDA: void __cdecl SwellBonnetZ()
// FUNCTION: CARM95 0x004a15b0
void SwellBonnetZ(void) {

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat.m[2][2] *= 1.02f;
}

// IDA: void __cdecl MoveBonnetDown()
// FUNCTION: CARM95 0x004a15e7
void MoveBonnetDown(void) {

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.translate.t.v[1] += .005f;
}

// IDA: void __cdecl MoveBonnetRight()
// FUNCTION: CARM95 0x004a161e
void MoveBonnetRight(void) {

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.translate.t.v[0] += .005f;
}

// IDA: void __cdecl MoveBonnetUp()
// FUNCTION: CARM95 0x004a1655
void MoveBonnetUp(void) {

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.translate.t.v[1] -= .005f;
}

// IDA: void __cdecl TiltBonnetDownX()
// FUNCTION: CARM95 0x004a168c
void TiltBonnetDownX(void) {

    BrMatrix34PreRotateX(&gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat, BR_ANGLE_DEG(.5f));
}

// IDA: void __cdecl TiltBonnetUpX()
// FUNCTION: CARM95 0x004a16b5
void TiltBonnetUpX(void) {

    BrMatrix34PreRotateX(&gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat, -BR_ANGLE_DEG(.5f));
}

// IDA: void __cdecl TiltBonnetDownY()
// FUNCTION: CARM95 0x004a16e1
void TiltBonnetDownY(void) {

    BrMatrix34PreRotateY(&gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat, BR_ANGLE_DEG(.5f));
}

// IDA: void __cdecl TiltBonnetUpY()
// FUNCTION: CARM95 0x004a170a
void TiltBonnetUpY(void) {

    BrMatrix34PreRotateY(&gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat, -BR_ANGLE_DEG(.5f));
}

// IDA: void __cdecl TiltBonnetDownZ()
// FUNCTION: CARM95 0x004a1736
void TiltBonnetDownZ(void) {

    BrMatrix34PreRotateZ(&gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat, BR_ANGLE_DEG(.5f));
}

// IDA: void __cdecl TiltBonnetUpZ()
// FUNCTION: CARM95 0x004a175f
void TiltBonnetUpZ(void) {

    BrMatrix34PreRotateZ(&gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat, -BR_ANGLE_DEG(.5f));
}

// IDA: void __cdecl ToggleCockpit()
// FUNCTION: CARM95 0x004a178b
void ToggleCockpit(void) {
    br_scalar ts;

    if ((&gProgram_state.current_car == gCar_to_view || gProgram_state.cockpit_on) && !gMap_mode) {
        if (!gAusterity_mode || gProgram_state.cockpit_on) {
            gProgram_state.cockpit_on = !gProgram_state.cockpit_on;
            if (gProgram_state.cockpit_on) {
                gCamera = gCamera_list[0];
            } else {
                gCamera = gCamera_list[1];
                InitialiseExternalCamera();
                PositionExternalCamera(&gProgram_state.current_car, 1);
            }
            AdjustRenderScreenSize();
            AdjustHeadups();
            MungeForwardSky();
        } else {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_NOT_ENOUGH_MEMORY));
        }
    }
}

// IDA: void __cdecl ToggleMirror()
// FUNCTION: CARM95 0x004a1895
void ToggleMirror(void) {

    gProgram_state.mirror_on = !gProgram_state.mirror_on;
    ReinitialiseRearviewCamera();
    if (gProgram_state.mirror_on) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 500, -4, GetMiscString(kMiscString_MirrorOn));
    } else {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 500, -4, GetMiscString(kMiscString_MirrorOff));
    }
}

// IDA: void __cdecl ConcussMe()
// dethrace: this is not referenced in the retail executables. Left over debug code.
// FUNCTION: CARM95 0x004a1919
void ConcussMe(void) {

    SufferFromConcussion(1.f);
    NewScreenWobble(IRandomPosNeg(15), IRandomPosNeg(10), IRandomBetween(10, 60));
    PratcamEvent(kPratcam_over_137mph);
}

// IDA: void __cdecl CheckHelp()
void CheckHelp(void) {
}

// IDA: void __cdecl CheckLoadSave()
// FUNCTION: CARM95 0x004a29b5
void CheckLoadSave(void) {
    int save_load_allowed;
    int switched_res;

    save_load_allowed = !gProgram_state.saving && !gProgram_state.loading && gProgram_state.prog_status == eProg_game_ongoing && !gProgram_state.dont_save_or_load;

    if (CmdKeyDown(KEYMAP_SAVE, KEYMAP_CTRL_SAVE)) {
        if (save_load_allowed) {
            FadePaletteDown();
            ClearEntireScreen();
            if (gProgram_state.racing) {
                GoingToInterfaceFromRace();
            }
            DoSaveGame(gProgram_state.racing == 0);
            if (gProgram_state.racing) {
                GoingBackToRaceFromInterface();
            }
        }
        WaitForNoKeys();
    }
    if (CmdKeyDown(KEYMAP_LOAD, KEYMAP_CTRL_LOAD)) {
        if (save_load_allowed && !gProgram_state.dont_load) {
            FadePaletteDown();
            ClearEntireScreen();
            if (gProgram_state.racing) {
                GoingToInterfaceFromRace();
            }
            if (DoLoadGame() && !gProgram_state.racing) {
                gProgram_state.prog_status = eProg_game_starting;
            }
            if (gProgram_state.racing) {
                GoingBackToRaceFromInterface();
            }
            PlayFlicsFromMemory();
        }
        WaitForNoKeys();
    }
}

// IDA: void __usercall CheckToggles(int pRacing@<EAX>)
// FUNCTION: CARM95 0x004a198a
void CheckToggles(int pRacing) {
    int i;
    int new_state;

    for (i = 0; i < COUNT_OF(gToggle_array); i++) {
        if ((!gToggle_array[i].in_game_only || pRacing)
            && ((!gTyping && !gEntering_message) || gToggle_array[i].key2 != -2)) {
            new_state = 0;
            if (gToggle_array[i].key1 == -2 || KeyIsDown(gToggle_array[i].key1)) {
                if (gToggle_array[i].key2 == -2 && gToggle_array[i].exact_modifiers) {
                    if (!PDKeyDown(KEY_SHIFT_ANY) && !PDKeyDown(KEY_ALT_ANY) && !PDKeyDown(KEY_CTRL_ANY) && !PDKeyDown(KEY_CTRL_ANY_2)) {
                        new_state = 1;
                    }
                } else {
                    if (KeyIsDown(gToggle_array[i].key2)) {
                        new_state = 1;
                    }
                }
            }
            if (gToggle_array[i].on_last_time != new_state) {
                gToggle_array[i].on_last_time = new_state;
                if (new_state) {
                    gToggle_array[i].action_proc();
                }
            }
        }
    }
}

// IDA: int __usercall CarWorldOffFallenCheckThingy@<EAX>(tCar_spec *pCar@<EAX>, int pCheck_around@<EDX>)
// FUNCTION: CARM95 0x004a1b4d
int CarWorldOffFallenCheckThingy(tCar_spec* pCar, int pCheck_around) {
    br_vector3 car_pos;
    br_vector3 offset_c;
    br_vector3 offset_w;
    int result;

    if (pCar->number_of_wheels_on_ground != 0) {
        return 0;
    }
    if (pCar->driver == eDriver_local_human && gCar_flying) {
        return 0;
    }
    if (gAction_replay_mode) {
        return 0;
    }
    BrVector3Copy(&car_pos, &pCar->car_master_actor->t.t.translate.t);
    if (FindYVerticallyBelow2(&car_pos) >= -100.f) {
        return 0;
    }
    BrVector3Set(&offset_c, 0.f, 1.f, 0.f);
    BrMatrix34ApplyV(&offset_w, &offset_c, &pCar->car_master_actor->t.t.mat);
    if (FindYVerticallyBelow2(&car_pos) >= -100.f) {
        // FIXME: testing twice using `FindYVerticallyBelow2' is meaningless
        return 0;
    }
    if (!pCheck_around) {
        return 1;
    }
    pCar->car_master_actor->t.t.translate.t.v[0] += 0.05f;
    result = CarWorldOffFallenCheckThingy(pCar, 0);
    pCar->car_master_actor->t.t.translate.t.v[0] -= 0.05f;
    if (!result) {
        return 0;
    }
    pCar->car_master_actor->t.t.translate.t.v[2] += 0.05f;
    result = CarWorldOffFallenCheckThingy(pCar, 0);
    pCar->car_master_actor->t.t.translate.t.v[2] -= 0.05f;
    if (!result) {
        return 0;
    }
    return 1;
}

// IDA: int __usercall HasCarFallenOffWorld@<EAX>(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004a1b2f
int HasCarFallenOffWorld(tCar_spec* pCar) {

    return CarWorldOffFallenCheckThingy(pCar, 1);
}

// IDA: void __cdecl CheckForBeingOutOfThisWorld()
// FUNCTION: CARM95 0x004a3328
void CheckForBeingOutOfThisWorld(void) {
    // GLOBAL: CARM95 0x53d638
    static tU32 the_time;
    // GLOBAL: CARM95 0x51cf3c
    static tU32 sLast_check;
    int time_step;

    the_time = PDGetTotalTime();

    if (gRecover_timer == 0 || ((gProgram_state.current_car.frame_collision_flag || gProgram_state.current_car.number_of_wheels_on_ground) && !IsCarInTheSea())) {
        gRecover_timer = 0;
        if ((the_time - sLast_check) > 200) {
            sLast_check = the_time;
            if (HasCarFallenOffWorld(&gProgram_state.current_car)) {
                gRecover_timer = 3000;
            }
        }
        if (IsCarInTheSea()) {
            if (!gRecover_timer) {
                gRecover_timer = 3000;
            }
        }
        return;
    }
    gRecover_timer -= gFrame_period;
    if (gRecover_timer <= 0 || IsCarInTheSea() == 2) {
        gRecover_timer = 0;
        RecoverCar();
        gHad_auto_recover = 1;
    }
}

// IDA: void __usercall CheckHornLocal(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004a34a8
void CheckHornLocal(tCar_spec* pCar) {

    if (pCar->keys.horn == 1 && pCar->horn_sound_tag == 0) {
        pCar->horn_sound_tag = DRS3StartSound(gEffects_outlet, 5209);
    } else if (pCar->keys.horn == 0 && pCar->horn_sound_tag != 0) {
        if (S3SoundStillPlaying(pCar->horn_sound_tag) != 0) {
            DRS3StopSound(pCar->horn_sound_tag);
            DRS3StopOutletSound(gEffects_outlet);
        }
        if (S3SoundStillPlaying(pCar->horn_sound_tag) == 0) {
            pCar->horn_sound_tag = 0;
        }
    }
}

// IDA: void __usercall CheckHorn3D(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004a3582
void CheckHorn3D(tCar_spec* pCar) {

    if (pCar->keys.horn && pCar->horn_sound_tag == 0) {
        pCar->horn_sound_tag = DRS3StartSound3D(gEffects_outlet,
            5209,
            &pCar->car_master_actor->t.t.translate.t,
            &pCar->velocity_bu_per_sec,
            0,
            255,
            ((pCar->car_ID & 7) << 12) + 0xc000,
            0x10000);
    } else {
        if (!pCar->keys.horn && pCar->horn_sound_tag != 0) {
            while (S3SoundStillPlaying(pCar->horn_sound_tag)) {
                DRS3StopSound(pCar->horn_sound_tag);
                DRS3StopOutletSound(gEffects_outlet);
            }
            if (!S3SoundStillPlaying(pCar->horn_sound_tag)) {
                pCar->horn_sound_tag = 0;
            }
        }
    }
}

// IDA: void __cdecl CheckHorns()
// FUNCTION: CARM95 0x004a343d
void CheckHorns(void) {
    int i;

    if (gNet_mode != eNet_mode_none) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            CheckHorn3D(gNet_players[i].car);
        }
    } else {
        CheckHornLocal(&gProgram_state.current_car);
    }
}

// IDA: void __cdecl SetRecovery()
// FUNCTION: CARM95 0x004a1d13
void SetRecovery(void) {

    if (gRace_finished
        || gProgram_state.current_car.knackered
        || gWait_for_it
        || gHad_auto_recover
        || gPalette_fade_time) {
        return;
    }

    if (gNet_mode == eNet_mode_none) {
        gRecover_car = 1;
        gRecover_timer = 0;
        return;
    }
    if (gProgram_state.current_car.time_to_recover) {
        if (GetRaceTime() + 600 >= gProgram_state.current_car.time_to_recover) {
            NewTextHeadupSlot2(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_TOO_LATE_TO_CANCEL), 1);
            gToo_late = 1;
        } else {
            gProgram_state.current_car.time_to_recover = 0;
            NewTextHeadupSlot2(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_RECOVERY_CANCELLED), 0);
        }
        return;
    }
    if (!CheckRecoverCost()) {
        return;
    }
    if (gCurrent_net_game->type == eNet_game_type_foxy) {
        if (gThis_net_player_index == gIt_or_fox) {
            gProgram_state.current_car.time_to_recover = GetRaceTime() + 5000;
            gRecover_timer = 0;
            gToo_late = 0;
            return;
        }
    } else {
        if (gCurrent_net_game->type != eNet_game_type_tag) {
            gProgram_state.current_car.time_to_recover = GetRaceTime() + 3000;
            gRecover_timer = 0;
            gToo_late = 0;
            return;
        }
        if (gThis_net_player_index != gIt_or_fox) {
            gProgram_state.current_car.time_to_recover = GetRaceTime() + 5000;
            gRecover_timer = 0;
            gToo_late = 0;
            return;
        }
    }
    gProgram_state.current_car.time_to_recover = GetRaceTime() + 1000;
    gRecover_timer = 0;
    gToo_late = 0;
}

// IDA: void __cdecl RecoverCar()
// FUNCTION: CARM95 0x004a20b0
void RecoverCar(void) {

    if (gNet_mode == eNet_mode_none || !gPalette_fade_time) {
        gRecover_car = 1;
    }
    gProgram_state.current_car.time_to_recover = 0;
}

// IDA: void __cdecl CheckMapRenderMove()
// FUNCTION: CARM95 0x004a3694
void CheckMapRenderMove(void) {
    int shift_down;
    int amount;
    float old_x;
    float old_y;

    old_y = gMap_render_y;
    old_x = gMap_render_x;
    if (gMap_mode) {
        amount = gFrame_period * .1f;
        if (KeyIsDown(KEYMAP_MOVE_UP)) {
            gMap_render_y -= amount;
        } else if (KeyIsDown(KEYMAP_MOVE_DOWN)) {
            gMap_render_y += amount;
        }
        if (KeyIsDown(KEYMAP_MOVE_LEFT)) {
            gMap_render_x -= amount;
        } else if (KeyIsDown(KEYMAP_MOVE_RIGHT)) {
            gMap_render_x += amount;
        }
        if (gMap_render_x != old_x || gMap_render_y != old_y) {
            SetIntegerMapRenders();
            if (gMap_render_x_i < gCurrent_graf_data->map_render_x_marg) {
                if (gReal_graf_data_index == 0) {
                    gMap_render_x = (gCurrent_graf_data->map_render_x_marg + 3) & ~3;
                } else {
                    gMap_render_x = ((gCurrent_graf_data->map_render_x_marg + 3) & ~3) / 2;
                }
            }
            if (gMap_render_y_i < gCurrent_graf_data->map_render_y_marg) {
                if (gReal_graf_data_index == 0) {
                    gMap_render_y = (gCurrent_graf_data->map_render_y_marg + 1) & ~1;
                } else {
                    gMap_render_y = (((gCurrent_graf_data->map_render_y_marg + 1) & ~1) - 40) / 2;
                }
            }
            if (gBack_screen->width - gCurrent_graf_data->map_render_x_marg - gMap_render_width_i < gMap_render_x_i) {
                if (gReal_graf_data_index == 0) {
                    gMap_render_x = (gBack_screen->width - gCurrent_graf_data->map_render_x_marg - gMap_render_width_i) & ~3;
                } else {
                    gMap_render_x = ((gBack_screen->width - gCurrent_graf_data->map_render_x_marg - gMap_render_width_i) & ~3) / 2;
                }
            }
            if (gBack_screen->height - gCurrent_graf_data->map_render_y_marg - gMap_render_height_i < gMap_render_y_i) {
                if (gReal_graf_data_index == 0) {
                    gMap_render_y = (gBack_screen->height - gCurrent_graf_data->map_render_y_marg - gMap_render_height_i) & ~1;
                } else {
                    gMap_render_y = (((gBack_screen->height - gCurrent_graf_data->map_render_y_marg - gMap_render_height_i) & ~3) - 40) / 2;
                }
            }
            SetIntegerMapRenders();
            AdjustRenderScreenSize();
        }
    }
}

// IDA: void __usercall ExplodeCar(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004a20e9
void ExplodeCar(tCar_spec* pCar) {
    br_vector3 tv;
    br_vector3 pos;

    pCar->last_car_car_collision = 0;
    pos.v[0] = .1449275f * pCar->cmpos.v[0];
    pos.v[1] = .1449275f * pCar->cmpos.v[1];
    pos.v[2] = pCar->bounds[0].min.v[2] + .3f * (pCar->bounds[0].max.v[2] - pCar->bounds[0].min.v[2]);
    BrMatrix34ApplyP(&tv, &pos, &pCar->car_master_actor->t.t.mat);
    CreatePuffOfSmoke(&tv, &pCar->v, 1.f, 1.f, 7, pCar);

    pos.v[2] = pCar->bounds[0].min.v[2] + .7f * (pCar->bounds[0].max.v[2] - pCar->bounds[0].min.v[2]);
    BrMatrix34ApplyP(&tv, &pos, &pCar->car_master_actor->t.t.mat);
    CreatePuffOfSmoke(&tv, &pCar->v, 1.f, 1.f, 7, pCar);

    DisableCar(pCar);
}

// IDA: void __usercall CheckRecoveryOfCars(tU32 pEndFrameTime@<EAX>)
// FUNCTION: CARM95 0x004a1ec9
void CheckRecoveryOfCars(tU32 pEndFrameTime) {
    int i;
    int time;
    char s[256];

    if (gProgram_state.current_car.time_to_recover) {
        if (gProgram_state.current_car.knackered) {
            gProgram_state.current_car.time_to_recover = 0;
        } else {
            time = (gProgram_state.current_car.time_to_recover - pEndFrameTime + 1000) / 1000;
            sprintf(s, "%s %d %s", GetMiscString(kMiscString_RECOVERY_IN), time, time > 1 ? GetMiscString(kMiscString_SECONDS) : GetMiscString(kMiscString_SECOND));
            if (!gToo_late) {
                NewTextHeadupSlot2(eHeadupSlot_misc, 0, 2000, -4, s, 0);
            }
            if (gProgram_state.current_car.time_to_recover <= pEndFrameTime) {
                RecoverCar();
            }
        }
    }
    if (gNet_mode) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (gThis_net_player_index != i && gNet_players[i].car->time_to_recover && gNet_players[i].car->time_to_recover <= pEndFrameTime) {
                gNet_players[i].player_status = ePlayer_status_recovering;
                gNet_players[i].car->message.type = 32;
                gNet_players[i].car->message.time = pEndFrameTime;
                ExplodeCar(gNet_players[i].car);
                gNet_players[i].car->time_to_recover = 0;
            }
        }
    }
}

// IDA: void __usercall LoseSomePSPowerups(int pNumber@<EAX>)
// FUNCTION: CARM95 0x004a39cc
void LoseSomePSPowerups(int pNumber) {
    int index;

    if (gNet_mode != eNet_mode_none && pNumber > 0) {
        while (pNumber--) {
            index = IRandomBetween(0, 2);
            if (gProgram_state.current_car.power_up_levels[index]) {
                gProgram_state.current_car.power_up_levels[index]--;
            }
        }
    }
}

// IDA: void __cdecl CheckOtherRacingKeys()
// FUNCTION: CARM95 0x004a2af6
void CheckOtherRacingKeys(void) {
    int i;
    int j;
    int new_level;
    int old_level;
    char s[256];
    tU32 cost;
    br_scalar ts;
    br_vector3 tv;
    int flip_up_flag;
    tCar_spec* car;
    float bodywork_repair_amount;
    // GLOBAL: CARM95 0x51cf40
    static tU32 total_repair_cost;
    // GLOBAL: CARM95 0x51cf44
    static tS3_sound_tag sound_tag;
    // GLOBAL: CARM95 0x53d640
    static br_scalar amount;
    // GLOBAL: CARM95 0x51cf48
    static int NeedToExpandBoundingBox;
    // GLOBAL: CARM95 0x53d600
    static int total_difference;
    // GLOBAL: CARM95 0x51cf4c
    static int stopped_repairing;

    car = GetCarSpec(eVehicle_self, 0);
    CheckMapRenderMove();
    CheckHorns();
    CheckForBeingOutOfThisWorld();
    if (gPalette_fade_time) {
        SortOutRecover(car);
    } else if (gNet_mode && NetGetPlayerStatus() == ePlayer_status_recovering) {
        NetPlayerStatusChanged(ePlayer_status_racing);
    }

    if ((gAuto_repair || KeyIsDown(KEYMAP_REPAIR)) && !gRace_finished && !gProgram_state.current_car.knackered && !gWait_for_it && !gEntering_message) {
        if (!gAuto_repair && gRepair_last_time == 0 && GetTotalTime() - gLast_repair_time < 1200) {
            gAuto_repair = 1;
        }
        gLast_repair_time = GetTotalTime();
        gRepair_last_time = 1;
        if (!NeedToExpandBoundingBox) {
            if (gFree_repairs
                || gNet_mode == eNet_mode_none
                || gProgram_state.credits_earned - gProgram_state.credits_lost >= 1) {
                bodywork_repair_amount = RepairCar(gProgram_state.current_car.car_ID, gFrame_period, &amount);
                NeedToExpandBoundingBox = bodywork_repair_amount > 0;
                cost = 0;
                for (j = 0; j < COUNT_OF(gProgram_state.current_car.damage_units); j++) {
                    old_level = gProgram_state.current_car.damage_units[j].damage_level;
                    if (amount == 0.0f) {
                        new_level = 0;
                    } else {
                        new_level = ((double)gProgram_state.current_car.damage_units[j].damage_level
                            - floor(bodywork_repair_amount / amount * (double)gProgram_state.current_car.damage_units[j].damage_level));
                    }
                    if (new_level >= 0) {
                        if (new_level < 100) {
                            gProgram_state.current_car.damage_units[j].damage_level = new_level;
                        } else {
                            gProgram_state.current_car.damage_units[j].damage_level = 99;
                        }
                    } else {
                        gProgram_state.current_car.damage_units[j].damage_level = 0;
                    }
                    gProgram_state.current_car.damage_units[j].smoke_last_level = gProgram_state.current_car.damage_units[j].damage_level;
                    if (gNet_mode) {
                        ts = gNet_repair_cost[gCurrent_net_game->type];
                    } else {
                        ts = gRepair_cost[gProgram_state.skill_level];
                    }
                    cost = (old_level - gProgram_state.current_car.damage_units[j].damage_level) * ts + cost;
                    total_difference += old_level - new_level;
                }
                if (!gFree_repairs) {
                    LoseSomePSPowerups(total_difference / 100);
                }
                total_difference %= 100;
                cost = 10 * (cost / 10);
                if (((!total_repair_cost && cost) || bodywork_repair_amount != 0.0f) && !sound_tag) {
                    sound_tag = DRS3StartSound(gCar_outlet, 5200);
                }
                if (gProgram_state.current_car.num_smoke_columns) {
                    StopCarSmoking(&gProgram_state.current_car);
                }
                if (!cost && bodywork_repair_amount == 0.0) {
                    gAuto_repair = 0;
                }
                if (!gFree_repairs) {
                    cost += SpendCredits(cost);
                }
                total_repair_cost += cost;
                if (total_repair_cost) {
                    if (gFree_repairs) {
                        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_RepairingForFree));
                    } else {
                        sprintf(s, "%s %d", GetMiscString(kMiscString_RepairCostColon), total_repair_cost);
                        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, s);
                    }
                }
            } else {
                if (!stopped_repairing) {
                    NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_CANNOT_AFFORD_TO_REPAIR));
                }
                gAuto_repair = 0;
                stopped_repairing = 1;
            }
        }

    } else {
        gRepair_last_time = 0;
        stopped_repairing = 0;
        total_repair_cost = 0;
        total_difference = 0;
        if (sound_tag) {
            for (i = 0; i < 10 && S3SoundStillPlaying(sound_tag); ++i) {
                DRS3StopSound(sound_tag);
            }
            sound_tag = 0;
        }
    }
    if (NeedToExpandBoundingBox) {
        NeedToExpandBoundingBox = ExpandBoundingBox(&gProgram_state.current_car) == 0;
    }
    if (!gRecover_car || gProgram_state.current_car.knackered) {
        gHad_auto_recover = 0;
    } else if (CheckRecoverCost()) {
        gRecover_timer = 0;
        SetFlipUpCar(car);
        if (gNet_mode != eNet_mode_none) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 1500, -4, " ");
        }
        if (gRecovery_voucher_count != 0) {
            gRecovery_voucher_count--;
            sprintf(s, "%s", GetMiscString(kMiscString_RecoveringForFree));
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 1500, -4, s);
        } else {
            if (gNet_mode) {
                cost = gNet_recovery_cost[gCurrent_net_game->type];
            } else {
                cost = gRecovery_cost[gProgram_state.skill_level];
            }
            SpendCredits(cost);
            if (gNet_mode) {
                cost = gNet_recovery_cost[gCurrent_net_game->type];
            } else {
                cost = gRecovery_cost[gProgram_state.skill_level];
            }
            sprintf(s, "%s %d", GetMiscString(kMiscString_RecoveryCostColon), cost);
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 1500, -4, s);
            LoseSomePSPowerups(2);
        }
        CancelPendingCunningStunt();
        PipeSingleSpecial(ePipe_special_fade);
    }
    gRecover_car = 0;
}

// IDA: int __cdecl CheckRecoverCost()
// FUNCTION: CARM95 0x004a2208
int CheckRecoverCost(void) {

    if (gProgram_state.current_car.knackered
        || gNet_mode == eNet_mode_none
        || (gProgram_state.credits_earned - gProgram_state.credits_lost) >= gNet_recovery_cost[gCurrent_net_game->type]
        || gRecovery_voucher_count) {
        return 1;
    }
    gProgram_state.credits_earned = 0;
    gProgram_state.credits_lost = 0;
    NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_CANNOT_AFFORD_TO_RECOVER));
    DoFancyHeadup(kFancyHeadupNetworkRaceNoMoreMoney);
    KnackerThisCar(&gProgram_state.current_car);
    SendGameplayToHost(eNet_gameplay_suicide, 0, 0, 0, 0);
    return 0;
}

// IDA: void __usercall SortOutRecover(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004a3a33
void SortOutRecover(tCar_spec* pCar) {
    int the_time;
    int val;
    // GLOBAL: CARM95 0x51cf50
    static int old_time;

    the_time = GetRaceTime() - gPalette_fade_time;
    if (the_time < 0) {
        gPalette_fade_time = 0;
        old_time = 0;
    }
    if (the_time < 500) {
        val = 256 - (the_time * 256) / 500;
    } else {
        if (old_time < 500) {
            FlipUpCar(pCar);
            PipeSingleSpecial(ePipe_special_fade);
        }
        pCar->doing_nothing_flag = 1;
        val = ((the_time - 1000) * 256) / 500;
        if (val >= 256) {
            val = 256;
            gPalette_fade_time = 0;
            old_time = 0;
            pCar->doing_nothing_flag = 0;
        }
    }
    if (val <= 0) {
        val = 0;
    }
    SetFadedPalette(val);
    old_time = the_time;
}

// IDA: void __usercall SetFlipUpCar(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004a22f4
void SetFlipUpCar(tCar_spec* pCar) {

    if (gNet_mode != eNet_mode_none && pCar->driver == eDriver_local_human) {
        DisableCar(pCar);
        gPalette_fade_time = GetRaceTime();
        NetPlayerStatusChanged(ePlayer_status_recovering);
    } else {
        FlipUpCar(pCar);
    }
}

// IDA: void __usercall FlipUpCar(tCar_spec *car@<EAX>)
// FUNCTION: CARM95 0x004a234a
void FlipUpCar(tCar_spec* car) {
    br_vector3 tv;
    br_vector3 dir;
    int new_pos;
    int i;
    int j;
    int l;
    int count;
    br_scalar dist;
    br_material* material;
    br_scalar t;

    count = 0;
    if (car->driver == eDriver_local_human && gNet_mode == eNet_mode_none) {
        FadePaletteDown();
        while (KeyIsDown(KEYMAP_REPAIR)) {
            ;
        }
    }
    car->doing_nothing_flag = 0;
    EnableCar(car);
    new_pos = 1;
    for (i = 0; i < 4; ++i) {
        if (car->susp_height[i >> 1] <= car->oldd[i]) {
            new_pos = 0;
        }
    }
    do {
        tv.v[0] = car->car_master_actor->t.t.mat.m[3][0] - car->last_safe_positions[0].m[3][0];
        tv.v[1] = car->car_master_actor->t.t.mat.m[3][1] - car->last_safe_positions[0].m[3][1];
        tv.v[2] = car->car_master_actor->t.t.mat.m[3][2] - car->last_safe_positions[0].m[3][2];
        if (BrVector3LengthSquared(&tv) > 8.3015966) {
            new_pos = 0;
        }
        BrMatrix34Copy(&car->car_master_actor->t.t.mat, &car->last_safe_positions[new_pos]);
        BrMatrix34Copy(&car->oldmat, &car->last_safe_positions[new_pos]);
        BrMatrix34Copy(&car->old_frame_mat, &car->oldmat);
        car->oldmat.m[3][0] = car->oldmat.m[3][0] * WORLD_SCALE;
        car->oldmat.m[3][1] = car->oldmat.m[3][1] * WORLD_SCALE;
        car->oldmat.m[3][2] = car->oldmat.m[3][2] * WORLD_SCALE;
        dir.v[0] = 0.0;
        dir.v[1] = 0.28985506;
        dir.v[2] = 0.0;
        FindFace(&car->car_master_actor->t.t.euler.t, &dir, &tv, &t, &material);
        if (t > 1.0) {
            car->car_master_actor->t.t.mat.m[3][0] += dir.v[0];
            car->car_master_actor->t.t.mat.m[3][1] += dir.v[1];
            car->car_master_actor->t.t.mat.m[3][2] += dir.v[2];
            car->oldmat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] * WORLD_SCALE;
            car->oldmat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] * WORLD_SCALE;
            car->oldmat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] * WORLD_SCALE;
            car->old_frame_mat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0];
            car->old_frame_mat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1];
            car->old_frame_mat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2];
        }
        tv.v[0] = 0.0;
        tv.v[1] = 0.0;
        tv.v[2] = -0.001;
        BrMatrix34ApplyV(&car->v, &tv, &car->car_master_actor->t.t.mat);
        car->omega.v[0] = 0.0;
        car->omega.v[1] = 0.0;
        car->omega.v[2] = 0.0;
        car->direction.v[0] = -car->oldmat.m[2][0];
        car->direction.v[1] = -car->oldmat.m[2][1];
        car->direction.v[2] = -car->oldmat.m[2][2];
        for (i = 0; i <= new_pos; i++) {
            for (j = 0; j < 4; j++) {
                BrMatrix34Copy(&car->last_safe_positions[j], &car->last_safe_positions[j + 1]);
            }
        }
        for (l = 0; l < 10; l++) {
            BrVector3Scale(&car->old_norm, &car->old_norm, 0.072463766);
            BrMatrix34ApplyV(&tv, &car->old_norm, &car->car_master_actor->t.t.mat);
            car->car_master_actor->t.t.mat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] + tv.v[0];
            car->car_master_actor->t.t.mat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] + tv.v[1];
            car->car_master_actor->t.t.mat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] + tv.v[2];
            car->oldmat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] * WORLD_SCALE;
            car->oldmat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] * WORLD_SCALE;
            car->oldmat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] * WORLD_SCALE;
            car->old_frame_mat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0];
            car->old_frame_mat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1];
            car->old_frame_mat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2];
            if (TestForCarInSensiblePlace(car)) {
                break;
            }
        }
        count++;
    } while (l == 10 && count < 3);
    car->oldmat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] * WORLD_SCALE;
    car->oldmat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] * WORLD_SCALE;
    car->oldmat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] * WORLD_SCALE;
    car->curvature = 0.0;
    for (j = 0; j < 4; ++j) {
        car->oldd[j] = car->ride_height;
    }
    car->revs = 0.0;
    car->gear = 0;
    car->auto_special_volume = 0;
    if (car->driver == eDriver_local_human) {
        InitialiseExternalCamera();
        PositionExternalCamera(car, 100u);
    }
}

// IDA: void __usercall GetPowerup(int pNum@<EAX>)
// FUNCTION: CARM95 0x004a291d
void GetPowerup(int pNum) {

    // FIXME: remove unittest variables from dethrace
    _unittest_controls_lastGetPowerup = pNum;

    GotPowerup(&gProgram_state.current_car, pNum);
}

// IDA: void __usercall CheckSystemKeys(int pRacing@<EAX>)
// FUNCTION: CARM95 0x004a293f
void CheckSystemKeys(int pRacing) {
    tU32 start_menu_time;
    int i;

    start_menu_time = PDGetTotalTime();
    CheckQuit();
    if (!gAction_replay_mode) {
        CheckLoadSave();
    }
    AddLostTime(PDGetTotalTime() - start_menu_time);
    CheckToggles(pRacing);
    if (pRacing & !gAction_replay_mode) {
        CheckOtherRacingKeys();
    }
}

// IDA: void __cdecl CheckKevKeys()
// FUNCTION: CARM95 0x004a3b39
void CheckKevKeys(void) {
    int i;
    tU32* value;
    char s[128];

    value = KevKeyService();
    if (value[0] == 0) {
        return;
    }

    for (i = 0; gKev_keys[i].action_proc != 0; i++) {
        if (gKev_keys[i].code == value[0] && gKev_keys[i].code2 == value[1]) {
            break;
        }
    }

    if (gKev_keys[i].action_proc) {
        if (gNet_mode) {
            if (gKev_keys[i].num == 0xA11EE75D) {
                strcpy(s, gNet_players[gThis_net_player_index].player_name);
                strcat(s, " ");
                strcat(s, GetMiscString(kMiscString_MANAGED_TO_CHEAT));
                NetSendHeadupToEverybody(s);
                gKev_keys[i].action_proc(gKev_keys[i].num);
            } else {
                strcpy(s, gNet_players[gThis_net_player_index].player_name);
                strcat(s, " ");
                strcat(s, GetMiscString(kMiscString_TRIED_TO_CHEAT));
                NetSendHeadupToAllPlayers(s);
            }
        } else {
            gKev_keys[i].action_proc(gKev_keys[i].num);
        }
    }
}

// IDA: void __cdecl BrakeInstantly()
// FUNCTION: CARM95 0x004a4159
void BrakeInstantly(void) {
    int i;

    gProgram_state.current_car.revs = 0.f;
    if (gProgram_state.current_car.number_of_wheels_on_ground != 0 && BrVector3LengthSquared(&gProgram_state.current_car.v) > 0.0001f) {
        PratcamEvent(kPratcam_instant_handbrake);
        for (i = 0; i < 5; i++) {
            DRS3StartSound(gCar_outlet, 9000 + i);
        }
    }
    BrVector3Set(&gProgram_state.current_car.v, 0.f, 0.f, 0.f);
}

// IDA: void __usercall PollCarControls(tU32 pTime_difference@<EAX>)
// FUNCTION: CARM95 0x004a3d80
void PollCarControls(tU32 pTime_difference) {
    int decay_steering;
    int decay_speed;
    float decay_rate;
    tS32 joyX;
    tS32 joyY;
    tCar_controls keys;
    tJoystick joystick;
    tCar_spec* c;

    c = &gProgram_state.current_car;

    memset(&keys, 0, sizeof(tCar_controls));
    joystick.left = -1;
    joystick.right = -1;
    joystick.acc = -1;
    joystick.dec = -1;
    if (gEntering_message) {
        memset(&c->keys, 0, sizeof(tCar_controls));
        c->joystick.left = -1;
        c->joystick.right = -1;
        c->joystick.acc = -1;
        c->joystick.dec = -1;
    } else {
        if (gKey_mapping[46] >= 115 || gKey_mapping[47] >= 115) {
            joystick.left = gJoy_array[gKey_mapping[46] - 115];
            joystick.right = gJoy_array[gKey_mapping[47] - 115];
            if (joystick.left < 0 && joystick.right < 0) {
                joystick.left = 0;
            }
        } else {
            if (KeyIsDown(46)) {
                keys.left = 1;
            }
            if (KeyIsDown(47)) {
                keys.right = 1;
            }
        }
        if (KeyIsDown(12)) {
            keys.holdw = 1;
        }
        if (KeyIsDown(53) || gRace_finished) {
            if (!gInstant_handbrake || gRace_finished) {
                keys.brake = 1;
            } else {
                BrakeInstantly();
            }
        }
        if (gKey_mapping[48] < 115) {
            if (KeyIsDown(48) && !gRace_finished && !c->knackered && !gWait_for_it) {
                keys.acc = 1;
            }
        } else {
            joystick.acc = gJoy_array[gKey_mapping[48] - 115];
            if (joystick.acc > 0xFFFF) {
                joystick.acc = 0xFFFF;
            }
        }
        if (gKey_mapping[49] < 115) {
            if (KeyIsDown(49) && !gRace_finished && !c->knackered && !gWait_for_it) {
                keys.dec = 1;
            }
        } else {
            joystick.dec = gJoy_array[gKey_mapping[49] - 115];
            if (joystick.dec > 0xFFFF) {
                joystick.dec = 0xFFFF;
            }
        }
        if (KeyIsDown(55) && c->gear >= 0) {
            keys.change_down = 1;
            c->just_changed_gear = 1;
            if (keys.acc || joystick.acc > 32000) {
                c->traction_control = 0;
            } else if (c->gear > 1 && !c->keys.change_down) {
                --c->gear;
            }
            if (gCountdown && !c->keys.change_down) {
                JumpTheStart();
            }
        }
        if (gCar_flying) {
            if (KeyIsDown(13)) {
                keys.up = 1;
            }
            if (KeyIsDown(11)) {
                keys.down = 1;
            }
        }
        if (KeyIsDown(58)) {
            if (!gEntering_message) {
                keys.horn = 1;
            }
        }
        c->keys = keys;
        c->joystick = joystick;
    }
}

// IDA: void __usercall PollCameraControls(tU32 pTime_difference@<EAX>)
// FUNCTION: CARM95 0x004a4214
void PollCameraControls(tU32 pTime_difference) {
    int flag;
    int left;
    int right;
    int swirl_mode;
    int up_and_down_mode;
    int going_up;
    // GLOBAL: CARM95 0x51cf54
    static int last_swirl_mode = 0;

    flag = 0;
    swirl_mode = gRace_finished && !gAction_replay_mode && (&gProgram_state.current_car == gCar_to_view || gCar_to_view->knackered);
    up_and_down_mode = swirl_mode && !gCamera_has_collided;
    going_up = gCamera_zoom > 1.0;
    if (last_swirl_mode != swirl_mode) {
        if (swirl_mode) {
            SaveCameraPosition(0);
        } else {
            RestoreCameraPosition(0);
        }
        last_swirl_mode = swirl_mode;
    }
    if (!gMap_mode && !gProgram_state.cockpit_on && (!gAction_replay_mode || gAction_replay_camera_mode <= eAction_replay_standard)) {
        if (KeyIsDown(31) || (up_and_down_mode && !going_up)) {
            gCamera_zoom = (double)pTime_difference * TIME_CONV_THING / (double)(2 * swirl_mode + 1) + gCamera_zoom;
            if (gCamera_zoom > 2.0f) {
                gCamera_zoom = 2.0f;
            }
            if (up_and_down_mode && gCamera_zoom > 1.0f) {
                gCamera_zoom = 1.0f;
            }
        }
        if (KeyIsDown(30) || (up_and_down_mode && going_up)) {
            gCamera_zoom = gCamera_zoom - (double)pTime_difference * TIME_CONV_THING / (double)(2 * swirl_mode + 1);
            if (gCamera_zoom < 0.1) {
                gCamera_zoom = 0.1;
                if (up_and_down_mode) {
                    if (gCamera_zoom < 1.0f) {
                        gCamera_zoom = 1.0f;
                    }
                }
            }
        }
        if (swirl_mode && gProgram_state.current_car.speedo_speed < 0.001449275362318841) {
            left = 1;
            right = 0;
        } else {
            left = KeyIsDown(32);
            right = KeyIsDown(33);
        }

        if ((gCamera_sign ? left : right)) {
            if (!gCamera_reset) {
                gCamera_yaw += BrDegreeToAngle(pTime_difference * 0.05f);
            }
            flag = 1;
        }
        if ((gCamera_sign ? right : left)) {
            if (!gCamera_reset) {
                gCamera_yaw -= BrDegreeToAngle(pTime_difference * 0.05f);
            }
            if (flag) {
                gCamera_yaw = 0;
                gCamera_reset = 1;
            }
        } else if (!flag) {
            gCamera_reset = 0;
        }
    }
}

// IDA: void __usercall SetFlag2(int i@<EAX>)
// FUNCTION: CARM95 0x004a45e8
void SetFlag2(int i) {

    gAllow_car_flying = 1;
    ToggleFlying();
    gAllow_car_flying = gCar_flying;
}

// IDA: void __cdecl ToggleFlying()
// FUNCTION: CARM95 0x004a460c
void ToggleFlying(void) {

    if (gAllow_car_flying && gNet_mode == eNet_mode_none) {
        gCar_flying = !gCar_flying;
        if (gCar_flying) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 500, -4, "We have lift off!!");
        } else {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 500, -4, "Back down to Earth");
        }
    } else {
        gCar_flying = 0;
    }
}

// IDA: void __cdecl ToggleInvulnerability()
// FUNCTION: CARM95 0x004a46a8
void ToggleInvulnerability(void) {

    gProgram_state.current_car.invulnerable = !gProgram_state.current_car.invulnerable;
    if (gProgram_state.current_car.invulnerable) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_Invulnerable));
    } else {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, "Vulnerability returns!");
    }
}

// IDA: void __cdecl MoreTime()
// FUNCTION: CARM95 0x004a4721
void MoreTime(void) {

    AwardTime(30);
}

// IDA: void __cdecl MuchMoreTime()
// FUNCTION: CARM95 0x004a4736
void MuchMoreTime(void) {

    AwardTime(300);
}

// IDA: void __cdecl ToggleTimerFreeze()
// FUNCTION: CARM95 0x004a474e
void ToggleTimerFreeze(void) {

    gFreeze_timer = !gFreeze_timer;
    if (gFreeze_timer) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_TimerFrozen));
    } else {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, "Timer thawed out");
    }
}

// IDA: void __cdecl EarnDosh()
// FUNCTION: CARM95 0x004a47c7
void EarnDosh(void) {

    EarnCredits(5000);
}

// IDA: void __cdecl LoseDosh()
// FUNCTION: CARM95 0x004a47df
void LoseDosh(void) {

    EarnCredits(-5000);
}

// IDA: void __cdecl ToggleMap()
// FUNCTION: CARM95 0x004a47f7
void ToggleMap(void) {
    // GLOBAL: CARM95 0x53d6a0
    static int old_indent;
    // GLOBAL: CARM95 0x53d634
    static int was_in_cockpit;

    if (gMap_mode == 0) {
        if (!gAction_replay_mode) {
            if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_foxy && gThis_net_player_index == gIt_or_fox) {
                NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_THE_FOX_CANNOT_DO_THAT));
            } else if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_tag && gThis_net_player_index != gIt_or_fox) {
                NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_ONLY_IT_CAN_DO_THAT));
            } else {
                old_indent = gRender_indent;
                gRender_indent = 0;
                was_in_cockpit = gProgram_state.cockpit_on;
                if (gProgram_state.cockpit_on) {
                    ToggleCockpit();
                }
                gMap_mode = PDGetTotalTime();
            }
        }
    } else {
        gMap_mode = 0;
        gRender_indent = old_indent;
        if (was_in_cockpit) {
            ToggleCockpit();
        }
    }
    AdjustRenderScreenSize();
}

// IDA: int __cdecl HornBlowing()
// FUNCTION: CARM95 0x004a492c
int HornBlowing(void) {

    return gProgram_state.current_car.keys.horn;
}

// IDA: void __cdecl ToggleArrow()
// FUNCTION: CARM95 0x004a4947
void ToggleArrow(void) {
    // GLOBAL: CARM95 0x53d630
    static br_actor* old_actor;

    return;

    if (gArrow_mode) {
        gProgram_state.current_car.car_model_actors[gProgram_state.current_car.principal_car_actor].actor = old_actor;
        BrActorRemove(gArrow_actor);
        BrActorAdd(gProgram_state.current_car.car_master_actor, old_actor);
        gArrow_mode = 0;
        if (gInfo_on) {
            ToggleInfo();
        }
    } else {
        old_actor = gProgram_state.current_car.car_model_actors[gProgram_state.current_car.principal_car_actor].actor;
        BrActorRemove(old_actor);
        BrActorAdd(gProgram_state.current_car.car_master_actor, gArrow_actor);
        gProgram_state.current_car.car_model_actors[gProgram_state.current_car.principal_car_actor].actor = gArrow_actor;
        gArrow_mode = 1;
        if (!gInfo_on) {
            ToggleInfo();
        }
    }
}

// IDA: int __cdecl GetRecoverVoucherCount()
// FUNCTION: CARM95 0x004a4a23
int GetRecoverVoucherCount(void) {

    return gRecovery_voucher_count;
}

// IDA: void __usercall AddVouchers(int pCount@<EAX>)
// FUNCTION: CARM95 0x004a4a38
void AddVouchers(int pCount) {

    gRecovery_voucher_count += pCount;
}

// IDA: void __cdecl ResetRecoveryVouchers()
// FUNCTION: CARM95 0x004a4a4c
void ResetRecoveryVouchers(void) {

    gRecovery_voucher_count = 0;
}

// IDA: void __cdecl CycleCarTexturingLevel()
// FUNCTION: CARM95 0x004a4a61
void CycleCarTexturingLevel(void) {
    tCar_texturing_level new_level;

    new_level = (GetCarTexturingLevel() + 1) % eCTL_count;
    SetCarTexturingLevel(new_level);
    switch (new_level) {
    case eCTL_none:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_NoCarTextures));
        break;
    case eCTL_transparent:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_TransparentCarTexturesOnly));
        break;
    case eCTL_full:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_FullCarTextures));
        break;
    case eCTL_count:
        break;
    }
}

// IDA: void __cdecl CycleWallTexturingLevel()
// FUNCTION: CARM95 0x004a4b28
void CycleWallTexturingLevel(void) {
    tWall_texturing_level new_level;

    new_level = (GetWallTexturingLevel() + 1) % eWTL_count;
    ReallySetWallTexturingLevel(new_level);
    SetWallTexturingLevel(new_level);
    switch (new_level) {
    case eWTL_none:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_NoWallTextures));
        break;
    case eWTL_linear:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_LinearWallTextures));
        break;
    case eWTL_full:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_BestWallTextures));
        break;
    case eWTL_count:
        break;
    }
}

// IDA: void __cdecl CycleRoadTexturingLevel()
// FUNCTION: CARM95 0x004a4bfb
void CycleRoadTexturingLevel(void) {
    tRoad_texturing_level new_level;

    new_level = (GetRoadTexturingLevel() + 1) % 3;
    ReallySetRoadTexturingLevel(new_level);
    SetRoadTexturingLevel(new_level);
    if (new_level == eRTL_none) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_NoRoadTextures));
    } else if (new_level == eRTL_full) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_RoadTextures));
    }
}

// IDA: void __cdecl CycleYonFactor()
// FUNCTION: CARM95 0x004a4ca1
void CycleYonFactor(void) {
    br_scalar new_factor;
    char factor_str[5];

    new_factor = GetYonFactor() / 2.f;
    if (new_factor < .1f) {
        new_factor = 1.f;
    }
    SetYonFactor(new_factor);
    if (new_factor > .75f) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_TrackAppearsVeryQuickly));
    } else if (new_factor > .375f) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_TrackAppearsQuiteQuickly));
    } else if (new_factor > .187f) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_TrackQppearsQuiteLate));
    } else {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_TrackAppearsVeryLate));
    }
}

// IDA: void __usercall SetSoundDetailLevel(int pLevel@<EAX>)
// FUNCTION: CARM95 0x004a4da4
void SetSoundDetailLevel(int pLevel) {

    gSound_detail_level = pLevel;
}

// IDA: void __usercall ReallySetSoundDetailLevel(int pLevel@<EAX>)
// FUNCTION: CARM95 0x004a4db7
void ReallySetSoundDetailLevel(int pLevel) {

    DRS3StopAllOutletSounds();
    DisposeSoundSources();
    gSound_detail_level = pLevel;
    InitSound();
    InitSoundSources();
}

// IDA: int __cdecl GetSoundDetailLevel()
// FUNCTION: CARM95 0x004a4dde
int GetSoundDetailLevel(void) {

    return gSound_detail_level;
}

// IDA: void __cdecl CycleSoundDetailLevel()
// FUNCTION: CARM95 0x004a4df3
void CycleSoundDetailLevel(void) {
    int new_level;

    new_level = (gSound_detail_level + 1) % 3;
    ReallySetSoundDetailLevel(new_level);
    SetSoundDetailLevel(new_level);
    switch (gSound_detail_level) {
    case 0:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_FewestSounds));
        break;
    case 1:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_PartialSound));
        break;
    case 2:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_AllSounds));
        break;
    }
}

// IDA: void __cdecl CycleCarSimplificationLevel()
// FUNCTION: CARM95 0x004a4ec8
void CycleCarSimplificationLevel(void) {
    char* src;
    char* dst;

    gCar_simplification_level = (gCar_simplification_level + 1) % 5;
    src = GetMiscString(kMiscString_CarSimplificationLevel_D);
    dst = BrMemAllocate(strlen(src), kMem_simp_level);
    sprintf(dst, src, gCar_simplification_level);
    NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, dst);
    BrMemFree(dst);
}

// IDA: void __cdecl ToggleAccessoryRendering()
// FUNCTION: CARM95 0x004a4f58
void ToggleAccessoryRendering(void) {
    int on;

    if (gNet_mode == eNet_mode_none) {
        on = !GetAccessoryRendering();
        SetAccessoryRendering(on);
        if (on) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_AccessoriesOn));
        } else {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_AccessoriesOff));
        }
    } else {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_NetGamesAlwaysAccessorized));
    }
}

// IDA: void __cdecl ToggleSmoke()
// FUNCTION: CARM95 0x004a500d
void ToggleSmoke(void) {
    int on;

    on = !GetSmokeOn();
    ReallySetSmokeOn(on);
    SetSmokeOn(on);
    if (on) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_SmokeOn));
    } else {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_SmokeOff));
    }
}

// IDA: void __usercall DrawSomeText2(tDR_font *pFont@<EAX>)
void DrawSomeText2(tDR_font* pFont) {
    int y;
    int i;
    char* txt[15] = {
        "Cops    Show all racers on map    Show peds on map",
        "Random pick-up generation    Pick-up respawn",
        "Open game    Closed game",
        "Grid start    Random start",
        "Random races    Sequential races",
        "Include opponents' cars in car choices",
        "Choose cars    manually    randomly    include Big APC",
        "Starting credits    0    2000    5000    10000    20000",
        "Driven to Destruction",
        "Car Crusher",
        "Carnage Accumulator",
        "Checkpoint Stampede",
        "Sudden Death",
        "Terminal Tag",
        "Fox 'n' Hounds"
    };

    ClearEntireScreen();
    y = 0;
    for (i = 0; i < 15; i++) {
        TransDRPixelmapText(gBack_screen, 0, y, pFont, txt[i], 320);
        y += pFont->height + 1;
    }

    PDScreenBufferSwap(0);
    PrintScreen();
}

// IDA: void __cdecl DrawSomeText()
// FUNCTION: CARM95 0x00485d80
void DrawSomeText(void) {
    DrawSomeText2(&gFonts[kFont_ORANGHED]);
    DrawSomeText2(&gFonts[kFont_BLUEHEAD]);
    DrawSomeText2(&gFonts[kFont_GREENHED]);
    DrawSomeText2(&gFonts[kFont_MEDIUMHD]);
    DrawSomeText2(&gFonts[kFont_NEWHITE]);
    DrawSomeText2(&gFonts[kFont_NEWRED]);
    DrawSomeText2(&gFonts[kFont_NEWBIGGR]);
}

// IDA: void __cdecl SaySorryYouLittleBastard()
void SaySorryYouLittleBastard(void) {
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl UserSendMessage()
// FUNCTION: CARM95 0x004a509e
void UserSendMessage(void) {

    if (gNet_mode != eNet_mode_none && gCurrent_net_game->options.enable_text_messages) {
        gEntering_message = 1;
    }
}

// IDA: void __cdecl EnterUserMessage()
// FUNCTION: CARM95 0x004a50cf
void EnterUserMessage(void) {
    // GLOBAL: CARM95 0x51cf58
    static int last_key;
    // GLOBAL: CARM95 0x51cf5c
    static int about_to_die;
    // GLOBAL: CARM95 0x53d69c
    static tU32 next_time;
    char* the_message;
    char* p;
    int len;
    int the_key;
    int abuse_num;

    if (!gEntering_message) {
        return;
    }
    if (gNet_mode == eNet_mode_none) {
        return;
    }
    if (!gCurrent_net_game->options.enable_text_messages) {
        return;
    }
    the_key = PDAnyKeyDown();
    if (gEntering_message == 1) {
        if (the_key != -1) {
            return;
        }
        gEntering_message = 2;
    }
    if (about_to_die) {
        if (the_key != -1) {
            return;
        }
        gEntering_message = 0;
        about_to_die = 0;
        return;
    }
    if (the_key == last_key) {
        if (next_time < PDGetTotalTime()) {
            next_time += 100;
        } else {
            the_key = -1;
        }
    } else {
        last_key = the_key;
        next_time = PDGetTotalTime() + 500;
    }
    switch (the_key) {
    case -1:
    case KEY_SHIFT_ANY:
        break;
    case KEY_CTRL_ANY:
    case KEY_CTRL_ANY_2:
    case KEY_TAB:
    case KEY_ESCAPE:
        about_to_die = 1;
        break;
    case KEY_BACKSPACE:
    case KEY_DELETE:
    case KEY_LEFT:
        len = strlen(&gString[20]);
        if (len > 0) {
            gString[20 + len - 1] = '\0';
        }
        break;
    case KEY_RETURN:
    case KEY_KP_ENTER:
        len = strlen(gNet_players[gThis_net_player_index].player_name);
        if (len <= 18) {
            the_message = gString + 18 - len;
            strcpy(the_message, gNet_players[gThis_net_player_index].player_name);
            the_message[len + 0] = ':';
            the_message[len + 1] = ' ';
            gString[COUNT_OF(gString) - 1] = '\0';
            NetSendHeadupToAllPlayers(the_message);
            gString[20] = '\0';
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_MESSAGE_SENT));
            about_to_die = 1;
        }
        break;
    default:
        if (gKey_mapping[KEYMAP_SEND_MESSAGE] == the_key) {
            about_to_die = 1;
        } else if (the_key <= KEY_KP_NUMLOCK || the_key >= KEY_SPACE) {
            len = strlen(&gString[20]);
            if (len < 64 - 1) {
                gString[20 + len] = PDGetASCIIFromKey(the_key);
                if (gString[20 + len] < gFonts[4].offset || gString[20 + len] >= gFonts[4].offset + gFonts[4].num_entries) {
                    gString[20 + len] = '\0';
                }
                gString[20 + len + 1] = '\0';
            }
        } else if (the_key < KEY_KP_0 || the_key > KEY_KP_9) {
            gEntering_message = 0;
        } else {
            if (the_key == KEY_KP_0) {
                abuse_num = 9;
            } else {
                abuse_num = the_key - KEY_KP_1;
            }
            if (gAbuse_text[abuse_num] != NULL) {
                strcpy(&gString[20], gAbuse_text[abuse_num]);
            }
        }
    }
}

// IDA: void __cdecl DisplayUserMessage()
// FUNCTION: CARM95 0x004a54a9
void DisplayUserMessage(void) {
    char* the_message;
    int len;
    tDR_font* font;

    font = &gFonts[FONT_NEWHITE];
    the_message = &gString[20];
    if (!gEntering_message || gNet_mode == eNet_mode_none) {
        return;
    }

    len = strlen(the_message);
    if (len < 63 && (PDGetTotalTime() & 512) != 0) {
        the_message[len] = '_';
        the_message[len + 1] = '\0';
    }
    DimRectangle(gBack_screen,
        15 * gBack_screen->width / 100,
        gCurrent_graf_data->net_message_enter_y - font->height,
        85 * gBack_screen->width / 100,
        gCurrent_graf_data->net_message_enter_y + 6 * font->height,
        1);

    TransDRPixelmapText(gBack_screen, 20 * gBack_screen->width / 100, gCurrent_graf_data->net_message_enter_y, font, GetMiscString(kMiscString_ENTER_MESSAGE), 100);
    OoerrIveGotTextInMeBoxMissus(
        FONT_NEWHITE,
        the_message,
        gBack_screen,
        20 * gBack_screen->width / 100,
        gCurrent_graf_data->net_message_enter_y + 2 * font->height,
        80 * gBack_screen->width / 100,
        gCurrent_graf_data->net_message_enter_y + 6 * font->height,
        0);
    the_message[len] = 0;
}

// IDA: void __cdecl InitAbuseomatic()
// FUNCTION: CARM95 0x004a567a
void InitAbuseomatic(void) {
    char path[256];
    char s[256];
    FILE* f;
    int i;
    int len;

    gString[20] = '\0';
    PDBuildAppPath(path);
    strcat(path, "ABUSE.TXT");
    for (i = 0; i < COUNT_OF(gAbuse_text); i++) {
        gAbuse_text[i] = NULL;
    }
    f = fopen(path, "rt");
    if (f == NULL) {
        return;
    }
    for (i = 0; i < COUNT_OF(gAbuse_text); i++) {
        if (fgets(s, COUNT_OF(s) - 1, f) == NULL) {
            break;
        }
        len = strlen(s);
        if (len > 63) {
            s[63] = '\0';
        }
        len = strlen(s);
        while (len != 0 && s[len - 1] < ' ') {
            s[len - 1] = '\0';
            len--;
        }
        gAbuse_text[i] = BrMemAllocate(strlen(s) + 1, kMem_abuse_text);
        strcpy(gAbuse_text[i], s);
    }
    fclose(f);
}

// IDA: void __cdecl DisposeAbuseomatic()
// FUNCTION: CARM95 0x004a5868
void DisposeAbuseomatic(void) {
    int i;

    for (i = 0; i < COUNT_OF(gAbuse_text); i++) {
        if (gAbuse_text[i] != NULL) {
            BrMemFree(gAbuse_text[i]);
        }
    }
}
