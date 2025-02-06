#include "world.hpp"

// ComponentID is not identical across runs, needs to be handled if serialization is required
namespace engine
{
    template <typename T>
    uint32_t getComponentID()
    {
        static int id = ComponentsRegistry::GetNewID();
        return id;
    }

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

    template <typename T>
    void World::addComponent(EntityID entityId)
    {
        ComponentID componentId = GetComponentID<T>();
        if (entityArchetypes[entityId]->type[componentId])
            return;
        Type newType = entityArchetypes[entityId]->type, oldType = newType;
        newType.set(componentId);
        auto &dstArchetype = archetypes[newType], &srcArchetype = archetypes[oldType];
        dstArchetype.addEntity();
        for (ComponentID i = 0; i < newType.size(); i++)
            if (oldType[i])
                dstArchetype.copyComponent(entityId, i, srcArchetype.getComponent(entityId, i));

        archetypes[oldType].removeEntity(entityId);
        entityArchetypes[entityId] = &archetypes[newType];
    }

    template <typename T>
    void World::removeComponent(EntityID entityId)
    {
        ComponentID componentId = GetComponentID<T>();
        if (!entityArchetypes[entityId]->type[componentId])
            return;
        Type newType = entityArchetypes[entityId]->type, oldType = newType;
        newType.reset(componentId);
        auto &dstArchetype = archetypes[newType], &srcArchetype = archetypes[oldType];
        for (ComponentID i = 0; i < newType.size(); i++)
            if (newType[i])
                dstArchetype.copyComponent(entityId, i, srcArchetype.getComponent(entityId, i));

        archetypes[oldType].removeEntity(entityId);
        entityArchetypes[entityId] = &archetypes[newType];
    }

    template <typename T>
    bool World::hasComponent(EntityID entityId)
    {
        return entityArchetypes[entityId]->type[GetComponentID<T>()];
    }

    template <typename T>
    T &World::getComponent(EntityID entityId)
    {
        if (!hasComponent(entityId, GetComponentID<T>()))
            return nullptr;
        return entityArchetypes[entityId]->getComponent<T>(entityId);
    }

    std::vector<Archetype *> World::getArchetypesWithComponents(Type type)
    {
        if (archetypesWithComponents.find(type) != archetypesWithComponents.end())
            return archetypesWithComponents[type];
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
        archetypesWithComponents[type] = superTypes;
        return superTypes;
    }

    std::vector<EntityID> World::getEntitiesWithComponents(Type type)
    {
        if (entitiesWithComponents.find(type) != entitiesWithComponents.end())
            return entitiesWithComponents[type];
        std::vector<EntityID> entities;
        for (auto archetype : getArchetypesWithComponents(type))
            for (auto &entity : archetype->indexToEntities)
                entities.push_back(entity.first);
        entitiesWithComponents[type] = entities;
        return entities;
    }
}
