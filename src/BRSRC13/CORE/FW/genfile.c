#include "genfile.h"
#include "harness/trace.h"

#include "CORE/V1DB/chunkids.h"
#include <stddef.h>
#include <stdlib.h>

br_file_enum_member file_type_FM[10] = {
    { FILE_TYPE_NONE, "FILE_TYPE_NONE" },
    { FILE_TYPE_ACTORS, "FILE_TYPE_ACTORS" },
    { FILE_TYPE_PIXELMAP, "FILE_TYPE_PIXELMAP" },
    { FILE_TYPE_LIGHT, "FILE_TYPE_LIGHT" },
    { FILE_TYPE_CAMERA, "FILE_TYPE_CAMERA" },
    { FILE_TYPE_MATERIAL_OLD, "FILE_TYPE_MATERIAL_OLD" },
    { FILE_TYPE_MODEL, "FILE_TYPE_MODEL" },
    { FILE_TYPE_ANIMATION, "FILE_TYPE_ANIMATION" },
    { FILE_TYPE_TREE, "FILE_TYPE_TREE" },
    { FILE_TYPE_MATERIAL, "FILE_TYPE_MATERIAL" },
};
br_file_enum file_type_F = { BR_ASIZE(file_type_FM), file_type_FM };

br_file_struct_member file_info_FM[2] = {
    { 15, offsetof(file_info, type), "type", &file_type_F },
    { 5, offsetof(file_info, version), "version", NULL },
};
br_file_struct file_info_F = { "file_info", BR_ASIZE(file_info_FM), file_info_FM, sizeof(file_info) };

// IDA: int __usercall FopRead_END@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_END(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    return 1;
}

// IDA: int __usercall FopWrite_END@<EAX>(br_datafile *df@<EAX>)
int FopWrite_END(br_datafile* df) {
    LOG_TRACE("(%p)", df);

    df->prims->chunk_write(df, CHUNKID_END, 0);
}

// IDA: int __usercall FopWrite_FILE_INFO@<EAX>(br_datafile *df@<EAX>, br_uint_32 type@<EDX>)
int FopWrite_FILE_INFO(br_datafile* df, br_uint_32 type) {
    file_info fi;
    LOG_TRACE("(%p, %d)", df, type);

    fi.type = type;
    fi.version = 2;
    df->prims->chunk_write(df, CHUNKID_FILE_INFO, df->prims->struct_size(df, &file_info_F, &fi));
    df->prims->struct_write(df, &file_info_F, &fi);
    return 0;
}
