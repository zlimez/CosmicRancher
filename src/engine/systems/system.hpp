#pragma once
#include "../world.hpp"

namespace engine
{
    class System
    {
    public:
        virtual ~System() {}
        virtual void init(World &world) = 0;
        virtual void update(World &world) = 0;
        virtual void cleanup() = 0;
    };
};