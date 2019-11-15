#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 53
void AbortRace();

// Offset: 56
// Size: 271
void F4Key();

// Offset: 328
// Size: 87
// EAX: i
void SetFlag(int i);

// Offset: 416
// Size: 42
// EAX: i
void FinishLap(int i);

// Offset: 460
// Size: 48
void EnsureSpecialVolumesHidden();

// Offset: 508
// Size: 48
void ShowSpecialVolumesIfRequ();

// Offset: 556
// Size: 261
// EAX: pIndex
void DoEditModeKey(int pIndex);

// Offset: 820
// Size: 41
void F5Key();

// Offset: 864
// Size: 44
void F6Key();

// Offset: 908
// Size: 44
void F7Key();

// Offset: 952
// Size: 44
void F8Key();

// Offset: 996
// Size: 44
void F10Key();

// Offset: 1040
// Size: 44
void F11Key();

// Offset: 1084
// Size: 44
void F12Key();

// Offset: 1128
// Size: 44
void NumberKey0();

// Offset: 1172
// Size: 44
void NumberKey1();

// Offset: 1216
// Size: 44
void NumberKey2();

// Offset: 1260
// Size: 44
void NumberKey3();

// Offset: 1304
// Size: 44
void NumberKey4();

// Offset: 1348
// Size: 44
void NumberKey5();

// Offset: 1392
// Size: 44
void NumberKey6();

// Offset: 1436
// Size: 44
void NumberKey7();

// Offset: 1480
// Size: 44
void NumberKey8();

// Offset: 1524
// Size: 44
void NumberKey9();

// Offset: 1568
// Size: 175
void LookLeft();

// Offset: 1744
// Size: 128
void LookForward();

// Offset: 1872
// Size: 175
void LookRight();

// Offset: 2048
// Size: 34
void DamageTest();

// Offset: 2084
// Size: 44
void TDamageEngine();

// Offset: 2128
// Size: 91
void TDamageDriver();

// Offset: 2220
// Size: 44
void TDamageTrans();

// Offset: 2264
// Size: 44
void TDamageSteering();

// Offset: 2308
// Size: 44
void TDamageLFWheel();

// Offset: 2352
// Size: 44
void TDamageLFBrake();

// Offset: 2396
// Size: 44
void TDamageLRBrake();

// Offset: 2440
// Size: 44
void TDamageLRWheel();

// Offset: 2484
// Size: 44
void TDamageRFWheel();

// Offset: 2528
// Size: 44
void TDamageRFBrake();

// Offset: 2572
// Size: 44
void TDamageRRBrake();

// Offset: 2616
// Size: 44
void TDamageRRWheel();

// Offset: 2660
// Size: 76
void MoveBonnetForward();

// Offset: 2736
// Size: 93
void SaveBonnet();

// Offset: 2832
// Size: 73
void MoveBonnetBackward();

// Offset: 2908
// Size: 76
void MoveBonnetLeft();

// Offset: 2984
// Size: 73
void ShrinkBonnetX();

// Offset: 3060
// Size: 73
void SwellBonnetX();

// Offset: 3136
// Size: 73
void ShrinkBonnetY();

// Offset: 3212
// Size: 73
void SwellBonnetY();

// Offset: 3288
// Size: 73
void ShrinkBonnetZ();

// Offset: 3364
// Size: 73
void SwellBonnetZ();

// Offset: 3440
// Size: 73
void MoveBonnetDown();

// Offset: 3516
// Size: 73
void MoveBonnetRight();

// Offset: 3592
// Size: 76
void MoveBonnetUp();

// Offset: 3668
// Size: 67
void TiltBonnetDownX();

// Offset: 3736
// Size: 67
void TiltBonnetUpX();

// Offset: 3804
// Size: 67
void TiltBonnetDownY();

// Offset: 3872
// Size: 67
void TiltBonnetUpY();

// Offset: 3940
// Size: 67
void TiltBonnetDownZ();

// Offset: 4008
// Size: 67
void TiltBonnetUpZ();

// Offset: 4076
// Size: 246
void ToggleCockpit();

// Offset: 4324
// Size: 149
void ToggleMirror();

// Offset: 4476
// Size: 130
void ConcussMe();

// Offset: 4608
// Size: 34
void CheckHelp();

// Offset: 4644
// Size: 313
void CheckLoadSave();

// Offset: 4960
// Size: 364
// EAX: pRacing
void CheckToggles(int pRacing);

// Offset: 5324
// Size: 444
// EAX: pCar
// EDX: pCheck_around
int CarWorldOffFallenCheckThingy(tCar_spec *pCar, int pCheck_around);

