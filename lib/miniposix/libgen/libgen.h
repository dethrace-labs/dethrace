#ifndef WIN32PORTS_LIBGEN_H
#define WIN32PORTS_LIBGEN_H

#ifndef _WIN32

#pragma message("this libgen.h implementation is for Windows only!")

#elif defined __MINGW32__

#include <../include/libgen.h>

#else

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

char *basename(char *);
char *dirname(char *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _WIN32 */

#endif /* WIN32PORTS_LIBGEN_H */
