#ifndef DEBUG_H
#define DEBUG_H

#include "br_types.h"
#include <stdlib.h>
#include <unistd.h>

#if defined _WIN32 && !defined sleep
#define sleep(x) _sleep(x)
#endif

extern int harness_debug_level;

void debug_printf(const char* fmt, const char* fn, const char* fmt2, ...);
void debug_print_vector3(const char* fmt, const char* fn, char* msg, br_vector3* v);
void debug_print_matrix34(const char* fmt, const char* fn, char* name, br_matrix34* m);

#define BLUE

#define LOG_TRACE(...)                                         \
    if (harness_debug_level >= 5) {                            \
        debug_printf("[TRACE] %s", __FUNCTION__, __VA_ARGS__); \
    }

#define LOG_TRACE10(...)                                       \
    if (harness_debug_level >= 10) {                           \
        debug_printf("[TRACE] %s", __FUNCTION__, __VA_ARGS__); \
    }
#define LOG_TRACE9(...)                                        \
    if (harness_debug_level >= 9) {                            \
        debug_printf("[TRACE] %s", __FUNCTION__, __VA_ARGS__); \
    }

#define LOG_TRACE8(...)                                        \
    if (harness_debug_level >= 8) {                            \
        debug_printf("[TRACE] %s", __FUNCTION__, __VA_ARGS__); \
    }

#define LOG_DEBUG(...) debug_printf("\033[0;34m[DEBUG] %s ", __FUNCTION__, __VA_ARGS__)
#define LOG_VEC(msg, v) debug_print_vector3("\033[0;34m[DEBUG] %s ", __FUNCTION__, msg, v)
#define LOG_MATRIX(msg, m) debug_print_matrix34("\033[0;34m[DEBUG] %s ", __FUNCTION__, msg, m)
#define LOG_INFO(...) debug_printf("\033[0;34m[INFO] %s ", __FUNCTION__, __VA_ARGS__)
#define LOG_WARN(...) debug_printf("\033[0;33m[WARN] %s ", __FUNCTION__, __VA_ARGS__)
#define LOG_PANIC(...)                                                \
    debug_printf("\033[0;31m[PANIC] %s ", __FUNCTION__, __VA_ARGS__); \
    exit(1);

#define NOT_IMPLEMENTED()                                                         \
    debug_printf("\033[0;31m[PANIC] %s ", __FUNCTION__, "%s", "not implemented"); \
    sleep(2);                                                                     \
    exit(1);

#define TELL_ME_IF_WE_PASS_THIS_WAY()                                                    \
    debug_printf("\033[0;31m[PANIC] %s ", __FUNCTION__, "%s", "code path not expected"); \
    exit(1);

#define STUB() \
    debug_printf("\033[0;31m[WARN] %s ", __FUNCTION__, "%s", "stubbed");

#define STUB_ONCE()                                                          \
    static int stub_printed = 0;                                             \
    if (!stub_printed) {                                                     \
        debug_printf("\033[0;31m[WARN] %s ", __FUNCTION__, "%s", "stubbed"); \
        stub_printed = 1;                                                    \
    }

//int count_open_fds();

#endif