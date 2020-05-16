#include "main.h"
#include <stdlib.h>

#include "common/drdebug.h"
#include "common/globvars.h"
#include "common/init.h"
#include "common/loading.h"
#include "common/loadsave.h"
#include "common/sound.h"
#include "common/structur.h"
#include "common/utility.h"
#include "pc-dos/dossys.h"

// Offset: 0
// Size: 161
void QuitGame() {
    NOT_IMPLEMENTED();
}

// Offset: 164
// Size: 97
tU32 TrackCount(br_actor* pActor, tU32* pCount) {
    unsigned int x;
    unsigned int z;
    int ad;
    float e;
    NOT_IMPLEMENTED();
}

// Offset: 264
// Size: 95
void CheckNumberOfTracks() {
    NOT_IMPLEMENTED();
}

// Offset: 360
// Size: 173
// EAX: pRacing
void ServiceTheGame(int pRacing) {
    NOT_IMPLEMENTED();
}

// Offset: 536
// Size: 41
void ServiceGame() {
    NOT_IMPLEMENTED();
}

// Offset: 580
// Size: 49
void ServiceGameInRace() {
    NOT_IMPLEMENTED();
}

// Offset: 632
// Size: 177
// EAX: pArgc
// EDX: pArgv
void GameMain(int pArgc, char** pArgv) {
    tPath_name CD_dir;

    PDSetFileVariables();
    PDBuildAppPath(gApplication_path);
    DrDebugMessage(*pArgv);

    strcat(gApplication_path, "DATA");

    UsePathFileToDetermineIfFullInstallation();

    if (!gCD_fully_installed && GetCDPathFromPathsTxtFile(CD_dir) && !PDCheckDriveExists(CD_dir)) {
        PDInitialiseSystem();
        fprintf(stderr, "Can't find the Carmageddon CD\n");
        exit(1);
    }
    InitialiseDeathRace(pArgc, pArgv);
    DoProgram();
    DoSaveGame(1);
}
