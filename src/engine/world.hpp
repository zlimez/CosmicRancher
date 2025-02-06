#pragma once
#include <unordered_map>
#include <vector>
#include "entity.hpp"
#include "id_pool.hpp"

namespace engine
{
    template <typename T>
    uint32_t getComponentID();

    class World
    {
    private:
        IdPool idPool; // Ids are contiguous hence possible to use vector instead of map
        std::vector<Archetype *> entityArchetypes;
        std::unordered_map<Type, Archetype> archetypes;
        std::unordered_map<Type, std::vector<Archetype *>> archetypesWithComponents;
        std::unordered_map<Type, std::vector<EntityID>> entitiesWithComponents;

    public:
        World(uint32 expectedEntities = 1000, uint32 = 10);

        EntityID createEntity(std::vector<ComponentID> &componentTypes);
        void destroyEntity(EntityID entityId);
        template <typename T>
        void addComponent(EntityID entityId);
        template <typename T>
        void removeComponent(EntityID entityId);
        template <typename T>
        T &getComponent(EntityID entityId);
        template <typename T>
        bool hasComponent(EntityID entityId);
        std::vector<Archetype *> getArchetypesWithComponents(Type type);
        std::vector<EntityID> getEntitiesWithComponents(Type type);
    };
}