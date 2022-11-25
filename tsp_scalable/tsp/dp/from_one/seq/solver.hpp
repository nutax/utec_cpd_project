#include "tsp/dp/solver.hpp"

namespace tsp
{
    namespace dp
    {
        namespace from_one
        {
            namespace seq
            {
                template <typename T, std::size_t N>
                using solver = tsp::dp::solver<T, N>;
            };
        };
    };
};