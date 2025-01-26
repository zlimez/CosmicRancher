#include "world.hpp"

// ComponentID is not identical across runs, needs to be handled if serialization is required
namespace engine
{
    template <typename T>
    uint32_t getComponentID()
    {
        static int id = Component::GetNewID();
        return id;
    }

    World::World(uint32 expectedEntities, uint32 expectedComponentCnt)
    {
        idPool = IdPool();
        entityArchetypes.reserve(expectedEntities);
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
        uint32 ind = archetype.addEntity();
        entityArchetypes[id] = archetype;
        return id;
    }

    void World::destroyEntity(EntityID entityId)
    {
        idPool.releaseId(entityId);
        auto &archetype = entityArchetypes[entityId];
        archetype.removeEntity(entityId);
    }

    template <typename T>
    void World::addComponent(EntityID entityId)
    {
        ComponentID componentId = GetComponentID<T>();
        if (entityArchetypes[entityId].type[componentId])
            return;
        Type newType = entityArchetypes[entityId].type, oldType = newType;
        newType.set(componentId);
        auto &dstArchetype = archetypes[newType], &srcArchetype = archetypes[oldType];
        dstArchetype.addEntity();
        for (ComponentID i = 0; i < newType.size(); i++)
            if (oldType[i])
                dstArchetype.copyComponent(entityId, i, srcArchetype.getComponent(entityId, i));

        archetypes[oldType].removeEntity(entityId);
        entityArchetypes[entityId] = archetypes[newType];
    }

    template <typename T>
    void World::removeComponent(EntityID entityId)
    {
        ComponentID componentId = GetComponentID<T>();
        if (!entityArchetypes[entityId].type[componentId])
            return;
        Type newType = entityArchetypes[entityId].type;
        newType.reset(componentId);
        auto &dstArchetype = archetypes[newType], &srcArchetype = archetypes[oldType];
        for (ComponentID i = 0; i < newType.size(); i++)
            if (newType[i])
                dstArchetype.copyComponent(entityId, i, srcArchetype.getComponent(entityId, i));

        archetypes[oldType].removeEntity(entityId);
        entityArchetypes[entityId] = archetypes[newType];
    }

    template <typename T>
    bool World::hasComponent(EntityID entityId)
    {
        return entityArchetypes[entityId].type[GetComponentID<T>()];
    }

    template <typename T>
    T &World::getComponent(EntityID entityId)
    {
        if (!hasComponent(entityId, GetComponentID<T>()))
            return nullptr;
        return entityArchetypes[entityId].getComponent<T>(entityId);
    }

    std::vector<EntityID> World::getEntitiesWithComponents(Type type)
    {
        std::vector<EntityID> entities;
        // TODO: Return entities with components
        return entities;
    }
}
