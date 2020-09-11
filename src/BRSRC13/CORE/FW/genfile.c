#include "genfile.h"
#include "harness.h"
#include <stdlib.h>

br_file_struct_member file_info_FM[2];
br_file_struct file_info_F;
br_file_enum_member file_type_FM[10];
br_file_enum file_type_F;
char rscid[50];

// IDA: int __usercall FopRead_END@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_END(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    return 1;
}

// IDA: int __usercall FopWrite_END@<EAX>(br_datafile *df@<EAX>)
int FopWrite_END(br_datafile* df) {
    LOG_TRACE("(%p)", df);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopWrite_FILE_INFO@<EAX>(br_datafile *df@<EAX>, br_uint_32 type@<EDX>)
int FopWrite_FILE_INFO(br_datafile* df, br_uint_32 type) {
    file_info fi;
    LOG_TRACE("(%p, %d)", df, type);
    NOT_IMPLEMENTED();
}
