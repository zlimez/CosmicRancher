#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include <iostream>
#include <SDL2/SDL.h>
#include "system.hpp"
#include "../../graphics/shader.hpp"
#include "../components/sprite.hpp"
#include "../components/transform.hpp"
#include "../../graphics/texture.hpp"
#include "../components/camera.hpp"

namespace engine
{
    class Renderer : public System
    {
    private:
        std::unordered_map<std::string, graphics::Shader> shaders;
        std::unordered_map<std::string, GLuint> textures;
        std::vector<std::tuple<uint32, Sprite *, Transform *>> sprites;
        SDL_Window *window;
        void setupSprite(Sprite &sprite, Transform &transform);

    public:
        Renderer(SDL_Window *window) : window(window) {}
        ~Renderer() {}
        void init(World &world) override;
        void update(World &world) override;
        void cleanup() override;
    };
};