#ifndef DEBUG_H
#define DEBUG_H

#include <stdlib.h>

#define BLUE
#define LOG_LEVEL 3

#define LOG_TRACE(...) debug_printf("[TRACE] %s", __FUNCTION__, __VA_ARGS__)

#define LOG_TRACE10(...)                                       \
    if (LOG_LEVEL >= 10) {                                     \
        debug_printf("[TRACE] %s", __FUNCTION__, __VA_ARGS__); \
    }
#define LOG_TRACE9(...)                                        \
    if (LOG_LEVEL >= 9) {                                      \
        debug_printf("[TRACE] %s", __FUNCTION__, __VA_ARGS__); \
    }

#define LOG_TRACE8(...)                                        \
    if (LOG_LEVEL >= 8) {                                      \
        debug_printf("[TRACE] %s", __FUNCTION__, __VA_ARGS__); \
    }

#define LOG_DEBUG(...) debug_printf("\033[0;34m[DEBUG] %s ", __FUNCTION__, __VA_ARGS__)
#define LOG_WARN(...) debug_printf("\033[0;33m[WARN] %s ", __FUNCTION__, __VA_ARGS__)
#define LOG_PANIC(...)                                                \
    debug_printf("\033[0;31m[PANIC] %s ", __FUNCTION__, __VA_ARGS__); \
    exit(1);

#define NOT_IMPLEMENTED()                                                         \
    debug_printf("\033[0;31m[PANIC] %s ", __FUNCTION__, "%s", "not implemented"); \
    exit(1);

void debug_printf(const char* fmt, const char* fn, const char* fmt2, ...);

#endif