#pragma once
#include "../defs.hpp"
#include "../entity.hpp"

namespace engine
{
    struct BoxCollider : Component
    {
        float width_ = 1.0f;
        float height_ = 1.0f;
        vec2 center_ = {0.0f, 0.0f};
        bool isTrigger_ = false;
        bool verticesSet_ = false;
        std::vector<vec2> vertices_;

        BoxCollider() : vertices_(4) {}
    };

    struct Collision
    {
        bool isTrigger_ = false;
        EntityID self_;
        EntityID other_;
        vec2 normal_;
        float depth_;
    };

    struct Collisions : Component
    {
        std::vector<Collision> collisions_;

        Collisions &operator=(const Collisions &other)
        {
            if (this != &other)
                collisions_ = std::move(other.collisions_);
            return *this;
        }
    };
}