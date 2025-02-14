#pragma once
#include "../defs.hpp"
#include "../entity.hpp"
#include "../../graphics/shader.hpp"

namespace engine
{
    struct Vertex
    {
        vec3 position = {0.0f, 0.0f, 0.0f};
        vec2 texCoords = {0.0f, 0.0f};
    };

    struct Texture
    {
        uint32 textureID = 0;
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
        std::vector<uint32> indices;
        Texture texture;
        ShaderParts shaderParts;
        graphics::Shader shader;
        bool flipX = false, flipY = false;
        uint32 order = 0;
        uint32 vao = 0, vbo = 0, ebo = 0;

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
                ebo = other.ebo;
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