#include "game.hpp"

Game::Game()
{
    running = false;
}

void Game::init(const char *title, int width, int height, bool fullscreen)
{
    int flags = SDL_WINDOW_OPENGL;
    if (fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        window = SDL_CreateWindow("OpenGL + SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        if (window)
        {
            glContext = SDL_GL_CreateContext(window);
            if (glContext)
            {
                running = true;

                // TODO: Place in scene system
                engine::ComponentsRegistry::instance().registerComponent<engine::Camera>();
                engine::ComponentsRegistry::instance().registerComponent<engine::Transform>();
                engine::ComponentsRegistry::instance().registerComponent<engine::Sprite>();

                std::cout << "Registering components\n";

                world = std::make_unique<engine::World>(10000, 10);
                map::Terrain grass = {0, 10}, water = {1, 10};
                std::vector<map::Terrain> terrains = {grass, water};
                std::vector<std::string> terrainSprites = {basePath + "../assets/art/tiles/grass/tile_31.png", basePath + "../assets/art/tiles/water/tile_0.png"};
                map::mapFor({10, 10}, {0, 0}, terrains, terrainSprites, *world);
                std::cout << "Finished map gen\n";

                std::vector<engine::ComponentID> camType = {engine::getComponentID<engine::Camera>(), engine::getComponentID<engine::Transform>()};
                auto camId = world->createEntity(camType);
                auto &cam = world->getComponent<engine::Camera>(camId);
                cam.height = height / 40;
                cam.width = width / 40;
                std::cout << "Finished camera\n";

                renderer = std::make_unique<engine::Renderer>(window);
                renderer->init(*world);
                std::cout << "Finished init\n";
            }
            else
            {
                std::cerr << "SDL GL Context Creation Error: " << SDL_GetError() << '\n';
                running = false;
            }
        }
        else
        {
            std::cerr << "SDL Window Creation Error: " << SDL_GetError() << '\n';
            running = false;
        }
    }
    else
    {
        std::cerr << "SDL Initialization Error: " << SDL_GetError() << '\n';
        running = false;
    }
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
