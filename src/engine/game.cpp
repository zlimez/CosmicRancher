#include "game.hpp"
#include <SDL2/SDL.h>
#include <iostream>

Game::Game()
{
    running = false;
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
                running = true;
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
