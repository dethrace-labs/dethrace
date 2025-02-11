// Based on https://gist.github.com/jvranish/4441299

// this has to be first
#include <windows.h>
//

#include <dbghelp.h>

#include "harness/config.h"
#include "harness/os.h"
#include "harness/trace.h"

#include <errno.h> /* errno, strerror */
#include <io.h>    /* _access_s, F_OK */
#include <stddef.h>
#include <stdio.h>  /* errno_t, FILE, fgets, fopen_s, fprintf*/
#include <stdlib.h> /* _splitpath */
#include <string.h> /* strcpy, strerror, strlen, strrchr */

#ifndef F_OK
#define F_OK 0
#endif

void dr_dprintf(char* fmt_string, ...);

static int stack_nbr = 0;
static char windows_program_name[1024];
static char path_addr2line[1024];

static char dirname_buf[_MAX_DIR];
static char fname_buf[_MAX_FNAME];

HANDLE directory_handle = NULL;
char last_found_file[260];

#if defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(__i386) || defined(_M_IX86)
#define DETHRACE_CPU_X86 1
#elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#define DETHRACE_CPU_X64 1
#elif defined(__aarch64__) || defined(_M_ARM64)
#define DETHRACE_CPU_ARM64 1
#elif defined(__arm__) || defined(_M_ARM)
#define DETHRACE_CPU_ARM32 1
#endif

#if !(defined(DETHRACE_CPU_X86) || defined(DETHRACE_CPU_X64) || defined(DETHRACE_CPU_ARM32) || defined(DETHRACE_CPU_ARM64))
#pragma message("Unsupported architecture: don't know how to StackWalk")
#endif

static BOOL print_addr2line_address_location(HANDLE const hProcess, const DWORD64 address) {
    char addr2line_cmd[1024] = { 0 };
    const char* program_name = windows_program_name;
    IMAGEHLP_MODULE64 module_info;

    if (path_addr2line[0] == '\0') {
        return FALSE;
    }

    memset(&module_info, 0, sizeof(module_info));
    module_info.SizeOfStruct = sizeof(module_info);
    if (SymGetModuleInfo64(hProcess, address, &module_info)) {
        program_name = module_info.ImageName;
    }

    sprintf(addr2line_cmd, "\"%.256s\" -f -p -e %.256s %lx", path_addr2line, program_name, (long int)address);

    system(addr2line_cmd);
    return TRUE;
}

static void printf_windows_message(const char* format, ...) {
    va_list ap;
    char win_msg[512];
    FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        win_msg, sizeof(win_msg) / sizeof(*win_msg),
        NULL);
    size_t win_msg_len = strlen(win_msg);
    while (win_msg[win_msg_len - 1] == '\r' || win_msg[win_msg_len - 1] == '\n' || win_msg[win_msg_len - 1] == ' ') {
        win_msg[win_msg_len - 1] = '\0';
        win_msg_len--;
    }
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, " (%s)\n", win_msg);
}

static void init_dbghelp(HANDLE const hProcess) {

    if (!SymInitialize(hProcess, NULL, FALSE)) {
        printf_windows_message("SymInitialize failed");
    }

    if (!SymRefreshModuleList(hProcess)) {
        printf_windows_message("SymRefreshModuleList failed");
    }
}

static void cleanup_dbghelp(HANDLE const hProcess) {

    SymCleanup(hProcess);
}

static BOOL print_dbghelp_address_location(HANDLE const hProcess, const DWORD64 address) {
    IMAGEHLP_MODULE64 module_info;
    union {
        char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(CHAR)];
        SYMBOL_INFO symbol_info;
    } symbol;
    DWORD64 dwDisplacement;
    DWORD lineColumn = 0;
    IMAGEHLP_LINE64 line;
    const char* image_file_name;
    const char* symbol_name;
    const char* file_name;
    char line_number[16];

    memset(&module_info, 0, sizeof(module_info));
    module_info.SizeOfStruct = sizeof(module_info);
    if (!SymGetModuleInfo64(hProcess, address, &module_info)) {
        return FALSE;
    }
    image_file_name = OS_Basename(module_info.ImageName);

    memset(&symbol, 0, sizeof(symbol));
    symbol.symbol_info.SizeOfStruct = sizeof(symbol.symbol_info);
    symbol.symbol_info.MaxNameLen = MAX_SYM_NAME;
    if (!SymFromAddr(hProcess, address, &dwDisplacement, &symbol.symbol_info)) {
        return FALSE;
    }
    symbol_name = symbol.symbol_info.Name;

    line.SizeOfStruct = sizeof(line);
    if (SymGetLineFromAddr64(hProcess, address, &lineColumn, &line)) {
        file_name = line.FileName;
        snprintf(line_number, sizeof(line_number), "Line %u", (unsigned int)line.LineNumber);
    } else {
        file_name = "";
        line_number[0] = '\0';
    }

    fprintf(stderr, "0x%lx %s!%s+0x%lx %s %s\n", (long unsigned int)address, image_file_name, symbol_name, (long unsigned int)dwDisplacement, file_name, line_number);
    return TRUE;
}

