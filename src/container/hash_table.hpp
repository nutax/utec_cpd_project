#include <unordered_map>

namespace container
{
    template <typename K, typename V>
    using hash_table = std::unordered_map<K, V>;
};