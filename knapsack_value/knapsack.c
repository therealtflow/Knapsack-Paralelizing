#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cblas.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LABEL_LENGTH 100

int max(int a, int b);
int countLines(FILE *file);
void parseItems(FILE *file, char **labels, int *val, int *wt);

double get_time_diff(struct timespec start, struct timespec end)
{
return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) /
1000000000.0;
}

//function that calculates the max value
//function starts at the # of items for n
int knapsack(int W, int* wt, int* val, int n)
{
    //if n = 0 or W = 0, there is no calculation to do
    int withoutItem, withItem;
    if (n == 0 || W == 0)
    {
        return 0;
    }
    //deincrimints n
    --n;
    //if the weight of spot n in the array is more than the weight we can carry, recursively call the function
    if (wt[n] > W)
    {
        return knapsack(W, wt, val, n);
    }
    //if the weight of spot n in the array is not more than the weight we can carry
    //this is the only part you are paralizing
    else
    {
		#pragma omp task firstprivate(wt, val, W, n) shared(withoutItem)
                //withoutitem & withItem recursively call each other
                withoutItem = knapsack(W, wt, val, n);
                
                //added in code to watch what happens
               // printf("withoutItem: %d\n", withoutItem);
                withItem = val[n] + knapsack(W - wt[n], wt, val, n);
                        //added in code to watch what happens
               // printf("withItem: %d\n", withItem);
               // printf("max: %d\n", max(withItem, withoutItem));
		#pragma omp taskwait
                return max(withItem, withoutItem);
    }
}

int main(int argc, char *argv[])
{

	struct timespec start_full, end_full, start_mm, end_mm;
	clock_gettime(CLOCK_MONOTONIC, &start_full);

    //checks to make sure file works (no parallel)
    FILE *file;
    int count, i, W = 50, maximumValue;
    int *val, *wt;
    char **labels;

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
    
    //declares all variables to be used and allocates space
    //count is n
    count = countLines(file);
    val = malloc(count * sizeof(int));
    wt = malloc(count * sizeof(int));
    labels = malloc(count * sizeof(char*));
    
    for (i = 0; i < count; i++)
    labels[i] = malloc(MAX_LABEL_LENGTH * sizeof(char));
    
    //parses items into arrays
    parseItems(file, labels, val, wt);
    //closes file
    fclose(file);

    //shows the items available
    printf("Items available:\n");
    for (i = 0; i < count; i++)
    {
 //       printf("Item %d: %s (Value: %d, Weight: %d)\n", i + 1, labels[i], val[i], wt[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &start_mm);

    //performs calculatio
#pragma omp parallel
    {
#pragma omp single
    maximumValue = knapsack(W, wt, val, count);
    }
    //prints result of calculation
    printf("\nMaximum weight capacity of the knapsack: %d\n", W);
    printf("Maximum value that can be accommodated: %d\n", maximumValue);

clock_gettime(CLOCK_MONOTONIC, &end_mm);

clock_gettime(CLOCK_MONOTONIC, &end_full);
printf("knapsack_Time, Total_Time\n");
printf("%.9f, %.9f",get_time_diff(start_mm, end_mm),get_time_diff(start_full, end_full));

    //frees space allocated
    for (i = 0; i < count; i++)
    free(labels[i]);
    free(labels);
    free(val);
    free(wt);


    return 0;
}

//functions to parallize
int max(int a, int b)
{
    return (a > b) ? a : b;
}

int countLines(FILE *file)
{
    int lines = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file))
        lines++;
    rewind(file);
    return lines;
}

void parseItems(FILE *file, char **labels, int *val, int *wt)
{
    char line[MAX_LINE_LENGTH];
    char *token;
    int index = 0;
    while (fgets(line, sizeof(line), file))
    {
        token = strtok(line, ",");
        strcpy(labels[index], token);
        token = strtok(NULL, ",");
        val[index] = atoi(token);
        token = strtok(NULL, ",");
        wt[index] = atoi(token);
        index++;
    }
}

