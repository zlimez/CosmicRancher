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
        const int offset_ = 20; // Number of rows/cols shifted when player hits a retile trigger, also distance of trigger from player at center
        int mw_, mh_;
        int rpt_, lpt_ = 0, bpt_ = 0, tpt_;
        std::array<EntityID, 4> retileTriggers_;

    public:
        void init(World &world) override;
        void update(World &world) override;
        void cleanup() override;
    };
}