#!/bin/bash

#SBATCH --job-name=tflojob
#SBATCH --partition=Centaurus
#SBATCH --time=00:30:00
#SBATCH --nodes=4

OMP_NUM_THREADS=1 ./knapsack 50_items.csv
OMP_NUM_THREADS=4 ./knapsack 50_items.csv

OMP_NUM_THREADS=1 ./knapsack_omp 50_items.csv
OMP_NUM_THREADS=4 ./knapsack_omp 50_items.csv
