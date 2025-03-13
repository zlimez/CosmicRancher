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

        Transform &operator=(const Transform &other)
        {
            if (this != &other)
            {
                isStatic = other.isStatic;
                position = other.position;
                rotation = other.rotation;
                scale = other.scale;
            }
            return *this;
        }
    };
}