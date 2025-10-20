// Based on https://gist.github.com/jvranish/4441299

#ifdef __TINYC__
/* tcc does potentially not have __int128_t, used in /usr/include/bits/link.h:99 */
#define SUPPORT_TRACEBACK 0
#else
#define SUPPORT_TRACEBACK 1
#endif

#define _GNU_SOURCE
#include "harness/config.h"
#include "harness/os.h"
#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <libgen.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#if SUPPORT_TRACEBACK
#include <execinfo.h>
#include <link.h>
#endif

#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))
#define MAX_STACK_FRAMES 64
static int stack_nbr = 0;
static char _program_name[1024];

static void* stack_traces[MAX_STACK_FRAMES];
static char name_buf[4096];
static DIR* directory_iterator;

struct dl_iterate_callback_data {
    int initialized;
    intptr_t start;
} dethrace_dl_data;

#if SUPPORT_TRACEBACK
static int dl_iterate_callback(struct dl_phdr_info* info, size_t size, void* data) {
    struct dl_iterate_callback_data* callback_data = data;

    if (strcmp(info->dlpi_name, "") == 0) {
        callback_data->start = info->dlpi_addr;
    }
    return 0;
}

static intptr_t get_dethrace_offset(void) {
    if (!dethrace_dl_data.initialized) {
        dethrace_dl_data.initialized = 1;
        dl_iterate_phdr(dl_iterate_callback, &dethrace_dl_data);
    }
    return dethrace_dl_data.start;
}

// Resolve symbol name and source location given the path to the executable and an address
int addr2line(char const* const program_name, void const* const addr) {
    char addr2line_cmd[512];

    /* have addr2line map the address to the related line in the code */
    sprintf(addr2line_cmd, "addr2line -f -p -e %.480s %p", program_name, addr - get_dethrace_offset());

    fprintf(stderr, "%d: ", stack_nbr++);
    return system(addr2line_cmd);
}

static void print_stack_trace(void) {
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
#endif

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
#if SUPPORT_TRACEBACK
    print_stack_trace();
#endif
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
    if (harness_game_config.verbose) {
        if (f == NULL) {
            fprintf(stderr, "Failed to open \"%s\" (%s)\n", pathname, strerror(errno));
        }
    }
    return f;
}

size_t OS_ConsoleReadPassword(char* pBuffer, size_t pBufferLen) {
    struct termios old, new;
    char c;
    size_t len;

    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new);

    len = 0;
    pBuffer[len] = '\0';
    while (1) {
        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == 0x7f) {
                if (len > 0) {
                    len--;
                    pBuffer[len] = '\0';
                    printf("\033[1D \033[1D");
                    fflush(stdout);
                    continue;
                }
            } else if (c == '\r' || c == '\n') {
                printf("\n");
                fflush(stdout);
                break;
            } else if (len < pBufferLen - 1) {
                if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                    pBuffer[len] = c;
                    printf("*");
                    fflush(stdout);
                    len++;
                    pBuffer[len] = '\0';
                }
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    return len;
}

char* OS_Dirname(const char* path) {
    strcpy(name_buf, path);
    return dirname(name_buf);
}

char* OS_Basename(const char* path) {
    strcpy(name_buf, path);
    return basename(name_buf);
}

char* OS_GetWorkingDirectory(char* argv0) {
    return OS_Dirname(argv0);
}

int OS_GetAdapterAddress(char* name, void* pSockaddr_in) {
    struct ifaddrs *ifaddr, *ifa;
    int found = 0;

    // Get the list of network interfaces
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return 0;
    }

    // Loop through all interfaces
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        if (strlen(name) == 0 || strcmp(name, ifa->ifa_name) == 0) {
            if (ifa->ifa_addr->sa_family == AF_INET) {
                *((struct sockaddr_in*)pSockaddr_in) = *(struct sockaddr_in*)ifa->ifa_addr;
                found = 1;
                break;
            }
        }
    }

    // Free the memory allocated by getifaddrs
    freeifaddrs(ifaddr);
    return found;
}

int OS_InitSockets(void) {
    return 0;
}

int OS_GetLastSocketError(void) {
    return errno;
}

void OS_CleanupSockets(void) {
    // no-op
}

int OS_SetSocketNonBlocking(int socket) {
    int flags = fcntl(socket, F_GETFL);
    flags |= O_NONBLOCK;
    return fcntl(socket, F_SETFL, flags);
}

int OS_CloseSocket(int socket) {
    return close(socket);
}
