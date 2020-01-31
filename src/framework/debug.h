#ifndef DEBUG_H
#define DEBUG_H

#define LOG_TRACE(...) debug_printf("[TRACE] %s", __FUNCTION__, __VA_ARGS__)
#define LOG_DEBUG(...) debug_printf("[DEBUG] %s ", __FUNCTION__, __VA_ARGS__)
#define LOG_WARN(...) debug_printf("[WARN] %s ", __FUNCTION__, __VA_ARGS__)
#define LOG_PANIC(...)                                      \
    debug_printf("[PANIC] %s ", __FUNCTION__, __VA_ARGS__); \
    exit(1);

void debug_printf(const char* fmt, const char* fn, const char* fmt2, ...);

#endif