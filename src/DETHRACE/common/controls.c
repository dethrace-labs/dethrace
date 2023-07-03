#include "controls.h"

#include "brender/brender.h"
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
int gRepair_last_time;
int gHad_auto_recover;
tU32 gLast_repair_time;
tEdit_mode gWhich_edit_mode = eEdit_mode_options;
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

tCheat gKev_keys[44] = {
    { .code = 0xa11ee75d, .code2 = 0xf805eddd, .action_proc = SetFlag, .num = 0x0a11ee75d },
    { .code = 0x564e78b9, .code2 = 0x99155115, .action_proc = SetFlag, .num = 0x0564e78b9 },
    { .code = 0x1f47e5e8, .code2 = 0xa715222c, .action_proc = SetFlag2, .num = 1 },
    { .code = 0x39d4c4c4, .code2 = 0xf0a2c5b0, .action_proc = FinishLap, .num = 0 },
    { .code = 0x2654216c, .code2 = 0xf8256d15, .action_proc = GetPowerup, .num = 1 },
    { .code = 0x4294ec89, .code2 = 0xc38ad60e, .action_proc = GetPowerup, .num = 2 },
    { .code = 0x2ceb2850, .code2 = 0xa0c2d27e, .action_proc = GetPowerup, .num = 3 },
    { .code = 0x2d5f3125, .code2 = 0x9ce8a541, .action_proc = GetPowerup, .num = 4 },
    { .code = 0x2169c78b, .code2 = 0x7f3c9229, .action_proc = GetPowerup, .num = 5 },
    { .code = 0x2203c0cb, .code2 = 0x79729be4, .action_proc = GetPowerup, .num = 6 },
    { .code = 0x34f4e3ec, .code2 = 0x868c534d, .action_proc = GetPowerup, .num = 7 },
    { .code = 0x34010316, .code2 = 0x591d1eb2, .action_proc = GetPowerup, .num = 8 },
    { .code = 0x214fe3bc, .code2 = 0x87285111, .action_proc = GetPowerup, .num = 9 },
    { .code = 0x2fabc390, .code2 = 0x0c93d9f7, .action_proc = GetPowerup, .num = 10 },
    { .code = 0x2902e890, .code2 = 0x40969f67, .action_proc = GetPowerup, .num = 11 },
    { .code = 0x2f458288, .code2 = 0x058074e2, .action_proc = GetPowerup, .num = 12 },
    { .code = 0x249da152, .code2 = 0x8f287346, .action_proc = GetPowerup, .num = 13 },
    { .code = 0x23aae08b, .code2 = 0xa668103d, .action_proc = GetPowerup, .num = 14 },
    { .code = 0x32130661, .code2 = 0x56f03459, .action_proc = GetPowerup, .num = 15 },
    { .code = 0x2734e944, .code2 = 0xfe1e4639, .action_proc = GetPowerup, .num = 16 },
    { .code = 0x28341139, .code2 = 0x355f6d02, .action_proc = GetPowerup, .num = 17 },
    { .code = 0x20508831, .code2 = 0x123d1961, .action_proc = GetPowerup, .num = 18 },
    { .code = 0x346b8bcb, .code2 = 0x4aba696c, .action_proc = GetPowerup, .num = 19 },
    { .code = 0x3fc93df0, .code2 = 0x29fa9efb, .action_proc = GetPowerup, .num = 20 },
    { .code = 0x2a80b09b, .code2 = 0x058516f5, .action_proc = GetPowerup, .num = 21 },
    { .code = 0x2f548fd1, .code2 = 0x696744da, .action_proc = GetPowerup, .num = 22 },
    { .code = 0x3cb74f32, .code2 = 0xb915d88d, .action_proc = GetPowerup, .num = 23 },
    { .code = 0x297b53ba, .code2 = 0x218d4d2d, .action_proc = GetPowerup, .num = 24 },
    { .code = 0x351bc37d, .code2 = 0xb2a63343, .action_proc = GetPowerup, .num = 25 },
    { .code = 0x2b624386, .code2 = 0x9ba6260e, .action_proc = GetPowerup, .num = 26 },
    { .code = 0x2ba4ae23, .code2 = 0xc163a76c, .action_proc = GetPowerup, .num = 27 },
    { .code = 0x2fb92dca, .code2 = 0x4ad7d54e, .action_proc = GetPowerup, .num = 32 },
    { .code = 0x3a42191b, .code2 = 0xeff70f4c, .action_proc = GetPowerup, .num = 35 },
    { .code = 0x2aca3190, .code2 = 0xd9004f25, .action_proc = GetPowerup, .num = 36 },
    { .code = 0x37c1f613, .code2 = 0xb7faf351, .action_proc = GetPowerup, .num = 37 },
    { .code = 0x2ba3f603, .code2 = 0x29f2425c, .action_proc = GetPowerup, .num = 38 },
    { .code = 0x416eff61, .code2 = 0x2667df4b, .action_proc = GetPowerup, .num = 39 },
    { .code = 0x2554125c, .code2 = 0x393ca35d, .action_proc = GetPowerup, .num = 41 },
    { .code = 0x3fff84d5, .code2 = 0x84a42df4, .action_proc = GetPowerup, .num = 42 },
    { .code = 0x37e83018, .code2 = 0xb609aee6, .action_proc = GetPowerup, .num = 43 },
    { .code = 0x2db03b19, .code2 = 0x924a84b7, .action_proc = GetPowerup, .num = 44 },
    { .code = 0x30a19fab, .code2 = 0x2b0c2782, .action_proc = GetPowerup, .num = 45 },
    { .code = 0x0, .code2 = 0x0, .action_proc = 0x0, .num = 0x0 }
};
int gAllow_car_flying;
int gEntering_message;
tU32 gPalette_fade_time; // was gRecover_timer
char* gAbuse_text[10];
char gString[84];
int gToo_late;
int gRecover_timer;
int gRecovery_voucher_count;
int gInstant_handbrake;
int gAuto_repair;

