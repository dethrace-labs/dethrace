
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#include <stdio.h>
#include <windows.h>
#endif

#include "brender.h"

extern int Harness_Init(int* argc, char* argv[]);
extern int original_main(int pArgc, char* pArgv[]);

void BR_CALLBACK _BrBeginHook(void) {
    struct br_device* BR_EXPORT BrDrv1SoftPrimBegin(char* arguments);
    struct br_device* BR_EXPORT BrDrv1SoftRendBegin(char* arguments);
    struct br_device* BR_EXPORT BrDrv1VirtualFramebufferBegin(char* arguments);
    struct br_device* BR_EXPORT BrDrv1GLBegin(char* arguments);

   // BrDevAddStatic(NULL, BrDrv1SoftPrimBegin, NULL);
   // BrDevAddStatic(NULL, BrDrv1SoftRendBegin, NULL);
   // BrDevAddStatic(NULL, BrDrv1VirtualFramebufferBegin, NULL);
   // BrDevAddStatic(NULL, BrDrv1GLBegin, NULL);
}

void BR_CALLBACK _BrEndHook(void) {
}

int main(int argc, char* argv[]) {
    int result;

#ifdef _WIN32XXX
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

    result = Harness_Init(&argc, argv);
    if (result != 0) {
        return result;
    }

    return original_main(argc, argv);
}