// Offset: 5768
// Size: 56
// EAX: pCar
int HasCarFallenOffWorld(tCar_spec *pCar);

// Offset: 5824
// Size: 250
void CheckForBeingOutOfThisWorld();

// Offset: 6076
// Size: 190
// EAX: pCar
void CheckHornLocal(tCar_spec *pCar);

// Offset: 6268
// Size: 249
// EAX: pCar
void CheckHorn3D(tCar_spec *pCar);

// Offset: 6520
// Size: 103
void CheckHorns();

// Offset: 6624
// Size: 413
void SetRecovery();

// Offset: 7040
// Size: 72
void RecoverCar();

// Offset: 7112
// Size: 765
void CheckMapRenderMove();

// Offset: 7880
// Size: 300
// EAX: pCar
void ExplodeCar(tCar_spec *pCar);

// Offset: 8180
// Size: 448
// EAX: pEndFrameTime
void CheckRecoveryOfCars(tU32 pEndFrameTime);

// Offset: 8628
// Size: 107
// EAX: pNumber
void LoseSomePSPowerups(int pNumber);

// Offset: 8736
// Size: 1771
void CheckOtherRacingKeys();

// Offset: 10508
// Size: 259
int CheckRecoverCost();

// Offset: 10768
// Size: 279
// EAX: pCar
void SortOutRecover(tCar_spec *pCar);

// Offset: 11048
// Size: 115
// EAX: pCar
void SetFlipUpCar(tCar_spec *pCar);

// Offset: 11164
// Size: 1385
// EAX: car
void FlipUpCar(tCar_spec *car);

// Offset: 12552
// Size: 50
// EAX: pNum
void GetPowerup(int pNum);

// Offset: 12604
// Size: 112
// EAX: pRacing
void CheckSystemKeys(int pRacing);

// Offset: 12716
// Size: 395
void CheckKevKeys();

// Offset: 13112
// Size: 190
void BrakeInstantly();

// Offset: 13304
// Size: 853
// EAX: pTime_difference
void PollCarControls(tU32 pTime_difference);

// Offset: 14160
// Size: 846
// EAX: pTime_difference
void PollCameraControls(tU32 pTime_difference);

// Offset: 15008
// Size: 62
// EAX: i
void SetFlag2(int i);

// Offset: 15072
// Size: 164
void ToggleFlying();

// Offset: 15236
// Size: 136
void ToggleInvulnerability();

// Offset: 15372
// Size: 44
void MoreTime();

// Offset: 15416
// Size: 44
void MuchMoreTime();

// Offset: 15460
// Size: 139
void ToggleTimerFreeze();

// Offset: 15600
// Size: 44
void EarnDosh();

// Offset: 15644
// Size: 44
void LoseDosh();

// Offset: 15688
// Size: 301
void ToggleMap();

// Offset: 15992
// Size: 51
int HornBlowing();

// Offset: 16044
// Size: 215
void ToggleArrow();

// Offset: 16260
// Size: 45
int GetRecoverVoucherCount();

// Offset: 16308
// Size: 46
// EAX: pCount
void AddVouchers(int pCount);

// Offset: 16356
// Size: 44
void ResetRecoveryVouchers();

// Offset: 16400
// Size: 194
void CycleCarTexturingLevel();

// Offset: 16596
// Size: 202
void CycleWallTexturingLevel();

// Offset: 16800
// Size: 157
void CycleRoadTexturingLevel();

// Offset: 16960
// Size: 246
void CycleYonFactor();

// Offset: 17208
// Size: 45
// EAX: pLevel
void SetSoundDetailLevel(int pLevel);

// Offset: 17256
// Size: 65
// EAX: pLevel
void ReallySetSoundDetailLevel(int pLevel);

// Offset: 17324
// Size: 45
int GetSoundDetailLevel();

// Offset: 17372
// Size: 209
void CycleSoundDetailLevel();

// Offset: 17584
// Size: 162
void CycleCarSimplificationLevel();

// Offset: 17748
// Size: 194
void ToggleAccessoryRendering();

// Offset: 17944
// Size: 155
void ToggleSmoke();

// Offset: 18100
// Size: 144
// EAX: pFont
void DrawSomeText2(tDR_font *pFont);

// Offset: 18244
// Size: 104
void DrawSomeText();

// Offset: 18348
// Size: 44
void SaySorryYouLittleBastard();

// Offset: 18392
// Size: 66
void UserSendMessage();

// Offset: 18460
// Size: 815
void EnterUserMessage();

// Offset: 19276
// Size: 435
void DisplayUserMessage();

// Offset: 19712
// Size: 347
void InitAbuseomatic();

// Offset: 20060
// Size: 94
void DisposeAbuseomatic();

#endif
