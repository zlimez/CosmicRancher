#include "world.hpp"

// ComponentID is not identical across runs, needs to be handled if serialization is required
namespace engine
{
    World::World(uint32 entityCnt, uint32 componentCnt)
    {
        idPool = IdPool();
        this->entityCnt = entityCnt;
        this->componentCnt = componentCnt;
        entityArchetypes.resize(entityCnt);
        if (componentCnt > MAX_COMPONENT_TYPES)
            throw std::runtime_error("Max component types reached");
        archetypes.reserve(1LL << componentCnt);
    }

    EntityID World::createEntity(std::vector<ComponentID> &componentTypes)
    {
        EntityID id = idPool.acquireId();
        Type type;
        for (auto component : componentTypes)
            type.set(component);

        if (!archetypes.count(type))
            archetypes[type].init(type);
        auto &archetype = archetypes[type];
        archetype.addEntity(id);
        entityArchetypes[id] = &archetype;
        return id;
    }

    void World::destroyEntity(EntityID entityId)
    {
        idPool.releaseId(entityId);
        entityArchetypes[entityId]->removeEntity(entityId);
        entityArchetypes[entityId] = nullptr;
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
        // Component count can be less than MAX_COMPONENT_TYPES, reduces number of subsets
        for (int i = componentCnt; i < MAX_COMPONENT_TYPES; i++)
            mask[i] = 0;

        while (subset.any())
        {
            Type fType = subset | type;
            if (archetypes.count(fType))
                superTypes.push_back(&archetypes[fType]);
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
