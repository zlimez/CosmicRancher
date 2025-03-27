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
        bool isColliding_ = false;
        vec2 normal_;
        float depth_;
    };

    class CollisionDetector : public System
    {
    private:
        // FIXME: During addComponent archetype change data is moved subsequent collision involving the same collider will be accessing freed memory not ideal
        std::vector<std::tuple<EntityID, BoxCollider *, Transform *>> staticColliders_;
        std::vector<std::tuple<EntityID, BoxCollider *, Transform *>> dynamicColliders_;
        std::vector<Collision> collisions_;
        std::unordered_set<EntityID> lastColEntities_, currColEntities_;
        void setVertices(BoxCollider &collider, Transform &transform);
        inline float getOverlap(float min1, float max1, float min2, float max2) { return std::min(max1, max2) - std::max(min1, min2); }
        void addCollision(Contact &contact, BoxCollider *col1, BoxCollider *col2, EntityID ent1, EntityID ent2);
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