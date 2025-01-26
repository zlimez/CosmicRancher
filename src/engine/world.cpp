#include "world.hpp"

namespace engine
{
    template <typename T>
    uint32_t GetComponentID()
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
        if (!archetype.initialized)
            archetype.init(type);
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
        Type newType = entityArchetypes[entityId].type;
        newType.set(componentId);
        entityArchetypes[entityId] = archetypes[newType];
        // TODO: Add component to entity move to new archetype
    }

    template <typename T>
    void World::removeComponent(EntityID entityId)
    {
        ComponentID componentId = GetComponentID<T>();
        if (!entityArchetypes[entityId].type[componentId])
            return;
        Type newType = entityArchetypes[entityId].type;
        newType.reset(componentId);
        entityArchetypes[entityId] = archetypes[newType];
        // TODO: Remove component from entity move to new archetype
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
