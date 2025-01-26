#include "entity.hpp"

namespace engine
{
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
        for (auto &row : componentRows)
        {
            auto componentRow = std::static_pointer_cast<ComponentRowBase>(row);
            componentRow->addDefaultBlock();
        }
        return count++;
    }

    ComponentID Component::GetNewID()
    {
        static ComponentID lastID = 0;
        if (lastID < MAX_COMPONENT_TYPES)
            return lastID++;
        else
            throw std::runtime_error("Max component types reached");
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

    void Archetype::init(Type type)
    {
        componentTypeRowIndices.resize(MAX_COMPONENT_TYPES, -1);
        this->type = type;
        int sc = 0, sbc = type.count(), i = 0;
        componentRows.resize(sbc);
        while (sc < sbc)
        {
            if (type[i])
                componentTypeRowIndices[i] = sc++;
            i++;
        }
    }

    template <typename T>
    T &Archetype::getComponent(EntityID entityId)
    {
        size_t c = entityIndices[entityId], r = componentTypeRowIndices[GetComponentID<T>()];
        auto componentRow = std::static_pointer_cast<ComponentRow<T>>(componentRows[r]);
        return componentRow->blocks[c];
    }
};