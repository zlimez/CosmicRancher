#pragma once
#include "system.hpp"
#include "input.hpp"
#include "../time.hpp"
#include "../components/transform.hpp"
#include "../components/camera.hpp"

namespace engine
{
    class ControllerSys : public System
    {
    public:
        ControllerSys() {}
        ~ControllerSys() {}
        void init(World &world) override;
        void update(World &world) override;
        void cleanup() override;
    };
}