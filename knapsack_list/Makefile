
all: knapsack knapsack_omp

knapsack_omp: knapsack.c Makefile
	$(CC) -Wall -Wextra -g -O3 itemlist.c knapsack.c -o knapsack_omp -fopenmp

knapsack: knapsack.c Makefile
	$(CC) -Wall -Wextra -g -O3 itemlist.c knapsack.c -o knapsack

clean:
	rm -rf knapsack knapsack_omp
