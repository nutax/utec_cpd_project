
// -------------------------------------
// AUTHORS
// -------------------------------------
/* José Ignacio Huby Ochoa */
/* Luis Berrospi Rodriguez */
/* Mario Jacobo Rios Gamboa */

// -------------------------------------
// DEPENDENCIES
// -------------------------------------

/* INCLUDES */
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

// -------------------------------------
// DATA
// -------------------------------------

/* CONSTANTS */
#define MAX_NODES 1000

/* TYPES */
typedef float dist_t;

/* STATIC ALLOCATIONS */
int n_nodes;
dist_t dist[MAX_NODES][MAX_NODES];

// -------------------------------------
// PROCEDURES
// -------------------------------------

/* INLINE */
#define CHECK_TIME(_name, ...)                                                                              \
    {                                                                                                       \
        struct timespec start, end;                                                                         \
        clock_gettime(CLOCK_REALTIME, &start);                                                              \
        __VA_ARGS__;                                                                                        \
        clock_gettime(CLOCK_REALTIME, &end);                                                                \
        double f = ((double)end.tv_sec * 1e9 + end.tv_nsec) - ((double)start.tv_sec * 1e9 + start.tv_nsec); \
        printf(_name " time %f ms\n", f / 1000000);                                                         \
    }

/* DECLARATIONS */
int main(int argc, char **argv);
void read();
void write();
void solve(int tries, int threads);

/* DEFINITIONS */
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Execution arguments missing!\n");
        return EXIT_FAILURE;
    }

    read();
    solve(atoi(argv[1]), atoi(argv[2]));
    write();

    return EXIT_SUCCESS;
}

void read()
{
}

void write()
{
}

void solve(int tries, int threads)
{
}