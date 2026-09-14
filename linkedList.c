#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

void linkedListInitialise(LinkedList *list)
{
    if (list != NULL)
    {
        list->head = NULL;
        list->length = 0U;
    }
}

/* Allocate both the generic node and its independent data copy. */
int linkedListPushFront(LinkedList *list, const void *data, size_t dataSize)
{
    LinkedListNode *newNode;
    int success;

    newNode = NULL;
    success = 0;

    if (list != NULL && data != NULL && dataSize > 0U)
    {
        newNode = (LinkedListNode *)malloc(sizeof(LinkedListNode));

        if (newNode != NULL)
        {
            newNode->data = malloc(dataSize);

            if (newNode->data != NULL)
            {
                memcpy(newNode->data, data, dataSize);
                newNode->dataSize = dataSize;
                newNode->next = list->head;
                list->head = newNode;
                list->length++;
                success = 1;
            }
            else
            {
                free(newNode);
            }
        }
    }

    return success;
}

/* Copy the newest value to the caller before releasing the owned node. */
int linkedListPopFront(LinkedList *list, void *output, size_t outputSize)
{
    LinkedListNode *oldNode;
    int success;

    oldNode = NULL;
    success = 0;

    if (list != NULL && list->head != NULL && output != NULL &&
        outputSize >= list->head->dataSize)
    {
        oldNode = list->head;
        memcpy(output, oldNode->data, oldNode->dataSize);
        list->head = oldNode->next;
        list->length--;
        free(oldNode->data);
        free(oldNode);
        success = 1;
    }

    return success;
}

int linkedListIsEmpty(const LinkedList *list)
{
    int empty;

    empty = 1;

    if (list != NULL && list->head != NULL)
    {
        empty = 0;
    }

    return empty;
}

void linkedListClear(LinkedList *list)
{
    LinkedListNode *oldNode;

    if (list != NULL)
    {
        while (list->head != NULL)
        {
            oldNode = list->head;
            list->head = oldNode->next;
            free(oldNode->data);
            free(oldNode);
        }

        list->length = 0U;
    }
}
