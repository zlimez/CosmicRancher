#include "game.hpp"

void Game::init(const char *title, int width, int height, bool fullscreen)
{
    int flags = SDL_WINDOW_OPENGL;
    if (fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "SDL Initialization Error: " << SDL_GetError() << '\n';
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window_ = SDL_CreateWindow("OpenGL + SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (!window_)
    {
        std::cerr << "SDL Window Creation Error: " << SDL_GetError() << '\n';
        exit(1);
    }

    glContext_ = SDL_GL_CreateContext(window_);
    if (!glContext_)
    {
        std::cerr << "SDL GL Context Creation Error: " << SDL_GetError() << '\n';
        exit(1);
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        exit(1);
    }

    running_ = true;
    // TODO: Place in scene system
    engine::ComponentsRegistry::instance().registerComponent<engine::Camera>();
    engine::ComponentsRegistry::instance().registerComponent<engine::Transform>();
    engine::ComponentsRegistry::instance().registerComponent<engine::Sprite>();
    engine::ComponentsRegistry::instance().registerComponent<engine::Controller>();
    engine::ComponentsRegistry::instance().registerComponent<engine::Movement>();
    engine::ComponentsRegistry::instance().registerComponent<engine::BoxCollider>();
    engine::ComponentsRegistry::instance().registerComponent<engine::Collisions>();
    engine::ComponentsRegistry::instance().registerComponent<engine::Map>();
    engine::ComponentsRegistry::instance().registerComponent<engine::MapCell>();

    world_ = std::make_unique<engine::World>(20000, 15);

    std::vector<engine::ComponentID> mapType = {engine::getComponentID<engine::Map>()};
    auto mapId = world_->createEntity(mapType);
    auto &map = world_->getComponent<engine::Map>(mapId);
    map.dim_ = {50, 50};
    engine::Terrain grass = {10, engine::basePath + "../assets/art/tiles/grass/tile_31.png"}, water = {10, engine::basePath + "../assets/art/tiles/water/tile_0.png"};
    map.terrains_.push_back(grass);
    map.terrains_.push_back(water);

    std::vector<engine::ComponentID> camType = {engine::getComponentID<engine::Camera>(), engine::getComponentID<engine::Transform>(), engine::getComponentID<engine::Controller>(), engine::getComponentID<engine::Movement>(), engine::getComponentID<engine::BoxCollider>()};
    auto camId = world_->createEntity(camType);
    auto &cam = world_->getComponent<engine::Camera>(camId);
    auto &camCollider = world_->getComponent<engine::BoxCollider>(camId);
    cam.height_ = (float)height / 20;
    cam.width_ = (float)width / 20;
    camCollider.height_ = 5.0f;
    camCollider.width_ = 5.0f;

    renderer_ = std::make_unique<engine::RenderSys>(window_);
    movementSys_ = std::make_unique<engine::MovementSys>();
    controllerSys_ = std::make_unique<engine::ControllerSys>();
    collisionDetector_ = std::make_unique<engine::CollisionDetector>();
    mapSys_ = std::make_unique<engine::MapSys>();

    mapSys_->init(*world_);
    renderer_->init(*world_);
    movementSys_->init(*world_);
    controllerSys_->init(*world_);
    collisionDetector_->init(*world_);
}

void Game::update()
{
    engine::InputSys::getInstance().handleEvents();
    if (engine::InputSys::getInstance().isQuit())
        running_ = false;

    controllerSys_->update(*world_);
    movementSys_->update(*world_);
    collisionDetector_->update(*world_);
    // mapSys->update(*world);
    renderer_->update(*world_);
}

void Game::cleanup()
{
    SDL_GL_DeleteContext(glContext_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}
