#include "harness.h"
#include "pc-dos/dossys.h"
#include "renderers/gl_renderer.h"
#include <stdlib.h>
#include <unistd.h>

extern int original_main(int pArgc, char* pArgv[]);

int main(int argc, char* argv[]) {

    Harness_Init(argv[0], &OpenGLRenderer);

    return original_main(argc, argv);
}
