#include <SDL2/SDL.h>
#include <stdio.h>
#include "engine/game.hpp"
#include "engine/time.hpp"

int main(int argc, char *argv[])
{
    Game game;
    game.init("Cosmic Rancher", 800, 600, false);
    Uint32 lastTime = SDL_GetTicks();
    while (game.isRunning())
    {
        game.handleEvents();

        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        Time::deltaTime = deltaTime;

        game.update();
        game.render();
    }
    game.cleanup();

    return 0;
}