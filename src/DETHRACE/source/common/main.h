#ifndef _MAIN_H_
#define _MAIN_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 161
void QuitGame();

// Offset: 164
// Size: 97
tU32 TrackCount(br_actor *pActor, tU32 *pCount);

// Offset: 264
// Size: 95
void CheckNumberOfTracks();

// Offset: 360
// Size: 173
// EAX: pRacing
void ServiceTheGame(int pRacing);

// Offset: 536
// Size: 41
void ServiceGame();

// Offset: 580
// Size: 49
void ServiceGameInRace();

// Offset: 632
// Size: 177
// EAX: pArgc
// EDX: pArgv
void GameMain(int pArgc, char **pArgv);

#endif
