#ifndef _MAIN_H_
#define _MAIN_H_

#include "dr_types.h"
#include "harness/compiler.h"

void QuitGame(void);

tU32 TrackCount(br_actor* pActor, tU32* pCount);

void CheckNumberOfTracks(void);

void ServiceTheGame(int pRacing);

void ServiceGame(void);

void ServiceGameInRace(void);

void GameMain(int pArgc, char** pArgv);

#endif
