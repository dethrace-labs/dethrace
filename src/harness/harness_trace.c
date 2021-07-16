#include "harness_trace.h"

#include <dirent.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>

int harness_debug_level = 7;

void debug_printf(const char* fmt, const char* fn, const char* fmt2, ...) {
    va_list ap;

    printf(fmt, fn);

    va_start(ap, fmt2);
    vprintf(fmt2, ap);
    va_end(ap);

    puts("\033[0m");
}

void debug_print_vector3(const char* fmt, const char* fn, char* msg, br_vector3* v) {
    printf(fmt, fn);
    printf("%s %f, %f, %f\n", msg, v->v[0], v->v[1], v->v[2]);
    puts("\033[0m");
}

void debug_print_matrix34(const char* fmt, const char* fn, char* msg, br_matrix34* m) {
    printf(fmt, fn);
    printf("matrix34 \"%s\"\n", msg);
    for (int i = 0; i < 4; i++) {
        printf("  %f, %f, %f\n", m->m[i][0], m->m[i][1], m->m[i][2]);
    }
    puts("\033[0m");
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