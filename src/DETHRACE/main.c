
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#include <stdio.h>
#include <windows.h>
#endif


#if defined(ANDROID) || defined(__ANDROID__)
#include <SDL2/SDL_main.h>
#endif

#include "brender.h"

extern int Harness_Init(int* argc, char* argv[]);
extern int original_main(int pArgc, char* pArgv[]);

void BR_CALLBACK _BrBeginHook(void) {
    struct br_device* BR_EXPORT BrDrv1SoftPrimBegin(char* arguments);
    struct br_device* BR_EXPORT BrDrv1SoftRendBegin(char* arguments);
    struct br_device* BR_EXPORT BrDrv1VirtualFramebufferBegin(char* arguments);
    struct br_device* BR_EXPORT BrDrv1GLBegin(char* arguments);

#if _MSC_VER != 1020
    BrDevAddStatic(NULL, BrDrv1SoftPrimBegin, NULL);
    BrDevAddStatic(NULL, BrDrv1SoftRendBegin, NULL);
    BrDevAddStatic(NULL, BrDrv1VirtualFramebufferBegin, NULL);
    BrDevAddStatic(NULL, BrDrv1GLBegin, NULL);
#endif
}

void BR_CALLBACK _BrEndHook(void) {
}
#ifdef ANDROID
int SDL_main(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    int result;

#ifdef _WIN32
#if _MSC_VER != 1020
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
#endif

    result = Harness_Init(&argc, argv);
    if (result != 0) {
        return result;
    }

    return original_main(argc, argv);
}
