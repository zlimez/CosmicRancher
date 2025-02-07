#pragma once
#include <glm/glm.hpp>
#include "../entity.hpp"
#include "../defs.hpp"

namespace engine
{
    struct Transform : Component
    {
        bool isStatic;
        vec2 position;
        vec2 scale;
        float rotation;

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
        float width;
        float height;
        vec2 center;

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