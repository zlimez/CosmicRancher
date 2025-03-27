#pragma once
#include <stdexcept>
#include <bitset>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <utility>

namespace engine
{
    constexpr unsigned int MAX_COMPONENT_TYPES = 64;
    using Type = std::bitset<MAX_COMPONENT_TYPES>;
    using uint32 = unsigned int;
    using ComponentID = unsigned int;
    using EntityID = unsigned int;

    ComponentID GetNewID();

    template <typename T>
    uint32_t getComponentID()
    {
        static int id = GetNewID();
        return id;
    }

    struct Entity
    {
        EntityID id;
    };

    struct Component
    {
        bool active_ = true;
    };

    struct ComponentRowBase
    {
        virtual ~ComponentRowBase() = default;
        virtual void *addBlock() = 0;
        virtual void removeBlock(size_t index) = 0;
        // virtual void moveBlock(size_t index, Component *component) = 0;
        virtual void copyBlock(size_t index, Component *component) = 0;
        virtual void *getBlock(size_t index) = 0;
    };

    // Stores the data of a component type
    template <typename T>
    struct ComponentRow : ComponentRowBase
    {
        std::vector<T> blocks_;

        void *addBlock()
        {
            blocks_.push_back(T());
            return &blocks_.back();
        }

        void removeBlock(size_t index)
        {
            std::swap(blocks_[index], blocks_.back());
            blocks_.pop_back();
        }

        // TODO: Figure out why when std::move is used in moveBlock, swap, push_back and defining move operator in Sprite etc. becomes problematic
        // void moveBlock(size_t index, Component *component) { blocks[index] = std::move(*static_cast<T *>(component)); }
        void copyBlock(size_t index, Component *component) { blocks_[index] = *static_cast<T *>(component); }
        void *getBlock(size_t index) { return &blocks_[index]; }
    };

    class ComponentsRegistry
    {
    private:
        std::function<std::shared_ptr<ComponentRowBase>()> componentRowFactory_[MAX_COMPONENT_TYPES];

    public:
        static ComponentsRegistry &instance();
        std::shared_ptr<ComponentRowBase> createComponentRow(ComponentID componentID);

        template <typename T>
        void registerComponent()
        {
            componentRowFactory_[getComponentID<T>()] = []()
            { return std::make_shared<ComponentRow<T>>(); };
        }
    };
    struct Archetype
    {
        Type type_;
        size_t count_ = 0;
        int componentToRow[MAX_COMPONENT_TYPES];           // maps component type to row index
        std::vector<std::shared_ptr<void>> componentRows_; // each row stores the data of a component type
        std::unordered_map<EntityID, size_t> entityIndices_;
        std::unordered_map<size_t, EntityID> indexToEntities_;

        size_t addEntity(EntityID entityId);
        size_t removeEntity(EntityID entityId);
        void moveComponent(EntityID entityId, ComponentID componentId, Component *component);
        Component *getComponent(EntityID entityId, ComponentID componentId);

        Archetype() = delete;
        Archetype(Type dtype) : type_(dtype)
        {
            int sc = 0, sbc = type_.count(), i = 0;
            while (sc < sbc)
            {
                if (type_[i])
                {
                    componentToRow[i] = sc++;
                    componentRows_.push_back(ComponentsRegistry::instance().createComponentRow(i));
                }
                i++;
            }
        }

        template <typename T>
        T &getComponent(EntityID entityId)
        {
            if (!entityIndices_.count(entityId))
                throw std::runtime_error("Entity does not exist");
            size_t c = entityIndices_[entityId], r = componentToRow[getComponentID<T>()];
            auto componentRow = std::static_pointer_cast<ComponentRow<T>>(componentRows_[r]);
            return componentRow->blocks_[c];
        }

        template <typename T>
        ComponentRow<T> &getComponentRow() { return *std::static_pointer_cast<ComponentRow<T>>(componentRows_[componentToRow[getComponentID<T>()]]); }
    };
}