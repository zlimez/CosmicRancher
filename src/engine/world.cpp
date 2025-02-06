#include "world.hpp"

// ComponentID is not identical across runs, needs to be handled if serialization is required
namespace engine
{
    World::World(uint32 expectedEntities, uint32 expectedComponentCnt)
    {
        idPool = IdPool();
        entityArchetypes.resize(expectedEntities);
        if (expectedComponentCnt > MAX_COMPONENT_TYPES)
            throw std::runtime_error("Max component types reached");
        archetypes.reserve(1LL << expectedComponentCnt);
    }

    EntityID World::createEntity(std::vector<ComponentID> &componentTypes)
    {
        EntityID id = idPool.acquireId();
        Type type;
        for (auto component : componentTypes)
            type.set(component);
        auto &archetype = archetypes[type];
        archetype.addEntity();
        entityArchetypes[id] = &archetype;
        return id;
    }

    void World::destroyEntity(EntityID entityId)
    {
        idPool.releaseId(entityId);
        entityArchetypes[entityId]->removeEntity(entityId);
    }

    std::vector<Archetype *> &World::getArchetypesWith(Type type)
    {
        if (archetypesWith.find(type) != archetypesWith.end())
            return archetypesWith[type];
        std::vector<Archetype *> superTypes;
        superTypes.push_back(&archetypes[type]);
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

        Type mask = ~type, subset = mask;
        while (subset.any())
        {
            superTypes.push_back(&archetypes[subset | type]);
            subset = (mask & minusOne(subset));
        }
        archetypesWith[type] = std::move(superTypes);
        return archetypesWith[type];
    }

    // TODO: When entities are destroyed or change archetypes, cache should be invalidated
    std::vector<EntityID> &World::getEntitiesWith(Type type)
    {
        if (entitiesWith.find(type) != entitiesWith.end())
            return entitiesWith[type];
        std::vector<EntityID> entities;
        for (auto archetype : getArchetypesWith(type))
            for (auto &entity : archetype->indexToEntities)
                entities.push_back(entity.first);
        entitiesWith[type] = std::move(entities);
        return entitiesWith[type];
    }
}