static void print_address_location(HANDLE hProcess, DWORD64 address) {
    IMAGEHLP_MODULE64 module_info;

    fprintf(stderr, "%d: ", stack_nbr++);
    if (print_dbghelp_address_location(hProcess, address)) {
        return;
    }
    if (print_addr2line_address_location(hProcess, address)) {
        return;
    }

    memset(&module_info, 0, sizeof(module_info));
    module_info.SizeOfStruct = sizeof(module_info);
    if (SymGetModuleInfo64(hProcess, address, &module_info)) {
        fprintf(stderr, "%s 0x%lx\n", module_info.ImageName, (long unsigned int)address);
        return;
    }
    fprintf(stderr, "0x%lx\n", (long unsigned int)address);
}

static void print_stacktrace(CONTEXT* context) {
    HANDLE hProcess = GetCurrentProcess();

    init_dbghelp(hProcess);

    STACKFRAME frame = { 0 };

    /* setup initial stack frame */
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrStack.Mode = AddrModeFlat;
    frame.AddrFrame.Mode = AddrModeFlat;
#if defined(DETHRACE_CPU_X86)
    DWORD machine_type = IMAGE_FILE_MACHINE_I386;
    frame.AddrFrame.Offset = context->Ebp;
    frame.AddrStack.Offset = context->Esp;
    frame.AddrPC.Offset = context->Eip;
#elif defined(DETHRACE_CPU_X64)
    DWORD machine_type = IMAGE_FILE_MACHINE_AMD64;
    frame.AddrFrame.Offset = context->Rbp;
    frame.AddrStack.Offset = context->Rsp;
    frame.AddrPC.Offset = context->Rip;
#elif defined(DETHRACE_CPU_ARM32)
    DWORD machine_type = IMAGE_FILE_MACHINE_ARM;
    frame.AddrFrame.Offset = context->Lr;
    frame.AddrStack.Offset = context->Sp;
    frame.AddrPC.Offset = context->Pc;
#elif defined(DETHRACE_CPU_ARM64)
    DWORD machine_type = IMAGE_FILE_MACHINE_ARM64;
    frame.AddrFrame.Offset = context->Fp;
    frame.AddrStack.Offset = context->Sp;
    frame.AddrPC.Offset = context->Pc;
#else
    fprintf(stderr, "Unsupported architecture: cannot produce a stacktrace\n");
#endif

    while (StackWalk(machine_type,
        GetCurrentProcess(),
        GetCurrentThread(),
        &frame,
        context,
        0,
        SymFunctionTableAccess,
        SymGetModuleBase,
        0)) {

        if (frame.AddrPC.Offset == frame.AddrReturn.Offset) {
            fprintf(stderr, "PC == Return Address => Possible endless callstack\n");
            break;
        }

        print_address_location(hProcess, frame.AddrPC.Offset);
    }
}

static LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS* ExceptionInfo) {
    HANDLE hProcess;
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
    hProcess = GetCurrentProcess();
    init_dbghelp(hProcess);
    /* If this is a stack overflow then we can't walk the stack, so just show
      where the error happened */
    if (EXCEPTION_STACK_OVERFLOW != ExceptionInfo->ExceptionRecord->ExceptionCode) {
        print_stacktrace(ExceptionInfo->ContextRecord);
    } else {
#if defined(DETHRACE_CPU_X86)
        DWORD64 addr = (DWORD64)ExceptionInfo->ContextRecord->Eip;
#elif defined(DETHRACE_CPU_X64)
        DWORD64 addr = (DWORD64)ExceptionInfo->ContextRecord->Rip;
#elif defined(DETHRACE_CPU_ARM32) || defined(DETHRACE_CPU_ARM64)
        DWORD64 addr = (DWORD64)ExceptionInfo->ContextRecord->Pc;
#endif
        print_address_location(hProcess, addr);
    }
    cleanup_dbghelp(hProcess);

    return EXCEPTION_EXECUTE_HANDLER;
}

void OS_InstallSignalHandler(char* program_name) {
    const char* env_addr2line;

    path_addr2line[0] = '\0';
    env_addr2line = getenv("ADDR2LINE");
    if (env_addr2line != NULL) {
        errno_t e = _access_s(env_addr2line, F_OK);
        if (e == 0) {
            strcpy(path_addr2line, env_addr2line);
        } else {
            fprintf(stderr, "ADDR2LINE does not exist (%s)\n", path_addr2line);
        }
    }
    strcpy(windows_program_name, program_name);
    SetUnhandledExceptionFilter(windows_exception_handler);
}

char* OS_GetFirstFileInDirectory(char* path) {
    char with_extension[256];
    WIN32_FIND_DATA find_data;

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

FILE* OS_fopen(const char* pathname, const char* mode) {
    FILE* f;
    errno_t err;

    f = NULL;
    err = fopen_s(&f, pathname, mode);
    if (harness_game_config.verbose) {
        if (err != 0) {
            fprintf(stderr, "Failed to open \"%s\" (%s)\r\n", pathname, strerror(err));
        }
    }

    return f;
}

size_t OS_ConsoleReadPassword(char* pBuffer, size_t pBufferLen) {
    // FIXME: unsafe implementation (echos the password)
    pBuffer[0] = '\0';
    fgets(pBuffer, pBufferLen, stdin);
    return strlen(pBuffer);
}

char* OS_Dirname(const char* path) {
    _splitpath(path, NULL, dirname_buf, NULL, NULL);
    return dirname_buf;
}

char* OS_Basename(const char* path) {
    _splitpath(path, NULL, NULL, fname_buf, NULL);
    return fname_buf;
}

char* OS_GetWorkingDirectory(char* argv0) {
    return OS_Dirname(argv0);
}
