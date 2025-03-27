#include "collision_detector.hpp"
#include "../../utils/logger.hpp"

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
        contact.depth_ = std::numeric_limits<float>::max();
        contact.normal_ = {0.0f, 0.0f};

        for (size_t i = 0; i < poly1.size() + poly2.size(); i++)
        {
            size_t j = (i < poly1.size()) ? i : i - poly1.size();
            vec2 edge = (i < poly1.size()) ? poly1[(j + 1) % poly1.size()] - poly1[j] : poly2[(j + 1) % poly2.size()] - poly2[j];
            vec2 axis = glm::normalize(vec2(-edge.y, edge.x));
            float minProj1, maxProj1, minProj2, maxProj2;
            projectPoly(poly1, axis, minProj1, maxProj1);
            projectPoly(poly2, axis, minProj2, maxProj2);
            float overlap = getOverlap(minProj1, maxProj1, minProj2, maxProj2);
            if (overlap <= 1e-6)
                return contact;
            if (overlap < contact.depth_)
            {
                contact.depth_ = overlap;
                contact.normal_ = axis;
            }
        }

        contact.isColliding_ = true;
        return contact;
    }

    void CollisionDetector::setVertices(BoxCollider &collider, Transform &transform)
    {
        float hw = collider.width_ / 2, hh = collider.height_ / 2;
        collider.vertices_[0] = {hw, hh};
        collider.vertices_[1] = {-hw, hh};
        collider.vertices_[2] = {-hw, -hh};
        collider.vertices_[3] = {hw, -hh};

        float angle = glm::radians(transform.rotation_);
        glm::mat2 rotation = glm::mat2(glm::vec2(cos(angle), -sin(angle)), glm::vec2(sin(angle), cos(angle)));

        for (auto &vertex : collider.vertices_)
        {
            vertex = rotation * vertex;
            vertex += transform.position_;
        }
    }

    void CollisionDetector::addCollision(Contact &contact, BoxCollider *col1, BoxCollider *col2, EntityID ent1, EntityID ent2)
    {
        currColEntities_.insert(ent1);
        currColEntities_.insert(ent2);

        Collision collision;
        collision.isTrigger_ = col1->isTrigger_ || col2->isTrigger_;
        collision.self_ = ent1;
        collision.other_ = ent2;
        collision.normal_ = contact.normal_;
        collision.depth_ = contact.depth_;
        collisions_.push_back(collision);
        collision.self_ = ent2;
        collision.other_ = ent1;
        collisions_.push_back(collision);
    }

    void CollisionDetector::init(World &world) {}

    void CollisionDetector::update(World &world)
    {
        staticColliders_.clear();
        dynamicColliders_.clear();
        collisions_.clear();
        currColEntities_.clear();

        Type colliderType;
        colliderType.set(getComponentID<BoxCollider>());
        colliderType.set(getComponentID<Transform>());
        auto &colliderArchs = world.getArchetypesWith(colliderType);
        for (auto colliderArch : colliderArchs)
        {
            auto &colliderRow = colliderArch->getComponentRow<BoxCollider>();
            auto &tfmRow = colliderArch->getComponentRow<Transform>();
            for (size_t i = 0; i < colliderArch->count_; i++)
            {
                if (!colliderRow.blocks_[i].active_)
                    continue;
                if (tfmRow.blocks_[i].isStatic_)
                {
                    staticColliders_.push_back({colliderArch->indexToEntities_[i], &colliderRow.blocks_[i], &tfmRow.blocks_[i]});
                    if (!colliderRow.blocks_[i].verticesSet_)
                    {
                        setVertices(colliderRow.blocks_[i], tfmRow.blocks_[i]);
                        colliderRow.blocks_[i].verticesSet_ = true;
                    }
                }
                else
                {
                    dynamicColliders_.push_back({colliderArch->indexToEntities_[i], &colliderRow.blocks_[i], &tfmRow.blocks_[i]});
                    setVertices(colliderRow.blocks_[i], tfmRow.blocks_[i]);
                }
            }
        }

        Type collisionsType;
        collisionsType.set(getComponentID<Collisions>());
        auto &collisionsArchs = world.getArchetypesWith(collisionsType);
        for (auto collisionsArch : collisionsArchs)
        {
            auto &collisionsRow = collisionsArch->getComponentRow<Collisions>();
            for (size_t i = 0; i < collisionsArch->count_; i++)
                collisionsRow.blocks_[i].collisions_.clear();
        }

        for (size_t i = 0; i < dynamicColliders_.size(); i++)
        {
            auto &[dynEntity, dynCollider, dynTfm] = dynamicColliders_[i];
            for (auto &[statEntity, statCollider, statTfm] : staticColliders_)
            {
                Contact contact = findSATCol(dynCollider->vertices_, statCollider->vertices_);
                if (contact.isColliding_)
                    addCollision(contact, statCollider, dynCollider, statEntity, dynEntity);
            }
            for (int j = i + 1; j < dynamicColliders_.size(); j++)
            {
                auto &[dynEntity2, dynCollider2, dynTfm2] = dynamicColliders_[j];
                Contact contact = findSATCol(dynCollider->vertices_, dynCollider2->vertices_);
                if (contact.isColliding_)
                    addCollision(contact, dynCollider, dynCollider2, dynEntity, dynEntity2);
            }
        }

        // NOTE: DO NOT addComponent with the detection segment will invalidate the pointers due to archetype change
        for (EntityID colEntity : currColEntities_)
            world.addComponent<Collisions>(colEntity);

        if (!currColEntities_.empty())
        {
            for (auto e : currColEntities_)
                std::cout << e << ' ';
            std::cout << '\n';
        }

        for (Collision &collision : collisions_)
            world.getComponent<Collisions>(collision.self_).collisions_.emplace_back(collision);

        // TODO: A better heuristic for removing collisions component (expensive)
        // for (EntityID lastColEntity : lastColEntities_)
        //     if (!currColEntities_.count(lastColEntity))
        //         world.removeComponent<Collisions>(lastColEntity);

        swap(lastColEntities_, currColEntities_);
    }

    void CollisionDetector::cleanup()
    {
        lastColEntities_.clear();
        currColEntities_.clear();
        staticColliders_.clear();
        dynamicColliders_.clear();
    }
}