#pragma once

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <vector>
#include <SDL2/SDL.h>
#include <iostream>
#include "world.hpp"
#include "helpers/map.hpp"
#include "systems/renderer.hpp"

class Game
{
private:
    bool running;
    SDL_Window *window;
    SDL_GLContext glContext;
    std::unique_ptr<engine::World> world;
    std::unique_ptr<engine::Renderer> renderer;

public:
    static std::string basePath;
    Game();

    void init(const char *title, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void cleanup();

    bool isRunning() { return running; }
};