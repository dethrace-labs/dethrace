#ifndef HARNESS_PLATFORM_H
#define HARNESS_PLATFORM_H

int Platform_IsDebuggerPresent(void);

void Platform_InstallSignalHandler(char* program_name);

void Platform_PrintStacktrace(void);

#endif
