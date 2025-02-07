#pragma once

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <vector>

class Game
{
private:
    bool running;
    SDL_Window *window;
    SDL_GLContext glContext;

public:
    Game();
    ~Game() {};

    void init(const char *title, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void cleanup();

    bool isRunning()
    {
        return running;
    }
};