#pragma once
#include "../defs.hpp"
#include "../entity.hpp"
#include "../../graphics/shader.hpp"

namespace engine
{
    struct Vertex
    {
        vec3 position;
        vec2 texCoords;
    };

    struct Texture
    {
        uint32 textureID;
        char *directory;
        char *file;
    };

    struct ShaderParts
    {
        static const char delim = '|';
        char *vtxPath;
        char *fragPath;
    };

    // NOTE: For now one sprite per entity, default stretch the texture to the sprite size
    struct Sprite : Component
    {
        std::vector<Vertex> vertices;
        Texture texture;
        ShaderParts shaders;
        graphics::Shader shader;
        bool flipX, flipY;
        uint32 order;
        uint32 vao, vbo;

        Sprite &operator=(const Sprite &other)
        {
            if (this != &other)
            {
                vertices = std::move(other.vertices);
                texture = other.texture;
                shaders = other.shaders;
                shader = other.shader;
                flipX = other.flipX;
                flipY = other.flipY;
                order = other.order;
                vao = other.vao;
                vbo = other.vbo;
            }

            return *this;
        }
    };
};