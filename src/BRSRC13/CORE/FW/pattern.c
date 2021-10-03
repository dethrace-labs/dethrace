#include "pattern.h"
#include "harness/trace.h"
#include <ctype.h>

// IDA: br_boolean __cdecl BrNamePatternMatch(char *p, char *s)
br_boolean BrNamePatternMatch(char* p, char* s) {
    char* cp;
    LOG_TRACE9("(\"%s\", \"%s\")", p, s);

    if (!p) {
        return 1;
    }
    if (!s) {
        return 0;
    }

    for (;; s++, p++) {

        if (*p == 0) {
            // if pattern string ends at the same point as search string, we match
            return *s == 0;
        }

        if (*p == '*') {
            cp = s;
            do {
                if (BrNamePatternMatch(p + 1, cp)) {
                    return 1;
                }
            } while (*cp++);
            return 0;
        }
        if (*p == '/') {
            return *s == 0;
        }
        if (*p == '?') {
            if (*s == 0) {
                return 0;
            }
            continue;
        }

        if (toupper(*p) != toupper(*s)) {
            return 0;
        }
    }
    return 1;
}
