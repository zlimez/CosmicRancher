#pragma once
#include "../defs.hpp"
#include "../entity.hpp"

namespace engine
{
    struct BoxCollider : Component
    {
        float width = 1.0f;
        float height = 1.0f;
        vec2 center = {0.0f, 0.0f};

        BoxCollider &operator=(const BoxCollider &other)
        {
            if (this != &other)
            {

                width = other.width;
                height = other.height;
                center = other.center;
            }
            return *this;
        }
    };
}