int _unittest_controls_lastGetPowerup = 0;

// IDA: void __cdecl AbortRace()
void AbortRace(void) {
    LOG_TRACE("()");

    if (!gRace_finished) {
        gAbandon_game = 1;
    }
}

// IDA: void __cdecl F4Key()
void F4Key(void) {
    char s[256];
    tEdit_mode old_edit_mode;
    LOG_TRACE("()");

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
        NewTextHeadupSlot2(4, 0, 2000, -4, s, 0);
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
void SetFlag(int i) {
    LOG_TRACE("(%d)", i);

    if (gNet_mode == eNet_mode_none) {
        NewTextHeadupSlot(4, 0, 3000, -4, "You Cheat!");
    }
    gI_am_cheating = i;
    F4Key();
}

// IDA: void __usercall FinishLap(int i@<EAX>)
void FinishLap(int i) {
    LOG_TRACE("(%d)", i);

    IncrementLap();
}

// IDA: void __cdecl EnsureSpecialVolumesHidden()
void EnsureSpecialVolumesHidden(void) {
    LOG_TRACE("()");

    if (gWhich_edit_mode == eEdit_mode_spec_vol) {
        HideSpecialVolumes();
    }
}

// IDA: void __cdecl ShowSpecialVolumesIfRequ()
void ShowSpecialVolumesIfRequ(void) {
    LOG_TRACE("()");

    if (gWhich_edit_mode == eEdit_mode_spec_vol) {
        ShowSpecialVolumes();
    }
}

// IDA: void __usercall DoEditModeKey(int pIndex@<EAX>)
void DoEditModeKey(int pIndex) {
    int modifiers;
    LOG_TRACE("(%d)", pIndex);

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
void F5Key(void) {
    LOG_TRACE("()");

    DoEditModeKey(0);
}

// IDA: void __cdecl F6Key()
void F6Key(void) {
    LOG_TRACE("()");

    DoEditModeKey(1);
}

// IDA: void __cdecl F7Key()
void F7Key(void) {
    LOG_TRACE("()");

    DoEditModeKey(2);
}

// IDA: void __cdecl F8Key()
void F8Key(void) {
    LOG_TRACE("()");

    DoEditModeKey(3);
}

// IDA: void __cdecl F10Key()
void F10Key(void) {
    LOG_TRACE("()");

    DoEditModeKey(4);
}

// IDA: void __cdecl F11Key()
void F11Key(void) {
    LOG_TRACE("()");

    DoEditModeKey(5);
}

// IDA: void __cdecl F12Key()
void F12Key(void) {
    LOG_TRACE("()");

    DoEditModeKey(6);
}

// IDA: void __cdecl NumberKey0()
void NumberKey0(void) {
    LOG_TRACE("()");

    DoEditModeKey(7);
}

// IDA: void __cdecl NumberKey1()
void NumberKey1(void) {
    LOG_TRACE("()");

    DoEditModeKey(8);
}

// IDA: void __cdecl NumberKey2()
void NumberKey2(void) {
    LOG_TRACE("()");

    DoEditModeKey(9);
}

// IDA: void __cdecl NumberKey3()
void NumberKey3(void) {
    LOG_TRACE("()");

    DoEditModeKey(10);
}

// IDA: void __cdecl NumberKey4()
void NumberKey4(void) {
    LOG_TRACE("()");

    DoEditModeKey(11);
}

// IDA: void __cdecl NumberKey5()
void NumberKey5(void) {
    LOG_TRACE("()");

    DoEditModeKey(12);
}

// IDA: void __cdecl NumberKey6()
void NumberKey6(void) {
    LOG_TRACE("()");

    DoEditModeKey(13);
}

// IDA: void __cdecl NumberKey7()
void NumberKey7(void) {
    LOG_TRACE("()");

    DoEditModeKey(14);
}

// IDA: void __cdecl NumberKey8()
void NumberKey8(void) {
    LOG_TRACE("()");

    DoEditModeKey(15);
}

// IDA: void __cdecl NumberKey9()
void NumberKey9(void) {
    LOG_TRACE("()");

    DoEditModeKey(16);
}

// IDA: void __cdecl LookLeft()
void LookLeft(void) {
    LOG_TRACE("()");

    if (gAusterity_mode) {
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(192));
    } else {
        PratcamEvent(27);
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
void LookForward(void) {
    LOG_TRACE("()");

    if (gProgram_state.which_view == eView_right) {
        PratcamEvent(27);
    } else if (gProgram_state.which_view == eView_left) {
        PratcamEvent(28);
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
void LookRight(void) {
    LOG_TRACE("()");

    if (gAusterity_mode) {
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(192));
    } else {
        PratcamEvent(28);
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
void DamageTest(void) {
    LOG_TRACE("()");
}

// IDA: void __cdecl TDamageEngine()
void TDamageEngine(void) {
    LOG_TRACE("()");

    DamageEngine(21);
}

// IDA: void __cdecl TDamageDriver()
void TDamageDriver(void) {
    LOG_TRACE("()");

    if (gProgram_state.current_car.damage_units[eDamage_driver].damage_level >= 80) {
        DamageUnit(&gProgram_state.current_car, eDamage_driver, 2);
    } else {
        DamageUnit(&gProgram_state.current_car, eDamage_driver, 80 - gProgram_state.current_car.damage_units[2].damage_level);
    }
}

// IDA: void __cdecl TDamageTrans()
void TDamageTrans(void) {
    LOG_TRACE("()");

    DamageTrans(21);
}

// IDA: void __cdecl TDamageSteering()
void TDamageSteering(void) {
    LOG_TRACE("()");

    DamageSteering(21);
}

// IDA: void __cdecl TDamageLFWheel()
void TDamageLFWheel(void) {
    LOG_TRACE("()");

    DamageLFWheel(21);
}

// IDA: void __cdecl TDamageLFBrake()
void TDamageLFBrake(void) {
    LOG_TRACE("()");

    DamageLFBrake(21);
}

// IDA: void __cdecl TDamageLRBrake()
void TDamageLRBrake(void) {
    LOG_TRACE("()");

    DamageLRBrake(21);
}

// IDA: void __cdecl TDamageLRWheel()
void TDamageLRWheel(void) {
    LOG_TRACE("()");

    DamageLRWheel(21);
}

// IDA: void __cdecl TDamageRFWheel()
void TDamageRFWheel(void) {
    LOG_TRACE("()");

    DamageRFWheel(21);
}

// IDA: void __cdecl TDamageRFBrake()
void TDamageRFBrake(void) {
    LOG_TRACE("()");

    DamageRFBrake(21);
}

// IDA: void __cdecl TDamageRRBrake()
void TDamageRRBrake(void) {
    LOG_TRACE("()");

    DamageRRBrake(21);
}

// IDA: void __cdecl TDamageRRWheel()
void TDamageRRWheel(void) {
    LOG_TRACE("()");

    DamageRRWheel(21);
}

// IDA: void __cdecl MoveBonnetForward()
void MoveBonnetForward(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.translate.t.v[2] -= .005f;
}

// IDA: void __cdecl SaveBonnet()
void SaveBonnet(void) {
    br_actor* bonny;
    tPath_name the_path;
    LOG_TRACE("()");

    bonny = gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor;
    PathCat(the_path, gApplication_path, bonny->identifier);
    BrActorSave(the_path, bonny);
}

// IDA: void __cdecl MoveBonnetBackward()
void MoveBonnetBackward(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.translate.t.v[2] += .005f;
}

// IDA: void __cdecl MoveBonnetLeft()
void MoveBonnetLeft(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.translate.t.v[0] -= .005f;
}

// IDA: void __cdecl ShrinkBonnetX()
void ShrinkBonnetX(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat.m[0][0] *= .98f;
}

// IDA: void __cdecl SwellBonnetX()
void SwellBonnetX(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat.m[0][0] *= 1.02f;
}

// IDA: void __cdecl ShrinkBonnetY()
void ShrinkBonnetY(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat.m[1][1] *= .98f;
}

// IDA: void __cdecl SwellBonnetY()
void SwellBonnetY(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat.m[1][1] *= 1.02f;
}

// IDA: void __cdecl ShrinkBonnetZ()
void ShrinkBonnetZ(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat.m[2][2] *= .98f;
}

// IDA: void __cdecl SwellBonnetZ()
void SwellBonnetZ(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat.m[2][2] *= 1.02f;
}

// IDA: void __cdecl MoveBonnetDown()
void MoveBonnetDown(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.translate.t.v[1] += .005f;
}

// IDA: void __cdecl MoveBonnetRight()
void MoveBonnetRight(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.translate.t.v[0] += .005f;
}

// IDA: void __cdecl MoveBonnetUp()
void MoveBonnetUp(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.translate.t.v[1] -= .005f;
}

// IDA: void __cdecl TiltBonnetDownX()
void TiltBonnetDownX(void) {
    LOG_TRACE("()");

    BrMatrix34PreRotateX(&gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat, BR_ANGLE_DEG(.5f));
}

// IDA: void __cdecl TiltBonnetUpX()
void TiltBonnetUpX(void) {
    LOG_TRACE("()");

    BrMatrix34PreRotateX(&gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat, -BR_ANGLE_DEG(.5f));
}

// IDA: void __cdecl TiltBonnetDownY()
void TiltBonnetDownY(void) {
    LOG_TRACE("()");

    BrMatrix34PreRotateY(&gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat, BR_ANGLE_DEG(.5f));
}

// IDA: void __cdecl TiltBonnetUpY()
void TiltBonnetUpY(void) {
    LOG_TRACE("()");

    BrMatrix34PreRotateY(&gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat, -BR_ANGLE_DEG(.5f));
}

// IDA: void __cdecl TiltBonnetDownZ()
void TiltBonnetDownZ(void) {
    LOG_TRACE("()");

    BrMatrix34PreRotateZ(&gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat, BR_ANGLE_DEG(.5f));
}

// IDA: void __cdecl TiltBonnetUpZ()
void TiltBonnetUpZ(void) {
    LOG_TRACE("()");

    BrMatrix34PreRotateZ(&gProgram_state.current_car.car_model_actors[gProgram_state.current_car.car_actor_count - 1].actor->t.t.mat, -BR_ANGLE_DEG(.5f));
}

// IDA: void __cdecl ToggleCockpit()
void ToggleCockpit(void) {
    br_scalar ts;
    LOG_TRACE("()");

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
            NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(192));
        }
    }
}

// IDA: void __cdecl ToggleMirror()
void ToggleMirror(void) {
    LOG_TRACE("()");

    gProgram_state.mirror_on = !gProgram_state.mirror_on;
    ReinitialiseRearviewCamera();
    if (gProgram_state.mirror_on) {
        NewTextHeadupSlot(4, 0, 500, -4, GetMiscString(2));
    } else {
        NewTextHeadupSlot(4, 0, 500, -4, GetMiscString(3));
    }
}

// IDA: void __cdecl ConcussMe()
// dethrace: this is not referenced in the retail executables. Left over debug code.
void ConcussMe(void) {
    LOG_TRACE("()");

    SufferFromConcussion(1.f);
    NewScreenWobble(IRandomPosNeg(15), IRandomPosNeg(10), IRandomBetween(10, 60));
    PratcamEvent(3);
}

// IDA: void __cdecl CheckHelp()
void CheckHelp(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckLoadSave()
void CheckLoadSave(void) {
    int save_load_allowed;
    int switched_res;
    LOG_TRACE8("()");

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
int CarWorldOffFallenCheckThingy(tCar_spec* pCar, int pCheck_around) {
    br_vector3 car_pos;
    br_vector3 offset_c;
    br_vector3 offset_w;
    int result;
    LOG_TRACE("(%p, %d)", pCar, pCheck_around);

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
int HasCarFallenOffWorld(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);

    return CarWorldOffFallenCheckThingy(pCar, 1);
}

// IDA: void __cdecl CheckForBeingOutOfThisWorld()
void CheckForBeingOutOfThisWorld(void) {
    static tU32 the_time;
    static tU32 sLast_check;
    int time_step;
    LOG_TRACE("()");

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
void CheckHornLocal(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);

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
void CheckHorn3D(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);
    STUB_ONCE();
}

// IDA: void __cdecl CheckHorns()
void CheckHorns(void) {
    int i;
    LOG_TRACE("()");

    if (gNet_mode != eNet_mode_none) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            CheckHorn3D(gNet_players[i].car);
        }
    } else {
        CheckHornLocal(&gProgram_state.current_car);
    }
}

// IDA: void __cdecl SetRecovery()
void SetRecovery(void) {
    LOG_TRACE("()");

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
            NewTextHeadupSlot2(4, 0, 2000, -4, GetMiscString(242), 1);
            gToo_late = 1;
        } else {
            gProgram_state.current_car.time_to_recover = 0;
            NewTextHeadupSlot2(4, 0, 2000, -4, GetMiscString(125), 0);
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
void RecoverCar(void) {
    LOG_TRACE("()");

    if (gNet_mode == eNet_mode_none || !gPalette_fade_time) {
        gRecover_car = 1;
    }
    gProgram_state.current_car.time_to_recover = 0;
}

// IDA: void __cdecl CheckMapRenderMove()
void CheckMapRenderMove(void) {
    int shift_down;
    int amount;
    float old_x;
    float old_y;
    LOG_TRACE("()");

    old_y = gMap_render_y;
    old_x = gMap_render_x;
    if (gMap_mode) {
        amount = gFrame_period * .1f;
        if (KeyIsDown(30)) {
            gMap_render_y -= amount;
        } else if (KeyIsDown(31)) {
            gMap_render_y += amount;
        }
        if (KeyIsDown(32)) {
            gMap_render_x -= amount;
        } else if (KeyIsDown(33)) {
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
void ExplodeCar(tCar_spec* pCar) {
    br_vector3 tv;
    br_vector3 pos;
    LOG_TRACE("(%p)", pCar);

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
void CheckRecoveryOfCars(tU32 pEndFrameTime) {
    int i;
    int time;
    char s[256];
    LOG_TRACE("(%d)", pEndFrameTime);

    if (gProgram_state.current_car.time_to_recover) {
        if (gProgram_state.current_car.knackered) {
            gProgram_state.current_car.time_to_recover = 0;
        } else {
            time = (gProgram_state.current_car.time_to_recover - pEndFrameTime + 1000) / 1000;
            sprintf(s, "%s %d %s", GetMiscString(97), time, time > 1 ? GetMiscString(99) : GetMiscString(98));
            if (!gToo_late) {
                NewTextHeadupSlot2(4, 0, 2000, -4, s, 0);
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
void LoseSomePSPowerups(int pNumber) {
    int index;
    LOG_TRACE("(%d)", pNumber);

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
    static tU32 total_repair_cost;
    static tS3_sound_tag sound_tag;
    static br_scalar amount;
    static int NeedToExpandBoundingBox;
    static int total_difference;
    static int stopped_repairing;
    LOG_TRACE("()");

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
                        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(4));
                    } else {
                        sprintf(s, "%s %d", GetMiscString(5), total_repair_cost);
                        NewTextHeadupSlot(4, 0, 1000, -4, s);
                    }
                }
            } else {
                if (!stopped_repairing) {
                    NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(95));
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
            NewTextHeadupSlot(4, 0, 1500, -4, " ");
        }
        if (gRecovery_voucher_count != 0) {
            gRecovery_voucher_count--;
            sprintf(s, "%s", GetMiscString(48));
            NewTextHeadupSlot(4, 0, 1500, -4, s);
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
            sprintf(s, "%s %d", GetMiscString(7), cost);
            NewTextHeadupSlot(4, 0, 1500, -4, s);
            LoseSomePSPowerups(2);
        }
        CancelPendingCunningStunt();
        PipeSingleSpecial(ePipe_special_fade);
    }
    gRecover_car = 0;
}

// IDA: int __cdecl CheckRecoverCost()
int CheckRecoverCost(void) {
    LOG_TRACE("()");

    if (gProgram_state.current_car.knackered
        || gNet_mode == eNet_mode_none
        || (gProgram_state.credits_earned - gProgram_state.credits_lost) >= gNet_recovery_cost[gCurrent_net_game->type]
        || gRecovery_voucher_count) {
        return 1;
    }
    gProgram_state.credits_earned = 0;
    gProgram_state.credits_lost = 0;
    NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(96));
    DoFancyHeadup(kFancyHeadupNetworkRaceNoMoreMoney);
    KnackerThisCar(&gProgram_state.current_car);
    SendGameplayToHost(eNet_gameplay_suicide, 0, 0, 0, 0);
    return 0;
}

// IDA: void __usercall SortOutRecover(tCar_spec *pCar@<EAX>)
void SortOutRecover(tCar_spec* pCar) {
    int the_time;
    int val;
    static int old_time;
    LOG_TRACE("(%p)", pCar);

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
void SetFlipUpCar(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);

    if (gNet_mode != eNet_mode_none && pCar->driver == eDriver_local_human) {
        DisableCar(pCar);
        gPalette_fade_time = GetRaceTime();
        NetPlayerStatusChanged(ePlayer_status_recovering);
    } else {
        FlipUpCar(pCar);
    }
}

// IDA: void __usercall FlipUpCar(tCar_spec *car@<EAX>)
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
    LOG_TRACE("(%p)", car);

    count = 0;
    if (car->driver == eDriver_local_human && gNet_mode == eNet_mode_none) {
        FadePaletteDown();
        while (KeyIsDown(44)) {
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
void GetPowerup(int pNum) {
    LOG_TRACE("()");

    // FIXME: remove unittest variables from dethrace
    _unittest_controls_lastGetPowerup = pNum;

    GotPowerup(&gProgram_state.current_car, pNum);
}

// IDA: void __usercall CheckSystemKeys(int pRacing@<EAX>)
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
                strcat(s, GetMiscString(225));
                NetSendHeadupToEverybody(s);
                gKev_keys[i].action_proc(gKev_keys[i].num);
            } else {
                strcpy(s, gNet_players[gThis_net_player_index].player_name);
                strcat(s, " ");
                strcat(s, GetMiscString(224));
                NetSendHeadupToAllPlayers(s);
            }
        } else {
            gKev_keys[i].action_proc(gKev_keys[i].num);
        }
    }
}

// IDA: void __cdecl BrakeInstantly()
void BrakeInstantly(void) {
    int i;
    LOG_TRACE("()");

    gProgram_state.current_car.revs = 0.f;
    if (gProgram_state.current_car.number_of_wheels_on_ground != 0 && BrVector3LengthSquared(&gProgram_state.current_car.v) > 0.0001f) {
        PratcamEvent(41);
        for (i = 0; i < 5; i++) {
            DRS3StartSound(gCar_outlet, 9000 + i);
        }
    }
    BrVector3Set(&gProgram_state.current_car.v, 0.f, 0.f, 0.f);
}

// IDA: void __usercall PollCarControls(tU32 pTime_difference@<EAX>)
void PollCarControls(tU32 pTime_difference) {
    int decay_steering;
    int decay_speed;
    float decay_rate;
    tS32 joyX;
    tS32 joyY;
    tCar_controls keys;
    tJoystick joystick;
    tCar_spec* c;
    LOG_TRACE("(%d)", pTime_difference);

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
void PollCameraControls(tU32 pTime_difference) {
    int flag;
    int left;
    int right;
    int swirl_mode;
    int up_and_down_mode;
    int going_up;
    static int last_swirl_mode = 0;
    LOG_TRACE("(%d)", pTime_difference);

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
void SetFlag2(int i) {
    LOG_TRACE("(%d)", i);

    gAllow_car_flying = 1;
    ToggleFlying();
    gAllow_car_flying = gCar_flying;
}

// IDA: void __cdecl ToggleFlying()
void ToggleFlying(void) {
    LOG_TRACE("()");

    if (gAllow_car_flying && gNet_mode == eNet_mode_none) {
        gCar_flying = !gCar_flying;
        if (gCar_flying) {
            NewTextHeadupSlot(4, 0, 500, -4, "We have lift off!!");
        } else {
            NewTextHeadupSlot(4, 0, 500, -4, "Back down to Earth");
        }
    } else {
        gCar_flying = 0;
    }
}

// IDA: void __cdecl ToggleInvulnerability()
void ToggleInvulnerability(void) {
    LOG_TRACE("()");

    gProgram_state.current_car.invulnerable = !gProgram_state.current_car.invulnerable;
    if (gProgram_state.current_car.invulnerable) {
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(0));
    } else {
        NewTextHeadupSlot(4, 0, 1000, -4, "Vulnerability returns!");
    }
}

// IDA: void __cdecl MoreTime()
void MoreTime(void) {
    LOG_TRACE("()");

    AwardTime(30);
}

// IDA: void __cdecl MuchMoreTime()
void MuchMoreTime(void) {
    LOG_TRACE("()");

    AwardTime(300);
}

// IDA: void __cdecl ToggleTimerFreeze()
void ToggleTimerFreeze(void) {
    LOG_TRACE("()");

    gFreeze_timer = !gFreeze_timer;
    if (gFreeze_timer) {
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(1));
    } else {
        NewTextHeadupSlot(4, 0, 1000, -4, "Timer thawed out");
    }
}

// IDA: void __cdecl EarnDosh()
void EarnDosh(void) {
    LOG_TRACE("()");

    EarnCredits(5000);
}

// IDA: void __cdecl LoseDosh()
void LoseDosh(void) {
    LOG_TRACE("()");

    EarnCredits(-5000);
}

// IDA: void __cdecl ToggleMap()
void ToggleMap(void) {
    static int old_indent;
    static int was_in_cockpit;
    LOG_TRACE("()");

    if (gMap_mode == 0) {
        if (!gAction_replay_mode) {
            if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_foxy && gThis_net_player_index == gIt_or_fox) {
                NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(214));
            } else if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_tag && gThis_net_player_index != gIt_or_fox) {
                NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(215));
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
int HornBlowing(void) {
    LOG_TRACE("()");

    return gProgram_state.current_car.keys.horn;
}

// IDA: void __cdecl ToggleArrow()
void ToggleArrow(void) {
    static br_actor* old_actor;
    LOG_TRACE("()");

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
int GetRecoverVoucherCount(void) {
    LOG_TRACE("()");

    return gRecovery_voucher_count;
}

// IDA: void __usercall AddVouchers(int pCount@<EAX>)
void AddVouchers(int pCount) {
    LOG_TRACE("(%d)", pCount);

    gRecovery_voucher_count += pCount;
}

// IDA: void __cdecl ResetRecoveryVouchers()
void ResetRecoveryVouchers(void) {
    LOG_TRACE("()");

    gRecovery_voucher_count = 0;
}

// IDA: void __cdecl CycleCarTexturingLevel()
void CycleCarTexturingLevel(void) {
    tCar_texturing_level new_level;
    LOG_TRACE("()");

    new_level = (GetCarTexturingLevel() + 1) % eCTL_count;
    SetCarTexturingLevel(new_level);
    switch (new_level) {
    case eCTL_none:
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(50));
        break;
    case eCTL_transparent:
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(51));
        break;
    case eCTL_full:
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(52));
        break;
    case eCTL_count:
        break;
    }
}

// IDA: void __cdecl CycleWallTexturingLevel()
void CycleWallTexturingLevel(void) {
    tWall_texturing_level new_level;
    LOG_TRACE("()");

    new_level = (GetWallTexturingLevel() + 1) % eWTL_count;
    ReallySetWallTexturingLevel(new_level);
    SetWallTexturingLevel(new_level);
    switch (new_level) {
    case eWTL_none:
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(55));
        break;
    case eWTL_linear:
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(56));
        break;
    case eWTL_full:
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(57));
        break;
    case eWTL_count:
        break;
    }
}

