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
        std::string filePath;
    };

    struct ShaderParts
    {
        static const char delim = '|';
        std::string vtxPath;
        std::string fragPath;
    };

    // NOTE: For now one sprite per entity, default stretch the texture to the sprite size
    struct Sprite : Component
    {
        std::vector<Vertex> vertices;
        Texture texture;
        ShaderParts shaderParts;
        graphics::Shader shader;
        bool flipX, flipY;
        uint32 order;
        uint32 vao, vbo;

        Sprite &operator=(const Sprite &other) noexcept
        {
            if (this != &other)
            {
                vertices = std::move(other.vertices);
                texture = other.texture;
                shaderParts = other.shaderParts;
                shader = other.shader;
                flipX = other.flipX;
                flipY = other.flipY;
                order = other.order;
                vao = other.vao;
                vbo = other.vbo;
            }

            return *this;
        }

        // Sprite &operator=(Sprite &&other) noexcept
        // {
        //     if (this != &other)
        //     {
        //         vertices = std::move(other.vertices);
        //         texture = std::move(other.texture);
        //         shaderParts = std::move(other.shaderParts);
        //         shader = std::move(other.shader);
        //         flipX = other.flipX;
        //         flipY = other.flipY;
        //         order = other.order;
        //         vao = other.vao;
        //         vbo = other.vbo;

        //         other.vao = 0;
        //         other.vbo = 0;
        //     }
        //     return *this;
        // }
    };
};