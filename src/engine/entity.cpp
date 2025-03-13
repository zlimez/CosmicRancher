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
        if (!componentRowFactory[componentID])
            throw std::runtime_error("Component not registered");
        return componentRowFactory[componentID]();
    }

    void Archetype::init(Type type)
    {
        this->type = type;
        std::cout << "Initializing archetype with type: " << type << '\n';
        int sc = 0, sbc = type.count(), i = 0;
        while (sc < sbc)
        {
            if (type[i])
            {
                componentToRow[i] = sc++;
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

    size_t Archetype::addEntity(EntityID id)
    {
        for (auto &row : componentRows)
        {
            auto componentRow = std::static_pointer_cast<ComponentRowBase>(row);
            componentRow->addBlock();
        }
        entityIndices[id] = count;
        indexToEntities[count] = id;
        return count++;
    }

    Component *Archetype::getComponent(EntityID entityId, ComponentID componentId)
    {
        if (!entityIndices.count(entityId))
            throw std::runtime_error("Entity does not exist");
        size_t c = entityIndices[entityId], r = componentToRow[componentId];
        auto componentRow = std::static_pointer_cast<ComponentRowBase>(componentRows[r]);
        return (Component *)componentRow->getBlock(c);
    }

    void Archetype::moveComponent(EntityID entityId, ComponentID componentId, Component *component)
    {
        if (!entityIndices.count(entityId))
            throw std::runtime_error("Entity does not exist");
        size_t c = entityIndices[entityId], r = componentToRow[componentId];
        auto componentRow = std::static_pointer_cast<ComponentRowBase>(componentRows[r]);
        componentRow->copyBlock(c, component);
    }

    // void Archetype::copyComponent(EntityID entityId, ComponentID componentId, Component *component)
    // {
    //     if (!entityIndices.count(entityId))
    //         throw std::runtime_error("Entity does not exist");
    //     size_t c = entityIndices[entityId], r = componentToRow[componentId];
    //     auto componentRow = std::static_pointer_cast<ComponentRowBase>(componentRows[r]);
    //     componentRow->copyBlock(c, component);
    // }
};