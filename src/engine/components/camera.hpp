#pragma once
#include "../entity.hpp"
#include "../defs.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

namespace engine
{
    struct Camera : Component
    {
        // NOTE: For now one camera in world, width and height should match viewport size
        float width, height;
        GLuint fbo;

        Camera &operator=(const Camera &other)
        {
            if (this != &other)
            {
                width = other.width;
                height = other.height;
                fbo = other.fbo;
            }
            return *this;
        }

        // Camera &operator=(Camera &&other)
        // {
        //     if (this != &other)
        //     {
        //         width = other.width;
        //         height = other.height;
        //         fbo = other.fbo;
        //         other.fbo = 0;
        //     }
        //     return *this;
        // }
    };
}