
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
#include <math.h>
#include <mpi.h>

// -------------------------------------
// DATA
// -------------------------------------

/* CONSTANTS */
#define MAX_NODES 1024
#define MAX_LINE_SIZE 1024
#define MAX_TRIES 4096
#define MAX_THREADS 64
#define CACHE_LINE 64

/* TYPES */

/* STATIC ALLOCATIONS */
int n_nodes, n_threads, n_tries, process_id, n_processes;
int global_best_i;
float global_best;

float xpos[MAX_NODES];
float ypos[MAX_NODES];
float dmat[MAX_NODES][MAX_NODES];
int path[MAX_TRIES][MAX_NODES];

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
void try_shuffle(int try, unsigned int *seed);
float try_opt2(int try);
int opt2_check(int try, int i, int j);
void opt2_reverse(int try, int i, int j);
float opt2_cost(int try);

/* DEFINITIONS */
int main(int argc, char **argv)
{
    init(argc, argv);

    // read();
    // solve();
    // write();

    return EXIT_SUCCESS;
}

void init(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &n_processes);
    if (process_id == 0)
    {
        printf("MPI initialized with %d processes\n", n_processes);
    }

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
        // printf("%s\n", buffer);
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
        // printf("%d ", path[global_best_i][i]);
    }
    printf("\n");
}

void solve()
{
    CHECK_TIME("DIST", solve_distances());
    CHECK_TIME("TSP", solve_tsp());
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

            float const dist = sqrt(xdist + ydist);
            // float const dist = xdist + ydist;

            dmat[i][j] = dist;
            dmat[j][i] = dist;
        }
    }
}

void solve_tsp()
{
    float local_best[n_tries][16];

    int const _n_tries = n_tries;
#pragma omp parallel shared(local_best) num_threads(n_threads)
    {
        unsigned int myseed = omp_get_thread_num();
#pragma omp for
        for (int i = 0; i < _n_tries; i++)
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
    try_shuffle(try, seed);
    return try_opt2(try);
}

void try_shuffle(int try, unsigned int *seed)
{
    for (int i = 0; i < n_nodes; ++i)
    {
        path[try][i] = i;
    }
    for (int i = 0; i < n_nodes - 1; i++)
    {
        int const j = i + rand_r(seed) / (RAND_MAX / (n_nodes - i) + 1);
        int const aux = path[try][j];
        path[try][j] = path[try][i];
        path[try][i] = aux;
    }
}

float try_opt2(int try)
{
    for (int i = 0; i < (n_nodes - 2); i++)
    {
        for (int j = i + 1; j < (n_nodes - 1); j++)
        {
            int is_better = opt2_check(try, i, j);
            if (is_better)
            {
                opt2_reverse(try, i, j);
                return try_opt2(try);
            }
        }
    }
    return opt2_cost(try);
}
int opt2_check(int try, int i, int j)
{
    int const i2 = (i - 1 + n_nodes) % n_nodes;
    int const j2 = j + 1;
    float const direct_dist = dmat[path[try][i]][path[try][i2]] + dmat[path[try][j]][path[try][j2]];
    float const indirect_dist = dmat[path[try][j]][path[try][i2]] + dmat[path[try][i]][path[try][j2]];
    return direct_dist > indirect_dist;
}
void opt2_reverse(int try, int i, int j)
{
    while (i < j)
    {
        int const aux = path[try][i];
        path[try][i] = path[try][j];
        path[try][j] = aux;
        i++;
        j--;
    }
}
float opt2_cost(int try)
{
    float cost = 0;
    for (int i = 0; i < (n_nodes - 1); i++)
    {
        cost += dmat[path[try][i]][path[try][i + 1]];
    }
    cost += dmat[path[try][n_nodes - 1]][path[try][0]];
    return cost;
}