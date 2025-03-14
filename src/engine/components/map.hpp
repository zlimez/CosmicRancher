#pragma once
#include "../entity.hpp"
#include "../defs.hpp"
#include "sprite.hpp"

namespace engine
{
    struct Terrain
    {
        int count;
        std::string spriteFile;
    };

    struct Map : Component
    {
        // NOTE: In cell count half of actual for each side
        std::pair<int, int> dim;
        std::vector<Terrain> terrains;
        // NOTE: In local space, convert to just use width and size with vertices produced by mapsys
        std::vector<Vertex> cellVertices = {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}}};

        Map &operator=(const Map &other) noexcept
        {
            if (this != &other)
            {
                dim = other.dim;
                terrains = std::move(other.terrains);
                cellVertices = std::move(other.cellVertices);
            }

            return *this;
        }
    };
}