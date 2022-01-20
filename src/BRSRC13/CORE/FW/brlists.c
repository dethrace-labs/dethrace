#include "brlists.h"
#include "harness/trace.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

// IDA: void __cdecl BrNewList(br_list *list)
void BrNewList(br_list* list) {
    LOG_TRACE10("(%p)", list);

    list->head = (br_node*)&list->_null;
    list->_null = NULL;
    list->tail = (br_node*)list;
}

// IDA: void __cdecl BrAddHead(br_list *list, br_node *node)
void BrAddHead(br_list* list, br_node* node) {
    LOG_TRACE10("(%p, %p)", list, node);
    assert(node != NULL);
    assert(list != NULL);
    assert(list->head != NULL);

    //node->prev = (br_node*)list;
    node->prev = (br_node*)&(list->head);
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
}

// IDA: void __cdecl BrAddTail(br_list *list, br_node *node)
void BrAddTail(br_list* list, br_node* node) {
    LOG_TRACE10("(%p, %p)", list, node);

    node->next = (br_node*)&list->_null;
    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;
}

// IDA: br_node* __cdecl BrRemHead(br_list *list)
br_node* BrRemHead(br_list* list) {
    br_node* n;
    LOG_TRACE10("(%p)", list);

    n = list->head;
    if (n == (br_node*)&list->_null) {
        return NULL;
    }
    list->head = n->next;
    n->next->prev = (br_node*)&list->head;
    return n;
}

// IDA: br_node* __cdecl BrRemTail(br_list *list)
br_node* BrRemTail(br_list* list) {
    br_node* n;
    LOG_TRACE10("(%p)", list);

    n = list->tail;
    if (n == (br_node*)&list->head) {
        return NULL;
    }
    list->tail = n->prev;
    n->prev->next = (br_node*)&list->_null;
    return n;
}

// IDA: void __cdecl BrInsert(br_list *list, br_node *here, br_node *node)
void BrInsert(br_list* list, br_node* here, br_node* node) {
    LOG_TRACE10("(%p, %p, %p)", list, here, node);

    node->prev = here;
    node->next = here->next;
    here->next->prev = node;
    here->next = node;
}

// IDA: br_node* __cdecl BrRemove(br_node *node)
br_node* BrRemove(br_node* node) {
    LOG_TRACE10("(%p)", node);

    node->next->prev = node->prev;
    node->prev->next = node->next;
    return node;
}

// IDA: void __cdecl BrSimpleNewList(br_simple_list *list)
void BrSimpleNewList(br_simple_list* list) {
    LOG_TRACE10("(%p)", list);
    list->head = NULL;
}

// IDA: void __cdecl BrSimpleAddHead(br_simple_list *list, br_simple_node *node)
void BrSimpleAddHead(br_simple_list* list, br_simple_node* node) {
    LOG_TRACE10("(%p, %p)", list, node);
    node->next = list->head;
    node->prev = (br_simple_node**)&list->head;
    if (list->head != NULL) {
        list->head->prev = &node->next;
    }
    list->head = node;
}

// IDA: br_simple_node* __cdecl BrSimpleRemHead(br_simple_list *list)
br_simple_node* BrSimpleRemHead(br_simple_list* list) {
    br_simple_node* node;
    LOG_TRACE10("(%p)", list);

    node = list->head;
    if (node != NULL) {
        *node->prev = node->next;
        if (node->next != NULL) {
            node->next->prev = node->prev;
        }
        node->prev = NULL;
        node->next = NULL;
    }
    return node;
}

// IDA: void __cdecl BrSimpleInsert(br_simple_list *list, br_simple_node *here, br_simple_node *node)
void BrSimpleInsert(br_simple_list* list, br_simple_node* here, br_simple_node* node) {
    LOG_TRACE10("(%p, %p, %p)", list, here, node);

    node->prev = &here->next;
    node->next = here->next;
    if (here->next != NULL) {
        here->next->prev = &node->next;
    }
    here->next = node;
}

// IDA: br_simple_node* __cdecl BrSimpleRemove(br_simple_node *node)
br_simple_node* BrSimpleRemove(br_simple_node* node) {
    LOG_TRACE10("(%p)", node);

    *node->prev = node->next;
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    node->next = NULL;
    node->prev = NULL;
    return node;
}
