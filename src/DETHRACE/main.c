#include "harness.h"
#include "pc-dos/dossys.h"
#include "renderers/gl_renderer.h"
#include <stdlib.h>

extern int _main(int pArgc, char* pArgv[]);

int _argc;
char** _argv;

int main_game_func(void* args);

int main(int argc, char* argv[]) {
    _argc = argc;
    _argv = argv;

    Harness_Init(argv[0], &OpenGLRenderer);
    Harness_RunWindowLoop(&main_game_func, NULL);
}

int main_game_func(void* args) {
    return _main(_argc, _argv);
}
