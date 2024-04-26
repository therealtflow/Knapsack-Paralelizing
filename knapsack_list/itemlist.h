#ifndef ITEMLIST_H
#define ITEMLIST_H

typedef struct Item
{
    int value;
    int weight;
    char *name;
} Item;

typedef struct Node
{
    Item item;
    struct Node *next;
} Node;

Node *createNode(Item item);
void appendNode(Node **head, Item item);
void printList(Node *head);
void freeList(Node *head);

#endif
