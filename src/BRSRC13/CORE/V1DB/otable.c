#include "otable.h"
#include "harness.h"

char rscid[50];

br_order_table* BrZsOrderTableAllocate(br_uint_16 size, br_uint_32 flags, br_uint_16 type) {
    br_order_table* order_table;
    NOT_IMPLEMENTED();
}

void BrZsOrderTableFree(br_order_table* order_table) {
    NOT_IMPLEMENTED();
}

br_order_table* BrZsActorOrderTableSet(br_actor* actor, br_order_table* order_table) {
    NOT_IMPLEMENTED();
}

br_order_table* BrZsActorOrderTableGet(br_actor* actor) {
    NOT_IMPLEMENTED();
}

br_order_table* BrZsOrderTableClear(br_order_table* order_table) {
    NOT_IMPLEMENTED();
}

void BrZsOrderTablePrimitiveInsert(br_order_table* order_table, br_primitive* primitive, br_uint_16 bucket) {
    NOT_IMPLEMENTED();
}

br_uint_16 BrZsPrimitiveBucketSelect(br_scalar* z, br_uint_16 type, br_scalar min_z, br_scalar max_z, br_uint_16 size, br_uint_16 sort_type) {
    br_uint_16 bucket;
    br_scalar zprim;
    br_scalar range;
    br_scalar scale;
    NOT_IMPLEMENTED();
}

void BrZsOrderTablePrimaryEnable(br_order_table* order_table) {
    NOT_IMPLEMENTED();
}

void BrZsOrderTablePrimaryDisable() {
    NOT_IMPLEMENTED();
}

void InsertOrderTableList(br_order_table* order_table) {
    br_order_table* previous_table;
    br_order_table* current_table;
    NOT_IMPLEMENTED();
}

void SetOrderTableBounds(br_bounds* bounds, br_order_table* order_table) {
    br_uint_32 i;
    br_scalar range;
    br_scalar element;
    br_scalar min_z;
    br_scalar max_z;
    br_vector3* min;
    br_vector3* max;
    NOT_IMPLEMENTED();
}

void SetOrderTableRange(br_order_table* order_table) {
    br_scalar range;
    NOT_IMPLEMENTED();
}

void RenderOrderTableList() {
    NOT_IMPLEMENTED();
}

void RenderPrimaryOrderTable() {
    br_uint_16 size;
    br_scalar bucket_size;
    br_scalar min_z;
    br_scalar max_z;
    br_primitive* prim;
    br_primitive** bucket;
    br_order_table* order_table;
    NOT_IMPLEMENTED();
}
