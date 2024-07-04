#ifndef HARNESS_OS_H
#define HARNESS_OS_H

#include <stdint.h>
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#include <io.h>
#define getcwd _getcwd
#define chdir _chdir
#define access _access
#define F_OK 0
#define strcasecmp _stricmp
#define strncasecmp _strnicmp

#if _MSC_VER < 1900
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif

#else
#include <unistd.h>
#endif

// Optional: install a handler to print stack trace during a crash
void OS_InstallSignalHandler(char* program_name);

FILE* OS_fopen(const char* pathname, const char* mode);

size_t OS_ConsoleReadPassword(char* pBuffer, size_t pBufferLen);

char* OS_Dirname(const char* path);

char* OS_Basename(const char* path);

char* OS_GetWorkingDirectory(char* argv0);

#endif
