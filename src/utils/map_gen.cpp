#include "map_gen.hpp"

namespace map
{
    std::vector<std::vector<int>> Voronoi::generate(int n, int m, std::vector<std::tuple<int, int, int>> &ogSeeds, std::vector<std::pair<int, int>> &extSeedCnt, int (*distFunc)(int, int, int, int))
    {
        std::vector<std::vector<int>> map(n, std::vector<int>(m, -1));
        std::vector<std::tuple<int, int, int>> seeds;

        for (auto [y, x, t] : ogSeeds)
        {
            map[y][x] = t;
            seeds.push_back({y, x, t});
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

        // Works efficiently if n * m >> total seedCnt
        int chosenInd = 0;
        for (auto [t, cnt] : extSeedCnt)
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