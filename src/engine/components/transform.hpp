#pragma once
#include <glm/glm.hpp>
#include "../entity.hpp"
#include "../defs.hpp"

namespace engine
{
    struct Transform : Component
    {
        bool isStatic_ = false;
        vec2 position_ = {0.0f, 0.0f};
        vec2 scale_ = {1.0f, 1.0f};
        float rotation_ = 0.0f;
    };

    struct Movement : Component
    {
        vec2 velocity_ = {0.0f, 0.0f};
    };
}