#include "dr_types.h"
#include "pc-dos/dossys.h"
#include "stack_trace_handler.h"
#include <stdlib.h>

extern int _main(int pArgc, char* pArgv[]);

// EAX: pArgc
// EDX: pArgv
// Moved from pc-dos/dossys.c to enable unit testing
int main(int argc, char* argv[]) {
    set_signal_handler(argv[0]);
    return _main(argc, argv);
}
