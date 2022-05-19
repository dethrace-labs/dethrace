#include "main.h"
#include <stdlib.h>

#include "controls.h"
#include "cutscene.h"
#include "drmem.h"
#include "errors.h"
#include "globvars.h"
#include "harness/config.h"
#include "harness/trace.h"
#include "init.h"
#include "input.h"
#include "loading.h"
#include "loadsave.h"
#include "network.h"
#include "pd/sys.h"
#include "s3/s3.h"
#include "sound.h"
#include "structur.h"
#include "utility.h"

// IDA: void __cdecl QuitGame()
void QuitGame() {
    LOG_TRACE("()");

    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo) {
        DoDemoGoodbye();
    }

    NOT_IMPLEMENTED();
}

// IDA: tU32 __cdecl TrackCount(br_actor *pActor, tU32 *pCount)
tU32 TrackCount(br_actor* pActor, tU32* pCount) {
    unsigned int x;
    unsigned int z;
    int ad;
    float e;
    LOG_TRACE("(%p, %p)", pActor, pCount);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckNumberOfTracks()
void CheckNumberOfTracks() {
    tU32 track_count;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ServiceTheGame(int pRacing@<EAX>)
void ServiceTheGame(int pRacing) {

    CheckMemory();
    if (!pRacing) {
        CyclePollKeys();
    }
    PollKeys();
    rand();
    if (PDServiceSystem(gFrame_period)) {
        QuitGame();
    }
    if (!pRacing) {
        CheckSystemKeys(0);
    }
    if (!pRacing && gSound_enabled) {
        if (gProgram_state.cockpit_on && gProgram_state.cockpit_image_index >= 0) {
            S3Service(1, 2);
        } else {
            S3Service(0, 2);
        }
    }
    NetService(pRacing);
}

// IDA: void __cdecl ServiceGame()
void ServiceGame() {
    ServiceTheGame(0);
}

// IDA: void __cdecl ServiceGameInRace()
void ServiceGameInRace() {
    LOG_TRACE("()");

    ServiceTheGame(1);
    CheckKevKeys();
}

// IDA: void __usercall GameMain(int pArgc@<EAX>, char **pArgv@<EDX>)
void GameMain(int pArgc, char** pArgv) {
    tPath_name CD_dir;

    PDSetFileVariables();
    PDBuildAppPath(gApplication_path);
    OpenDiagnostics();

    strcat(gApplication_path, "DATA");

    UsePathFileToDetermineIfFullInstallation();
    if (!gCD_fully_installed && GetCDPathFromPathsTxtFile(CD_dir) && !PDCheckDriveExists(CD_dir)) {
        PDInitialiseSystem();
        fprintf(stderr, "Can't find the Carmageddon CD\n");
        exit(1);
    }
    InitialiseDeathRace(pArgc, pArgv);
    DoProgram();
    QuitGame();
}
