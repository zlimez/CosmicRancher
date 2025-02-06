#pragma once

#include <glad/glad.h>
#include "system.hpp"
#include "../components/sprite.hpp"
#include "../components/transform.hpp"
#include "../../utils/texture.hpp"

namespace engine
{
    class Renderer : System
    {
        void init(World &world) override;
        void update(World &world) override;
    };
};