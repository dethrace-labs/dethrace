#include "otable.h"

char rscid[50];

// Offset: 23
// Size: 216
br_order_table* BrZsOrderTableAllocate(br_uint_16 size, br_uint_32 flags, br_uint_16 type) {
    br_order_table* order_table;
}

// Offset: 258
// Size: 70
void BrZsOrderTableFree(br_order_table* order_table) {
}

// Offset: 351
// Size: 76
br_order_table* BrZsActorOrderTableSet(br_actor* actor, br_order_table* order_table) {
}

// Offset: 450
// Size: 70
br_order_table* BrZsActorOrderTableGet(br_actor* actor) {
}

// Offset: 540
// Size: 98
br_order_table* BrZsOrderTableClear(br_order_table* order_table) {
}

// Offset: 668
// Size: 168
void BrZsOrderTablePrimitiveInsert(br_order_table* order_table, br_primitive* primitive, br_uint_16 bucket) {
}

// Offset: 862
// Size: 634
br_uint_16 BrZsPrimitiveBucketSelect(br_scalar* z, br_uint_16 type, br_scalar min_z, br_scalar max_z, br_uint_16 size, br_uint_16 sort_type) {
    br_uint_16 bucket;
    br_scalar zprim;
    br_scalar range;
    br_scalar scale;
}

// Offset: 1524
// Size: 56
void BrZsOrderTablePrimaryEnable(br_order_table* order_table) {
}

// Offset: 1609
// Size: 40
void BrZsOrderTablePrimaryDisable() {
}

// Offset: 1670
// Size: 236
// EAX: order_table
void InsertOrderTableList(br_order_table* order_table) {
    br_order_table* previous_table;
    br_order_table* current_table;
}

// Offset: 1926
// Size: 275
// EAX: bounds
// EDX: order_table
void SetOrderTableBounds(br_bounds* bounds, br_order_table* order_table) {
    br_uint_32 i;
    br_scalar range;
    br_scalar element;
    br_scalar min_z;
    br_scalar max_z;
    br_vector3* min;
    br_vector3* max;
}

// Offset: 2220
// Size: 175
// EAX: order_table
void SetOrderTableRange(br_order_table* order_table) {
    br_scalar range;
}

// Offset: 2416
// Size: 111
void RenderOrderTableList() {
}

// Offset: 2551
// Size: 553
void RenderPrimaryOrderTable() {
    br_uint_16 size;
    br_scalar bucket_size;
    br_scalar min_z;
    br_scalar max_z;
    br_primitive* prim;
    br_primitive** bucket;
    br_order_table* order_table;
}
