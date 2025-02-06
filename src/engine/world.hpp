#pragma once
#include <unordered_map>
#include <vector>
#include "entity.hpp"
#include "id_pool.hpp"

namespace engine
{
    class World
    {
    private:
        IdPool idPool; // Ids are contiguous hence possible to use vector instead of map
        std::vector<Archetype *> entityArchetypes;
        std::unordered_map<Type, Archetype> archetypes;
        std::unordered_map<Type, std::vector<Archetype *>> archetypesWith;
        std::unordered_map<Type, std::vector<EntityID>> entitiesWith;

    public:
        World(uint32 expectedEntities = 1000, uint32 = 10);

        EntityID createEntity(std::vector<ComponentID> &componentTypes);
        void destroyEntity(EntityID entityId);
        std::vector<Archetype *> &getArchetypesWith(Type type);
        std::vector<EntityID> &getEntitiesWith(Type type);

        template <typename T>
        void addComponent(EntityID entityId)
        {
            ComponentID componentId = getComponentID<T>();
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
        void removeComponent(EntityID entityId)
        {
            ComponentID componentId = getComponentID<T>();
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
        bool hasComponent(EntityID entityId)
        {
            return entityArchetypes[entityId]->type[getComponentID<T>()];
        }

        template <typename T>
        T &getComponent(EntityID entityId)
        {
            if (!hasComponent(entityId, getComponentID<T>()))
                return nullptr;
            return entityArchetypes[entityId]->getComponent<T>(entityId);
        }
    };
}