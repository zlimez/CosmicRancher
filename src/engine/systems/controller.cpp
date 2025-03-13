#include "controller.hpp"

namespace engine
{
    void ControllerSys::init(World &world) {}

    void ControllerSys::update(World &world)
    {
        Type controllerType;
        controllerType.set(getComponentID<Controller>());
        controllerType.set(getComponentID<Movement>());
        auto &controllerArchs = world.getArchetypesWith(controllerType);

        for (auto controllerArch : controllerArchs)
        {
            auto &controllerRow = controllerArch->getComponentRow<Controller>();
            auto &movementRow = controllerArch->getComponentRow<Movement>();
            for (size_t i = 0; i < controllerArch->count; i++)
            {
                auto &controller = controllerRow.blocks[i];
                auto &movement = movementRow.blocks[i];
                int xv = 0, yv = 0;
                if (InputSys::getInstance().getKeyState(controller.upKey) == KeyState::DOWN)
                    yv++;
                if (InputSys::getInstance().getKeyState(controller.downKey) == KeyState::DOWN)
                    yv--;
                if (InputSys::getInstance().getKeyState(controller.leftKey) == KeyState::DOWN)
                    xv--;
                if (InputSys::getInstance().getKeyState(controller.rightKey) == KeyState::DOWN)
                    xv++;

                if (controller.hasAcc)
                {
                    movement.velocity.x += xv * controller.acceleration * Time::deltaTime;
                    movement.velocity.x = std::clamp(movement.velocity.x, -controller.maxSpeed, controller.maxSpeed);
                    movement.velocity.y += xv * controller.acceleration * Time::deltaTime;
                    movement.velocity.y = std::clamp(movement.velocity.x, -controller.maxSpeed, controller.maxSpeed);
                }
                else
                {
                    movement.velocity.x = xv * controller.maxSpeed;
                    movement.velocity.y = yv * controller.maxSpeed;
                }
            }
        }
    }

    void ControllerSys::cleanup() {}
}