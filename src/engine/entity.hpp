#pragma once
#include <stdexcept>
#include <bitset>
#include <vector>
#include <memory>
#include <unordered_map>

namespace engine
{
    constexpr unsigned int MAX_COMPONENT_TYPES = 64;
    using Type = std::bitset<MAX_COMPONENT_TYPES>;
    using uint32 = unsigned int;
    using ComponentID = unsigned int;
    using EntityID = unsigned int;

    struct Entity
    {
        EntityID id;
    };

    struct Component
    {
        bool active = true;
        static ComponentID GetNewID();
    };

    struct ComponentRowBase
    {
        virtual ~ComponentRowBase() = default;
        virtual void *addDefaultBlock() = 0;
        virtual void removeBlock(size_t index) = 0;
    };

    // Stores the data of a component type
    template <typename T>
    struct ComponentRow : ComponentRowBase
    {
        std::vector<T> blocks;
        void *addDefaultBlock() override;
        void removeBlock(size_t index) override;
    };

    struct Archetype
    {
        bool initialized = false;
        Type type;
        size_t count;
        std::vector<int> componentTypeRowIndices;
        std::vector<std::shared_ptr<void>> componentRows;
        std::unordered_map<EntityID, size_t> entityIndices, indexToEntities;

        void init(Type type);
        size_t addEntity();
        size_t removeEntity(EntityID entityId);
        template <typename T>
        T &getComponent(EntityID entityId);
    };
}