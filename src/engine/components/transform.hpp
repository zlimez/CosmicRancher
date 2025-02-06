#pragma once
#include <glm/glm.hpp>
#include "../entity.hpp"
#include "../defs.hpp"

namespace engine
{
    struct Transform : Component
    {
        vec2 position;
        vec2 scale;
        float rotation;
    };

    struct BoxCollider : Component
    {
        float width;
        float height;
        vec2 center;
    };
}