// IDA: void __cdecl CycleRoadTexturingLevel()
void CycleRoadTexturingLevel(void) {
    tRoad_texturing_level new_level;
    LOG_TRACE("()");

    new_level = (GetRoadTexturingLevel() + 1) % 3;
    ReallySetRoadTexturingLevel(new_level);
    SetRoadTexturingLevel(new_level);
    if (new_level == eRTL_none) {
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(53));
    } else if (new_level == eRTL_full) {
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(54));
    }
}

// IDA: void __cdecl CycleYonFactor()
void CycleYonFactor(void) {
    br_scalar new_factor;
    char factor_str[5];
    LOG_TRACE("()");

    new_factor = GetYonFactor() / 2.f;
    if (new_factor < .1f) {
        new_factor = 1.f;
    }
    SetYonFactor(new_factor);
    if (new_factor > .75f) {
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(100));
    } else if (new_factor > .375f) {
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(101));
    } else if (new_factor > .187f) {
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(102));
    } else {
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(103));
    }
}

// IDA: void __usercall SetSoundDetailLevel(int pLevel@<EAX>)
void SetSoundDetailLevel(int pLevel) {
    LOG_TRACE("(%d)", pLevel);

    gSound_detail_level = pLevel;
}

// IDA: void __usercall ReallySetSoundDetailLevel(int pLevel@<EAX>)
void ReallySetSoundDetailLevel(int pLevel) {
    LOG_TRACE("(%d)", pLevel);

    DRS3StopAllOutletSounds();
    DisposeSoundSources();
    gSound_detail_level = pLevel;
    InitSound();
    InitSoundSources();
}

