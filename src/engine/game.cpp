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

    world = std::make_unique<engine::World>(20000, 10);
    map::Terrain grass = {0, 10}, water = {1, 10};
    std::vector<map::Terrain> terrains = {grass, water};
    std::vector<std::string> terrainSprites = {basePath + "../assets/art/tiles/grass/tile_31.png", basePath + "../assets/art/tiles/water/tile_0.png"};
    map::mapFor({100, 100}, {0, 0}, terrains, terrainSprites, *world);

    std::vector<engine::ComponentID> camType = {engine::getComponentID<engine::Camera>(), engine::getComponentID<engine::Transform>()};
    auto camId = world->createEntity(camType);
    auto &cam = world->getComponent<engine::Camera>(camId);
    cam.height = (float)height / 20;
    cam.width = (float)width / 20;

    renderer = std::make_unique<engine::Renderer>(window);
    renderer->init(*world);
}

void Game::update()
{
    handleEvents();
    renderer->update(*world);
}

void Game::cleanup()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        running = false;
        break;

    default:
        break;
    }
}

std::string Game::basePath = SDL_GetBasePath();
