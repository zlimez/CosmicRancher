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
        float width = 100.0f, height = 100.0f;
        GLuint fbo = 0;
    };

    struct Controller : Component
    {
        SDL_KeyCode upKey = SDLK_w;
        SDL_KeyCode downKey = SDLK_s;
        SDL_KeyCode leftKey = SDLK_a;
        SDL_KeyCode rightKey = SDLK_d;

        bool hasAcc = false;
        float maxSpeed = 10.0f;
        float acceleration = 10.0f;
    };
}