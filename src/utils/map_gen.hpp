#pragma once
#include <vector>
#include <memory>
#include <unordered_set>
#include <random>
#include <algorithm>

namespace map
{
    inline int manhattan(int x1, int y1, int x2, int y2) { return abs(x1 - x2) + abs(y1 - y2); }
    inline int euclidean(int x1, int y1, int x2, int y2) { return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); }
    inline int chebyshev(int x1, int y1, int x2, int y2) { return std::max(abs(x1 - x2), abs(y1 - y2)); }

    std::vector<std::vector<int>> voronoi(std::pair<int, int> dim, std::vector<std::pair<int, int>> &ranSeeds, int (*distFunc)(int, int, int, int) = euclidean, std::vector<std::tuple<int, int, int>> *fixedSeeds = nullptr);
}
