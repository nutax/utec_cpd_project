
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
#include <omp.h>

// -------------------------------------
// DATA
// -------------------------------------

/* CONSTANTS */
#define MAX_NODES 1024
#define MAX_LINE_SIZE 1024
#define MAX_TRIES 128
#define MAX_THREADS 64
#define CACHE_LINE 64

/* TYPES */

/* STATIC ALLOCATIONS */
int n_nodes, n_threads, n_tries;
int global_best_i;
float global_best;

float xpos[MAX_NODES];
float ypos[MAX_NODES];
float dmat[MAX_NODES][MAX_NODES];
float path[MAX_TRIES][MAX_NODES];

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
float tsp_try(int try, unsigned int *seed);

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
    assert(argc >= 4);

    n_tries = atoi(argv[1]);
    assert(n_tries > 0);
    assert(n_tries <= MAX_TRIES);

    n_threads = atoi(argv[2]);
    assert(n_threads > 0);
    assert(n_threads <= MAX_THREADS);

    n_nodes = atoi(argv[3]);
    assert(n_nodes > 0);
    assert(n_nodes <= MAX_NODES);
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
    printf("Best aproximation length: %f\n", global_best);
    for (int i = 0; i < n_nodes; ++i)
    {
        printf("%d ", path[global_best_i][i]);
    }
    printf("\n");
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
    float local_best[n_tries][16];

#pragma omp parallel default(none) shared(local_best)
    {
        unsigned int myseed = omp_get_thread_num();
#pragma omp for
        for (int i = 0; i < n_tries; i++)
        {
            local_best[i][0] = tsp_try(i, &myseed);
        }
    }

    global_best = local_best[0][0];
    global_best_i = 0;
    for (int i = 1; i < n_tries; ++i)
    {
        if (local_best[i][0] < global_best)
        {
            global_best_i = i;
            global_best = local_best[i][0];
        }
    }
}

float tsp_try(int try, unsigned int *seed)
{
    return 0.0f;
}