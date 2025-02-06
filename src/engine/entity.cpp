#include "entity.hpp"

namespace engine
{
    ComponentID ComponentsRegistry::GetNewID()
    {
        static ComponentID lastID = 0;
        if (lastID < MAX_COMPONENT_TYPES)
            return lastID++;
        else
            throw std::runtime_error("Max component types reached");
    }

    ComponentsRegistry &ComponentsRegistry::instance()
    {
        static ComponentsRegistry instance;
        return instance;
    }

    template <typename T>
    void ComponentsRegistry::registerComponent()
    {
        componentRowFactory[GetComponentID<T>()] = []()
        { return std::make_shared<ComponentRow<T>>(); };
    }

    std::shared_ptr<ComponentRowBase> ComponentsRegistry::createComponentRow(ComponentID componentID)
    {
        if (!componentRowFactory[componentID])
            throw std::runtime_error("Component not registered");
        return componentRowFactory[componentID]();
    }

    template <typename T>
    void *ComponentRow<T>::addDefaultBlock()
    {
        blocks.push_back(T());
        return &blocks.back();
    }

    template <typename T>
    void ComponentRow<T>::removeBlock(size_t index)
    {
        std::swap(blocks[index], blocks.back());
        blocks.pop_back();
    }

    template <typename T>
    void ComponentRow<T>::copyBlock(size_t index, Component *component)
    {
        blocks[index] = *component;
    }

    template <typename T>
    void *ComponentRow<T>::getBlock(size_t index)
    {
        return &blocks[index];
    }

    void Archetype::init(Type type)
    {
        this->type = type;
        int sc = 0, sbc = type.count(), i = 0;
        while (sc < sbc)
        {
            if (type[i])
            {
                componentTypeRowIndices[i] = sc++;
                componentRows.push_back(ComponentsRegistry::instance().createComponentRow(i));
            }
            i++;
        }
    }

    size_t Archetype::removeEntity(EntityID entityId)
    {
        size_t index = entityIndices[entityId];
        EntityID lastEntity = indexToEntities[count];
        entityIndices[lastEntity] = index;
        indexToEntities[index] = lastEntity;
        entityIndices.erase(entityId);
        indexToEntities.erase(count);
        for (auto &row : componentRows)
        {
            auto componentRow = std::static_pointer_cast<ComponentRowBase>(row);
            componentRow->removeBlock(index);
        }
        return --count;
    }

    size_t Archetype::addEntity()
    {
        if (!initialized)
            init(type);
        for (auto &row : componentRows)
        {
            auto componentRow = std::static_pointer_cast<ComponentRowBase>(row);
            componentRow->addDefaultBlock();
        }
        return count++;
    }

    template <typename T>
    T &Archetype::getComponent(EntityID entityId)
    {
        if (!entityIndices[entityId])
            throw std::runtime_error("Entity does not exist");
        size_t c = entityIndices[entityId], r = componentTypeRowIndices[GetComponentID<T>()];
        auto componentRow = std::static_pointer_cast<ComponentRow<T>>(componentRows[r]);
        return componentRow->blocks[c];
    }

    Component *Archetype::getComponent(EntityID entityId, ComponentID componentId)
    {
        if (!entityIndices[entityId])
            throw std::runtime_error("Entity does not exist");
        size_t c = entityIndices[entityId], r = componentTypeRowIndices[componentId];
        auto componentRow = std::static_pointer_cast<ComponentRowBase>(componentRows[r]);
        return (Component *)componentRow->getBlock(c);
    }

    void Archetype::copyComponent(EntityID entityId, ComponentID componentId, Component *component)
    {
        if (!entityIndices[entityId])
            throw std::runtime_error("Entity does not exist");
        size_t c = entityIndices[entityId], r = componentTypeRowIndices[componentId];
        auto componentRow = std::static_pointer_cast<ComponentRowBase>(componentRows[r]);
        componentRow->copyBlock(c, component);
    }
};