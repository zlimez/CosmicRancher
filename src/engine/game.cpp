#include "game.hpp"
#include "systems/map_gen.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include "texture_manager.hpp"

Game::Game()
{
    running = false;
    src.h = src.w = 16;
    dst.h = dst.w = 16;
    src.x = src.y = 0;
}

void Game::init(const char *title, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        if (window)
        {
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer)
            {
                std::vector<std::tuple<int, int, int>> ogSeeds;
                std::vector<std::pair<int, int>> extSeedCnt = {{1, 10}, {2, 10}, {3, 10}};
                map = Voronoi::generate(100, 100, ogSeeds, extSeedCnt, DistFuncs::manhattan);

                texture1 = TextureManager::LoadTexture("../assets/art/tiles/tile_65.png");
                texture2 = TextureManager::LoadTexture("../assets/art/tiles/tile_67.png");
                texture3 = TextureManager::LoadTexture("../assets/art/tiles/tile_68.png");
                running = true;
            }
        }
    }
    else
    {
        std::cout << "SDL Initialization Error: " << SDL_GetError() << std::endl;
        running = false;
    }
}

void Game::update()
{
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            dst.x = j * 16;
            dst.y = i * 16;
            switch (map[i][j])
            {
            case 1:
                if (texture1 == nullptr)
                {
                    std::cerr << "Error: texture1 is not initialized" << std::endl;
                    return;
                }
                TextureManager::Draw(texture1, src, dst);
                break;
            case 2:
                if (texture2 == nullptr)
                {
                    std::cerr << "Error: texture2 is not initialized" << std::endl;
                    return;
                }
                TextureManager::Draw(texture2, src, dst);
                break;
            case 3:
                if (texture3 == nullptr)
                {
                    std::cerr << "Error: texture3 is not initialized" << std::endl;
                    return;
                }
                TextureManager::Draw(texture3, src, dst);
                break;
            default:
                break;
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void Game::cleanup()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
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

SDL_Renderer *Game::renderer = nullptr;
