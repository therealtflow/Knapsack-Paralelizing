#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "itemlist.h"
#include <cblas.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024

int max(int a, int b);
int countLines(FILE *file);
void parseItems(FILE *file, Node **head);

double get_time_diff(struct timespec start, struct timespec end)
{
return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) /
1000000000.0;
}

int knapsack(int W, Node *items, Node **optimal)
{
    if (items == NULL || W == 0)
    {
        *optimal = NULL;
        return 0;
    }
    // if new item can't even be considered, just assume we don't take it
    else if (items->item.weight > W)
    {
        return knapsack(W, items->next, optimal);
    }

    Node* withOptimal = NULL;
    Node* withoutOptimal = NULL;
    int withValue = 0;
    int withoutValue = 0; 

    
#pragma omp task firstprivate(W, items, withoutOptimal) shared (withoutValue)
    withoutValue = knapsack(W, items->next, &withoutOptimal);
    
    withValue = items->item.value + knapsack(W - items->item.weight, items->next, &withOptimal);

#pragma omp taskwait
    Node *newNode = createNode(items->item);
    
    newNode->next = withOptimal;
    withOptimal = newNode;

    // compare if taking the item makes more sense than not taking the item
    // return the higher value, free the lower value
    if (withValue > withoutValue)
    {
        freeList(withoutOptimal);
        *optimal = withOptimal;
        return withValue;
    }
    else
    {
        freeList(withOptimal);
        *optimal = withoutOptimal;
        return withoutValue;
    }

}

int main(int argc, char *argv[])
{
    
    struct timespec start_full, end_full, start_mm, end_mm;
    clock_gettime(CLOCK_MONOTONIC, &start_full);
    
    int maximumValue;
    FILE* file;
    Node* head = NULL;
    Node* optimalItems = NULL;

    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    file = fopen(argv[1], "r");
    if (!file)
    {
        perror("Error opening file");
        return 1;
    }

    parseItems(file, &head);
    fclose(file);

    int W = 50;
    
    clock_gettime(CLOCK_MONOTONIC, &start_mm);
    
#pragma omp parallel
    {
#pragma omp single
    {
        maximumValue = knapsack(W, head, &optimalItems);
    }
    }
    

    printf("Optimal items to take (Name, Value, Weight):\n");
    printList(optimalItems);

    printf("\nMaximum weight capacity of the knapsack: %d\n", W);
    printf("Maximum value that can be accommodated: %d\n", maximumValue);
    
    clock_gettime(CLOCK_MONOTONIC, &end_mm);
    clock_gettime(CLOCK_MONOTONIC, &end_full);
    printf("knapsack_Time, Total_Time\n");
    printf("%.9f, %.9f",get_time_diff(start_mm, end_mm),get_time_diff(start_full, end_full));


    freeList(head);
    freeList(optimalItems);

    return 0;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

void parseItems(FILE *file, Node **head)
{
    char line[MAX_LINE_LENGTH];
    char *token;
    Item item;
    while (fgets(line, sizeof(line), file))
    {
        token = strtok(line, ",");
        item.name = strdup(token);
        token = strtok(NULL, ",");
        item.value = atoi(token);
        token = strtok(NULL, ",");
        item.weight = atoi(token);
        appendNode(head, item);
    }
}

