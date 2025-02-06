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
        virtual void *addBlock() = 0;
        virtual void removeBlock(size_t index) = 0;
        virtual void copyBlock(size_t index, Component *component) = 0;
        virtual void *getBlock(size_t index) = 0;
    };

    // Stores the data of a component type
    template <typename T>
    struct ComponentRow : ComponentRowBase
    {
        std::vector<T> blocks;

        void *addBlock()
        {
            blocks.push_back(T());
            return &blocks.back();
        }

        void removeBlock(size_t index)
        {
            std::swap(blocks[index], blocks.back());
            blocks.pop_back();
        }

        void copyBlock(size_t index, Component *component)
        {
            blocks[index] = *static_cast<T *>(component);
        }

        void *getBlock(size_t index)
        {
            return &blocks[index];
        }
    };

    class ComponentsRegistry
    {
    private:
        std::function<std::shared_ptr<ComponentRowBase>()> componentRowFactory[MAX_COMPONENT_TYPES];

    public:
        static ComponentsRegistry &instance();
        static ComponentID GetNewID();
        std::shared_ptr<ComponentRowBase> createComponentRow(ComponentID componentID);

        template <typename T>
        void registerComponent()
        {
            componentRowFactory[getComponentID<T>()] = []()
            { return std::make_shared<ComponentRow<T>>(); };
        }
    };

    template <typename T>
    uint32_t getComponentID()
    {
        static int id = ComponentsRegistry::GetNewID();
        return id;
    }

    struct Archetype
    {
        bool initialized = false;
        Type type;
        size_t count;
        int componentToRow[MAX_COMPONENT_TYPES];          // maps component type to row index
        std::vector<std::shared_ptr<void>> componentRows; // each row stores the data of a component type
        std::unordered_map<EntityID, size_t> entityIndices;
        std::unordered_map<size_t, EntityID> indexToEntities;

        size_t addEntity();
        size_t removeEntity(EntityID entityId);
        void copyComponent(EntityID entityId, ComponentID componentId, Component *component);
        Component *getComponent(EntityID entityId, ComponentID componentId);

        template <typename T>
        T getComponent(EntityID entityId)
        {
            if (!entityIndices[entityId])
                throw std::runtime_error("Entity does not exist");
            size_t c = entityIndices[entityId], r = componentToRow[getComponentID<T>()];
            auto componentRow = std::static_pointer_cast<ComponentRow<T>>(componentRows[r]);
            return componentRow->blocks[c];
        }

        template <typename T>
        ComponentRow<T> &getComponentRow()
        {
            return *std::static_pointer_cast<ComponentRow<T>>(componentRows[componentToRow[getComponentID<T>()]]);
        }

    private:
        void init(Type type);
    };
}