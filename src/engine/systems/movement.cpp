#include "movement.hpp"

namespace engine
{
    void MovementSys::init(World &world) {}
    void MovementSys::update(World &world)
    {
        Type movingType;
        movingType.set(getComponentID<Transform>());
        movingType.set(getComponentID<Movement>());
        auto &movingArchs = world.getArchetypesWith(movingType);
        for (auto movingArch : movingArchs)
        {
            auto &transformRow = movingArch->getComponentRow<Transform>();
            auto &movementRow = movingArch->getComponentRow<Movement>();
            for (size_t i = 0; i < movingArch->count; i++)
            {
                auto &transform = transformRow.blocks[i];
                auto &movement = movementRow.blocks[i];
                transform.position += movement.velocity * Time::deltaTime;
            }
        }
    }
    void MovementSys::cleanup() {}
}