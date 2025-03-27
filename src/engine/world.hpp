#pragma once
#include <unordered_map>
#include <vector>
#include <iostream>
#include "entity.hpp"
#include "id_pool.hpp"

namespace engine
{
    class World
    {
    private:
        uint32 entityCnt_, componentCnt_;
        IdPool idPool_; // Ids are contiguous hence possible to use vector instead of map
        std::vector<Archetype *> entityArchetypes_;
        std::unordered_map<Type, Archetype> archetypes_; // NOTE: Reference & pointer remains valid even with rehash
        std::unordered_map<Type, std::vector<Archetype *>> archetypesWith_;
        std::unordered_map<Type, std::vector<EntityID>> entitiesWith_;

        void addArchetype(Type type);

    public:
        World(uint32 entityCnt = 1000, uint32 componentCnt = 10);

        EntityID createEntity(std::vector<ComponentID> &componentTypes);
        void destroyEntity(EntityID entityId);
        std::vector<Archetype *> &getArchetypesWith(Type type);
        std::vector<EntityID> &getEntitiesWith(Type type);

        template <typename T>
        T &addComponent(EntityID entityId)
        {
            ComponentID componentId = getComponentID<T>();
            if (entityArchetypes_[entityId]->type_[componentId])
                return entityArchetypes_[entityId]->getComponent<T>(entityId);
            Type oldType = entityArchetypes_[entityId]->type_;
            Type newType = oldType;
            newType.set(componentId);
            if (!archetypes_.count(newType))
                addArchetype(newType);
            auto &dstArchetype = archetypes_.at(newType), &srcArchetype = archetypes_.at(oldType);
            dstArchetype.addEntity(entityId);
            for (ComponentID i = 0; i < newType.size(); i++)
                if (oldType[i])
                    dstArchetype.moveComponent(entityId, i, srcArchetype.getComponent(entityId, i));

            archetypes_.at(oldType).removeEntity(entityId);
            entityArchetypes_[entityId] = &dstArchetype;
            return dstArchetype.getComponent<T>(entityId);
        }

        template <typename T>
        void removeComponent(EntityID entityId)
        {
            ComponentID componentId = getComponentID<T>();
            if (!entityArchetypes_[entityId]->type_[componentId])
                return;
            Type oldType = entityArchetypes_[entityId]->type_;
            Type newType = oldType;
            newType.reset(componentId);
            if (!archetypes_.count(newType))
                addArchetype(newType);
            auto &dstArchetype = archetypes_.at(newType), &srcArchetype = archetypes_.at(oldType);
            for (ComponentID i = 0; i < newType.size(); i++)
                if (newType[i])
                    dstArchetype.moveComponent(entityId, i, srcArchetype.getComponent(entityId, i));

            archetypes_.at(oldType).removeEntity(entityId);
            entityArchetypes_[entityId] = &archetypes_.at(newType);
        }

        template <typename T>
        bool hasComponent(EntityID entityId) { return entityArchetypes_[entityId]->type_[getComponentID<T>()]; }

        template <typename T>
        T &getComponent(EntityID entityId)
        {
            if (!entityArchetypes_[entityId]->type_[getComponentID<T>()])
                throw std::runtime_error("Entity " + std::to_string(entityId) + " does not have component " + std::string(typeid(T).name()) + " componentId " + std::to_string(getComponentID<T>()));
            return entityArchetypes_[entityId]->getComponent<T>(entityId);
        }
    };
}