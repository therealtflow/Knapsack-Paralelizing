all: knapsack knapsack_omp

knapsack_omp: knapsack.c Makefile
	clang -Wall -Wextra -g -O3 knapsack.c -o knapsack_omp -fopenmp

knapsack: knapsack.c Makefile
	clang -Wall -Wextra -g -O3 knapsack.c -o knapsack

clean:
	rm -rf knapsack knapsack_omp
