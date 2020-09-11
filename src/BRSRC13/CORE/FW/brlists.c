#include "brlists.h"
#include "harness.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

char rscid[49];

// IDA: void __cdecl BrNewList(br_list *list)
void BrNewList(br_list* list) {
    list->_null = NULL;
    list->tail = (br_node*)list;
    list->head = (br_node*)&list->_null;
}

// IDA: void __cdecl BrAddHead(br_list *list, br_node *node)
void BrAddHead(br_list* list, br_node* node) {
    assert(node != NULL);
    assert(list != NULL);
    assert(list->head != NULL);
    node->prev = (br_node*)list;
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
}

// IDA: void __cdecl BrAddTail(br_list *list, br_node *node)
void BrAddTail(br_list* list, br_node* node) {
    LOG_TRACE("(%p, %p)", list, node);
    NOT_IMPLEMENTED();
}

// IDA: br_node* __cdecl BrRemHead(br_list *list)
br_node* BrRemHead(br_list* list) {
    br_node *n;
    LOG_TRACE("(%p)", list);
    NOT_IMPLEMENTED();
}

// IDA: br_node* __cdecl BrRemTail(br_list *list)
br_node* BrRemTail(br_list* list) {
    br_node *n;
    LOG_TRACE("(%p)", list);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrInsert(br_list *list, br_node *here, br_node *node)
void BrInsert(br_list* list, br_node* here, br_node* node) {
    LOG_TRACE("(%p, %p, %p)", list, here, node);
    NOT_IMPLEMENTED();
}

// IDA: br_node* __cdecl BrRemove(br_node *node)
br_node* BrRemove(br_node* node) {
    LOG_TRACE("(%p)", node);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrSimpleNewList(br_simple_list *list)
void BrSimpleNewList(br_simple_list* list) {
    LOG_TRACE10("(%p)", list);
    list->head = NULL;
}

// IDA: void __cdecl BrSimpleAddHead(br_simple_list *list, br_simple_node *node)
void BrSimpleAddHead(br_simple_list* list, br_simple_node* node) {
    node->next = list->head;
    node->prev = (br_simple_node**)list;
    if (node->next) {
        node->next->prev = (br_simple_node**)node;
    }
    list->head = node;
}

// IDA: br_simple_node* __cdecl BrSimpleRemHead(br_simple_list *list)
br_simple_node* BrSimpleRemHead(br_simple_list* list) {
    br_simple_node *node;
    LOG_TRACE("(%p)", list);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrSimpleInsert(br_simple_list *list, br_simple_node *here, br_simple_node *node)
void BrSimpleInsert(br_simple_list* list, br_simple_node* here, br_simple_node* node) {
    LOG_TRACE("(%p, %p, %p)", list, here, node);
    NOT_IMPLEMENTED();
}

// IDA: br_simple_node* __cdecl BrSimpleRemove(br_simple_node *node)
br_simple_node* BrSimpleRemove(br_simple_node* node) {
    *node->prev = node->next;
    if (node->next) {
        node->next->prev = node->prev;
    }
    node->next = NULL;
    node->prev = NULL;
    return node;
}
