#include <stdio.h>

#define BYTE_RANGE 256;

typedef struct {
    char id_length;  // length of id field (number of bytes - max 255)
    char map_type;   // colormap field (0 or 1; no map or 256 entry palette)
    char image_type; // ( 0 - no image data included
                     //	  1 - uncompressed, color mapped image
                     //	  2 - uncompressed, RGB image
                     //	  3 - uncompressed, black & white image
                     //	  9 - run-length encoded(RLE-lossless compression),color mapped image
                     //	 10 - RLE, RGB image
                     //	 11 - compressed, black & white image )

    int map_first;       // first entry index for color map
    int map_length;      // total number of entries in color map
    char map_entry_size; // number of bits per entry

    int x; // x cooridinate of origin
    int y; // y cooridinate of origin

    int width;  // width in pixels
    int height; // height in pixels

    char bits_per_pixel; // number of bits per pixel

    char misc; // srsly? "scan origin and alpha bits" this example uses scan origin
               // honestly, don't know what's going on here. we pass in a hex value
               // :shrug_emoji:
} targa_header;

int little_endianify(int number) {
    return number % BYTE_RANGE;
}

int big_endianify(int number) {
    return number / BYTE_RANGE;
}

////// write header function

void write_header(targa_header header, FILE* tga) {
    fputc(header.id_length, tga);
    fputc(header.map_type, tga);
    fputc(header.image_type, tga);

    fputc(little_endianify(header.map_first), tga);
    fputc(big_endianify(header.map_first), tga);

    fputc(little_endianify(header.map_length), tga);
    fputc(big_endianify(header.map_length), tga);

    fputc(header.map_entry_size, tga);

    fputc(little_endianify(header.x), tga);
    fputc(big_endianify(header.x), tga);
    fputc(little_endianify(header.y), tga);
    fputc(big_endianify(header.y), tga);

    fputc(little_endianify(header.width), tga);
    fputc(big_endianify(header.width), tga);
    fputc(little_endianify(header.height), tga);
    fputc(big_endianify(header.height), tga);

    fputc(header.bits_per_pixel, tga);
    fputc(header.misc, tga);
}

int dump_palette(void) {
    FILE* tga;
    targa_header header;

    int block_size = 10;

    header.id_length = 0;
    header.map_type = 1;
    header.image_type = 1; // uncompressed RGB image

    header.map_first = 0;
    header.map_length = 256;
    header.map_entry_size = 24;

    header.x = 0;
    header.y = 0;

    int orig_width = 16;
    int orig_height = 16;
    header.width = orig_width * block_size + orig_width;
    header.height = orig_height * block_size + orig_height + 1;

    header.bits_per_pixel = 8;
    header.misc = 0x20; // scan from upper left corner

    tga = fopen("palette.tga", "wb");
    write_header(header, tga);

    FILE* palfile = fopen("/opt/carma1sp/DATA/REG/PALETTES/DRRENDER.PAL", "rb");
    char pal[256 * 4];
    fseek(palfile, 64, SEEK_SET);
    fread(&pal, 4, 256, palfile);

    for (int i = 0; i < 256; i++) {
        fputc(pal[i * 4 + 3], tga);
        fputc(pal[i * 4 + 2], tga);
        fputc(pal[i * 4 + 1], tga);
        printf("%d: %c %c %c\n", i, pal[i * 4 + 1], pal[i * 4 + 2], pal[i * 4 + 3]);
    }

    for (int black_line = 0; black_line < header.width; black_line++) {
        fputc(0, tga);
    }

    for (int row = 0; row < orig_height; row++) {
        for (int block_line = 0; block_line < block_size; block_line++) {
            for (int col = 0; col < orig_width; col++) {
                for (int b = 0; b < block_size; b++) {
                    fputc(row * orig_width + col, tga);
                }
                fputc(0, tga);
            }
        }
        for (int black_line = 0; black_line < header.width; black_line++) {
            fputc(0, tga);
        }
    }
    fclose(tga);
    return 0;
}

int dump_shadetable(void) {
    FILE* tga;
    targa_header header;

    int block_size = 10;

    header.id_length = 0;
    header.map_type = 1;
    header.image_type = 1; // uncompressed RGB image

    header.map_first = 0;
    header.map_length = 256;
    header.map_entry_size = 24;

    header.x = 0;
    header.y = 0;

    int orig_width = 256;
    int orig_height = 16;
    header.width = orig_width * block_size + orig_width;
    header.height = orig_height * block_size + orig_height + 1;

    header.bits_per_pixel = 8;
    header.misc = 0x20; // scan from upper left corner

    tga = fopen("shadetable.tga", "wb");
    write_header(header, tga);

    FILE* palfile = fopen("/opt/carma1sp/DATA/REG/PALETTES/DRRENDER.PAL", "rb");
    char pal[256 * 4];
    fseek(palfile, 64, SEEK_SET);
    fread(&pal, 4, 256, palfile);

    for (int i = 0; i < 256; i++) {
        fputc(pal[i * 4 + 3], tga);
        fputc(pal[i * 4 + 2], tga);
        fputc(pal[i * 4 + 1], tga);
    }

    FILE* stfile = fopen("/opt/carma1sp/DATA/SHADETAB/STAAAAAA.TAB", "rb");
    fseek(stfile, 52, SEEK_SET);
    char st[256 * 16];
    fread(&st, 256 * 16, 1, stfile);

    for (int black_line = 0; black_line < header.width; black_line++) {
        fputc(0, tga);
    }

    for (int row = 0; row < orig_height; row++) {
        for (int block_line = 0; block_line < block_size; block_line++) {
            for (int col = 0; col < orig_width; col++) {
                for (int b = 0; b < block_size; b++) {
                    fputc(st[row * orig_width + col], tga);
                }
                fputc(0, tga);
            }
        }
        for (int black_line = 0; black_line < header.width; black_line++) {
            fputc(0, tga);
        }
    }
    fclose(tga);
    return 0;
}

int main() {
    // dump_palette();
    dump_shadetable();
    return 0;
}
