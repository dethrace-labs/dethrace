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
#ifndef WIN32PORTS_STRINGS_H
#define WIN32PORTS_STRINGS_H

#ifndef _WIN32

#pragma message("this strings.h implementation is for Windows only!")

#elif defined __MINGW32__

#include <../include/strings.h>

#else

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <ctype.h>
#include <string.h>

#define bcmp(s1, s2, n) memcmp(s1, s2, n)
#define bcopy(s, d, n) memcpy(d, s, n)
#define bzero(s, n) memset(s, 0, n)

inline void explicit_bzero(void *s, size_t n)
{
    volatile char *vs = (volatile char *)s;
    while (n) {
        *vs++ = 0;
        n--;
    }
}

#define index(s, c) strchr(s, c)
#define rindex(s, c) strrchr(s, c)

inline int ffs(int i)
{
    int bit;

    if (0 == i)
        return 0;

    for (bit = 1; !(i & 1); ++bit)
        i >>= 1;
    return bit;
}

inline int ffsl(long i)
{
    int bit;

    if (0 == i)
        return 0;

    for (bit = 1; !(i & 1); ++bit)
        i >>= 1;
    return bit;
}

inline int ffsll(long long i)
{
    int bit;

    if (0 == i)
        return 0;

    for (bit = 1; !(i & 1); ++bit)
        i >>= 1;
    return bit;
}

#define strcasecmp(s1, s2) _stricmp(s1, s2)
#define strncasecmp(s1, s2, n) _strnicmp(s1, s2, n)
#define strcasecmp_l(s1, s2, loc) _stricmp_l(s1, s2, loc)
#define strncasecmp_l(s1, s2, n, loc) _strnicmp_l(s1, s2, n, loc)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _WIN32 */

#endif /* WIN32PORTS_STRINGS_H */
