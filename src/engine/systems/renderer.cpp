#include "renderer.hpp"

namespace engine
{
    void Renderer::init(World &world)
    {
        Type spriteType;
        spriteType.set(getComponentID<Sprite>());
        spriteType.set(getComponentID<Transform>());
        auto &archs = world.getArchetypesWith(spriteType);
        for (auto arch : archs)
        {
            auto &sprites = arch->getComponentRow<Sprite>();
            auto &transforms = arch->getComponentRow<Transform>();
            // TODO: Load textures
            for (size_t i = 0; i < arch->count; i++)
            {
                auto &sprite = sprites.blocks[i];
                auto &transform = transforms.blocks[i];

                sprite.texture.textureID = texFromFile(sprite.texture.texturePath.c_str(), "../assets/textures/");

                glGenVertexArrays(1, &sprite.vao);
                glGenBuffers(1, &sprite.vbo);
                glBindVertexArray(sprite.vao);
                glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * sprite.vertices.size(), nullptr, transform.isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }
        }
    }

    void Renderer::update(World &world)
    {
        // Render
    }
};