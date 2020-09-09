#include "harness.h"
#include "pc-dos/dossys.h"
#include "renderers/gl_renderer.h"
#include <stdlib.h>
#include <unistd.h>

extern int original_main(int pArgc, char* pArgv[]);

int _argc;
char** _argv;

int game_thread_func(void* args) {
    int exit_code;
    exit_code = original_main(_argc, _argv);
    // TODO: maybe we should be kinder here and post a message to the window thread to shutdown cleanly.
    exit(exit_code);
}

int main(int argc, char* argv[]) {
    _argc = argc;
    _argv = argv;
    int result;

    Harness_Init(argv[0], &OpenGLRenderer);

    char* root_dir = getenv("DETHRACE_ROOT_DIR");
    if (!root_dir) {
        LOG_PANIC("DETHRACE_ROOT_DIR is not set");
    }
    printf("DETHRACE_ROOT_DIR: %s\n", root_dir);
    result = chdir(root_dir);
    if (result != 0) {
        LOG_PANIC("Failed to chdir. Returned %d", result);
    }

    Harness_RunWindowLoop(&game_thread_func, NULL);
}
