#include "pc-dos/dossys.h"

extern int _main(int pArgc, char* pArgv[]);

// EAX: pArgc
// EDX: pArgv
// Moved from pc-dos/dossys.c to enable unit testing
int main(int pArgc, char* pArgv[]) {
    return _main(pArgc, pArgv);
}
