#pragma once
#include "system.hpp"
#include "../../utils/map_gen.hpp"
#include "../components/map.hpp"
#include "../components/sprite.hpp"
#include "../components/transform.hpp"
#include "../components/collider.hpp"
#include "../entity.hpp"
#include <array>

namespace engine
{
    // TODO: Only has Voronoi for now, no wrap around like what a spherical map would have
    class MapSys : public System
    {
    private:
        int mw, mh;
        int rpt, lpt = 0, bpt = 0, tpt;
        std::array<EntityID, 4> retileTriggers;

    public:
        void init(World &world) override;
        void update(World &world) override;
        void cleanup() override;
    };
}