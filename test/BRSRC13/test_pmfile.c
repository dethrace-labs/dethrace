#include "tests.h"

#include "CORE/FW/datafile.h"
#include "CORE/FW/resource.h"
#include "CORE/PIXELMAP/pixelmap.h"
#include "CORE/PIXELMAP/pmfile.h"

uint8_t binary_pixelmap_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x1a,
    0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x04, 0x00, 0x04, 0x00, 0x05, 0x00,
    0x00, 0x74, 0x65, 0x73, 0x74, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6c, 0x6d,
    0x61, 0x70, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x88, 0x00,
    0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03, 0x00,
    0x04, 0x00, 0x04, 0x00, 0x05, 0x00, 0x05, 0x00, 0x06, 0x00, 0x06, 0x00,
    0x07, 0x00, 0x07, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02,
    0x02, 0x01, 0x03, 0x03, 0x03, 0x01, 0x04, 0x04, 0x04, 0x01, 0x05, 0x05,
    0x05, 0x01, 0x06, 0x06, 0x06, 0x01, 0x07, 0x07, 0x07, 0x01, 0x08, 0x00,
    0x00, 0x02, 0x02, 0x02, 0x01, 0x02, 0x03, 0x04, 0x02, 0x02, 0x04, 0x06,
    0x03, 0x02, 0x05, 0x08, 0x04, 0x02, 0x06, 0x0a, 0x05, 0x02, 0x07, 0x0c,
    0x06, 0x02, 0x08, 0x0e, 0x07, 0x02, 0x09, 0x00, 0x00, 0x03, 0x03, 0x03,
    0x01, 0x03, 0x04, 0x06, 0x02, 0x03, 0x05, 0x09, 0x03, 0x03, 0x06, 0x0c,
    0x04, 0x03, 0x07, 0x0f, 0x05, 0x03, 0x08, 0x12, 0x06, 0x03, 0x09, 0x15,
    0x07, 0x03, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
char *text_pixelmap_lone =
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_PIXELMAP                       # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*PIXELMAP         9" HOST_NL
    "  struct    br_pixelmap" HOST_NL
    "    enum_8    BR_PMT_RGBA_8888                         # type" HOST_NL
    "    uint_16   32                                       # row_bytes" HOST_NL
    "    uint_16   8                                        # width" HOST_NL
    "    uint_16   4                                        # height" HOST_NL
    "    uint_16   4                                        # origin_x" HOST_NL
    "    uint_16   5                                        # origin_y" HOST_NL
    "    uint_16   0                                        # mip_offset" HOST_NL
    "    asciz     \"test pixelmap\"                          # identifier" HOST_NL
    "*PIXELS           6" HOST_NL
    "  block 32" HOST_NL
    "  size 4" HOST_NL
    "    00000000: 0000000000010001000200020003000300040004000500050006000600070007" HOST_NL
    "    00000020: 0000010101010102020201030303010404040105050501060606010707070108" HOST_NL
    "    00000040: 00000202020102030402020406030205080402060a0502070c0602080e070209" HOST_NL
    "    00000060: 000003030301030406020305090303060c0403070f050308120603091507030a" HOST_NL
    "*END              0" HOST_NL;
