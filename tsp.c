
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
#define N 10
#define INF 1000000
#define POSSIBILITY_NOT_EVALUATED_VALUE -1
#define ALL_NODES_VISITED_VALUE ((1 << N) - 1)
#define NONE_VISITED 0

/* TYPES */

/* STATIC ALLOCATIONS */
// random distance matrix size 15x15
int dist[N][N] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    {1, 0, 1, 2, 3, 4, 5, 6, 7, 8},
    {2, 1, 0, 1, 2, 3, 4, 5, 6, 7},
    {3, 2, 1, 0, 1, 2, 3, 4, 5, 6},
    {4, 3, 2, 1, 0, 1, 2, 3, 4, 5},
    {5, 4, 3, 2, 1, 0, 1, 2, 3, 4},
    {6, 5, 4, 3, 2, 1, 0, 1, 2, 3},
    {7, 6, 5, 4, 3, 2, 1, 0, 1, 2},
    {8, 7, 6, 5, 4, 3, 2, 1, 0, 1},
    {9, 8, 7, 6, 5, 4, 3, 2, 1, 0}};
int memo[1 << N][N];
int res;

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
#define MIN(x, y) (x < y) ? x : y
#define ALL_NODES_VISITED(x) (x == ALL_NODES_VISITED_VALUE)
#define POSSIBILITY_EVALUATED(x) (x != POSSIBILITY_NOT_EVALUATED_VALUE)
#define NODE_IS_NOT_VISITED(arr, x) ((arr & (1 << x)) == 0)
#define MARK_NODE_VISITED(arr, x) (arr | (1 << x))
#define TEST(_name, _f)         \
    CHECK_TIME("READ", read()); \
    CHECK_TIME(_name, _f);      \
    CHECK_TIME("WRITE", write())

/* DECLARATIONS */
int main(int argc, char **argv);
void read();
void tsp_seq();
void write();
int tsp_seq_dp(int current_node, int visited_nodes);

/* DEFINITIONS */
int main(int argc, char **argv)
{
    TEST("TSP_SEQ", tsp_seq());
    return EXIT_SUCCESS;
}

void read()
{
    for (int visited_nodes = 0; visited_nodes < (1 << N); ++visited_nodes)
    {
        for (int current_node = 0; current_node < N; ++current_node)
        {
            memo[visited_nodes][current_node] = POSSIBILITY_NOT_EVALUATED_VALUE;
        }
    }
}

void tsp_seq()
{
    int global_best = INF;

    // PARALELIZAR ESTE FOR CON MPI
    for (int initial_node = 0; initial_node < N; initial_node++)
    {
        int local_best = tsp_seq_dp(initial_node, MARK_NODE_VISITED(NONE_VISITED, initial_node));
        global_best = MIN(global_best, local_best);
    }
    res = global_best;
}

void write()
{
    printf("The cost of most efficient tour = %d\n", res);
}

int tsp_seq_dp(int current_node, int visited_nodes)
{
    if (ALL_NODES_VISITED(visited_nodes))
    {
        return dist[current_node][0];
    }

    if (POSSIBILITY_EVALUATED(memo[visited_nodes][current_node]))
    {
        return memo[visited_nodes][current_node];
    }

    int global_best = INF;

    // PARALELIZAR ESTE FOR CON OPENMP SOLO EN EL PRIMER LLAMADO DE LA FUNCIÓN
    // ES DECIR, LAS SIGUIENTES LLAMADAS RECURSIVAS NO SPAWNEAN MÁS HILOS
    for (int neighbor_node = 0; neighbor_node < N; neighbor_node++)
    {
        if (NODE_IS_NOT_VISITED(visited_nodes, neighbor_node))
        {
            int local_best = dist[current_node][neighbor_node];
            local_best += tsp_seq_dp(neighbor_node, MARK_NODE_VISITED(visited_nodes, neighbor_node));
            global_best = MIN(global_best, local_best);
        }
    }

    return memo[visited_nodes][current_node] = global_best;
}