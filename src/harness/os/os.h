#ifndef HARNESS_PLATFORM_H
#define HARNESS_PLATFORM_H

int OS_IsDebuggerPresent(void);

void OS_InstallSignalHandler(char* program_name);

void OS_PrintStacktrace(void);

#endif
