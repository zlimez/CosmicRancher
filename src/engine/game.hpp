#pragma once
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <vector>
#include <SDL2/SDL.h>
#include <iostream>
#include "world.hpp"
#include "helpers/map.hpp"
#include "systems/renderer.hpp"
#include "systems/input.hpp"
#include "systems/movement.hpp"
#include "systems/controller.hpp"
#include "systems/collision_detector.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/texture.hpp"

class Game
{
private:
    bool running = false;
    SDL_Window *window;
    SDL_GLContext glContext;
    std::unique_ptr<engine::World> world;
    std::unique_ptr<engine::RenderSys> renderer;
    std::unique_ptr<engine::MovementSys> movementSys;
    std::unique_ptr<engine::ControllerSys> controllerSys;
    std::unique_ptr<engine::CollisionDetector> collisionDetector;

public:
    static std::string basePath;

    void init(const char *title, int width, int height, bool fullscreen);
    void update();
    void cleanup();

    bool isRunning() { return running; }
};