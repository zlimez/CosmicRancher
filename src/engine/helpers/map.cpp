#include "map.hpp"

namespace map
{
    // index of terrainSprites should correspond to the type in ranSeeds and fixedSeeds
    void mapFor(std::pair<int, int> dim, std::pair<float, float> btmLeft, std::vector<Terrain> &terrains, std::vector<std::string> &terrainSprites, engine::World &world)
    {
        auto [n, m] = dim;
        auto base = voronoi(dim, terrains);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                std::vector<engine::ComponentID> tileType = {engine::getComponentID<engine::Sprite>(), engine::getComponentID<engine::Transform>()};
                engine::EntityID entity = world.createEntity(tileType);
                auto &sprite = world.getComponent<engine::Sprite>(entity);
                auto &transform = world.getComponent<engine::Transform>(entity);
                sprite.texture.filePath = terrainSprites[base[i][j]];
                sprite.vertices = {
                    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
                    {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
                    {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}},
                    {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}}};

                sprite.indices = {0, 1, 2, 2, 3, 0};
                sprite.shaderParts = {Game::basePath + "../src/graphics/shaders/tex_vtx.glsl", Game::basePath + "../src/graphics/shaders/tex_frag.glsl"};
                transform.position = {btmLeft.first + j, btmLeft.second + i};
                transform.isStatic = true;
            }
        }
    }
}