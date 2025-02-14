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