// IDA: int __cdecl GetSoundDetailLevel()
int GetSoundDetailLevel(void) {
    LOG_TRACE("()");

    return gSound_detail_level;
}

// IDA: void __cdecl CycleSoundDetailLevel()
void CycleSoundDetailLevel(void) {
    int new_level;
    LOG_TRACE("()");

    new_level = (gSound_detail_level + 1) % 3;
    ReallySetSoundDetailLevel(new_level);
    SetSoundDetailLevel(new_level);
    switch (new_level) {
    case 0:
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(116));
        break;
    case 1:
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(117));
        break;
    case 2:
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(118));
        break;
    }
}

// IDA: void __cdecl CycleCarSimplificationLevel()
void CycleCarSimplificationLevel(void) {
    char* src;
    char* dst;
    LOG_TRACE("()");

    gCar_simplification_level = (gCar_simplification_level + 1) % 5;
    src = GetMiscString(119);
    dst = BrMemAllocate(strlen(src), kMem_simp_level);
    sprintf(dst, src, gCar_simplification_level);
    NewTextHeadupSlot(4, 0, 2000, -4, dst);
    BrMemFree(dst);
}

// IDA: void __cdecl ToggleAccessoryRendering()
void ToggleAccessoryRendering(void) {
    int on;
    LOG_TRACE("()");

    if (gNet_mode == eNet_mode_none) {
        on = !GetAccessoryRendering();
        SetAccessoryRendering(on);
        if (on) {
            NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(120));
        } else {
            NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(121));
        }
    } else {
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(124));
    }
}

