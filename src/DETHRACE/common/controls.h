#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern tToggle_element gToggle_array[43];
extern int gRepair_last_time;
extern int gHad_auto_recover;
extern tU32 gLast_repair_time;
extern tEdit_mode gWhich_edit_mode;
extern char* gEdit_mode_names[10];
extern tEdit_func* gEdit_funcs[10][18][8];
extern tCheat gKev_keys[44];
extern int gAllow_car_flying;
extern int gEntering_message;
extern tU32 gPalette_fade_time;
extern char* gAbuse_text[10];
extern char gString[84];
extern int gToo_late;
extern int gRecover_timer;
extern int gRecovery_voucher_count;
extern int gInstant_handbrake;
extern int gAuto_repair;

void AbortRace(void);

void F4Key(void);

void SetFlag(int i);

void FinishLap(int i);

void EnsureSpecialVolumesHidden(void);

void ShowSpecialVolumesIfRequ(void);

void DoEditModeKey(int pIndex);

void F5Key(void);

void F6Key(void);

void F7Key(void);

void F8Key(void);

void F10Key(void);

void F11Key(void);

void F12Key(void);

void NumberKey0(void);

void NumberKey1(void);

void NumberKey2(void);

void NumberKey3(void);

void NumberKey4(void);

void NumberKey5(void);

void NumberKey6(void);

void NumberKey7(void);

void NumberKey8(void);

void NumberKey9(void);

void LookLeft(void);

void LookForward(void);

void LookRight(void);

void DamageTest(void);

void TDamageEngine(void);

void TDamageDriver(void);

void TDamageTrans(void);

void TDamageSteering(void);

void TDamageLFWheel(void);

void TDamageLFBrake(void);

void TDamageLRBrake(void);

void TDamageLRWheel(void);

void TDamageRFWheel(void);

void TDamageRFBrake(void);

void TDamageRRBrake(void);

void TDamageRRWheel(void);

void MoveBonnetForward(void);

void SaveBonnet(void);

void MoveBonnetBackward(void);

void MoveBonnetLeft(void);

void ShrinkBonnetX(void);

void SwellBonnetX(void);

void ShrinkBonnetY(void);

void SwellBonnetY(void);

void ShrinkBonnetZ(void);

void SwellBonnetZ(void);

void MoveBonnetDown(void);

void MoveBonnetRight(void);

void MoveBonnetUp(void);

void TiltBonnetDownX(void);

void TiltBonnetUpX(void);

void TiltBonnetDownY(void);

void TiltBonnetUpY(void);

void TiltBonnetDownZ(void);

void TiltBonnetUpZ(void);

void ToggleCockpit(void);

void ToggleMirror(void);

void ConcussMe(void);

void CheckHelp(void);

void CheckLoadSave(void);

void CheckToggles(int pRacing);

int CarWorldOffFallenCheckThingy(tCar_spec* pCar, int pCheck_around);

int HasCarFallenOffWorld(tCar_spec* pCar);

void CheckForBeingOutOfThisWorld(void);

void CheckHornLocal(tCar_spec* pCar);

void CheckHorn3D(tCar_spec* pCar);

void CheckHorns(void);

void SetRecovery(void);

void RecoverCar(void);

void CheckMapRenderMove(void);

void ExplodeCar(tCar_spec* pCar);

void CheckRecoveryOfCars(tU32 pEndFrameTime);

void LoseSomePSPowerups(int pNumber);

void CheckOtherRacingKeys(void);

int CheckRecoverCost(void);

void SortOutRecover(tCar_spec* pCar);

void SetFlipUpCar(tCar_spec* pCar);

void FlipUpCar(tCar_spec* car);

void GetPowerup(int pNum);

void CheckSystemKeys(int pRacing);

void CheckKevKeys(void);

void BrakeInstantly(void);

void PollCarControls(tU32 pTime_difference);

void PollCameraControls(tU32 pTime_difference);

void SetFlag2(int i);

void ToggleFlying(void);

void ToggleInvulnerability(void);

void MoreTime(void);

void MuchMoreTime(void);

void ToggleTimerFreeze(void);

void EarnDosh(void);

void LoseDosh(void);

void ToggleMap(void);

int HornBlowing(void);

void ToggleArrow(void);

int GetRecoverVoucherCount(void);

void AddVouchers(int pCount);

void ResetRecoveryVouchers(void);

void CycleCarTexturingLevel(void);

void CycleWallTexturingLevel(void);

void CycleRoadTexturingLevel(void);

void CycleYonFactor(void);

void SetSoundDetailLevel(int pLevel);

void ReallySetSoundDetailLevel(int pLevel);

int GetSoundDetailLevel(void);

void CycleSoundDetailLevel(void);

void CycleCarSimplificationLevel(void);

void ToggleAccessoryRendering(void);

void ToggleSmoke(void);

void DrawSomeText2(tDR_font* pFont);

void DrawSomeText(void);

void SaySorryYouLittleBastard(void);

void UserSendMessage(void);

void EnterUserMessage(void);

void DisplayUserMessage(void);

void InitAbuseomatic(void);

void DisposeAbuseomatic(void);

#endif
