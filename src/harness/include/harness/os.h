#ifndef HARNESS_PLATFORM_H
#define HARNESS_PLATFORM_H

#include <stdint.h>

// Required

// Required: return timestamp in milliseconds.
uint32_t OS_GetTime(void);

// Required: begin a directory iteration and return name of first file
char* OS_GetFirstFileInDirectory(char* path);

// Required: continue directory iteration. If no more files, return NULL
char* OS_GetNextFileInDirectory(void);

// Required: sleep for specified milliseconds
void OS_Sleep(int ms);

// Optional: return true if a debugger is detected
int OS_IsDebuggerPresent(void);

// Optional: install a handler to print stack trace during a crash
void OS_InstallSignalHandler(char* program_name);

#endif
