gcc -std=c11 -fopenmp -Ofast -march=native main.c -I. && ./a.out 16 8 984 < ../data/large/984.tsp && rm a.out