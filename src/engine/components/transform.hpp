#pragma once
#include <glm/glm.hpp>
#include "../entity.hpp"
#include "../defs.hpp"

namespace engine
{
    struct Transform : Component
    {
        bool isStatic = false;
        vec2 position = {0.0f, 0.0f};
        vec2 scale = {1.0f, 1.0f};
        float rotation = 0.0f;
    };

    struct Movement : Component
    {
        vec2 velocity = {0.0f, 0.0f};
    };
}