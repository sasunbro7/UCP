#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

/* Each node owns a byte-for-byte copy, so the list is not tied to Game data. */
typedef struct LinkedListNode
{
    void *data;
    size_t dataSize;
    struct LinkedListNode *next;
} LinkedListNode;

typedef struct
{
    LinkedListNode *head;
    size_t length;
} LinkedList;

void linkedListInitialise(LinkedList *list);
int linkedListPushFront(LinkedList *list, const void *data, size_t dataSize);
int linkedListPopFront(LinkedList *list, void *output, size_t outputSize);
int linkedListIsEmpty(const LinkedList *list);
void linkedListClear(LinkedList *list);

#endif
