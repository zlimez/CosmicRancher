#include "input.hpp"
#include <iostream>

namespace engine
{
    void InputSys::handleEvents()
    {
        SDL_Event event;
        // TODO: Add gestures
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                keyStates[event.key.keysym.sym] = KeyState::DOWN;
                break;
            case SDL_KEYUP:
                keyStates[event.key.keysym.sym] = KeyState::UP;
                break;
            }
        }
    }
}