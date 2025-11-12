#include "harness/trace.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>

int harness_debug_level = 4;

void debug_printf(const char* fmt, const char* fn, const char* fmt2, ...) {
    va_list ap;

    printf(fmt, fn);

    va_start(ap, fmt2);
    vprintf(fmt2, ap);
    va_end(ap);

    puts("\033[0m");
}

void panic_printf(const char* fmt, const char* fn, const char* fmt2, ...) {
    va_list ap;

    // Create directories first
    mkdir("/storage/emulated/0/Alpha3", 0755);
    mkdir("/storage/emulated/0/Alpha3/dethrace", 0755);
    
    FILE* fp = fopen("/storage/emulated/0/Alpha3/dethrace/dethrace.log", "a");
    
    if (fp == NULL) {
        // Log to stderr as fallback
        fp = stderr;
        printf("CRITICAL: Cannot open log file at /storage/emulated/0/Alpha3/dethrace/dethrace.log\n");
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    fprintf(fp, "[%02d:%02d:%02d] PANIC: ", t->tm_hour, t->tm_min, t->tm_sec);
    printf("\033[0;31m");
    printf(fmt, fn);
    fprintf(fp, fmt, fn);

    va_start(ap, fmt2);
    vprintf(fmt2, ap);
    vfprintf(fp, fmt2, ap);
    va_end(ap);
    
    fprintf(fp, "\n");
    fflush(fp);  // Ensure data is written immediately
    
    if (fp != stderr) {
        fclose(fp);
    }
    puts("\033[0m");
}
