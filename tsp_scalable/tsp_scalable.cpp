// -------------------------------------
// AUTHORS
// -------------------------------------
/* José Ignacio Huby Ochoa */
/* Luis Berrospi Rodriguez */
/* Mario Jacobo Rios Gamboa */

#include <iostream>
#include <stdlib.h>
#include "tsp/dp/from_one/seq/solver.hpp"

using distance_t = float;
int constexpr N = 20;

tsp::dp::solver<distance_t, N>::memo memo;
tsp::dp::solver<distance_t, N>::matrix matrix = {{{0,
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
                                                   60},
                                                  {84,
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
                                                   64},
                                                  {20,
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
                                                   23},
                                                  {27,
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
                                                   68},
                                                  {63,
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
                                                   44},
                                                  {46,
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
                                                   76},
                                                  {8,
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
                                                   56},
                                                  {65,
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
                                                   1},
                                                  {19,
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
                                                   9},
                                                  {76,
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
                                                   83},
                                                  {87,
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
                                                   91},
                                                  {71,
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
                                                   76},
                                                  {71,
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
                                                   68},
                                                  {99,
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
                                                   46},
                                                  {38,
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
                                                   15},
                                                  {77,
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
                                                   6},
                                                  {71,
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
                                                   8},
                                                  {7,
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
                                                   85},
                                                  {100,
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
                                                   3},
                                                  {60,
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
                                                   0}}};

tsp::dp::from_one::seq::solver<distance_t, N> solver_seq(memo, matrix);

int main()
{
    memo.reserve(1 << N);
    for (std::size_t i = 0; i < N; ++i)
        std::cout << solver_seq.solve(i) << std::endl;
    return EXIT_SUCCESS;
}