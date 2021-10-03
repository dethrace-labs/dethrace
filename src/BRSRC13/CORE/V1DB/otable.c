#include "otable.h"
#include "harness/trace.h"

// IDA: br_order_table* __cdecl BrZsOrderTableAllocate(br_uint_16 size, br_uint_32 flags, br_uint_16 type)
br_order_table* BrZsOrderTableAllocate(br_uint_16 size, br_uint_32 flags, br_uint_16 type) {
    br_order_table* order_table;
    LOG_TRACE("(%d, %d, %d)", size, flags, type);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZsOrderTableFree(br_order_table *order_table)
void BrZsOrderTableFree(br_order_table* order_table) {
    LOG_TRACE("(%p)", order_table);
    NOT_IMPLEMENTED();
}

// IDA: br_order_table* __cdecl BrZsActorOrderTableSet(br_actor *actor, br_order_table *order_table)
br_order_table* BrZsActorOrderTableSet(br_actor* actor, br_order_table* order_table) {
    LOG_TRACE("(%p, %p)", actor, order_table);
    NOT_IMPLEMENTED();
}

// IDA: br_order_table* __cdecl BrZsActorOrderTableGet(br_actor *actor)
br_order_table* BrZsActorOrderTableGet(br_actor* actor) {
    LOG_TRACE("(%p)", actor);
    NOT_IMPLEMENTED();
}

// IDA: br_order_table* __cdecl BrZsOrderTableClear(br_order_table *order_table)
br_order_table* BrZsOrderTableClear(br_order_table* order_table) {
    LOG_TRACE("(%p)", order_table);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZsOrderTablePrimitiveInsert(br_order_table *order_table, br_primitive *primitive, br_uint_16 bucket)
void BrZsOrderTablePrimitiveInsert(br_order_table* order_table, br_primitive* primitive, br_uint_16 bucket) {
    LOG_TRACE("(%p, %p, %d)", order_table, primitive, bucket);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_16 __cdecl BrZsPrimitiveBucketSelect(br_scalar *z, br_uint_16 type, br_scalar min_z, br_scalar max_z, br_uint_16 size, br_uint_16 sort_type)
br_uint_16 BrZsPrimitiveBucketSelect(br_scalar* z, br_uint_16 type, br_scalar min_z, br_scalar max_z, br_uint_16 size, br_uint_16 sort_type) {
    br_uint_16 bucket;
    br_scalar zprim;
    br_scalar range;
    br_scalar scale;
    LOG_TRACE("(%p, %d, %f, %f, %d, %d)", z, type, min_z, max_z, size, sort_type);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZsOrderTablePrimaryEnable(br_order_table *order_table)
void BrZsOrderTablePrimaryEnable(br_order_table* order_table) {
    LOG_TRACE("(%p)", order_table);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZsOrderTablePrimaryDisable()
void BrZsOrderTablePrimaryDisable() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall InsertOrderTableList(br_order_table *order_table@<EAX>)
void InsertOrderTableList(br_order_table* order_table) {
    br_order_table* previous_table;
    br_order_table* current_table;
    LOG_TRACE("(%p)", order_table);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetOrderTableBounds(br_bounds *bounds@<EAX>, br_order_table *order_table@<EDX>)
void SetOrderTableBounds(br_bounds* bounds, br_order_table* order_table) {
    br_uint_32 i;
    br_scalar range;
    br_scalar element;
    br_scalar min_z;
    br_scalar max_z;
    br_vector3* min;
    br_vector3* max;
    LOG_TRACE("(%p, %p)", bounds, order_table);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetOrderTableRange(br_order_table *order_table@<EAX>)
void SetOrderTableRange(br_order_table* order_table) {
    br_scalar range;
    LOG_TRACE("(%p)", order_table);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RenderOrderTableList()
void RenderOrderTableList() {
    br_order_table* order_table;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RenderPrimaryOrderTable()
void RenderPrimaryOrderTable() {
    br_uint_16 m;
    br_uint_16 size;
    br_scalar bucket_size;
    br_scalar min_z;
    br_scalar max_z;
    br_primitive* prim;
    br_primitive** bucket;
    br_order_table* order_table;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
