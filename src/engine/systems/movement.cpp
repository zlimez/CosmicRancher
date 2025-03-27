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
            for (size_t i = 0; i < movingArch->count_; i++)
            {
                auto &transform = transformRow.blocks_[i];
                auto &movement = movementRow.blocks_[i];
                transform.position_ += movement.velocity_ * Time::deltaTime_;
            }
        }
    }
    void MovementSys::cleanup() {}
}