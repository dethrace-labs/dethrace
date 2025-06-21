#ifndef HARNESS_TRACE_H
#define HARNESS_TRACE_H

#include "brender.h"
#include <stdlib.h>

extern int harness_debug_level;

void debug_printf(const char* fmt, const char* fn, const char* fmt2, ...);
void panic_printf(const char* fmt, const char* fn, const char* fmt2, ...);
void debug_print_vector3(const char* fmt, const char* fn, char* msg, br_vector3* v);
void debug_print_matrix34(const char* fmt, const char* fn, char* name, br_matrix34* m);
void debug_print_matrix4(const char* fmt, const char* fn, char* name, br_matrix4* m);

#define BLUE

#if 1 // _MSC_VER == 1020

#define LOG_TRACE()
#define LOG_TRACE8()
#define LOG_TRACE9()
#define LOG_TRACE10()
#define LOG_DEBUG(a)
#define LOG_DEBUG2(a, b)
#define LOG_DEBUG3(a, b, c)
#define LOG_INFO(a)
#define LOG_INFO2(a, b)
#define LOG_INFO3(a, b, c)
#define LOG_WARN(a)
#define LOG_WARN2(a, b)
#define LOG_WARN3(a, b, c)
#define LOG_PANIC(a) abort()
#define LOG_PANIC2(a, b)
#define LOG_WARN_ONCE()
#define NOT_IMPLEMENTED() abort()
#define TELL_ME_IF_WE_PASS_THIS_WAY() abort()
#define STUB_ONCE()

#else

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
#define LOG_INFO(...) debug_printf("[INFO] %s ", __FUNCTION__, __VA_ARGS__)
#define LOG_WARN(...) debug_printf("\033[0;33m[WARN] %s ", __FUNCTION__, __VA_ARGS__)
#define LOG_PANIC(...)                                          \
    do {                                                        \
        panic_printf("[PANIC] %s ", __FUNCTION__, __VA_ARGS__); \
        abort();                                                \
    } while (0)

#define LOG_WARN_ONCE(...)                                               \
    static int warn_printed = 0;                                         \
    if (!warn_printed) {                                                 \
        debug_printf("\033[0;33m[WARN] %s ", __FUNCTION__, __VA_ARGS__); \
        warn_printed = 1;                                                \
    }

#define NOT_IMPLEMENTED() \
    LOG_PANIC("not implemented")

#define TELL_ME_IF_WE_PASS_THIS_WAY() \
    LOG_PANIC("code path not expected")

#define STUB_ONCE()                                                          \
    static int stub_printed = 0;                                             \
    if (!stub_printed) {                                                     \
        debug_printf("\033[0;31m[WARN] %s ", __FUNCTION__, "%s", "stubbed"); \
        stub_printed = 1;                                                    \
    }

#endif

#endif // ifdef
