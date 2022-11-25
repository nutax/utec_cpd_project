#include <bitset>
#include <array>
#include <unordered_map>
#include <limits>
#include <algorithm>

namespace tsp
{
    namespace dp
    {
        template <typename T, std::size_t N>
        class solver
        {
        public:
            using set = std::bitset<N>;
            using array = std::array<T, N>;
            using memo = std::unordered_map<set, array>;
            using matrix = std::array<std::array<T, N>, N>;

        protected:
            static constexpr T INF = std::numeric_limits<T>::max();
            memo &path_distances;
            matrix &distance_matrix;
            T res = 0;

            virtual T visit_unvisiteds(std::size_t current_node, set const &visited_nodes)
            {
                auto global_best = INF;

                for (std::size_t neighbor_node = 0; neighbor_node < N; neighbor_node++)
                {
                    if (!visited_nodes[neighbor_node])
                    {
                        auto local_best = distance_matrix[current_node][neighbor_node];
                        auto new_visted_nodes = visited_nodes;
                        new_visted_nodes.set(neighbor_node);
                        local_best += solve(neighbor_node, new_visted_nodes);
                        global_best = std::min(global_best, local_best);
                    }
                }

                return global_best;
            }

            T solve(std::size_t current_node, set const &visited_nodes)
            {
                if (visited_nodes.all())
                {
                    return distance_matrix[current_node][0];
                }

                auto path_it = path_distances.find(visited_nodes);
                if (path_it != path_distances.end())
                {
                    auto const distance = path_it->second[current_node];
                    if (distance > 0)
                    {
                        return distance;
                    }
                }

                auto best = visit_unvisiteds(current_node, visited_nodes);

                return path_distances[visited_nodes][current_node] = best;
            }

        public:
            solver(memo &path_distances, matrix &distance_matrix) : path_distances{path_distances}, distance_matrix{distance_matrix} {};

            T solve(std::size_t starting_node)
            {
                set visited_nodes;
                visited_nodes.set(starting_node);
                res = solve(starting_node, visited_nodes);
                return res;
            }

            T get() const { return res; }

            void clear()
            {
                res = 0;
                path_distances.clear();
            }
        };
    };
};