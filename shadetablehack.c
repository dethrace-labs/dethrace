#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "src/BRSRC13/include/brender/br_types.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

    FILE* f = fopen("/opt/carma1sp/DATA/SHADETAB/STAAAAAA.TAB", "r+");

    for (int row = 8; row < 16; row++) {
        fseek(f, row * 256 + 52, SEEK_SET);
        for (int col = 0; col < 256; col++) {
            char c = (char)255; // (char)row + 16;
            fwrite(&c, 1, 1, f);
        }
    }
    fclose(f);
}
