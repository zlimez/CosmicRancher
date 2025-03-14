#include "mapsys.hpp"

namespace engine
{
    // index of terrainSprites should correspond to the type in ranSeeds and fixedSeeds
    void MapSys::init(engine::World &world)
    {
        Type mapType;
        mapType.set(engine::getComponentID<engine::Map>());
        auto &mapArchs = world.getArchetypesWith(mapType);

        for (auto mapArch : mapArchs)
        {
            std::cout << "Map Arch " << mapArch->type << '\n';
            auto &maps = mapArch->getComponentRow<engine::Map>();

            // NOTE: assumes one map for now, layer can be added later
            auto &map = maps.blocks[0];
            auto [n, m] = map.dim;
            std::vector<std::pair<int, int>> terrainSeeds(map.terrains.size());
            std::vector<std::string> terrainSprites(map.terrains.size());

            for (int i = 0; i < map.terrains.size(); i++)
            {
                terrainSeeds[i] = {i, map.terrains[i].count};
                terrainSprites[i] = map.terrains[i].spriteFile;
            }

            auto base = map::voronoi({2 * n, 2 * m}, terrainSeeds);
            std::cout << "Base made\n";
            for (int i = 0; i < 2 * n; i++)
            {
                for (int j = 0; j < 2 * m; j++)
                {
                    // std::cout << base[i][j] << ' ';
                    std::vector<engine::ComponentID> tileType = {engine::getComponentID<engine::Sprite>(), engine::getComponentID<engine::Transform>()};
                    engine::EntityID entity = world.createEntity(tileType);
                    auto &sprite = world.getComponent<engine::Sprite>(entity);
                    auto &transform = world.getComponent<engine::Transform>(entity);

                    sprite.texture.filePath = terrainSprites[base[i][j]];
                    sprite.vertices = map.cellVertices;
                    sprite.indices = {0, 1, 2, 2, 3, 0};
                    sprite.shaderParts = {engine::basePath + "../src/graphics/shaders/tex_vtx.glsl", engine::basePath + "../src/graphics/shaders/tex_frag.glsl"};

                    transform.position = {j - m, i - n};
                    transform.isStatic = true;
                }
            }
            break;
        }
    }

    void MapSys::update(engine::World &world)
    {
    }

    void MapSys::cleanup()
    {
    }
}