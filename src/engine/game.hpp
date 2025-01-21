#pragma once

#include <string>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include "scene.hpp" // Add the necessary include path for the "scene.hpp" header file.

class Game
{
private:
    bool running;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Scene *activeScene;

public:
    Game();
    ~Game() {}

    void init(const char *title, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void cleanup();

    bool isRunning()
    {
        return running;
    }
};