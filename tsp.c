
// -------------------------------------
// AUTHORS
// -------------------------------------
/* Created by José Ignacio Huby Ochoa */

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
#define N 4
#define INF 10000000
#define POSSIBILITY_NOT_EVALUATED_VALUE -1
#define ALL_NODES_VISITED_VALUE ((1 << N) - 1)
#define NONE_VISITED 0

/* TYPES */

/* STATIC ALLOCATIONS */
int dist[N][N] = {
    {0, 10, 15, 20},
    {10, 0, 25, 25},
    {15, 25, 0, 30},
    {20, 25, 30, 0},
};
int memo[N][1 << N];
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

/* DECLARATIONS */
int main(int argc, char **argv);
void read();
void transform();
void write();
int tsp(int i, int mask);

/* DEFINITIONS */
int main(int argc, char **argv)
{
    CHECK_TIME("READ", read());
    CHECK_TIME("TRANSFORM", transform());
    CHECK_TIME("WRITE", write());
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

void transform()
{
    int global_best = INF;
    for (int current_node = 0; current_node < N; current_node++)
    {
        int local_best = dist[current_node][1];
        local_best += tsp(current_node, MARK_NODE_VISITED(NONE_VISITED, current_node));
        global_best = MIN(global_best, local_best);
    }
    res = global_best;
}

void write()
{
    printf("The cost of most efficient tour = %d", res);
}

int tsp(int current_node, int visited_nodes)
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

    for (int neighbor_node = 0; neighbor_node < N; neighbor_node++)
    {
        if (NODE_IS_NOT_VISITED(visited_nodes, neighbor_node))
        {
            int local_best = dist[current_node][neighbor_node];
            local_best += tsp(neighbor_node, MARK_NODE_VISITED(visited_nodes, neighbor_node));
            global_best = MIN(global_best, local_best);
        }
    }

    return memo[visited_nodes][current_node] = global_best;
}