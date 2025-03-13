#pragma once
#include "system.hpp"
#include "../components/collider.hpp"
#include "../components/transform.hpp"
#include <unordered_set>

// NOTE: Detects collision between entities with BoxCollider components, should send a collision message -> needs storage for message
namespace engine
{
    struct Contact
    {
        bool isColliding = false;
        vec2 normal;
        float depth;
    };

    class CollisionDetector : public System
    {
    private:
        std::vector<std::tuple<EntityID, BoxCollider *, Transform *>> staticColliders;
        std::vector<std::tuple<EntityID, BoxCollider *, Transform *>> dynamicColliders;
        std::unordered_set<EntityID> lastCollidedEntities, currCollidedEntities;
        void setVertices(BoxCollider &collider, Transform &transform);
        inline float getOverlap(float min1, float max1, float min2, float max2) { return std::min(max1, max2) - std::max(min1, min2); }
        void projectPoly(const std::vector<vec2> &poly, const vec2 &axis, float &minProj, float &maxProj);
        Contact findSATCol(const std::vector<vec2> &poly1, const std::vector<vec2> &poly2);

    public:
        CollisionDetector() {}
        ~CollisionDetector() {}
        void init(World &world) override;
        void update(World &world) override;
        void cleanup() override;
    };
};