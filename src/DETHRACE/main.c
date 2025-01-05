
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#include <stdio.h>
#include <windows.h>
#endif

#include "brender.h"

extern void Harness_Init(int* argc, char* argv[]);
extern int original_main(int pArgc, char* pArgv[]);

void BR_CALLBACK _BrBeginHook(void) {
    struct br_device* BR_EXPORT BrDrv1SoftPrimBegin(char* arguments);
    struct br_device* BR_EXPORT BrDrv1SoftRendBegin(char* arguments);

    BrDevAddStatic(NULL, BrDrv1SoftPrimBegin, NULL);
    BrDevAddStatic(NULL, BrDrv1SoftRendBegin, NULL);
    // BrDevAddStatic(NULL, BrDrv1SDL2Begin, NULL);
}

void BR_CALLBACK _BrEndHook(void) {
}

#ifdef __3DS__
#include <3ds.h>
PrintConsole console;

int dethrace_main(int argc, char* argv[]);
void mainThreadFunc(void* arg) {
    char** argv = (char**)arg;
    int argc = 0;
    while (argv[argc] != NULL) {
        argc++;
    }

    int result = dethrace_main(argc, argv);
    threadExit(result);
}

int main(int argc, char* argv[]) {
    APT_SetAppCpuTimeLimit(89);
    osSetSpeedupEnable(true);
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, &console);

    if (argc == 0 || argv == NULL) {
        static char program_name[] = "dethrace";
        static char* fake_argv[] = {program_name, NULL};

        argc = 1;
        argv = fake_argv;
    }

    Thread mainThread = threadCreate(mainThreadFunc, argv, 0x800000, 0x30, -1, true);
    if (!mainThread) {
        return -1;
    }

    threadJoin(mainThread, U64_MAX);
    threadFree(mainThread);

    return 0;
}

int dethrace_main(int argc, char* argv[]) {
#else
int main(int argc, char* argv[]) {
#endif
#ifdef _WIN32
    /* Attach to the console that started us if any */
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        /* We attached successfully, lets redirect IO to the consoles handles if not already redirected */
        if (_fileno(stdout) == -2 || _get_osfhandle(_fileno(stdout)) == -2) {
            freopen("CONOUT$", "w", stdout);
        }

        if (_fileno(stderr) == -2 || _get_osfhandle(_fileno(stderr)) == -2) {
            freopen("CONOUT$", "w", stderr);
        }

        if (_fileno(stdin) == -2 || _get_osfhandle(_fileno(stdin)) == -2) {
            freopen("CONIN$", "r", stdin);
        }
    }
#endif
    Harness_Init(&argc, argv);

    return original_main(argc, argv);
}
