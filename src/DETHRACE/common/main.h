#ifndef _MAIN_H_
#define _MAIN_H_

#include "br_types.h"
#include "dr_types.h"



void QuitGame();

tU32 TrackCount(br_actor* pActor, tU32* pCount);

void CheckNumberOfTracks();

void ServiceTheGame(int pRacing);

void ServiceGame();

void ServiceGameInRace();

void GameMain(int pArgc, char** pArgv);

#endif
