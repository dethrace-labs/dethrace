/*
MIT License
Copyright (c) 2019 win32ports
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#ifndef WIN32PORTS_DIRENT_H
#define WIN32PORTS_DIRENT_H

#ifndef _WIN32

#pragma message("this dirent.h implementation is for Windows only!")

#elif defined __MINGW32__

#include <../include/dirent.h>

#else

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <sys/types.h>
#include <wchar.h>

#ifndef NAME_MAX
#define NAME_MAX 260
#endif /* NAME_MAX */

#ifndef DT_UNKNOWN
#define DT_UNKNOWN 0
#endif /* DT_UNKNOWN */

#ifndef DT_FIFO
#define DT_FIFO 1
#endif /* DT_FIFO */

#ifndef DT_CHR
#define DT_CHR 2
#endif /* DT_CHR */

#ifndef DT_DIR
#define DT_DIR 4
#endif /* DT_DIR */

#ifndef DT_BLK
#define DT_BLK 6
#endif /* DT_BLK */

#ifndef DT_REG
#define DT_REG 8
#endif /* DT_REF */

#ifndef DT_LNK
#define DT_LNK 10
#endif /* DT_LNK */

#ifndef DT_SOCK
#define DT_SOCK 12
#endif /* DT_SOCK */

#ifndef DT_WHT
#define DT_WHT 14
#endif /* DT_WHT */

#ifndef _DIRENT_HAVE_D_NAMLEN
#define _DIRENT_HAVE_D_NAMLEN 1
#endif /* _DIRENT_HAVE_D_NAMLEN */

#ifndef _DIRENT_HAVE_D_RECLEN
#define _DIRENT_HAVE_D_RECLEN 1
#endif /* _DIRENT_HAVE_D_RECLEN */

#ifndef _DIRENT_HAVE_D_OFF
#define _DIRENT_HAVE_D_OFF 1
#endif /* _DIRENT_HAVE_D_OFF */

#ifndef _DIRENT_HAVE_D_TYPE
#define _DIRENT_HAVE_D_TYPE 1
#endif /* _DIRENT_HAVE_D_TYPE  */

#ifndef NTFS_MAX_PATH
#define NTFS_MAX_PATH 32768
#endif /* NTFS_MAX_PATH */

typedef struct __dir DIR;

typedef struct ino_t
{
    unsigned long long serial;
    unsigned char fileid[16];
} __ino_t;

struct dirent
{
    __ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned char d_namelen;
    unsigned char d_type;
    char d_name[NAME_MAX];
};

int closedir(DIR *dirp);
DIR *opendir(const char *name);
DIR *_wopendir(const wchar_t *name);
DIR *fdopendir(int fd);
struct dirent *readdir(DIR *dirp);
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);
void seekdir(DIR *dirp, long int offset);
void rewinddir(DIR *dirp);
long int telldir(DIR *dirp);
int dirfd(DIR *dirp);
int scandir(const char *dirp,
    struct dirent ***namelist,
    int (*filter)(const struct dirent *),
    int (*compar)(const struct dirent **, const struct dirent **));
int alphasort(const void *a, const void *b);
int versionsort(const void *a, const void *b);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _WIN32 */

#endif /* WIN32PORTS_DIRENT_H */
