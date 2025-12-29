#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {

    int *x = malloc(20);
    *x = 1;
    printf("x: %d\n", *x);
    printf("freeing block...\n");
    free(x);
    printf("x: %d\n", *x);
    return 0;
}
