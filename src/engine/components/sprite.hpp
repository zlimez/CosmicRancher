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
        std::vector<Vertex> vertices_;
        std::vector<uint32> indices_;
        Texture texture_;
        ShaderParts shaderParts_;
        graphics::Shader shader_;
        bool flipX_ = false, flipY_ = false;
        uint32 order = 0;
        uint32 vao_ = 0, vbo_ = 0, ebo_ = 0;

        Sprite &operator=(const Sprite &other) noexcept
        {
            if (this != &other)
            {
                vertices_ = std::move(other.vertices_);
                texture_ = other.texture_;
                shaderParts_ = other.shaderParts_;
                shader_ = other.shader_;
                flipX_ = other.flipX_;
                flipY_ = other.flipY_;
                order = other.order;
                vao_ = other.vao_;
                vbo_ = other.vbo_;
                ebo_ = other.ebo_;
            }

            return *this;
        }

        ~Sprite()
        {
            if (vao_)
                glDeleteVertexArrays(1, &vao_);
            if (vbo_)
                glDeleteBuffers(1, &vbo_);
            if (ebo_)
                glDeleteBuffers(1, &ebo_);
        }
    };
};