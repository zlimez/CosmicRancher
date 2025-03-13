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
        bool isTrigger = false;
        bool verticesSet = false;
        std::vector<vec2> vertices;

        BoxCollider() : vertices(4) {}
    };

    struct Collision
    {
        bool isTrigger = false;
        EntityID otherEntity;
        vec2 normal;
        float depth;
    };

    struct Collisions : Component
    {
        std::vector<Collision> collisions;

        Collisions &operator=(const Collisions &other)
        {
            if (this != &other)
                collisions = std::move(other.collisions);
            return *this;
        }
    };
}