// IDA: void __cdecl ToggleSmoke()
void ToggleSmoke(void) {
    int on;
    LOG_TRACE("()");

    on = !GetSmokeOn();
    ReallySetSmokeOn(on);
    SetSmokeOn(on);
    if (on) {
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(122));
    } else {
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(123));
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
void DrawSomeText(void) {
    DrawSomeText2(&gFonts[1]);
    DrawSomeText2(&gFonts[2]);
    DrawSomeText2(&gFonts[3]);
    DrawSomeText2(&gFonts[4]);
    DrawSomeText2(&gFonts[6]);
    DrawSomeText2(&gFonts[7]);
    DrawSomeText2(&gFonts[8]);
}

// IDA: void __cdecl SaySorryYouLittleBastard()
void SaySorryYouLittleBastard(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl UserSendMessage()
void UserSendMessage(void) {
    LOG_TRACE("()");

    if (gNet_mode != eNet_mode_none && gCurrent_net_game->options.enable_text_messages) {
        gEntering_message = 1;
    }
}

// IDA: void __cdecl EnterUserMessage()
void EnterUserMessage(void) {
    static int last_key;
    static int about_to_die;
    static tU32 next_time;
    char* the_message;
    char* p;
    int len;
    int the_key;
    int abuse_num;
    LOG_TRACE("()");
    STUB_ONCE();
}

// IDA: void __cdecl DisplayUserMessage()
void DisplayUserMessage(void) {
    char* the_message;
    int len;
    tDR_font* font;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitAbuseomatic()
void InitAbuseomatic(void) {
    char path[256];
    char s[256];
    FILE* f;
    int i;
    int len;
    LOG_TRACE("()");

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
void DisposeAbuseomatic(void) {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gAbuse_text); i++) {
        if (gAbuse_text[i] != NULL) {
            BrMemFree(gAbuse_text[i]);
        }
    }
}
