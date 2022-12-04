// Based on https://gist.github.com/jvranish/4441299

#define _GNU_SOURCE
#include "harness/os.h"
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <link.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))

static int stack_nbr = 0;
static char _program_name[1024];
#define MAX_STACK_FRAMES 64
static void* stack_traces[MAX_STACK_FRAMES];
#define TRACER_PID_STRING "TracerPid:"
DIR* directory_iterator;
uint32_t first_clock_time = 0;

struct dl_iterate_callback_data {
    int initialized;
    intptr_t start;
} dethrace_dl_data;

static int dl_iterate_callback(struct dl_phdr_info* info, size_t size, void* data) {
    struct dl_iterate_callback_data* callback_data = data;

    if (strcmp(info->dlpi_name, "") == 0) {
        callback_data->start = info->dlpi_addr;
    }
    return 0;
}

static intptr_t get_dethrace_offset() {
    if (!dethrace_dl_data.initialized) {
        dethrace_dl_data.initialized = 1;
        dl_iterate_phdr(dl_iterate_callback, &dethrace_dl_data);
    }
    return dethrace_dl_data.start;
}

uint32_t OS_GetTime() {
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    if (first_clock_time == 0) {
        first_clock_time = spec.tv_sec * 1000 + spec.tv_nsec / 1000000;
    }
    return (spec.tv_sec * 1000 + spec.tv_nsec / 1000000) - first_clock_time;
}

void OS_Sleep(int delay_ms) {
    struct timespec ts;
    ts.tv_sec = delay_ms / 1000;
    ts.tv_nsec = (delay_ms % 1000) * 1000000;
    nanosleep(&ts, &ts);
}

char* OS_GetFirstFileInDirectory(char* path) {
    directory_iterator = opendir(path);
    if (directory_iterator == NULL) {
        return NULL;
    }
    return OS_GetNextFileInDirectory();
}

char* OS_GetNextFileInDirectory(void) {
    struct dirent* entry;

    if (directory_iterator == NULL) {
        return NULL;
    }
    while ((entry = readdir(directory_iterator)) != NULL) {
        if (entry->d_type == DT_REG) {
            return entry->d_name;
        }
    }
    closedir(directory_iterator);
    directory_iterator = NULL;
    return NULL;
}

void OS_Basename(char* path, char* base) {
    strcpy(base, basename(path));
}

int OS_IsDebuggerPresent() {
    char buf[4096];
    int status_fd;
    ssize_t num_read;
    char* tracer_pid_ptr;
    char* char_ptr;

    status_fd = open("/proc/self/status", O_RDONLY);
    if (status_fd == -1) {
        return 0;
    }

    num_read = read(status_fd, buf, sizeof(buf) - 1);
    close(status_fd);
    if (num_read <= 0) {
        return 0;
    }

    buf[num_read] = '\0';
    tracer_pid_ptr = strstr(buf, TRACER_PID_STRING);
    if (tracer_pid_ptr == NULL) {
        return 0;
    }

    for (char_ptr = tracer_pid_ptr + sizeof(TRACER_PID_STRING) - 1; char_ptr <= buf + num_read; ++char_ptr) {
        if (isspace(*char_ptr)) {
            continue;
        } else {
            return isdigit(*char_ptr) != 0 && *char_ptr != '0';
        }
    }

    return 0;
}

// Resolve symbol name and source location given the path to the executable and an address
int addr2line(char const* const program_name, void const* const addr) {
    char addr2line_cmd[512] = { 0 };

    /* have addr2line map the address to the related line in the code */
    sprintf(addr2line_cmd, "addr2line -f -p -e %.256s %p", program_name, addr - get_dethrace_offset());

    fprintf(stderr, "%d: ", stack_nbr++);
    return system(addr2line_cmd);
}

void print_stack_trace() {
    int i, trace_size = 0;
    char** messages = (char**)NULL;

    fputs("\nStack trace:\n", stderr);

    trace_size = backtrace(stack_traces, MAX_STACK_FRAMES);
    messages = backtrace_symbols(stack_traces, trace_size);

    /* skip the first couple stack frames (as they are this function and
     our handler) and also skip the last frame as it's (always?) junk. */
    for (i = 3; i < (trace_size - 1); ++i) {
        if (addr2line(_program_name, stack_traces[i]) != 0) {
            printf("  error determining line # for: %s\n", messages[i]);
        }
    }
    if (messages) {
        free(messages);
    }
}

