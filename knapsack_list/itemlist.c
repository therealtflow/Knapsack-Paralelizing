#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "itemlist.h"

Node *createNode(Item item)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        return NULL;
    }
    newNode->item = item;
    newNode->next = NULL;
    return newNode;
}

void appendNode(Node **head, Item item)
{
    Node *newNode = createNode(item);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        Node *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void printList(Node *head)
{
    Node *temp = head;
    while (temp != NULL)
    {
        printf("Item: %s, Value: %d, Weight: %d\n", temp->item.name, temp->item.value, temp->item.weight);
        temp = temp->next;
    }
}

void freeList(Node *head)
{
    Node *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
