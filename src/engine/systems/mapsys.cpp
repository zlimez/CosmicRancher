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
            // NOTE: All cells assumed to have width height 1
            for (int i = 0; i < 2 * n; i++)
            {
                for (int j = 0; j < 2 * m; j++)
                {
                    std::vector<engine::ComponentID> tileType = {engine::getComponentID<engine::Sprite>(), engine::getComponentID<engine::Transform>(), engine::getComponentID<engine::MapCell>()};
                    engine::EntityID entity = world.createEntity(tileType);
                    auto &sprite = world.getComponent<engine::Sprite>(entity);
                    auto &transform = world.getComponent<engine::Transform>(entity);
                    auto &cell = world.getComponent<engine::MapCell>(entity);

                    sprite.texture.filePath = terrainSprites[base[i][j]];
                    sprite.vertices = map.cellVertices;
                    sprite.indices = {0, 1, 2, 2, 3, 0};
                    sprite.shaderParts = {engine::basePath + "../src/graphics/shaders/tex_vtx.glsl", engine::basePath + "../src/graphics/shaders/tex_frag.glsl"};

                    transform.position = {j - m, i - n};
                    transform.isStatic = true;

                    cell.pos = {j, i};
                }
            }

            rpt = 2 * m - 1;
            tpt = 2 * n - 1;
            mw = 2 * m;
            mh = 2 * n;
            break;
        }

        // NOTE: Assume player at 0,0 when placing triggers
        std::vector<engine::ComponentID> triggerType = {engine::getComponentID<engine::BoxCollider>(), engine::getComponentID<engine::Transform>()};
        const float offset = 20.0f; // NOTE: should depend on camera zoom surrounds the play such that the map infinitely extends and wraps around
        float height = offset * 2, width = 10.0f;
        std::vector<std::tuple<float, float, vec2>> triggers = {
            {height, width, {offset, 0.0f}},   // right
            {height, width, {-offset, 0.0f}},  // left
            {width, height, {0.0f, offset}},   // top
            {width, height, {0.0f, -offset}}}; // bottom

        for (size_t i = 0; i < 4; i++)
        {
            EntityID triggerId = world.createEntity(triggerType);
            retileTriggers[i] = triggerId;
            auto &trigger = world.getComponent<engine::BoxCollider>(triggerId);
            auto &transform = world.getComponent<engine::Transform>(triggerId);
            trigger.height = std::get<0>(triggers[i]);
            trigger.width = std::get<1>(triggers[i]);
            transform.position = std::get<2>(triggers[i]);
            transform.isStatic = true;
            trigger.isTrigger = true;
        }
    }

    void MapSys::update(engine::World &world)
    {
        const int shiftSize = 20; // NOTE: should match offset

        int miny = -1, maxy = -1, minx = -1, maxx = -1;
        bool ywraps = false, xwraps = false;
        int yshift = mh, xshift = mw;
        // NOTE: Temp assume all collisions are with player
        for (size_t i = 0; i < 4; i++)
        {
            if (!world.hasComponent<engine::Collisions>(retileTriggers[i]))
                continue;
            if (i == 0)
            {
                minx = lpt;
                maxx = (minx + shiftSize) % mw;
                xwraps = maxx < minx;
                lpt = maxx;
            }
            else if (i == 1)
            {
                maxx = (rpt + 1) % mw;
                minx = (maxx - shiftSize + mw) % mw;
                xwraps = maxx < minx;
                rpt = (minx - 1) % mw;
                xshift = -xshift;
            }
            else if (i == 2)
            {
                miny = bpt;
                maxy = (miny + shiftSize) % mh;
                ywraps = maxy < miny;
                bpt = maxy;
            }
            else
            {
                maxy = (tpt + 1) % mh;
                miny = (maxy - shiftSize + mh) % mh;
                ywraps = maxy < miny;
                tpt = (miny - 1) % mh;
                yshift = -yshift;
            }
        }

        Type mapCellType;
        mapCellType.set(engine::getComponentID<engine::MapCell>());
        mapCellType.set(engine::getComponentID<engine::Transform>());
        auto &mapCellArchs = world.getArchetypesWith(mapCellType);

        for (auto mapCellArch : mapCellArchs)
        {
            auto &mapCells = mapCellArch->getComponentRow<engine::MapCell>();
            auto &transforms = mapCellArch->getComponentRow<engine::Transform>();
            for (size_t i = 0; i < mapCellArch->count; i++)
            {
                auto &mapCell = mapCells.blocks[i];
                auto &transform = transforms.blocks[i];
                if (minx != -1 &&
                    ((!xwraps && minx <= mapCell.pos.first && mapCell.pos.first < maxx) || (xwraps && (minx <= mapCell.pos.first || mapCell.pos.first < maxx))))
                    transform.position.x += xshift;

                if (miny != -1 &&
                    ((!ywraps && miny <= mapCell.pos.second && mapCell.pos.second < maxy) || (ywraps && (miny <= mapCell.pos.second || mapCell.pos.second < maxy))))
                    transform.position.y += yshift;
            }
        }
    }

    void MapSys::cleanup() {}
}