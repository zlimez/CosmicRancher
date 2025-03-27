#include "entity.hpp"

namespace engine
{
    ComponentID GetNewID()
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

    std::shared_ptr<ComponentRowBase> ComponentsRegistry::createComponentRow(ComponentID componentID)
    {
        if (!componentRowFactory_[componentID])
            throw std::runtime_error("Component not registered");
        return componentRowFactory_[componentID]();
    }

    size_t Archetype::removeEntity(EntityID entityId)
    {
        size_t index = entityIndices_[entityId];
        if (count_)
        {
            EntityID lastEntity = indexToEntities_[count_ - 1];

            entityIndices_[lastEntity] = index;
            indexToEntities_[index] = lastEntity;
        }
        entityIndices_.erase(entityId);
        indexToEntities_.erase(count_ - 1);
        for (auto &row : componentRows_)
        {
            auto componentRow = std::static_pointer_cast<ComponentRowBase>(row);
            componentRow->removeBlock(index);
        }
        return --count_;
    }

    size_t Archetype::addEntity(EntityID id)
    {
        for (auto &row : componentRows_)
        {
            auto componentRow = std::static_pointer_cast<ComponentRowBase>(row);
            componentRow->addBlock();
        }
        entityIndices_[id] = count_;
        indexToEntities_[count_] = id;
        return count_++;
    }

    Component *Archetype::getComponent(EntityID entityId, ComponentID componentId)
    {
        if (!entityIndices_.count(entityId))
            throw std::runtime_error("Entity does not exist");
        size_t c = entityIndices_[entityId], r = componentToRow[componentId];
        auto componentRow = std::static_pointer_cast<ComponentRowBase>(componentRows_[r]);
        return static_cast<Component *>(componentRow->getBlock(c));
    }

    void Archetype::moveComponent(EntityID entityId, ComponentID componentId, Component *component)
    {
        if (!entityIndices_.count(entityId))
            throw std::runtime_error("Entity does not exist");
        size_t c = entityIndices_[entityId], r = componentToRow[componentId];
        auto componentRow = std::static_pointer_cast<ComponentRowBase>(componentRows_[r]);
        componentRow->copyBlock(c, component);
    }
};