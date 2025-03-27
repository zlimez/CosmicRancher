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
            auto &map = maps.blocks_[0];
            auto [n, m] = map.dim_;
            std::vector<std::pair<int, int>> terrainSeeds(map.terrains_.size());
            std::vector<std::string> terrainSprites(map.terrains_.size());

            for (int i = 0; i < map.terrains_.size(); i++)
            {
                terrainSeeds[i] = {i, map.terrains_[i].count_};
                terrainSprites[i] = map.terrains_[i].spriteFile_;
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

                    sprite.texture_.filePath = terrainSprites[base[i][j]];
                    sprite.vertices_ = map.cellVertices_;
                    sprite.indices_ = {0, 1, 2, 2, 3, 0};
                    sprite.shaderParts_ = {engine::basePath + "../src/graphics/shaders/tex_vtx.glsl", engine::basePath + "../src/graphics/shaders/tex_frag.glsl"};

                    transform.position_ = {j - m, i - n};
                    transform.isStatic_ = true;

                    cell.pos_ = {j, i};
                }
            }

            rpt_ = 2 * m - 1;
            tpt_ = 2 * n - 1;
            mw_ = 2 * m;
            mh_ = 2 * n;
            break;
        }

        // NOTE: Assume player at 0,0 when placing triggers
        std::vector<engine::ComponentID> triggerType = {engine::getComponentID<engine::BoxCollider>(), engine::getComponentID<engine::Transform>()};
        // NOTE: should depend on camera zoom surrounds the play such that the map infinitely extends and wraps around
        float height = offset_ * 2, width = 10.0f;
        std::vector<std::tuple<float, float, vec2>> triggers = {
            {height, width, {offset_, 0.0f}},   // right
            {height, width, {-offset_, 0.0f}},  // left
            {width, height, {0.0f, offset_}},   // top
            {width, height, {0.0f, -offset_}}}; // bottom

        for (size_t i = 0; i < 4; i++)
        {
            EntityID triggerId = world.createEntity(triggerType);
            retileTriggers_[i] = triggerId;
            auto &trigger = world.getComponent<engine::BoxCollider>(triggerId);
            auto &transform = world.getComponent<engine::Transform>(triggerId);
            trigger.height_ = std::get<0>(triggers[i]);
            trigger.width_ = std::get<1>(triggers[i]);
            transform.position_ = std::get<2>(triggers[i]);
            transform.isStatic_ = true;
            trigger.isTrigger_ = true;
        }
    }

    void MapSys::update(engine::World &world)
    {
        int miny = -1, maxy = -1, minx = -1, maxx = -1;
        bool ywraps = false, xwraps = false;
        int yshift = mh_, xshift = mw_;
        std::vector<Transform *> triggerTfms(4);
        // NOTE: Temp assume all collisions are with player
        for (size_t i = 0; i < 4; i++)
        {
            if (!world.hasComponent<engine::Collisions>(retileTriggers_[i]))
                continue;
            triggerTfms[i] = &world.getComponent<engine::Transform>(retileTriggers_[i]);
            if (i == 0)
            {
                minx = lpt_;
                maxx = (minx + offset_) % mw_;
                xwraps = maxx < minx;
                lpt_ = maxx;
            }
            else if (i == 1)
            {
                maxx = (rpt_ + 1) % mw_;
                minx = (maxx - offset_ + mw_) % mw_;
                xwraps = maxx < minx;
                rpt_ = (minx - 1) % mw_;
                xshift = -xshift;
            }
            else if (i == 2)
            {
                miny = bpt_;
                maxy = (miny + offset_) % mh_;
                ywraps = maxy < miny;
                bpt_ = maxy;
            }
            else
            {
                maxy = (tpt_ + 1) % mh_;
                miny = (maxy - offset_ + mh_) % mh_;
                ywraps = maxy < miny;
                tpt_ = (miny - 1) % mh_;
                yshift = -yshift;
            }
        }

        for (auto &triggerTfm : triggerTfms)
        {
            triggerTfm->position_.x += xshift;
            triggerTfm->position_.y += yshift;
        }

        Type mapCellType;
        mapCellType.set(engine::getComponentID<engine::MapCell>());
        mapCellType.set(engine::getComponentID<engine::Transform>());
        auto &mapCellArchs = world.getArchetypesWith(mapCellType);

        for (auto mapCellArch : mapCellArchs)
        {
            auto &mapCells = mapCellArch->getComponentRow<engine::MapCell>();
            auto &transforms = mapCellArch->getComponentRow<engine::Transform>();
            for (size_t i = 0; i < mapCellArch->count_; i++)
            {
                auto &mapCell = mapCells.blocks_[i];
                auto &transform = transforms.blocks_[i];
                if (minx != -1 &&
                    ((!xwraps && minx <= mapCell.pos_.first && mapCell.pos_.first < maxx) || (xwraps && (minx <= mapCell.pos_.first || mapCell.pos_.first < maxx))))
                    transform.position_.x += xshift;

                if (miny != -1 &&
                    ((!ywraps && miny <= mapCell.pos_.second && mapCell.pos_.second < maxy) || (ywraps && (miny <= mapCell.pos_.second || mapCell.pos_.second < maxy))))
                    transform.position_.y += yshift;
            }
        }
    }

    void MapSys::cleanup() {}
}