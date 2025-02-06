#pragma once
#include "../defs.hpp"
#include "../entity.hpp"

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
        std::string texturePath;
    };

    // NOTE: For now one sprite per entity, default stretch the texture to the sprite size
    struct Sprite : Component
    {
        std::vector<Vertex> vertices;
        Texture texture;
        bool flipX, flipY;
        uint32 order;
        uint32 vao, vbo;

        Sprite &operator=(const Sprite &other)
        {
            if (this != &other)
            {
                vertices = std::move(other.vertices);
                texture = other.texture;
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