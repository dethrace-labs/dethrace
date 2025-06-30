#include <stdio.h>
#include <string.h>

void OS_InstallSignalHandler(char* program_name) {
}

char* OS_GetFirstFileInDirectory(char* path) {
    return NULL;
}

// Required: continue directory iteration. If no more files, return NULL
char* OS_GetNextFileInDirectory(void) {
    return NULL;
}

FILE* OS_fopen(const char* pathname, const char* mode) {
    return NULL;
}

size_t OS_ConsoleReadPassword(char* pBuffer, size_t pBufferLen) {
    return 0;
}

char* OS_Dirname(const char* path) {
    return NULL;
}

char* OS_Basename(const char* path) {
    return NULL;
}

char* OS_GetWorkingDirectory(char* argv0) {
    return NULL;
}

int OS_GetAdapterAddress(char* name, void* pSockaddr_in) {
    return 0;
}

int OS_InitSockets(void) {
    return 0;
}

int OS_GetLastSocketError(void) {
    return 0;
}

void OS_CleanupSockets(void) {
}

int OS_SetSocketNonBlocking(int socket) {
    return 0;
}

int OS_CloseSocket(int socket) {
    return 0;
}
