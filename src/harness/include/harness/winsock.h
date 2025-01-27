/*
 * Inspiration taken from https://github.com/TheAssemblyArmada/Vanilla-Conquer/blob/vanilla/common/sockets.h
 */

#ifndef HARNESS_WINSOCK_H
#define HARNESS_WINSOCK_H

#include <assert.h>

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>

#else /* Assume posix style sockets on non-windows */

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h> // for getaddrinfo() and freeaddrinfo()
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // for close()

#define WSAEISCONN EISCONN
#define WSAEINPROGRESS EINPROGRESS
#define WSAEALREADY EALREADY
#define WSAEADDRINUSE EADDRINUSE
#define WSAEADDRNOTAVAIL EADDRNOTAVAIL
#define WSAEBADF EBADF
#define WSAECONNREFUSED ECONNREFUSED
#define WSAEINTR EINTR
#define WSAENOTSOCK ENOTSOCK
#define WSAEWOULDBLOCK EWOULDBLOCK
#define WSAEINVAL EINVAL
#define WSAETIMEDOUT ETIMEDOUT

#define MAKEWORD(x, y) ((y) << 8 | (x))

typedef struct WSADATA {
} WSADATA;

int WSAStartup(int version, WSADATA* data);
int WSAGetLastError(void);
int WSACleanup(void);
int ioctlsocket(int handle, long cmd, unsigned long* argp);
int closesocket(int handle);

#endif /* _WIN32 */

#endif