static void signal_handler(int sig, siginfo_t* siginfo, void* context) {
    (void)context;
    fputs("\n******************\n", stderr);

    switch (sig) {
    case SIGSEGV:
        fputs("Caught SIGSEGV\n", stderr);
        break;
    case SIGINT:
        fputs("Caught SIGINT\n", stderr);
        break;
    case SIGFPE:
        switch (siginfo->si_code) {
        case FPE_INTDIV:
            fputs("Caught SIGFPE: FPE_INTDIV\n", stderr);
            break;
        case FPE_INTOVF:
            fputs("Caught SIGFPE: FPE_INTOVF\n", stderr);
            break;
        case FPE_FLTDIV:
            fputs("Caught SIGFPE: FPE_FLTDIV\n", stderr);
            break;
        case FPE_FLTOVF:
            fputs("Caught SIGFPE: FPE_FLTOVF\n", stderr);
            break;
        case FPE_FLTUND:
            fputs("Caught SIGFPE: FPE_FLTUND\n", stderr);
            break;
        case FPE_FLTRES:
            fputs("Caught SIGFPE: FPE_FLTRES\n", stderr);
            break;
        case FPE_FLTINV:
            fputs("Caught SIGFPE: FPE_FLTINV\n", stderr);
            break;
        case FPE_FLTSUB:
            fputs("Caught SIGFPE: FPE_FLTSUB\n", stderr);
            break;
        default:
            fputs("Caught SIGFPE: Arithmetic Exception\n", stderr);
            break;
        }
        break;
    case SIGILL:
        switch (siginfo->si_code) {
        case ILL_ILLOPC:
            fputs("Caught SIGILL: ILL_ILLOPC\n", stderr);
            break;
        case ILL_ILLOPN:
            fputs("Caught SIGILL: ILL_ILLOPN\n", stderr);
            break;
        case ILL_ILLADR:
            fputs("Caught SIGILL: ILL_ILLADR\n", stderr);
            break;
        case ILL_ILLTRP:
            fputs("Caught SIGILL: ILL_ILLTRP\n", stderr);
            break;
        case ILL_PRVOPC:
            fputs("Caught SIGILL: ILL_PRVOPC\n", stderr);
            break;
        case ILL_PRVREG:
            fputs("Caught SIGILL: ILL_PRVREG\n", stderr);
            break;
        case ILL_COPROC:
            fputs("Caught SIGILL: ILL_COPROC\n", stderr);
            break;
        case ILL_BADSTK:
            fputs("Caught SIGILL: ILL_BADSTK\n", stderr);
            break;
        default:
            fputs("Caught SIGILL: Illegal Instruction\n", stderr);
            break;
        }
        break;
    case SIGTERM:
        fputs("Caught SIGTERM\n", stderr);
        break;
    case SIGABRT:
        fputs("Caught SIGABRT\n", stderr);
        break;
    default:
        break;
    }
    fputs("******************\n", stderr);
    print_stack_trace();
    exit(1);
}

void resolve_full_path(char* path, const char* argv0) {
    if (argv0[0] == '/') { // run with absolute path
        strcpy(path, argv0);
    } else { // run with relative path
        if (NULL == getcwd(path, PATH_MAX)) {
            perror("getcwd error");
            return;
        }
        strcat(path, "/");
        strcat(path, argv0);
    }
}

void OS_InstallSignalHandler(char* program_name) {
    resolve_full_path(_program_name, program_name);

    /* setup alternate stack */
    {
        stack_t ss = {};
        ss.ss_sp = malloc(2 * MINSIGSTKSZ);
        if (ss.ss_sp == NULL) {
            err(1, "malloc");
        }
        ss.ss_size = 2 * MINSIGSTKSZ;
        ss.ss_flags = 0;

        if (sigaltstack(&ss, NULL) != 0) {
            err(1, "sigaltstack");
        }
    }

    /* register our signal handlers */
    {
        struct sigaction sig_action = {};
        sig_action.sa_sigaction = signal_handler;
        sigemptyset(&sig_action.sa_mask);

        sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;

        if (sigaction(SIGSEGV, &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
        if (sigaction(SIGFPE, &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
        if (sigaction(SIGINT, &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
        if (sigaction(SIGILL, &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
        if (sigaction(SIGTERM, &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
        if (sigaction(SIGABRT, &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
    }
}

FILE* OS_fopen(const char* pathname, const char* mode) {
    FILE* f = fopen(pathname, mode);
    if (f != NULL) {
        return f;
    }
    char buffer[512];
    char buffer2[512];
    strcpy(buffer, pathname);
    strcpy(buffer2, pathname);
    char* pDirName = dirname(buffer);
    char* pBaseName = basename(buffer2);
    DIR* pDir = opendir(pDirName);
    if (pDir == NULL) {
        return NULL;
    }
    for (struct dirent* pDirent = readdir(pDir); pDirent != NULL; pDirent = readdir(pDir)) {
        if (strcasecmp(pBaseName, pDirent->d_name) == 0) {
            strcat(pDirName, "/");
            strcat(pDirName, pDirent->d_name);
            f = fopen(pDirName, mode);
            break;
        }
    }
    closedir(pDir);
    return f;
}

void OS_AllocateActionReplayBuffer(char** pBuffer, unsigned* pBuffer_size) {
    static int allocated = 0;
    static char* buffer = NULL;
    static unsigned buffer_size = 0;
    unsigned i;
    const int wanted_sizes[] = {
        20000000,
        16000000,
        6000000,
        4000000,
        500000,
    };

    if (!allocated) {
        allocated = 1;
        buffer_size = 0;
        for (i = 0; i < ARRAY_SIZE(wanted_sizes); i++) {
            buffer = malloc(wanted_sizes[i]);
            if (buffer != NULL) {
                buffer_size = wanted_sizes[i];
                break;
            }
        }
    }
    *pBuffer = buffer;
    *pBuffer_size = buffer_size;
}
