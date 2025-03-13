#pragma once
#include "system.hpp"
#include "../components/transform.hpp"
#include "../time.hpp"

namespace engine
{
    class MovementSys : public System
    {
    public:
        MovementSys() {}
        ~MovementSys() {}
        void init(World &world) override;
        void update(World &world) override;
        void cleanup() override;
    };
}