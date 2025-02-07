#include "map_gen.hpp"

namespace map
{
    std::vector<std::vector<int>> voronoi(std::pair<int, int> dim, std::vector<std::pair<int, int>> &ranSeeds, int (*distFunc)(int, int, int, int), std::vector<std::tuple<int, int, int>> *fixedSeeds)
    {
        auto [n, m] = dim;
        std::vector<std::vector<int>> map(n, std::vector<int>(m, -1));
        std::vector<std::tuple<int, int, int>> seeds;

        if (fixedSeeds)
        {
            for (auto [y, x, t] : *fixedSeeds)
            {
                map[y][x] = t;
                seeds.push_back({y, x, t});
            }
        }

        std::vector<std::pair<int, int>> allCells;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
            {
                if (map[i][j] != -1)
                    continue;
                allCells.push_back({i, j});
            }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(allCells.begin(), allCells.end(), g);

        int chosenInd = 0;
        for (auto [t, cnt] : ranSeeds)
        {
            for (int i = 0; i < cnt; i++)
            {
                auto [y, x] = allCells[chosenInd++];
                map[y][x] = t;
                seeds.push_back({y, x, t});
            }
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (map[i][j] != -1)
                    continue;
                int minDist = 1e9;
                for (auto [y, x, t] : seeds)
                {
                    int ndist = distFunc(i, j, y, x);
                    if (ndist < minDist)
                    {
                        minDist = ndist;
                        map[i][j] = t;
                    }
                }
            }
        }

        return map;
    }
}