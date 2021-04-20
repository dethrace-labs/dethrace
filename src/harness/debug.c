#include "harness.h"

#include <dirent.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>

void debug_printf(const char* fmt, const char* fn, const char* fmt2, ...) {
    va_list ap;

    fprintf(stderr, fmt, fn);

    va_start(ap, fmt2);
    vfprintf(stderr, fmt2, ap);
    va_end(ap);

    fputs("\033[0m\n", stderr);
}

// int count_open_fds() {
//     DIR* dp = opendir("/dev/fd/");
//     struct dirent* de;
//     int count = -3; // '.', '..', dp

//     if (dp == NULL)
//         return -1;

//     while ((de = readdir(dp)) != NULL)
//         count++;

//     (void)closedir(dp);

//     return count;
// }