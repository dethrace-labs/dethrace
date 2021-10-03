#ifndef _REGISTER_H_
#define _REGISTER_H_

#include "brender/br_types.h"

void* BrRegistryNew(br_registry* reg);

void* BrRegistryClear(br_registry* reg);

void* BrRegistryAdd(br_registry* reg, void* item);

int BrRegistryAddMany(br_registry* reg, void** items, int n);

void* BrRegistryRemove(br_registry* reg, void* item);

int BrRegistryRemoveMany(br_registry* reg, void** items, int n);

void* BrRegistryFind(br_registry* reg, char* pattern);

int BrRegistryFindMany(br_registry* reg, char* pattern, void** items, int max);

int BrRegistryCount(br_registry* reg, char* pattern);

int BrRegistryEnum(br_registry* reg, char* pattern, br_enum_cbfn* callback, void* arg);

void* BrRegistryNewStatic(br_registry* reg, br_registry_entry* base, int limit);

void* BrRegistryAddStatic(br_registry* reg, br_registry_entry* base, void* item);

void* BrRegistryRemoveStatic(br_registry* reg, void* item);

#endif
