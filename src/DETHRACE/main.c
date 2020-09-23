#include "harness.h"
#include "pc-dos/dossys.h"
#include "renderers/gl_renderer.h"
#include <stdlib.h>
#include <unistd.h>

extern int original_main(int pArgc, char* pArgv[]);

int main(int argc, char* argv[]) {
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

    return original_main(argc, argv);
}
