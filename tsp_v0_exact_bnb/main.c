// C++ program to solve Traveling Salesman Problem
// using Branch and Bound.
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#define N 20

#define CHECK_TIME(_name, ...)                                                                              \
    {                                                                                                       \
        struct timespec start, end;                                                                         \
        clock_gettime(CLOCK_REALTIME, &start);                                                              \
        __VA_ARGS__;                                                                                        \
        clock_gettime(CLOCK_REALTIME, &end);                                                                \
        double f = ((double)end.tv_sec * 1e9 + end.tv_nsec) - ((double)start.tv_sec * 1e9 + start.tv_nsec); \
        printf(_name " time %f ms\n", f / 1000000);                                                         \
    }

// final_path[] stores the final solution ie, the
// path of the salesman.
int final_path[N + 1];

// visited[] keeps track of the already visited nodes
// in a particular path
int visited[N];

// Stores the final minimum weight of shortest tour.
int final_res = INT32_MAX;

// Function to copy temporary solution to
// the final solution
void copyToFinal(int curr_path[])
{
    for (int i = 0; i < N; i++)
        final_path[i] = curr_path[i];
    final_path[N] = curr_path[0];
}

// Function to find the minimum edge cost
// having an end at the vertex i
int firstMin(int adj[N][N], int i)
{
    int min = INT32_MAX;
    for (int k = 0; k < N; k++)
        if (adj[i][k] < min && i != k)
            min = adj[i][k];
    return min;
}

// function to find the second minimum edge cost
// having an end at the vertex i
int secondMin(int adj[N][N], int i)
{
    int first = INT32_MAX, second = INT32_MAX;
    for (int j = 0; j < N; j++)
    {
        if (i == j)
            continue;

        if (adj[i][j] <= first)
        {
            second = first;
            first = adj[i][j];
        }
        else if (adj[i][j] <= second &&
                 adj[i][j] != first)
            second = adj[i][j];
    }
    return second;
}

// function that takes as arguments:
// curr_bound -> lower bound of the root node
// curr_weight-> stores the weight of the path so far
// level-> current level while moving in the search
//		 space tree
// curr_path[] -> where the solution is being stored which
//			 would later be copied to final_path[]
void TSPRec(int adj[N][N], int curr_bound, int curr_weight,
            int level, int curr_path[], int start)
{
    // base case is when we have reached level N which
    // means we have covered all the nodes once
    if (level == N)
    {
        // check if there is an edge from last vertex in
        // path back to the first vertex
        if (adj[curr_path[level - 1]][curr_path[start]] != 0)
        {
            // curr_res has the total weight of the
            // solution we got
            int curr_res = curr_weight +
                           adj[curr_path[level - 1]][curr_path[start]];

            // Update final result and final path if
            // current result is better.
            if (curr_res < final_res)
            {
                copyToFinal(curr_path);
                final_res = curr_res;
            }
        }
        return;
    }

    // for any other level iterate for all vertices to
    // build the search space tree recursively
    for (int i = 0; i < N; i++)
    {
        // Consider next vertex if it is not same (diagonal
        // entry in adjacency matrix and not visited
        // already)
        if (adj[curr_path[level - 1]][i] != 0 &&
            visited[i] == 0)
        {
            int temp = curr_bound;
            curr_weight += adj[curr_path[level - 1]][i];

            // different computation of curr_bound for
            // level 2 from the other levels
            if (level == 1)
                curr_bound -= ((firstMin(adj, curr_path[level - 1]) +
                                firstMin(adj, i)) /
                               2);
            else
                curr_bound -= ((secondMin(adj, curr_path[level - 1]) +
                                firstMin(adj, i)) /
                               2);

            // curr_bound + curr_weight is the actual lower bound
            // for the node that we have arrived on
            // If current lower bound < final_res, we need to explore
            // the node further
            if (curr_bound + curr_weight < final_res)
            {
                curr_path[level] = i;
                visited[i] = 1;

                // call TSPRec for the next level
                TSPRec(adj, curr_bound, curr_weight, level + 1,
                       curr_path, start);
            }

            // Else we have to prune the node by resetting
            // all changes to curr_weight and curr_bound
            curr_weight -= adj[curr_path[level - 1]][i];
            curr_bound = temp;

            // Also reset the visited array
            memset(visited, 0, sizeof(visited));
            for (int j = 0; j <= level - 1; j++)
                visited[curr_path[j]] = 1;
        }
    }
}

// This function sets up final_path[]
void TSP(int adj[N][N], int start)
{
    int curr_path[N + 1];

    // Calculate initial lower bound for the root node
    // using the formula 1/2 * (sum of first min +
    // second min) for all edges.
    // Also initialize the curr_path and visited array
    int curr_bound = 0;
    memset(curr_path, -1, sizeof(curr_path));
    memset(visited, 0, sizeof(visited));

    // Compute initial bound
    for (int i = 0; i < N; i++)
        curr_bound += (firstMin(adj, i) +
                       secondMin(adj, i));

    // Rounding off the lower bound to an integer
    curr_bound = (curr_bound & 1) ? curr_bound / 2 + 1 : curr_bound / 2;

    // We start at vertex 1 so the first vertex
    // in curr_path[] is 0
    visited[start] = 1;
    curr_path[0] = start;

    // Call to TSPRec for curr_weight equal to
    // 0 and level 1
    TSPRec(adj, curr_bound, 0, 1, curr_path, start);
}

// Driver code
int main()
{
    // Adjacency matrix for the given graph
    int adj[N][N] = {
        {
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
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
            INT32_MAX,
        },
    };

    CHECK_TIME("TSP BnB", for (int i = 0; i < N; ++i) TSP(adj, i));

    printf("Minimum cost : %d\n", final_res);
    printf("Path Taken : ");
    for (int i = 0; i <= N; i++)
        printf("%d ", final_path[i]);

    return 0;
}
