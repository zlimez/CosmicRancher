#include "renderer.hpp"

namespace engine
{
    void Renderer::setupSprite(Sprite &sprite, Transform &transform)
    {
        std::cout << "Setting up sprite\n";
        sprite.texture.textureID = graphics::texFromFile(sprite.texture.filePath);
        std::cout << "Texture ID: " << sprite.texture.textureID << '\n';
        std::string shaderKey = std::string(sprite.shaderParts.vtxPath) + sprite.shaderParts.delim + std::string(sprite.shaderParts.fragPath);
        if (!shaders.count(shaderKey))
            shaders[shaderKey] = graphics::Shader(sprite.shaderParts.vtxPath, sprite.shaderParts.fragPath);
        sprite.shader = shaders[shaderKey];
        std::cout << "Shader ID: " << sprite.shader.ID << '\n';

        glGenVertexArrays(1, &sprite.vao);
        glGenBuffers(1, &sprite.vbo);
        std::cout << "VAO: " << sprite.vao << " VBO: " << sprite.vbo << " Vertices size: " << sprite.vertices.size() << '\n';
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

    void Renderer::init(World &world)
    {
        Type spriteType;
        spriteType.set(getComponentID<Sprite>());
        spriteType.set(getComponentID<Transform>());
        auto &spriteArchs = world.getArchetypesWith(spriteType);

        std::cout << "Got sprite archetypes\n";

        for (auto spriteArch : spriteArchs)
        {
            auto &spriteRow = spriteArch->getComponentRow<Sprite>();
            auto &transformRow = spriteArch->getComponentRow<Transform>();
            for (size_t i = 0; i < spriteArch->count; i++)
                sprites.push_back({spriteRow.blocks[i].order, &spriteRow.blocks[i], &transformRow.blocks[i]});
        }

        std::sort(sprites.begin(), sprites.end(), [](auto &a, auto &b)
                  { return std::get<0>(a) < std::get<0>(b); });
        std::cout << "Sorted sprites " << sprites.size() << '\n';

        for (auto &sprite : sprites)
            setupSprite(*std::get<1>(sprite), *std::get<2>(sprite));
    }

    void Renderer::update(World &world)
    {
        Type camType;
        camType.set(getComponentID<Camera>());
        camType.set(getComponentID<Transform>());
        auto &camArchs = world.getArchetypesWith(camType);

        for (auto camArch : camArchs)
        {
            auto &cams = camArch->getComponentRow<Camera>();
            auto &transforms = camArch->getComponentRow<Transform>();

            // assumes one cam in world for now
            auto &cam = cams.blocks[0];
            auto &transform = transforms.blocks[0];
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view, glm::vec3(-transform.position.x, -transform.position.y, 0.0f));
            glm::mat4 proj = glm::ortho(-cam.width / 2, cam.width / 2, -cam.height / 2, cam.height / 2, -1.0f, 1.0f);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // TODO: Change to use UBO instead of setting view proj for every shader
            for (auto &sprite : sprites)
            {
                auto &[order, spritePtr, transformPtr] = sprite;
                spritePtr->shader.use();
                spritePtr->shader.set("projection", proj);
                spritePtr->shader.set("view", view);
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(transformPtr->position.x, transformPtr->position.y, 0.0f));
                model = glm::rotate(model, glm::radians(transformPtr->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::scale(model, glm::vec3(transformPtr->scale.x * (spritePtr->flipX ? -1 : 1), transformPtr->scale.y * (spritePtr->flipY ? -1 : 1), 1.0f));
                spritePtr->shader.set("model", model);
                glBindVertexArray(spritePtr->vao);
                glDrawArrays(GL_TRIANGLES, 0, spritePtr->vertices.size());
            }
            break;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }

    void Renderer::cleanup()
    {
        for (auto &shader : shaders)
            glDeleteProgram(shader.second.ID);
        for (auto &sprite : sprites)
        {
            auto &[order, spritePtr, transformPtr] = sprite;
            glDeleteVertexArrays(1, &spritePtr->vao);
            glDeleteBuffers(1, &spritePtr->vbo);
        }
    }
};