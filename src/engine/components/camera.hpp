#pragma once
#include <SDL2/SDL.h>
#include "../entity.hpp"
#include "../defs.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

namespace engine
{
    struct Camera : Component
    {
        // NOTE: For now one camera in world, width and height should match viewport size
        float width_ = 100.0f, height_ = 100.0f;
        GLuint fbo_ = 0;
    };

    struct Controller : Component
    {
        SDL_KeyCode upKey_ = SDLK_w;
        SDL_KeyCode downKey_ = SDLK_s;
        SDL_KeyCode leftKey_ = SDLK_a;
        SDL_KeyCode rightKey_ = SDLK_d;

        bool hasAcc_ = false;
        float maxSpeed_ = 10.0f;
        float acceleration_ = 10.0f;
    };
}