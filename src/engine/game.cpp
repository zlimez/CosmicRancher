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

    window = SDL_CreateWindow("OpenGL + SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (!window)
    {
        std::cerr << "SDL Window Creation Error: " << SDL_GetError() << '\n';
        exit(1);
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        std::cerr << "SDL GL Context Creation Error: " << SDL_GetError() << '\n';
        exit(1);
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        exit(1);
    }

    running = true;
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
    // std::cout << "Components registered\n";

    world = std::make_unique<engine::World>(20000, 10);

    std::vector<engine::ComponentID> mapType = {engine::getComponentID<engine::Map>()};
    auto mapId = world->createEntity(mapType);
    auto &map = world->getComponent<engine::Map>(mapId);
    map.dim = {50, 50};
    // std::cout << "engine::basePath: " << engine::basePath << '\n';
    engine::Terrain grass = {10, engine::basePath + "../assets/art/tiles/grass/tile_31.png"}, water = {10, engine::basePath + "../assets/art/tiles/water/tile_0.png"};
    map.terrains.push_back(grass);
    map.terrains.push_back(water);

    std::vector<engine::ComponentID> camType = {engine::getComponentID<engine::Camera>(), engine::getComponentID<engine::Transform>(), engine::getComponentID<engine::Controller>(), engine::getComponentID<engine::Movement>(), engine::getComponentID<engine::BoxCollider>()};
    auto camId = world->createEntity(camType);
    auto &cam = world->getComponent<engine::Camera>(camId);
    auto &camCollider = world->getComponent<engine::BoxCollider>(camId);
    cam.height = (float)height / 20;
    cam.width = (float)width / 20;
    camCollider.height = 10.0f;
    camCollider.width = 10.0f;
    // std::cout << "Cam set\n";

    renderer = std::make_unique<engine::RenderSys>(window);
    movementSys = std::make_unique<engine::MovementSys>();
    controllerSys = std::make_unique<engine::ControllerSys>();
    collisionDetector = std::make_unique<engine::CollisionDetector>();
    mapSys = std::make_unique<engine::MapSys>();

    mapSys->init(*world);
    renderer->init(*world);
    movementSys->init(*world);
    controllerSys->init(*world);
    collisionDetector->init(*world);
}

void Game::update()
{
    engine::InputSys::getInstance().handleEvents();
    if (engine::InputSys::getInstance().isQuit())
        running = false;

    controllerSys->update(*world);
    movementSys->update(*world);
    renderer->update(*world);
}

void Game::cleanup()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
