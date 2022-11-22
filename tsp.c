
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
#define N 20
#define INF 1000000
#define POSSIBILITY_NOT_EVALUATED_VALUE -1
#define ALL_NODES_VISITED_VALUE ((1 << N) - 1)
#define NONE_VISITED 0

/* TYPES */

/* STATIC ALLOCATIONS */
// random distance matrix size 15x15
int dist[N][N] = {
    {
        0,
        84,
        20,
        27,
        63,
        46,
        8,
        65,
        19,
        76,
        87,
        71,
        71,
        99,
        38,
        77,
        71,
        7,
        100,
        60,
    },
    {
        84,
        0,
        2,
        61,
        8,
        44,
        91,
        6,
        99,
        56,
        66,
        91,
        80,
        53,
        2,
        18,
        82,
        10,
        78,
        64,
    },
    {
        20,
        2,
        0,
        64,
        52,
        34,
        58,
        14,
        99,
        70,
        86,
        6,
        48,
        37,
        91,
        42,
        47,
        34,
        58,
        23,
    },
    {
        27,
        61,
        64,
        0,
        31,
        44,
        41,
        75,
        96,
        75,
        10,
        29,
        90,
        9,
        57,
        23,
        52,
        43,
        87,
        68,
    },
    {
        63,
        8,
        52,
        31,
        0,
        47,
        69,
        70,
        77,
        12,
        52,
        37,
        53,
        45,
        26,
        59,
        97,
        28,
        18,
        44,
    },
    {
        46,
        44,
        34,
        44,
        47,
        0,
        89,
        90,
        45,
        37,
        72,
        84,
        3,
        30,
        99,
        34,
        4,
        22,
        69,
        76,
    },
    {
        8,
        91,
        58,
        41,
        69,
        89,
        0,
        99,
        26,
        24,
        42,
        57,
        14,
        96,
        36,
        39,
        13,
        7,
        4,
        56,
    },
    {
        65,
        6,
        14,
        75,
        70,
        90,
        99,
        0,
        79,
        55,
        74,
        27,
        41,
        9,
        35,
        38,
        80,
        75,
        70,
        1,
    },
    {
        19,
        99,
        99,
        96,
        77,
        45,
        26,
        79,
        0,
        75,
        44,
        57,
        41,
        99,
        90,
        40,
        43,
        79,
        78,
        9,
    },
    {
        76,
        56,
        70,
        75,
        12,
        37,
        24,
        55,
        75,
        0,
        99,
        78,
        11,
        41,
        28,
        70,
        63,
        56,
        55,
        83,
    },
    {
        87,
        66,
        86,
        10,
        52,
        72,
        42,
        74,
        44,
        99,
        0,
        24,
        58,
        34,
        41,
        71,
        76,
        63,
        69,
        91,
    },
    {
        71,
        91,
        6,
        29,
        37,
        84,
        57,
        27,
        57,
        78,
        24,
        0,
        29,
        77,
        85,
        31,
        60,
        59,
        56,
        76,
    },
    {
        71,
        80,
        48,
        90,
        53,
        3,
        14,
        41,
        41,
        11,
        58,
        29,
        0,
        60,
        56,
        98,
        92,
        16,
        35,
        68,
    },
    {
        99,
        53,
        37,
        9,
        45,
        30,
        96,
        9,
        99,
        41,
        34,
        77,
        60,
        0,
        22,
        72,
        55,
        47,
        75,
        46,
    },
    {
        38,
        2,
        91,
        57,
        26,
        99,
        36,
        35,
        90,
        28,
        41,
        85,
        56,
        22,
        0,
        40,
        49,
        64,
        96,
        15,
    },
    {
        77,
        18,
        42,
        23,
        59,
        34,
        39,
        38,
        40,
        70,
        71,
        31,
        98,
        72,
        40,
        0,
        18,
        49,
        53,
        6,
    },
    {
        71,
        82,
        47,
        52,
        97,
        4,
        13,
        80,
        43,
        63,
        76,
        60,
        92,
        55,
        49,
        18,
        0,
        45,
        74,
        8,
    },
    {
        7,
        10,
        34,
        43,
        28,
        22,
        7,
        75,
        79,
        56,
        63,
        59,
        16,
        47,
        64,
        49,
        45,
        0,
        8,
        85,
    },
    {
        100,
        78,
        58,
        87,
        18,
        69,
        4,
        70,
        78,
        55,
        69,
        56,
        35,
        75,
        96,
        53,
        74,
        8,
        0,
        3,
    },
    {
        60,
        64,
        23,
        68,
        44,
        76,
        56,
        1,
        9,
        83,
        91,
        76,
        68,
        46,
        15,
        6,
        8,
        85,
        3,
        0,
    },
};
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
#define TEST(_name, _f)    \
    read();                \
    CHECK_TIME(_name, _f); \
    write()

/* DECLARATIONS */
int main(int argc, char **argv);
void read();
void tsp_seq();
void write();
int tsp_seq_dp(int current_node, int visited_nodes);
int tsp_threads_2_dp(int current_node, int visited_nodes);
void tsp_threads_1();
void tsp_threads_2();

/* DEFINITIONS */
int main(int argc, char **argv)
{
    TEST("TSP_SEQ", tsp_seq());
    TEST("TSP_THR_1", tsp_threads_1());
    TEST("TSP_THR_2", tsp_threads_2());
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

void tsp_threads_1()
{
    int global_best = INF;

#pragma omp parallel for
    for (int initial_node = 0; initial_node < N; initial_node++)
    {
        int local_best = tsp_seq_dp(initial_node, MARK_NODE_VISITED(NONE_VISITED, initial_node));
        global_best = MIN(global_best, local_best);
    }
    res = global_best;
}

void tsp_threads_2()
{
    int global_best = INF;

    for (int initial_node = 0; initial_node < N; initial_node++)
    {
        int local_best = tsp_threads_2_dp(initial_node, MARK_NODE_VISITED(NONE_VISITED, initial_node));
        global_best = MIN(global_best, local_best);
    }
    res = global_best;
}

int tsp_threads_2_dp(int current_node, int visited_nodes)
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

#pragma omp parallel for
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