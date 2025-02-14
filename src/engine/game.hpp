#pragma once

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <vector>
#include <SDL2/SDL.h>
#include <iostream>
#include "world.hpp"
#include "helpers/map.hpp"
#include "systems/renderer.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/texture.hpp"

class Game
{
private:
    bool running = false;
    SDL_Window *window;
    SDL_GLContext glContext;
    std::unique_ptr<engine::World> world;
    std::unique_ptr<engine::Renderer> renderer;

    // graphics::Shader shader;
    // unsigned int VAO, VBO, EBO, texID;

public:
    static std::string basePath;

    void init(const char *title, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void cleanup();

    bool isRunning() { return running; }
};