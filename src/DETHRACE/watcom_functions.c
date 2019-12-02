#include "watcom_functions.h"

#include <ctype.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
  #define snprintf _snprintf
  #define vsnprintf _vsnprintf
  #define strcasecmp _stricmp
  #define strncasecmp _strnicmp
#else
#include <strings.h>
#endif

void splitpath( char *path, char *drive, char *dir, char *fname, char *ext ) {
  #if defined(_WIN32) || defined(_WIN64)
    _splitpath(path, drive, dr, fname, ext);
  #else
    // shortcut - we only ever call this method asking for 'fname'
    // 9 is hardcoded to match `basename` defined in `Usage`
    strncpy(fname, basename(path), 9);

  #endif
}
