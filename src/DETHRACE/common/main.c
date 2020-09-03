#include "main.h"
#include <stdlib.h>

#include "controls.h"
#include "errors.h"
#include "globvars.h"
#include "init.h"
#include "input.h"
#include "loading.h"
#include "loadsave.h"
#include "pc-dos/dossys.h"
#include "sound.h"
#include "structur.h"
#include "utility.h"

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
    if (!pRacing) {
        CyclePollKeys();
    }
    PollKeys();
    rand();
    if (PDServiceSystem(gFrame_period)) {
        //sub_500B0(v3);
    }
    if (!pRacing) {
        CheckSystemKeys(0);
    }
    if (!pRacing && gSound_enabled) {
        // TODO: sound?
        // S3Service(gProgram_state.cockpit_on && gProgram_state.cockpit_image_index >= 0, 2);
    }
}

// Offset: 536
// Size: 41
void ServiceGame() {
    ServiceTheGame(0);
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
    dr_dprintf(gApplication_path);

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
