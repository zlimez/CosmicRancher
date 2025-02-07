#pragma once
#include "../world.hpp"
#include "../../utils/map_gen.hpp"
#include "../components/sprite.hpp"
#include "../components/transform.hpp"
#include "../entity.hpp"
#include "../game.hpp"

namespace map
{
    // Terrain type and count
    using Terrain = std::pair<int, int>;
    // TODO: Only has Voronoi for now, no wrap around like what a spherical map would have
    void mapFor(std::pair<int, int> dim, std::pair<float, float> btmLeft, std::vector<Terrain> &terrains, std::vector<std::string> &terrainSprites, engine::World &world);
}