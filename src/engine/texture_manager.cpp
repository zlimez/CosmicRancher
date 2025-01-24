#include "texture_manager.hpp"
#include "game.hpp"
#include <iostream>

SDL_Texture *TextureManager::LoadTexture(const char *fileName)
{
    if (!Game::renderer)
    {
        std::cerr << "Renderer is not initialized: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Renderer is not initialized");
    }
    SDL_Surface *surface = IMG_Load(fileName);
    if (!surface)
    {
        std::cout << "Failed to load surface: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to load surface");
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    if (!texture)
    {
        std::cout << "Failed to load texture: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to load texture");
    }

    SDL_FreeSurface(surface);
    return texture;
}

void TextureManager::Draw(SDL_Texture *texture, SDL_Rect source, SDL_Rect destination)
{
    SDL_RenderCopy(Game::renderer, texture, &source, &destination);
    if (SDL_GetError()[0] != '\0')
    {
        std::cout << "Error rendering texture: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Error rendering texture");
    }
}
