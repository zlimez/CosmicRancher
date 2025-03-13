#include "collision_detector.hpp"

namespace engine
{
    void CollisionDetector::projectPoly(const std::vector<vec2> &poly, const vec2 &axis, float &minProj, float &maxProj)
    {
        minProj = glm::dot(poly[0], axis);
        maxProj = minProj;
        for (size_t i = 1; i < poly.size(); i++)
        {
            float proj = glm::dot(poly[i], axis);
            minProj = std::min(minProj, proj);
            maxProj = std::max(maxProj, proj);
        }
    }

    Contact CollisionDetector::findSATCol(const std::vector<vec2> &poly1, const std::vector<vec2> &poly2)
    {
        Contact contact;
        contact.depth = std::numeric_limits<float>::max();
        contact.normal = {0.0f, 0.0f};

        for (size_t i = 0; i < poly1.size() + poly2.size(); i++)
        {
            vec2 edge = i < poly1.size() ? poly1[(i + 1) % poly1.size()] - poly1[i] : poly2[(i + 1) % poly2.size()] - poly2[i];
            vec2 axis = glm::normalize(vec2(-edge.y, edge.x));
            float minProj1, maxProj1, minProj2, maxProj2;
            projectPoly(poly1, axis, minProj1, maxProj1);
            projectPoly(poly2, axis, minProj2, maxProj2);
            float overlap = getOverlap(minProj1, maxProj1, minProj2, maxProj2);
            if (overlap <= 0)
                return contact;
            if (overlap < contact.depth)
            {
                contact.depth = overlap;
                contact.normal = axis;
            }
        }

        return contact;
    }

    void CollisionDetector::setVertices(BoxCollider &collider, Transform &transform)
    {
        float hw = collider.width / 2, hh = collider.height / 2;
        collider.vertices[0] = {hw, hh};
        collider.vertices[1] = {-hw, hh};
        collider.vertices[2] = {-hw, -hh};
        collider.vertices[3] = {hw, -hh};

        float angle = glm::radians(transform.rotation);
        glm::mat2 rotation = glm::mat2(glm::vec2(cos(angle), -sin(angle)), glm::vec2(sin(angle), cos(angle)));

        for (auto &vertex : collider.vertices)
        {
            vertex = rotation * vertex;
            vertex += transform.position;
        }
    }

    void CollisionDetector::init(World &world) {}

    void CollisionDetector::update(World &world)
    {
        staticColliders.clear();
        dynamicColliders.clear();

        Type colliderType;
        colliderType.set(getComponentID<BoxCollider>());
        colliderType.set(getComponentID<Transform>());
        auto &colliderArchs = world.getArchetypesWith(colliderType);

        for (auto colliderArch : colliderArchs)
        {
            auto &colliderRow = colliderArch->getComponentRow<BoxCollider>();
            auto &tfmRow = colliderArch->getComponentRow<Transform>();
            for (size_t i = 0; i < colliderArch->count; i++)
            {
                if (!colliderRow.blocks[i].active)
                    continue;
                if (tfmRow.blocks[i].isStatic)
                {
                    staticColliders.push_back({colliderArch->indexToEntities[i], &colliderRow.blocks[i], &tfmRow.blocks[i]});
                    if (!colliderRow.blocks[i].verticesSet)
                    {
                        setVertices(colliderRow.blocks[i], tfmRow.blocks[i]);
                        colliderRow.blocks[i].verticesSet = true;
                    }
                }
                else
                {
                    dynamicColliders.push_back({colliderArch->indexToEntities[i], &colliderRow.blocks[i], &tfmRow.blocks[i]});
                    setVertices(colliderRow.blocks[i], tfmRow.blocks[i]);
                }
            }
        }

        Type collisionsType;
        collisionsType.set(getComponentID<Collisions>());
        auto &collisionsArchs = world.getArchetypesWith(collisionsType);
        for (auto collisionsArch : collisionsArchs)
        {
            auto &collisionsRow = collisionsArch->getComponentRow<Collisions>();
            for (size_t i = 0; i < collisionsArch->count; i++)
                collisionsRow.blocks[i].collisions.clear();
        }

        for (size_t i = 0; i < dynamicColliders.size(); i++)
        {
            auto &[dynEntity, dynCollider, dynTfm] = dynamicColliders[i];
            for (auto &[statEntity, statCollider, statTfm] : staticColliders)
            {
                Contact contact = findSATCol(dynCollider->vertices, statCollider->vertices);
                if (contact.isColliding)
                {
                    currCollidedEntities.insert(statEntity);
                    currCollidedEntities.insert(dynEntity);

                    Collision collision;
                    collision.isTrigger = dynCollider->isTrigger || statCollider->isTrigger;
                    collision.otherEntity = statEntity;
                    collision.normal = contact.normal;
                    collision.depth = contact.depth;

                    world.addComponent<Collisions>(dynEntity).collisions.push_back(collision);
                    world.addComponent<Collisions>(statEntity).collisions.push_back(collision);
                }
            }
            for (int j = i + 1; j < dynamicColliders.size(); j++)
            {
                auto &[dynEntity2, dynCollider2, dynTfm2] = dynamicColliders[j];
                Contact contact = findSATCol(dynCollider->vertices, dynCollider2->vertices);
                if (contact.isColliding)
                {
                    currCollidedEntities.insert(dynEntity);
                    currCollidedEntities.insert(dynEntity2);

                    Collision collision;
                    collision.isTrigger = dynCollider->isTrigger || dynCollider2->isTrigger;
                    collision.otherEntity = dynEntity2;
                    collision.normal = contact.normal;
                    collision.depth = contact.depth;

                    world.addComponent<Collisions>(dynEntity).collisions.push_back(collision);
                    world.addComponent<Collisions>(dynEntity2).collisions.push_back(collision);
                }
            }
        }

        // TODO: A better heuristic for removing collisions component (expensive)
        for (EntityID lastCollidedEntity : lastCollidedEntities)
            if (!currCollidedEntities.count(lastCollidedEntity))
                world.removeComponent<Collisions>(lastCollidedEntity);

        swap(lastCollidedEntities, currCollidedEntities);
        currCollidedEntities.clear();
    }

    void CollisionDetector::cleanup()
    {
        lastCollidedEntities.clear();
        currCollidedEntities.clear();
        staticColliders.clear();
        dynamicColliders.clear();
    }
}