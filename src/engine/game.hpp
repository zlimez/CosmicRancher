#pragma once

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <vector>
// #include "scene.hpp"

class Game
{
private:
    bool running;
    SDL_Window *window;
    std::vector<std::vector<int>> map;
    SDL_Texture *texture1, *texture2, *texture3;
    SDL_Rect src, dst;
    // Scene *activeScene;

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