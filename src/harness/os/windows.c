// Based on https://gist.github.com/jvranish/4441299

// this has to be first
#include <windows.h>

#include <imagehlp.h>

#include "harness/os.h"
#include <assert.h>
#include <direct.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN64
#define Esp Rsp
#define Eip Rip
#define Ebp Rbp
#endif

static int stack_nbr = 0;
static char _program_name[1024];
LARGE_INTEGER qpc_start_time, EndingTime, ElapsedMicroseconds;
LARGE_INTEGER qpc_ticks_per_sec;

HANDLE directory_handle = NULL;
char last_found_file[260];

uint32_t OS_GetTime() {
    LARGE_INTEGER now;
    if (qpc_start_time.QuadPart == 0) {
        QueryPerformanceFrequency(&qpc_ticks_per_sec);
        QueryPerformanceCounter(&qpc_start_time);
    }

    QueryPerformanceCounter(&now);
    return (uint32_t)(((now.QuadPart - qpc_start_time.QuadPart) * 1000) / qpc_ticks_per_sec.QuadPart);
}

void OS_Sleep(int delay_ms) {
    Sleep(delay_ms);
}

char* OS_GetFirstFileInDirectory(char* path) {
    char with_extension[256];
    WIN32_FIND_DATA find_data;
    HANDLE hFind = NULL;

    strcpy(with_extension, path);
    strcat(with_extension, "\\*.???");
    directory_handle = FindFirstFile(with_extension, &find_data);
    if (directory_handle == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    strcpy(last_found_file, find_data.cFileName);
    return last_found_file;
}

// Required: continue directory iteration. If no more files, return NULL
char* OS_GetNextFileInDirectory(void) {
    WIN32_FIND_DATA find_data;
    if (directory_handle == NULL) {
        return NULL;
    }

    while (FindNextFile(directory_handle, &find_data)) {
        strcpy(last_found_file, find_data.cFileName);
        return last_found_file;
    }
    FindClose(directory_handle);
    return NULL;
}

void OS_Basename(char* path, char* base) {
    _splitpath(path, NULL, NULL, base, NULL);
}

int OS_IsDebuggerPresent() {
    return IsDebuggerPresent();
}

int addr2line(char const* const program_name, void const* const addr) {
    char addr2line_cmd[512] = { 0 };

    sprintf(addr2line_cmd, "addr2line -f -p -e %.256s %p", program_name, addr);

    fprintf(stderr, "%d: ", stack_nbr++);
    return system(addr2line_cmd);
}

void print_stacktrace(CONTEXT* context) {

    SymInitialize(GetCurrentProcess(), 0, true);

    STACKFRAME frame = { 0 };

    /* setup initial stack frame */
    frame.AddrPC.Offset = context->Eip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context->Esp;
    frame.AddrStack.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context->Ebp;
    frame.AddrFrame.Mode = AddrModeFlat;

    while (StackWalk(IMAGE_FILE_MACHINE_I386,
        GetCurrentProcess(),
        GetCurrentThread(),
        &frame,
        context,
        0,
        SymFunctionTableAccess,
        SymGetModuleBase,
        0)) {
        addr2line(_program_name, (void*)frame.AddrPC.Offset);
    }

    SymCleanup(GetCurrentProcess());
}

LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS* ExceptionInfo) {
    switch (ExceptionInfo->ExceptionRecord->ExceptionCode) {
    case EXCEPTION_ACCESS_VIOLATION:
        fputs("Error: EXCEPTION_ACCESS_VIOLATION\n", stderr);
        break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        fputs("Error: EXCEPTION_ARRAY_BOUNDS_EXCEEDED\n", stderr);
        break;
    case EXCEPTION_BREAKPOINT:
        fputs("Error: EXCEPTION_BREAKPOINT\n", stderr);
        break;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        fputs("Error: EXCEPTION_DATATYPE_MISALIGNMENT\n", stderr);
        break;
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        fputs("Error: EXCEPTION_FLT_DENORMAL_OPERAND\n", stderr);
        break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        fputs("Error: EXCEPTION_FLT_DIVIDE_BY_ZERO\n", stderr);
        break;
    case EXCEPTION_FLT_INEXACT_RESULT:
        fputs("Error: EXCEPTION_FLT_INEXACT_RESULT\n", stderr);
        break;
    case EXCEPTION_FLT_INVALID_OPERATION:
        fputs("Error: EXCEPTION_FLT_INVALID_OPERATION\n", stderr);
        break;
    case EXCEPTION_FLT_OVERFLOW:
        fputs("Error: EXCEPTION_FLT_OVERFLOW\n", stderr);
        break;
    case EXCEPTION_FLT_STACK_CHECK:
        fputs("Error: EXCEPTION_FLT_STACK_CHECK\n", stderr);
        break;
    case EXCEPTION_FLT_UNDERFLOW:
        fputs("Error: EXCEPTION_FLT_UNDERFLOW\n", stderr);
        break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        fputs("Error: EXCEPTION_ILLEGAL_INSTRUCTION\n", stderr);
        break;
    case EXCEPTION_IN_PAGE_ERROR:
        fputs("Error: EXCEPTION_IN_PAGE_ERROR\n", stderr);
        break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        fputs("Error: EXCEPTION_INT_DIVIDE_BY_ZERO\n", stderr);
        break;
    case EXCEPTION_INT_OVERFLOW:
        fputs("Error: EXCEPTION_INT_OVERFLOW\n", stderr);
        break;
    case EXCEPTION_INVALID_DISPOSITION:
        fputs("Error: EXCEPTION_INVALID_DISPOSITION\n", stderr);
        break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        fputs("Error: EXCEPTION_NONCONTINUABLE_EXCEPTION\n", stderr);
        break;
    case EXCEPTION_PRIV_INSTRUCTION:
        fputs("Error: EXCEPTION_PRIV_INSTRUCTION\n", stderr);
        break;
    case EXCEPTION_SINGLE_STEP:
        fputs("Error: EXCEPTION_SINGLE_STEP\n", stderr);
        break;
    case EXCEPTION_STACK_OVERFLOW:
        fputs("Error: EXCEPTION_STACK_OVERFLOW\n", stderr);
        break;
    default:
        fputs("Error: Unrecognized Exception\n", stderr);
        break;
    }
    fflush(stderr);
    /* If this is a stack overflow then we can't walk the stack, so just show
      where the error happened */
    if (EXCEPTION_STACK_OVERFLOW != ExceptionInfo->ExceptionRecord->ExceptionCode) {
        print_stacktrace(ExceptionInfo->ContextRecord);
    } else {
        addr2line(_program_name, (void*)ExceptionInfo->ContextRecord->Eip);
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

void OS_InstallSignalHandler(char* program_name) {
    strcpy(_program_name, program_name);
    SetUnhandledExceptionFilter(windows_exception_handler);
}

FILE* OS_fopen(const char* pathname, const char* mode) {
    FILE* f;
    errno_t err;

    f = NULL;
    err = fopen_s(&f, pathname, mode);
    if (err != 0) {
        fprintf(stderr, "Failed to open \"%s\"", pathname);
    }

    return f;
}
