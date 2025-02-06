#pragma once
#include <stdexcept>
#include <bitset>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

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
    };

    struct ComponentRowBase
    {
        virtual ~ComponentRowBase() = default;
        virtual void *addDefaultBlock() = 0;
        virtual void removeBlock(size_t index) = 0;
        virtual void copyBlock(size_t index, Component *component) = 0;
        virtual void *getBlock(size_t index) = 0;
    };

    class ComponentsRegistry
    {
    private:
        std::function<std::shared_ptr<ComponentRowBase>()> componentRowFactory[MAX_COMPONENT_TYPES];

    public:
        static ComponentsRegistry &instance();
        static ComponentID GetNewID();
        template <typename T>
        void registerComponent();
        std::shared_ptr<ComponentRowBase> createComponentRow(ComponentID componentID);
    };

    // Stores the data of a component type
    template <typename T>
    struct ComponentRow : ComponentRowBase
    {
        std::vector<T> blocks;
        void *addDefaultBlock() override;
        void removeBlock(size_t index) override;
        void copyBlock(size_t index, Component *component) override;
        void *getBlock(size_t index) override;
    };

    struct Archetype
    {
        bool initialized = false;
        Type type;
        size_t count;
        int componentTypeRowIndices[MAX_COMPONENT_TYPES];
        std::vector<std::shared_ptr<void>> componentRows;
        std::unordered_map<EntityID, size_t> entityIndices;
        std::unordered_map<size_t, EntityID> indexToEntities;

        size_t addEntity();
        size_t removeEntity(EntityID entityId);
        template <typename T>
        T &getComponent(EntityID entityId);
        Component *getComponent(EntityID entityId, ComponentID componentId);
        void copyComponent(EntityID entityId, ComponentID componentId, Component *component);

    private:
        void init(Type type);
    };
}