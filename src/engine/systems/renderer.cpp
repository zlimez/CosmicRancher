#include "renderer.hpp"

// NOTE: A lot of optimizations possible
namespace engine
{
    void Renderer::setupSprite(Sprite &sprite, Transform &transform)
    {
        if (sprite.vao)
            return;
        if (!sprite.texture.textureID)
            if (!textures.count(sprite.texture.filePath))
                textures[sprite.texture.filePath] = graphics::texFromFile(sprite.texture.filePath);
        sprite.texture.textureID = textures[sprite.texture.filePath];

        std::string shaderKey = std::string(sprite.shaderParts.vtxPath) + sprite.shaderParts.delim + std::string(sprite.shaderParts.fragPath);
        if (!shaders.count(shaderKey))
            shaders[shaderKey] = graphics::Shader(sprite.shaderParts.vtxPath, sprite.shaderParts.fragPath);
        sprite.shader = shaders[shaderKey];

        glGenVertexArrays(1, &sprite.vao);
        glGenBuffers(1, &sprite.vbo);
        glGenBuffers(1, &sprite.ebo);
        glBindVertexArray(sprite.vao);
        glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * sprite.vertices.size(), &(sprite.vertices[0]), transform.isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * sprite.indices.size(), &(sprite.indices[0]), transform.isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }

    void Renderer::getSprites(World &world)
    {
        Type spriteType;
        spriteType.set(getComponentID<Sprite>());
        spriteType.set(getComponentID<Transform>());
        auto &spriteArchs = world.getArchetypesWith(spriteType);
        spriteObjs.clear();

        for (auto spriteArch : spriteArchs)
        {
            auto &spriteRow = spriteArch->getComponentRow<Sprite>();
            auto &transformRow = spriteArch->getComponentRow<Transform>();
            for (size_t i = 0; i < spriteArch->count; i++)
            {
                if (!spriteRow.blocks[i].active)
                    continue;
                setupSprite(spriteRow.blocks[i], transformRow.blocks[i]);
                spriteObjs.push_back({spriteRow.blocks[i].order, &spriteRow.blocks[i], &transformRow.blocks[i]});
            }
        }

        std::sort(spriteObjs.begin(), spriteObjs.end(), [](auto &a, auto &b)
                  { return std::get<0>(a) < std::get<0>(b); });
    }

    void Renderer::init(World &world)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer::update(World &world)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        getSprites(world);

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

            // utils::Logger::logOnce("Cam pos " + std::to_string(transform.position.x) + ' ' + std::to_string(transform.position.y) + ' ' + std::to_string(cam.width) + ' ' + std::to_string(cam.height));
            view = glm::translate(view, glm::vec3(-transform.position.x, -transform.position.y, 0.0f));
            view = glm::rotate(view, glm::radians(-transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 proj = glm::ortho(-cam.width / 2, cam.width / 2, -cam.height / 2, cam.height / 2, -1.0f, 1.0f);
            uint32 currShaderId = 0;
            GLuint currTexId = 0;

            // TODO: Change to use UBO instead of setting view proj for every shader
            for (auto &spriteObj : spriteObjs)
            {
                auto &[order, sprite, tfm] = spriteObj;
                if (sprite->vao == 0 || sprite->vbo == 0 || sprite->texture.textureID == 0)
                    utils::Logger::logOnce("VAO or VBO or textureID not generated properly!");
                if (sprite->vertices.empty())
                    utils::Logger::logOnce("Sprite has no vertices!");
                if (sprite->shader.ID == 0)
                    utils::Logger::logOnce("Shader not generated properly!");

                if (currShaderId != sprite->shader.ID)
                {
                    sprite->shader.use();
                    sprite->shader.set("projection", proj);
                    sprite->shader.set("view", view);
                    currShaderId = sprite->shader.ID;
                }

                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(tfm->position.x, tfm->position.y, 0.0f));
                model = glm::rotate(model, glm::radians(tfm->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::scale(model, glm::vec3(tfm->scale.x * (sprite->flipX ? -1 : 1), tfm->scale.y * (sprite->flipY ? -1 : 1), 1.0f));
                sprite->shader.set("model", model);

                if (currTexId != sprite->texture.textureID)
                {
                    glActiveTexture(GL_TEXTURE0);
                    sprite->shader.set("texture_diffuse", 0);
                    glBindTexture(GL_TEXTURE_2D, sprite->texture.textureID);
                    currTexId = sprite->texture.textureID;
                }
                glBindVertexArray(sprite->vao);
                glDrawElements(GL_TRIANGLES, sprite->indices.size(), GL_UNSIGNED_INT, 0);
            }
            break;
        }

        SDL_GL_SwapWindow(window);
    }

    void Renderer::cleanup()
    {
        for (auto &shader : shaders)
            glDeleteProgram(shader.second.ID);
        for (auto &texture : textures)
            glDeleteTextures(1, &texture.second);
        shaders.clear();
        textures.clear();
        spriteObjs.clear();
    }
};