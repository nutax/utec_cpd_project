#include <array>

namespace container
{
    template <typename T, std::size_t N, std::size_t M>
    using matrix = std::array<std::array<T, M>, N>;
};