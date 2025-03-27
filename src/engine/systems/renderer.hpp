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
    class RenderSys : public System
    {
    private:
        std::unordered_map<std::string, graphics::Shader> shaders_;
        std::unordered_map<std::string, GLuint> textures_;
        std::vector<std::tuple<uint32, Sprite *, Transform *>> spriteObjs_;
        SDL_Window *window_;
        void setupSprite(Sprite &sprite, Transform &transform);
        void getSprites(World &world);

    public:
        RenderSys(SDL_Window *window) : window_(window) {}
        ~RenderSys() {}
        void init(World &world) override;
        void update(World &world) override;
        void cleanup() override;
    };
};