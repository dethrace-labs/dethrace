#ifndef _DATAFILE_H_
#define _DATAFILE_H_

#include "br_types.h"

void DfPush(int type, void* value, int count);

void* DfPop(int type, int* countp);

void* DfTop(int type, int* countp);

int DfTopType();

int TextReadLine(br_datafile* df, char** ident, char** data);

br_uint_16 scalarTypeConvert(br_datafile* df, br_uint_16 t);

br_uint_32 DfStructWriteBinary(br_datafile* df, br_file_struct* str, void* base);

br_uint_32 DfStructReadBinary(br_datafile* df, br_file_struct* str, void* base);

int DfStructSizeBinary(br_datafile* df, br_file_struct* str, void* base);

int EnumFromString(br_file_enum* e, char* str);

char* EnumToString(br_file_enum* e, int num);

br_uint_32 DfStructWriteText(br_datafile* df, br_file_struct* str, void* base);

br_uint_32 StructWriteTextSub(br_datafile* df, br_file_struct* str, void* base, int indent);

br_uint_32 DfStructReadText(br_datafile* df, br_file_struct* str, void* base);

br_uint_32 StructReadTextSub(br_datafile* df, br_file_struct* str, void* base);

int DfStructSizeText(br_datafile* df, br_file_struct* str, void* base);

br_uint_32 DfStructWriteArray(br_datafile* df, br_file_struct* str, void* base, int n);

br_uint_32 DfStructReadArray(br_datafile* df, br_file_struct* str, void* base, int n);

int DfChunkWriteText(br_datafile* df, br_uint_32 id, br_uint_32 length);

int DfChunkReadText(br_datafile* df, br_uint_32* plength);

int DfChunkWriteBinary(br_datafile* df, br_uint_32 id, br_uint_32 length);

int DfChunkReadBinary(br_datafile* df, br_uint_32* plength);

void DfCountWriteText(br_datafile* df, br_uint_32 count);

br_uint_32 DfCountReadText(br_datafile* df);

void DfCountWriteBinary(br_datafile* df, br_uint_32 count);

br_uint_32 DfCountReadBinary(br_datafile* df);

int DfCountSizeText(br_datafile* df);

int DfCountSizeBinary(br_datafile* df);

br_uint_8* BlockWriteSetup(void* base, int block_size, int block_stride, int block_count, int size);

int DfBlockWriteText(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size);

void* DfBlockReadText(br_datafile* df, void* base, int* count, int size, int mtype);

int DfBlockWriteBinary(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size);

void* DfBlockReadBinary(br_datafile* df, void* base, int* count, int size, int mtype);

int DfBlockSizeText(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size);

int DfBlockSizeBinary(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size);

char* DfNameReadText(br_datafile* df, char* name);

int DfNameWriteText(br_datafile* df, char* name);

int DfNameSizeText(br_datafile* df, char* name);

char* DfNameReadBinary(br_datafile* df, char* name);

int DfNameWriteBinary(br_datafile* df, char* name);

int DfNameSizeBinary(br_datafile* df, char* name);

int DfSkipText(br_datafile* df, br_uint_32 length);

int DfSkipBinary(br_datafile* df, br_uint_32 length);

int DfChunksInterpret(br_datafile* df, br_chunks_table* table);

void BrNullOther();

int DfFileIdentify(br_uint_8* magics, br_size_t n_magics);

br_datafile* DfOpen(char* name, int write, br_token scalar_type);

void DfClose(br_datafile* df);

int BrWriteModeSet(int mode);

#endif
