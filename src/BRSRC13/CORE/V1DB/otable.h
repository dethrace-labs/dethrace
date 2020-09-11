#ifndef _OTABLE_H_
#define _OTABLE_H_

#include "br_types.h"

br_order_table* BrZsOrderTableAllocate(br_uint_16 size, br_uint_32 flags, br_uint_16 type);

void BrZsOrderTableFree(br_order_table* order_table);

br_order_table* BrZsActorOrderTableSet(br_actor* actor, br_order_table* order_table);

br_order_table* BrZsActorOrderTableGet(br_actor* actor);

br_order_table* BrZsOrderTableClear(br_order_table* order_table);

void BrZsOrderTablePrimitiveInsert(br_order_table* order_table, br_primitive* primitive, br_uint_16 bucket);

br_uint_16 BrZsPrimitiveBucketSelect(br_scalar* z, br_uint_16 type, br_scalar min_z, br_scalar max_z, br_uint_16 size, br_uint_16 sort_type);

void BrZsOrderTablePrimaryEnable(br_order_table* order_table);

void BrZsOrderTablePrimaryDisable();

void InsertOrderTableList(br_order_table* order_table);

void SetOrderTableBounds(br_bounds* bounds, br_order_table* order_table);

void SetOrderTableRange(br_order_table* order_table);

void RenderOrderTableList();

void RenderPrimaryOrderTable();

#endif
