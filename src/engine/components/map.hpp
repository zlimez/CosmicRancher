#pragma once
#include "../entity.hpp"
#include "../defs.hpp"
#include "sprite.hpp"

namespace engine
{
    struct Terrain
    {
        int count_;
        std::string spriteFile_;
    };

    struct Map : Component
    {
        // NOTE: In cell count half of actual for each side
        std::pair<int, int> dim_;
        std::vector<Terrain> terrains_;
        // NOTE: In local space, convert to just use width and size with vertices produced by mapsys
        std::vector<Vertex> cellVertices_ = {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}}};

        Map &operator=(const Map &other) noexcept
        {
            if (this != &other)
            {
                dim_ = other.dim_;
                terrains_ = std::move(other.terrains_);
                cellVertices_ = std::move(other.cellVertices_);
            }

            return *this;
        }
    };

    struct MapCell : Component
    {
        std::pair<int, int> pos_;
    };
}