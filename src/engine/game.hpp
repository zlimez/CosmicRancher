#pragma once
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <vector>
#include <SDL2/SDL.h>
#include <iostream>
#include "world.hpp"
#include "systems/mapsys.hpp"
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
    bool running_ = false;
    SDL_Window *window_;
    SDL_GLContext glContext_;
    std::unique_ptr<engine::World> world_;
    std::unique_ptr<engine::RenderSys> renderer_;
    std::unique_ptr<engine::MapSys> mapSys_;
    std::unique_ptr<engine::MovementSys> movementSys_;
    std::unique_ptr<engine::ControllerSys> controllerSys_;
    std::unique_ptr<engine::CollisionDetector> collisionDetector_;

public:
    void init(const char *title, int width, int height, bool fullscreen);
    void update();
    void cleanup();

    bool isRunning() { return running_; }
};