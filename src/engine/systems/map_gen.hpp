#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <random>

class Voronoi
{
public:
    // extSeedCnt is the number of seeds per "terrain" type, should generally match the expected ratio of terrain types, ogSeeds can be used for special areas like the poles
    static std::vector<std::vector<int>> generate(int n, int m, std::vector<std::tuple<int, int, int>> &ogSeeds, std::vector<std::pair<int, int>> &extSeedCnt, int (*distFunc)(int, int, int, int));
};

class DistFuncs
{
public:
    static int manhattan(int x1, int y1, int x2, int y2)
    {
        return abs(x1 - x2) + abs(y1 - y2);
    }

    static int euclidean(int x1, int y1, int x2, int y2)
    {
        return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    }

    static int chebyshev(int x1, int y1, int x2, int y2)
    {
        return std::max(abs(x1 - x2), abs(y1 - y2));
    }
};