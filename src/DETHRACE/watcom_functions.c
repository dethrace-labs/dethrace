#include "watcom_functions.h"

#include <ctype.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>

int stricmp(const char *a, const char *b) {
  int ca, cb;
  do {
     ca = (unsigned char) *a++;
     cb = (unsigned char) *b++;
     ca = tolower(toupper(ca));
     cb = tolower(toupper(cb));
   } while (ca == cb && ca != '\0');
   return ca - cb;
}

void splitpath( char *path, char *drive, char *dir, char *fname, char *ext ) {
  #if defined(_WIN32) || defined(_WIN64)
    _splitpath(path, drive, dr, fname, ext);
  #else
    // shortcut - we only ever call this method asking for 'fname'
    // 9 is hardcoded to match `basename` defined in `Usage`
    strncpy(fname, basename(path), 9);

  #endif
}
