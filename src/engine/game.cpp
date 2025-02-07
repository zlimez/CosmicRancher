#include "game.hpp"
#include <SDL2/SDL.h>
#include <iostream>

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
        window = SDL_CreateWindow("OpenGL + SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        if (window)
        {
            glContext = SDL_GL_CreateContext(window);
            if (glContext)
                running = true;
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
