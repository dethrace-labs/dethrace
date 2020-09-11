#include "brlists.h"
#include "harness.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

char rscid[49];

void BrNewList(br_list* list) {
    list->_null = NULL;
    list->tail = (br_node*)list;
    list->head = (br_node*)&list->_null;
}

void BrAddHead(br_list* list, br_node* node) {
    assert(node != NULL);
    assert(list != NULL);
    assert(list->head != NULL);
    node->prev = (br_node*)list;
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
}

void BrAddTail(br_list* list, br_node* node) {
    NOT_IMPLEMENTED();
}

br_node* BrRemHead(br_list* list) {
    br_node* n;
    NOT_IMPLEMENTED();
}

br_node* BrRemTail(br_list* list) {
    br_node* n;
    NOT_IMPLEMENTED();
}

void BrInsert(br_list* list, br_node* here, br_node* node) {
    NOT_IMPLEMENTED();
}

br_node* BrRemove(br_node* node) {
    NOT_IMPLEMENTED();
}

void BrSimpleNewList(br_simple_list* list) {
    LOG_TRACE10("(%p)", list);
    list->head = NULL;
}

void BrSimpleAddHead(br_simple_list* list, br_simple_node* node) {
    node->next = list->head;
    node->prev = (br_simple_node**)list;
    if (node->next) {
        node->next->prev = (br_simple_node**)node;
    }
    list->head = node;
}

br_simple_node* BrSimpleRemHead(br_simple_list* list) {
    br_simple_node* node;
    NOT_IMPLEMENTED();
}

void BrSimpleInsert(br_simple_list* list, br_simple_node* here, br_simple_node* node) {
    NOT_IMPLEMENTED();
}

br_simple_node* BrSimpleRemove(br_simple_node* node) {
    *node->prev = node->next;
    if (node->next) {
        node->next->prev = node->prev;
    }
    node->next = NULL;
    node->prev = NULL;
    return node;
}
