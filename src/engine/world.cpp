#include "world.hpp"

// ComponentID is not identical across runs, needs to be handled if serialization is required
namespace engine
{
    World::World(uint32 entityCnt, uint32 componentCnt)
    {
        idPool_ = IdPool();
        this->entityCnt_ = entityCnt;
        this->componentCnt_ = componentCnt;
        entityArchetypes_.resize(entityCnt);
        if (componentCnt > MAX_COMPONENT_TYPES)
            throw std::runtime_error("Max component types reached");
        archetypes_.reserve(1LL << componentCnt);
    }

    void World::addArchetype(Type type)
    {
        archetypes_.emplace(type, type);
        for (auto &[etype, archs] : archetypesWith_)
            if ((etype & type) == etype)
                archs.push_back(&archetypes_.at(type));
    }

    EntityID World::createEntity(std::vector<ComponentID> &componentTypes)
    {
        EntityID id = idPool_.acquireId();
        Type type;
        for (auto component : componentTypes)
            type.set(component);

        if (!archetypes_.count(type))
            addArchetype(type);
        auto &archetype = archetypes_.at(type);
        archetype.addEntity(id);
        entityArchetypes_[id] = &archetype;
        return id;
    }

    void World::destroyEntity(EntityID entityId)
    {
        idPool_.releaseId(entityId);
        entityArchetypes_[entityId]->removeEntity(entityId);
        entityArchetypes_[entityId] = nullptr;
    }

    std::vector<Archetype *> &World::getArchetypesWith(Type type)
    {
        if (archetypesWith_.find(type) != archetypesWith_.end())
            return archetypesWith_[type];
        std::vector<Archetype *> superTypes;
        if (archetypes_.count(type))
            superTypes.push_back(&archetypes_.at(type));
        auto minusOne = [](Type type)
        {
            size_t i = 0;
            while (!type[i])
            {
                type[i] = 1;
                i++;
            }
            type[i] = 0;
            return type;
        };

        Type mask = ~type, subset = ~type;
        // Component count can be less than MAX_COMPONENT_TYPES, reduces number of subsets
        for (int i = componentCnt_; i < MAX_COMPONENT_TYPES; i++)
            mask[i] = 0;

        while (subset.any())
        {
            Type fType = subset | type;
            if (archetypes_.count(fType))
                superTypes.push_back(&archetypes_.at(fType));
            subset = (mask & minusOne(subset));
        }
        archetypesWith_[type] = std::move(superTypes);
        return archetypesWith_[type];
    }

    // TODO: When entities are destroyed or change archetypes, cache should be invalidated
    std::vector<EntityID> &World::getEntitiesWith(Type type)
    {
        if (entitiesWith_.find(type) != entitiesWith_.end())
            return entitiesWith_[type];
        std::vector<EntityID> entities;
        for (auto archetype : getArchetypesWith(type))
            for (auto &entity : archetype->indexToEntities_)
                entities.push_back(entity.first);
        entitiesWith_[type] = std::move(entities);
        return entitiesWith_[type];
    }
}
