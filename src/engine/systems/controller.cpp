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
            for (size_t i = 0; i < controllerArch->count_; i++)
            {
                auto &controller = controllerRow.blocks_[i];
                auto &movement = movementRow.blocks_[i];
                int xv = 0, yv = 0;
                if (InputSys::getInstance().getKeyState(controller.upKey_) == KeyState::DOWN)
                    yv++;
                if (InputSys::getInstance().getKeyState(controller.downKey_) == KeyState::DOWN)
                    yv--;
                if (InputSys::getInstance().getKeyState(controller.leftKey_) == KeyState::DOWN)
                    xv--;
                if (InputSys::getInstance().getKeyState(controller.rightKey_) == KeyState::DOWN)
                    xv++;

                if (controller.hasAcc_)
                {
                    movement.velocity_.x += xv * controller.acceleration_ * Time::deltaTime_;
                    movement.velocity_.x = std::clamp(movement.velocity_.x, -controller.maxSpeed_, controller.maxSpeed_);
                    movement.velocity_.y += xv * controller.acceleration_ * Time::deltaTime_;
                    movement.velocity_.y = std::clamp(movement.velocity_.x, -controller.maxSpeed_, controller.maxSpeed_);
                }
                else
                {
                    movement.velocity_.x = xv * controller.maxSpeed_;
                    movement.velocity_.y = yv * controller.maxSpeed_;
                }
            }
        }
    }

    void ControllerSys::cleanup() {}
}