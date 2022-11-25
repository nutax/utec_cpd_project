
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
#define MAX_NODES 1024
#define MAX_LINE_SIZE 1024
#define CACHE_LINE 64

/* TYPES */

/* STATIC ALLOCATIONS */
int n_nodes, n_threads, n_tries;

float xpos[MAX_NODES];
float ypos[MAX_NODES];
float dmat[MAX_NODES][MAX_NODES];

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
void init(int argc, char **argv);
void read();
void read_metadata();
void read_coords();
void write();
void solve();
void solve_distances();
void solve_tsp();

/* DEFINITIONS */
int main(int argc, char **argv)
{
    init(argc, argv);

    read();
    solve();
    write();

    return EXIT_SUCCESS;
}

void init(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Execution arguments missing!\n");
        exit(EXIT_FAILURE);
    }
    n_tries = atoi(argv[1]);
    n_threads = atoi(argv[2]);
    n_nodes = atoi(argv[3]);
}

void read()
{
    read_metadata();
    read_coords();
}

void read_metadata()
{
    char buffer[MAX_LINE_SIZE];
    for (int i = 0; i < 8; ++i)
    {
        scanf(" %[^\n]", buffer);
        printf("%s\n", buffer);
    }
}

void read_coords()
{
    int node_index;
    for (int i = 0; i < n_nodes; ++i)
    {
        scanf("%d %f %f", &node_index, xpos + i, ypos + i);
    }
}

void write()
{
}

void solve()
{
    solve_distances();
    solve_tsp();
}

void solve_distances()
{
    for (int i = 0; i < (n_nodes - 1); ++i)
    {
        for (int j = i + 1; j < n_nodes; ++j)
        {
            float const xdiff = xpos[i] - xpos[j];
            float const ydiff = ypos[i] - ypos[j];

            float const xdist = xdiff * xdiff;
            float const ydist = ydiff * ydiff;

            float const dist = xdist + ydist;

            dmat[i][j] = dist;
            dmat[j][i] = dist;
        }
    }
}

void solve_tsp()
{
    
}