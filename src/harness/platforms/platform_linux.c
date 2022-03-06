#include "platforms/platform.h"

#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define TRACER_PID_STRING "TracerPid:"

int PlatformIsDebuggerPresent() {
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

    for (char_ptr = tracer_pid_ptr + sizeof(TRACER_PID_STRING) - 1; char_ptr <= buf + num_read; ++char_ptr)
    {
        if (isspace(*char_ptr)) {
            continue;
        } else {
            return isdigit(*char_ptr) != 0 && *char_ptr != '0';
        }
    }

    return 0;
}
