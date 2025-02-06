#pragma once
#include "../world.hpp"

namespace engine
{
    class System
    {
        virtual void init(World &world) = 0;
        virtual void update(World &world) = 0;
    };
};