#pragma once

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <vector>

class Game
{
private:
    bool running;
    SDL_Window *window;

public:
    Game();
    ~Game() {};

    static SDL_Renderer *renderer;

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