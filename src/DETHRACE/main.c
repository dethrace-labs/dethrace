#include "harness.h"
#include "pc-dos/dossys.h"
#include "renderers/gl_renderer.h"
#include <stdlib.h>
#include <unistd.h>

extern int original_main(int pArgc, char* pArgv[]);

int main(int argc, char* argv[]) {
#ifdef _WIN32
    /* Attach to the console that started us if any */
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        /* We attached successfully, lets redirect IO to the consoles handles if not already redirected */
        if (_fileno(stdout) == -2 || _get_osfhandle(fileno(stdout)) == -2) {
            freopen("CONOUT$", "w", stdout);
        }

        if (_fileno(stderr) == -2 || _get_osfhandle(fileno(stderr)) == -2) {
            freopen("CONOUT$", "w", stderr);
        }

        if (_fileno(stdin) == -2 || _get_osfhandle(fileno(stdin)) == -2) {
            freopen("CONIN$", "r", stdin);
        }
    }
#endif

    Harness_Init(argv[0], &OpenGLRenderer);

    return original_main(argc, argv);
}
