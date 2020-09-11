#ifndef _BRLISTS_H_
#define _BRLISTS_H_

#include "br_types.h"

void BrNewList(br_list* list);

void BrAddHead(br_list* list, br_node* node);

void BrAddTail(br_list* list, br_node* node);

br_node* BrRemHead(br_list* list);

br_node* BrRemTail(br_list* list);

void BrInsert(br_list* list, br_node* here, br_node* node);

br_node* BrRemove(br_node* node);

void BrSimpleNewList(br_simple_list* list);

void BrSimpleAddHead(br_simple_list* list, br_simple_node* node);

br_simple_node* BrSimpleRemHead(br_simple_list* list);

void BrSimpleInsert(br_simple_list* list, br_simple_node* here, br_simple_node* node);

br_simple_node* BrSimpleRemove(br_simple_node* node);

#endif
