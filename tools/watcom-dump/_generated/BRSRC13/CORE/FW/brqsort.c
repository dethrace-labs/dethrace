#include "brqsort.h"

char rscid[49];

// Offset: 8
// Size: 560
void BrQsort(void *basep, unsigned int nelems, unsigned int size, br_qsort_cbfn *comp) {
    char *stack[40];
    char **sp;
    char *i;
    char *j;
    char *limit;
    unsigned int thresh;
    char *base;
    unsigned int width;
    void (*swap_func)(char*, char*, unsigned int);
}

// Offset: 579
// Size: 81
// EAX: a
// EDX: b
// EBX: nbytes
void swap_chars(char *a, char *b, unsigned int nbytes) {
    char tmp;
}

// Offset: 670
// Size: 95
// EAX: ap
// EDX: bp
// EBX: nints
void swap_ints(char *ap, char *bp, unsigned int nints) {
    int *a;
    int *b;
    int tmp;
}

// Offset: 776
// Size: 85
// EAX: ap
// EDX: bp
// EBX: nints
void swap_int_1(char *ap, char *bp, unsigned int nints) {
    int *a;
    int *b;
    int tmp;
}