static void test_pmfile_save_load_pixelmap_lone() {
    br_pixelmap *pixelmap_ref;
    int res;
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    br_colour *rgb_pixels;
    br_pixelmap* pixelmaps_read[2];
    int i;
    int j;

    pixelmap_ref = BrPixelmapAllocate(BR_PMT_RGBA_8888, 8, 4, NULL, BR_PMAF_NORMAL);
    pixelmap_ref->identifier = BrResStrDup(pixelmap_ref, "test pixelmap");
    pixelmap_ref->origin_x = 4;
    pixelmap_ref->origin_y = 5;

    // Don't use BrPixelmapPixelSet to avoid dependencies
    rgb_pixels = pixelmap_ref->pixels;
    for (j = 0; j < pixelmap_ref->height; j++) {
        for (i = 0; i < pixelmap_ref->width; i++) {
            rgb_pixels[j*pixelmap_ref->width+i] = BR_COLOUR_RGBA(i, j, i+j, i*j);
        }
    }

    create_temp_file(tmpfilename_binary, "pixelmap_lone_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrPixelmapSave(tmpfilename_binary, pixelmap_ref);
    TEST_ASSERT_EQUAL_INT32(1, res);

    create_temp_file(tmpfilename_text, "pixelmap_lone_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrPixelmapSave(tmpfilename_text, pixelmap_ref);
    TEST_ASSERT_EQUAL_INT32(1, res);
    
    LOG_DEBUG("Read binary pixelmap");
    pixelmaps_read[0] = BrPixelmapLoad(tmpfilename_binary);
    LOG_DEBUG("Read text pixelmap");
    pixelmaps_read[1] = BrPixelmapLoad(tmpfilename_text);
    
    for (i = 0; i < BR_ASIZE(pixelmaps_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_ASSERT_NOT_NULL(pixelmaps_read[i]);
        TEST_ASSERT_EQUAL_STRING(pixelmap_ref->identifier, pixelmaps_read[i]->identifier);
        TEST_ASSERT_EQUAL_UINT16(pixelmap_ref->width, pixelmaps_read[i]->width);
        TEST_ASSERT_EQUAL_UINT16(pixelmap_ref->height, pixelmaps_read[i]->height);
        TEST_ASSERT_EQUAL_UINT16(pixelmap_ref->origin_x, pixelmaps_read[i]->origin_x);
        TEST_ASSERT_EQUAL_UINT16(pixelmap_ref->origin_y, pixelmaps_read[i]->origin_y);
        TEST_ASSERT_EQUAL_UINT32(pixelmap_ref->origin_y, pixelmaps_read[i]->origin_y);
        TEST_ASSERT_EQUAL_UINT32(pixelmap_ref->mip_offset, pixelmaps_read[i]->mip_offset);
        TEST_ASSERT_EQUAL_UINT8(pixelmap_ref->type, pixelmaps_read[i]->type);
        TEST_ASSERT_NULL(pixelmaps_read[i]->map);

        TEST_ASSERT_EQUAL_UINT32_ARRAY(pixelmap_ref->pixels, pixelmaps_read[i]->pixels, pixelmap_ref->width * pixelmap_ref->height);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_pixelmap_lone, tmpfilename_binary, sizeof(binary_pixelmap_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_pixelmap_lone, tmpfilename_text);

    BrResFree(pixelmaps_read[0]);
    BrResFree(pixelmaps_read[1]);
}

uint8_t binary_pixelmap_mapped[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x21,
    0x03, 0x00, 0x08, 0x00, 0x08, 0x00, 0x04, 0x00, 0x04, 0x00, 0x05, 0x00,
    0x00, 0x74, 0x65, 0x73, 0x74, 0x20, 0x6d, 0x61, 0x70, 0x70, 0x65, 0x64,
    0x20, 0x70, 0x69, 0x78, 0x65, 0x6c, 0x6d, 0x61, 0x70, 0x00, 0x00, 0x00,
    0x00, 0x3d, 0x00, 0x00, 0x00, 0x15, 0x07, 0x00, 0x40, 0x00, 0x10, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6e, 0x69, 0x63, 0x65, 0x20,
    0x6d, 0x61, 0x70, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x48,
    0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x01, 0x01, 0x00, 0x02, 0x02, 0x02, 0x00, 0x03, 0x03, 0x03,
    0x00, 0x04, 0x04, 0x04, 0x00, 0x05, 0x05, 0x05, 0x00, 0x06, 0x06, 0x06,
    0x00, 0x07, 0x07, 0x07, 0x00, 0x08, 0x08, 0x08, 0x00, 0x09, 0x09, 0x09,
    0x00, 0x0a, 0x0a, 0x0a, 0x00, 0x0b, 0x0b, 0x0b, 0x00, 0x0c, 0x0c, 0x0c,
    0x00, 0x0d, 0x0d, 0x0d, 0x00, 0x0e, 0x0e, 0x0e, 0x00, 0x0f, 0x0f, 0x0f,
    0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21,
    0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
char *text_pixelmap_mapped =
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_PIXELMAP                       # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*PIXELMAP         9" HOST_NL
    "  struct    br_pixelmap" HOST_NL
    "    enum_8    BR_PMT_INDEX_8                           # type" HOST_NL
    "    uint_16   8                                        # row_bytes" HOST_NL
    "    uint_16   8                                        # width" HOST_NL
    "    uint_16   4                                        # height" HOST_NL
    "    uint_16   4                                        # origin_x" HOST_NL
    "    uint_16   5                                        # origin_y" HOST_NL
    "    uint_16   0                                        # mip_offset" HOST_NL
    "    asciz     \"test mapped pixelmap\"                   # identifier" HOST_NL
    "*PIXELMAP         9" HOST_NL
    "  struct    br_pixelmap" HOST_NL
    "    enum_8    BR_PMT_RGBX_888                          # type" HOST_NL
    "    uint_16   64                                       # row_bytes" HOST_NL
    "    uint_16   16                                       # width" HOST_NL
    "    uint_16   1                                        # height" HOST_NL
    "    uint_16   0                                        # origin_x" HOST_NL
    "    uint_16   0                                        # origin_y" HOST_NL
    "    uint_16   0                                        # mip_offset" HOST_NL
    "    asciz     \"nice map\"                               # identifier" HOST_NL
    "*PIXELS           4" HOST_NL
    "  block 16" HOST_NL
    "  size 4" HOST_NL
    "    00000000: 0000000000010101000202020003030300040404000505050006060600070707" HOST_NL
    "    00000020: 0008080800090909000a0a0a000b0b0b000c0c0c000d0d0d000e0e0e000f0f0f" HOST_NL
    "*ADD_MAP          0" HOST_NL
    "*PIXELS           3" HOST_NL
    "  block 32" HOST_NL
    "  size 1" HOST_NL
    "    00000000: 0001020304050607101112131415161720212223242526273031323334353637" HOST_NL
    "*END              0" HOST_NL;
static void test_pmfile_save_load_pixelmap_mapped() {
    br_pixelmap *pixelmap_ref;
    br_pixelmap *map_ref;
    int res;
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    br_colour *rgb_pixels;
    br_uint_8 *indices;
    br_pixelmap* pixelmaps_read[2];
    int i;
    int j;

    pixelmap_ref = BrPixelmapAllocate(BR_PMT_INDEX_8, 8, 4, NULL, BR_PMAF_NORMAL);
    pixelmap_ref->identifier = BrResStrDup(pixelmap_ref, "test mapped pixelmap");
    pixelmap_ref->origin_x = 4;
    pixelmap_ref->origin_y = 5;
    map_ref = BrPixelmapAllocate(BR_PMT_RGBX_888, 16, 1, NULL, BR_PMAF_NORMAL);
    BrResAdd(pixelmap_ref, map_ref);
    pixelmap_ref->map = map_ref;
    pixelmap_ref->map->identifier = BrResStrDup(pixelmap_ref->map, "nice map");

    // Don't use BrPixelmapPixelSet to avoid dependencies
    indices = pixelmap_ref->pixels;
    for (j = 0; j < pixelmap_ref->height; j++) {
        for (i = 0; i < pixelmap_ref->width; i++) {
            indices[j*pixelmap_ref->width+i] = j * 16 + i;
        }
    }

    // Don't use BrPixelmapPixelSet to avoid dependencies
    rgb_pixels = map_ref->pixels;
    for (i = 0; i < map_ref->width; i++) {
        rgb_pixels[i] = BR_COLOUR_RGBA(i, i, i, 0);
    }

    create_temp_file(tmpfilename_binary, "pixelmap_map_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrPixelmapSave(tmpfilename_binary, pixelmap_ref);
    TEST_ASSERT_EQUAL_INT32(1, res);

    create_temp_file(tmpfilename_text, "pixelmap_map_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrPixelmapSave(tmpfilename_text, pixelmap_ref);
    TEST_ASSERT_EQUAL_INT32(1, res);

    LOG_DEBUG("Read binary pixelmap");
    pixelmaps_read[0] = BrPixelmapLoad(tmpfilename_binary);
    LOG_DEBUG("Read text pixelmap");
    pixelmaps_read[1] = BrPixelmapLoad(tmpfilename_text);
    
    for (i = 0; i < BR_ASIZE(pixelmaps_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_ASSERT_NOT_NULL(pixelmaps_read[i]);
        TEST_ASSERT_EQUAL_STRING(pixelmap_ref->identifier, pixelmaps_read[i]->identifier);
        TEST_ASSERT_EQUAL_UINT16(pixelmap_ref->width, pixelmaps_read[i]->width);
        TEST_ASSERT_EQUAL_UINT16(pixelmap_ref->height, pixelmaps_read[i]->height);
        TEST_ASSERT_EQUAL_UINT16(pixelmap_ref->origin_x, pixelmaps_read[i]->origin_x);
        TEST_ASSERT_EQUAL_UINT16(pixelmap_ref->origin_y, pixelmaps_read[i]->origin_y);
        TEST_ASSERT_EQUAL_UINT32(pixelmap_ref->origin_y, pixelmaps_read[i]->origin_y);
        TEST_ASSERT_EQUAL_UINT32(pixelmap_ref->mip_offset, pixelmaps_read[i]->mip_offset);
        TEST_ASSERT_EQUAL_UINT8(pixelmap_ref->type, pixelmaps_read[i]->type);

        TEST_ASSERT_EQUAL_UINT8_ARRAY(pixelmap_ref->pixels, pixelmaps_read[i]->pixels, pixelmap_ref->width * pixelmap_ref->height);
        TEST_ASSERT_EQUAL_UINT32_ARRAY(pixelmap_ref->map->pixels, pixelmaps_read[i]->map->pixels, pixelmap_ref->map->width * pixelmap_ref->map->height);
    }

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_pixelmap_mapped, tmpfilename_binary, sizeof(binary_pixelmap_mapped));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_pixelmap_mapped, tmpfilename_text);

    BrResFree(pixelmaps_read[0]);
    BrResFree(pixelmaps_read[1]);
    BrResFree(pixelmap_ref);    
}

uint8_t binary_pixelmap_mip[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x21,
    0x03, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x04, 0x00, 0x05, 0x00,
    0x01, 0x74, 0x65, 0x73, 0x74, 0x20, 0x6d, 0x69, 0x70, 0x70, 0x65, 0x64,
    0x20, 0x70, 0x69, 0x78, 0x65, 0x6c, 0x6d, 0x61, 0x70, 0x00, 0x00, 0x00,
    0x00, 0x21, 0x00, 0x00, 0x00, 0x5d, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
    0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21,
    0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
    0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,
    0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51,
    0x52, 0x53, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
char *text_pixelmap_mip =
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_PIXELMAP                       # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*PIXELMAP         9" HOST_NL
    "  struct    br_pixelmap" HOST_NL
    "    enum_8    BR_PMT_INDEX_8                           # type" HOST_NL
    "    uint_16   8                                        # row_bytes" HOST_NL
    "    uint_16   8                                        # width" HOST_NL
    "    uint_16   8                                        # height" HOST_NL
    "    uint_16   4                                        # origin_x" HOST_NL
    "    uint_16   5                                        # origin_y" HOST_NL
    "    uint_16   1                                        # mip_offset" HOST_NL
    "    asciz     \"test mipped pixelmap\"                   # identifier" HOST_NL
    "*PIXELS           5" HOST_NL
    "  block 85" HOST_NL
    "  size 1" HOST_NL
    "    00000000: 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f" HOST_NL
    "    00000020: 202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f" HOST_NL
    "    00000040: 404142434445464748494a4b4c4d4e4f5051525354" HOST_NL
    "*END              0" HOST_NL;
static void test_pmfile_save_load_pixelmap_mip() {
    br_pixelmap *pixelmap_ref;
    int res;
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    br_pixelmap *pixelmaps_read[2];
    br_uint_8 *indices;
    int i;
    int mip_size;

    pixelmap_ref = BrPixelmapAllocate(BR_PMT_INDEX_8, 8, 8, NULL, BR_PMAF_NO_PIXELS);
    pixelmap_ref->identifier = BrResStrDup(pixelmap_ref, "test mipped pixelmap");
    pixelmap_ref->origin_x = 4;
    pixelmap_ref->origin_y = 5;
    pixelmap_ref->mip_offset = 1;

    mip_size = 0;
    for (int i = pixelmap_ref->width; i != 0; i >>=1) {
        mip_size += i * i;
    }
    LOG_DEBUG("mip size=%d", mip_size);

    pixelmap_ref->pixels = BrResAllocate(pixelmap_ref, mip_size * sizeof(br_uint_8), BR_MEMORY_PIXELS);

    // Don't use BrPixelmapPixelSet to avoid dependencies
    indices = pixelmap_ref->pixels;
    for (i = 0; i < mip_size; i++) {
        indices[i] = i;
    }

    create_temp_file(tmpfilename_binary, "pixelmap_mip_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrPixelmapSave(tmpfilename_binary, pixelmap_ref);
    TEST_ASSERT_EQUAL_INT32(1, res);

    create_temp_file(tmpfilename_text, "pixelmap_mip_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrPixelmapSave(tmpfilename_text, pixelmap_ref);
    TEST_ASSERT_EQUAL_INT32(1, res);

    LOG_DEBUG("Read binary pixelmap");
    pixelmaps_read[0] = BrPixelmapLoad(tmpfilename_binary);
    LOG_DEBUG("Read text pixelmap");
    pixelmaps_read[1] = BrPixelmapLoad(tmpfilename_text);
    
    for (i = 0; i < BR_ASIZE(pixelmaps_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_ASSERT_NOT_NULL(pixelmaps_read[i]);
        TEST_ASSERT_EQUAL_STRING(pixelmap_ref->identifier, pixelmaps_read[i]->identifier);
        TEST_ASSERT_EQUAL_UINT16(pixelmap_ref->width, pixelmaps_read[i]->width);
        TEST_ASSERT_EQUAL_UINT16(pixelmap_ref->height, pixelmaps_read[i]->height);
        TEST_ASSERT_EQUAL_UINT16(pixelmap_ref->origin_x, pixelmaps_read[i]->origin_x);
        TEST_ASSERT_EQUAL_UINT16(pixelmap_ref->origin_y, pixelmaps_read[i]->origin_y);
        TEST_ASSERT_EQUAL_UINT32(pixelmap_ref->origin_y, pixelmaps_read[i]->origin_y);
        TEST_ASSERT_EQUAL_UINT32(pixelmap_ref->mip_offset, pixelmaps_read[i]->mip_offset);
        TEST_ASSERT_EQUAL_UINT8(pixelmap_ref->type, pixelmaps_read[i]->type);
        TEST_ASSERT_NULL(pixelmaps_read[i]->map);

        TEST_ASSERT_EQUAL_UINT8_ARRAY(pixelmap_ref->pixels, pixelmaps_read[i]->pixels, mip_size);
    }

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_pixelmap_mip, tmpfilename_binary, sizeof(binary_pixelmap_mip));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_pixelmap_mip, tmpfilename_text);

    BrResFree(pixelmaps_read[0]);
    BrResFree(pixelmaps_read[1]);
    BrResFree(pixelmap_ref);    
}


uint8_t binary_pixelmap_many[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x1e,
    0x08, 0x00, 0x10, 0x00, 0x04, 0x00, 0x04, 0x00, 0x01, 0x00, 0x02, 0x00,
    0x00, 0x74, 0x65, 0x73, 0x74, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6c, 0x6d,
    0x61, 0x70, 0x20, 0x75, 0x6e, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00,
    0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00,
    0x03, 0x00, 0x03, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02,
    0x02, 0x01, 0x03, 0x03, 0x03, 0x01, 0x04, 0x00, 0x00, 0x02, 0x02, 0x02,
    0x01, 0x02, 0x03, 0x04, 0x02, 0x02, 0x04, 0x06, 0x03, 0x02, 0x05, 0x00,
    0x00, 0x03, 0x03, 0x03, 0x01, 0x03, 0x04, 0x06, 0x02, 0x03, 0x05, 0x09,
    0x03, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x1e, 0x07, 0x00, 0x0c, 0x00, 0x03,
    0x00, 0x03, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x74, 0x65, 0x73, 0x74,
    0x20, 0x70, 0x69, 0x78, 0x65, 0x6c, 0x6d, 0x61, 0x70, 0x20, 0x64, 0x6f,
    0x73, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00,
    0x00, 0x09, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x02, 0x02, 0x02, 0x01, 0x03, 0x00, 0x00, 0x02, 0x02, 0x02, 0x01,
    0x02, 0x03, 0x04, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x1f, 0x08, 0x00,
    0x08, 0x00, 0x02, 0x00, 0x02, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x74,
    0x65, 0x73, 0x74, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6c, 0x6d, 0x61, 0x70,
    0x20, 0x74, 0x72, 0x65, 0x73, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00,
    0x00, 0x18, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3d, 0x00, 0x00, 0x00, 0x21, 0x08, 0x00, 0x0c, 0x00, 0x03, 0x00,
    0x01, 0x00, 0x07, 0x00, 0x08, 0x00, 0x00, 0x74, 0x65, 0x73, 0x74, 0x20,
    0x70, 0x69, 0x78, 0x65, 0x6c, 0x6d, 0x61, 0x70, 0x20, 0x6d, 0x61, 0x72,
    0x69, 0x61, 0x21, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x14,
    0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
char *text_pixelmap_many =
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_PIXELMAP                       # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*PIXELMAP         9" HOST_NL
    "  struct    br_pixelmap" HOST_NL
    "    enum_8    BR_PMT_RGBA_8888                         # type" HOST_NL
    "    uint_16   16                                       # row_bytes" HOST_NL
    "    uint_16   4                                        # width" HOST_NL
    "    uint_16   4                                        # height" HOST_NL
    "    uint_16   1                                        # origin_x" HOST_NL
    "    uint_16   2                                        # origin_y" HOST_NL
    "    uint_16   0                                        # mip_offset" HOST_NL
    "    asciz     \"test pixelmap uno\"                      # identifier" HOST_NL
    "*PIXELS           4" HOST_NL
    "  block 16" HOST_NL
    "  size 4" HOST_NL
    "    00000000: 0000000000010001000200020003000300000101010101020202010303030104" HOST_NL
    "    00000020: 0000020202010203040202040603020500000303030103040602030509030306" HOST_NL
    "*END              0" HOST_NL
    "*PIXELMAP         9" HOST_NL
    "  struct    br_pixelmap" HOST_NL
    "    enum_8    BR_PMT_RGBX_888                          # type" HOST_NL
    "    uint_16   12                                       # row_bytes" HOST_NL
    "    uint_16   3                                        # width" HOST_NL
    "    uint_16   3                                        # height" HOST_NL
    "    uint_16   3                                        # origin_x" HOST_NL
    "    uint_16   4                                        # origin_y" HOST_NL
    "    uint_16   0                                        # mip_offset" HOST_NL
    "    asciz     \"test pixelmap dos\"                      # identifier" HOST_NL
    "*PIXELS           4" HOST_NL
    "  block 9" HOST_NL
    "  size 4" HOST_NL
    "    00000000: 0000000000010001000200020000010101010102020201030000020202010203" HOST_NL
    "    00000020: 04020204" HOST_NL
    "*END              0" HOST_NL
    "*PIXELMAP         9" HOST_NL
    "  struct    br_pixelmap" HOST_NL
    "    enum_8    BR_PMT_RGBA_8888                         # type" HOST_NL
    "    uint_16   8                                        # row_bytes" HOST_NL
    "    uint_16   2                                        # width" HOST_NL
    "    uint_16   2                                        # height" HOST_NL
    "    uint_16   5                                        # origin_x" HOST_NL
    "    uint_16   6                                        # origin_y" HOST_NL
    "    uint_16   0                                        # mip_offset" HOST_NL
    "    asciz     \"test pixelmap tres\"                     # identifier" HOST_NL
    "*PIXELS           3" HOST_NL
    "  block 4" HOST_NL
    "  size 4" HOST_NL
    "    00000000: 00000000000100010000010101010102" HOST_NL
    "*END              0" HOST_NL
    "*PIXELMAP         9" HOST_NL
    "  struct    br_pixelmap" HOST_NL
    "    enum_8    BR_PMT_RGBA_8888                         # type" HOST_NL
    "    uint_16   12                                       # row_bytes" HOST_NL
    "    uint_16   3                                        # width" HOST_NL
    "    uint_16   1                                        # height" HOST_NL
    "    uint_16   7                                        # origin_x" HOST_NL
    "    uint_16   8                                        # origin_y" HOST_NL
    "    uint_16   0                                        # mip_offset" HOST_NL
    "    asciz     \"test pixelmap maria!\"                   # identifier" HOST_NL
    "*PIXELS           3" HOST_NL
    "  block 3" HOST_NL
    "  size 4" HOST_NL
    "    00000000: 000000000001000100020002" HOST_NL
    "*END              0" HOST_NL;
static void test_pmfile_save_load_pixelmap_many() {
#define PIXELMAP_MANY_NB (4)
#define PIXELMAP_MANY_READ_CAPACITY (PIXELMAP_MANY_NB*5)
    br_pixelmap *pixelmaps_ref[PIXELMAP_MANY_NB];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    br_uint_32 *pixels32;
    br_pixelmap* pixelmaps_read[2][PIXELMAP_MANY_NB*5];
    int i;
    int j;
    int k;
    int res;

    pixelmaps_ref[0] = BrPixelmapAllocate(BR_PMT_RGBA_8888, 4, 4, NULL, BR_PMAF_NORMAL);
    pixelmaps_ref[0]->identifier = BrResStrDup(pixelmaps_ref[0], "test pixelmap uno");
    pixelmaps_ref[0]->origin_x = 1;
    pixelmaps_ref[0]->origin_y = 2;

    pixelmaps_ref[1] = BrPixelmapAllocate(BR_PMT_RGBX_888, 3, 3, NULL, BR_PMAF_NORMAL);
    pixelmaps_ref[1]->identifier = BrResStrDup(pixelmaps_ref[0], "test pixelmap dos");
    pixelmaps_ref[1]->origin_x = 3;
    pixelmaps_ref[1]->origin_y = 4;

    pixelmaps_ref[2] = BrPixelmapAllocate(BR_PMT_RGBA_8888, 2, 2, NULL, BR_PMAF_NORMAL);
    pixelmaps_ref[2]->identifier = BrResStrDup(pixelmaps_ref[0], "test pixelmap tres");
    pixelmaps_ref[2]->origin_x = 5;
    pixelmaps_ref[2]->origin_y = 6;

    pixelmaps_ref[3] = BrPixelmapAllocate(BR_PMT_RGBA_8888, 3, 1, NULL, BR_PMAF_NORMAL);
    pixelmaps_ref[3]->identifier = BrResStrDup(pixelmaps_ref[0], "test pixelmap maria!");
    pixelmaps_ref[3]->origin_x = 7;
    pixelmaps_ref[3]->origin_y = 8;

    // Don't use BrPixelmapPixelSet to avoid dependencies
    for (k = 0; k < PIXELMAP_MANY_NB; ++k) {
        pixels32 = pixelmaps_ref[k]->pixels;
        for (j = 0; j < pixelmaps_ref[k]->height; j++) {
            for (i = 0; i < pixelmaps_ref[k]->width; i++) {
                pixels32[j*pixelmaps_ref[k]->width+i] = BR_COLOUR_RGBA(i, j, i+j, i*j);
            }
        }
    }

    create_temp_file(tmpfilename_binary, "pixelmaps_many_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrPixelmapSaveMany(tmpfilename_binary, pixelmaps_ref, PIXELMAP_MANY_NB);
    TEST_ASSERT_EQUAL_INT32(PIXELMAP_MANY_NB, res);

    create_temp_file(tmpfilename_text, "pixelmaps_many_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrPixelmapSaveMany(tmpfilename_text, pixelmaps_ref, PIXELMAP_MANY_NB);
    TEST_ASSERT_EQUAL_INT32(PIXELMAP_MANY_NB, res);
    
    LOG_DEBUG("Read binary pixelmaps");
    res = BrPixelmapLoadMany(tmpfilename_binary, pixelmaps_read[0], PIXELMAP_MANY_READ_CAPACITY);
    TEST_ASSERT_EQUAL_INT32(PIXELMAP_MANY_NB, res);
    LOG_DEBUG("Read text pixelmaps");
    res = BrPixelmapLoadMany(tmpfilename_text, pixelmaps_read[1], PIXELMAP_MANY_READ_CAPACITY);
    TEST_ASSERT_EQUAL_INT32(PIXELMAP_MANY_NB, res);

    for (i = 0; i < 2; i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        for (k = 0; k < PIXELMAP_MANY_NB; k++) {
            TEST_ASSERT_NOT_NULL(pixelmaps_read[i][k]);
            TEST_ASSERT_EQUAL_STRING(pixelmaps_ref[k]->identifier, pixelmaps_read[i][k]->identifier);
            TEST_ASSERT_EQUAL_UINT16(pixelmaps_ref[k]->width, pixelmaps_read[i][k]->width);
            TEST_ASSERT_EQUAL_UINT16(pixelmaps_ref[k]->height, pixelmaps_read[i][k]->height);
            TEST_ASSERT_EQUAL_UINT16(pixelmaps_ref[k]->origin_x, pixelmaps_read[i][k]->origin_x);
            TEST_ASSERT_EQUAL_UINT16(pixelmaps_ref[k]->origin_y, pixelmaps_read[i][k]->origin_y);
            TEST_ASSERT_EQUAL_UINT32(pixelmaps_ref[k]->origin_y, pixelmaps_read[i][k]->origin_y);
            TEST_ASSERT_EQUAL_UINT32(pixelmaps_ref[k]->mip_offset, pixelmaps_read[i][k]->mip_offset);
            TEST_ASSERT_EQUAL_UINT8(pixelmaps_ref[k]->type, pixelmaps_read[i][k]->type);
            TEST_ASSERT_NULL(pixelmaps_read[i][k]->map);

            TEST_ASSERT_EQUAL_UINT32_ARRAY(pixelmaps_ref[k]->pixels, pixelmaps_read[i][k]->pixels, pixelmaps_ref[k]->width * pixelmaps_ref[k]->height);
        }
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_pixelmap_many, tmpfilename_binary, sizeof(binary_pixelmap_many));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_pixelmap_many, tmpfilename_text);

    for (k = 0; k < PIXELMAP_MANY_NB; k++) {
        LOG_DEBUG("Freeing %d", k);
        BrResFree(pixelmaps_ref[k]);
        BrResFree(pixelmaps_read[0][k]);
        BrResFree(pixelmaps_read[1][k]);
    }
}

static void test_pmfile_BrPixelmapLoad() {
    REQUIRES_DATA_DIRECTORY();

    br_pixelmap* pm;
    pm = BrPixelmapLoad("DATA/PIXELMAP/LOADSCRN.PIX");
    TEST_ASSERT_NOT_NULL(pm);
    TEST_ASSERT_EQUAL_STRING("loadscrn.pix", pm->identifier);
    TEST_ASSERT_EQUAL_INT(320, pm->width);
    TEST_ASSERT_EQUAL_INT(320, pm->row_bytes);
    TEST_ASSERT_EQUAL_INT(200, pm->height);
}

void test_pmfile_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_pmfile_save_load_pixelmap_lone);
    RUN_TEST(test_pmfile_save_load_pixelmap_mapped);
    RUN_TEST(test_pmfile_save_load_pixelmap_mip);
    RUN_TEST(test_pmfile_save_load_pixelmap_many);
    RUN_TEST(test_pmfile_BrPixelmapLoad);
}
