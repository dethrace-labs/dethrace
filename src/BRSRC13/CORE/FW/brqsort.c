#include "brqsort.h"

// Global variables
char rscid[49];

// Offset: 8
// Size: 560
void BrQsort(void* basep, unsigned int nelems, unsigned int size, br_qsort_cbfn comp) {
    char* stack[40];
    char** sp;
    char* i;
    char* j;
    char* limit;
    unsigned int thresh;
    char* base;
    unsigned int width;
    void (*swap_func)(char*, char*, unsigned int);

    width = size;
    swap_func = swap_chars;

    if (size == 4) { // Do we have exactly 1 32bit in worth of data at a time?
        swap_func = swap_int_1;
    } else if (!(size & 3)) { // Is our data divisible by 4 so can be swapped 32bit at a time?
        width >>= 2;
        swap_func = swap_ints;
    }

    base = (char*)basep;
    thresh = 7 * size;
    sp = stack;

    for (limit = (char*)basep + size * nelems;; limit = sp[1]) {
        while (limit - base > thresh) {
            swap_func(
                &base[size * ((limit - base) / size >> 1)],
                base,
                width);
            i = &base[size];
            j = &limit[-size];

            if (comp(&base[size], &limit[-size]) > 0) {
                swap_func(i, j, width);
            }

            if (comp(base, j) > 0) {
                swap_func(base, j, width);
            }

            if (comp(i, base) > 0) {
                swap_func(i, base, width);
            }

            while (1) {
                do {
                    i += size;
                } while (comp(i, base) < 0);

                do {
                    j -= size;
                } while (comp(j, base) > 0);

                if (i > j) {
                    break;
                }

                swap_func(i, j, width);
            }

            swap_func(base, j, width);

            if (j - base <= limit - i) {
                *sp = i;
                sp[1] = limit;
                limit = j;
            } else {
                *sp = base;
                sp[1] = j;
                base = i;
            }

            sp += 2;
        }

        j = base;

        for (i = &base[size]; i < limit; i += size) {
            while (comp(j, &j[size]) > 0) {
                swap_func(j, &j[size], width);

                if (j == base) {
                    break;
                }

                j -= size;
            }

            j = i;
        }

        if (stack == sp) {
            break;
        }

        sp -= 2;
        base = *sp;
    }
}

// Offset: 579
// Size: 81
// EAX: a
// EDX: b
// EBX: nbytes
void swap_chars(char* a, char* b, unsigned int nbytes) {
    char tmp;

    do {
        tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    } while (--nbytes);
}

// Offset: 670
// Size: 95
// EAX: ap
// EDX: bp
// EBX: nints
void swap_ints(char* ap, char* bp, unsigned int nints) {
    int* a = (int*)ap;
    int* b = (int*)bp;
    int tmp;

    do {
        tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    } while (--nints);
}

// Offset: 776
// Size: 85
// EAX: ap
// EDX: bp
// EBX: nints
void swap_int_1(char* ap, char* bp, unsigned int nints) {
    int* a = (int*)ap;
    int* b = (int*)bp;
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}
