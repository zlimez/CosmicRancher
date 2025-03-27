#include "renderer.hpp"

// NOTE: A lot of optimizations possible
namespace engine
{
    void RenderSys::setupSprite(Sprite &sprite, Transform &transform)
    {
        if (sprite.vao_)
            return;
        if (!sprite.texture_.textureID)
            if (!textures_.count(sprite.texture_.filePath))
                textures_[sprite.texture_.filePath] = graphics::texFromFile(sprite.texture_.filePath);
        sprite.texture_.textureID = textures_[sprite.texture_.filePath];

        std::string shaderKey = std::string(sprite.shaderParts_.vtxPath) + sprite.shaderParts_.delim + std::string(sprite.shaderParts_.fragPath);
        if (!shaders_.count(shaderKey))
            shaders_[shaderKey] = graphics::Shader(sprite.shaderParts_.vtxPath, sprite.shaderParts_.fragPath);
        sprite.shader_ = shaders_[shaderKey];

        glGenVertexArrays(1, &sprite.vao_);
        glGenBuffers(1, &sprite.vbo_);
        glGenBuffers(1, &sprite.ebo_);
        glBindVertexArray(sprite.vao_);
        glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * sprite.vertices_.size(), &(sprite.vertices_[0]), transform.isStatic_ ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite.ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * sprite.indices_.size(), &(sprite.indices_[0]), transform.isStatic_ ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }

    void RenderSys::getSprites(World &world)
    {
        Type spriteType;
        spriteType.set(getComponentID<Sprite>());
        spriteType.set(getComponentID<Transform>());
        auto &spriteArchs = world.getArchetypesWith(spriteType);
        spriteObjs_.clear();

        for (auto spriteArch : spriteArchs)
        {
            auto &spriteRow = spriteArch->getComponentRow<Sprite>();
            auto &transformRow = spriteArch->getComponentRow<Transform>();
            for (size_t i = 0; i < spriteArch->count_; i++)
            {
                if (!spriteRow.blocks_[i].active_)
                    continue;
                setupSprite(spriteRow.blocks_[i], transformRow.blocks_[i]);
                spriteObjs_.push_back({spriteRow.blocks_[i].order, &spriteRow.blocks_[i], &transformRow.blocks_[i]});
            }
        }

        std::sort(spriteObjs_.begin(), spriteObjs_.end(), [](auto &a, auto &b)
                  { return std::get<0>(a) < std::get<0>(b); });
    }

    void RenderSys::init(World &world)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void RenderSys::update(World &world)
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
            if (!camArch->count_) // FIXME: Temp solution to find that first unique cam
                continue;

            // assumes one cam in world for now
            auto &cam = cams.blocks_[0];
            auto &transform = transforms.blocks_[0];
            glm::mat4 view = glm::mat4(1.0f);

            // utils::Logger::logOnce("Cam pos " + std::to_string(transform.position.x) + ' ' + std::to_string(transform.position.y) + ' ' + std::to_string(cam.width) + ' ' + std::to_string(cam.height));
            view = glm::translate(view, glm::vec3(-transform.position_.x, -transform.position_.y, 0.0f));
            view = glm::rotate(view, glm::radians(-transform.rotation_), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 proj = glm::ortho(-cam.width_ / 2, cam.width_ / 2, -cam.height_ / 2, cam.height_ / 2, -1.0f, 1.0f);
            uint32 currShaderId = 0;
            GLuint currTexId = 0;

            // TODO: Change to use UBO instead of setting view proj for every shader
            for (auto &spriteObj : spriteObjs_)
            {
                auto &[order, sprite, tfm] = spriteObj;
                if (sprite->vao_ == 0 || sprite->vbo_ == 0 || sprite->texture_.textureID == 0)
                    utils::Logger::logOnce("VAO or VBO or textureID not generated properly!");
                if (sprite->vertices_.empty())
                    utils::Logger::logOnce("Sprite has no vertices!");
                if (sprite->shader_.ID == 0)
                    utils::Logger::logOnce("Shader not generated properly!");

                if (currShaderId != sprite->shader_.ID)
                {
                    sprite->shader_.use();
                    sprite->shader_.set("projection", proj);
                    sprite->shader_.set("view", view);
                    currShaderId = sprite->shader_.ID;
                }

                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(tfm->position_.x, tfm->position_.y, 0.0f));
                model = glm::rotate(model, glm::radians(tfm->rotation_), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::scale(model, glm::vec3(tfm->scale_.x * (sprite->flipX_ ? -1 : 1), tfm->scale_.y * (sprite->flipY_ ? -1 : 1), 1.0f));
                sprite->shader_.set("model", model);

                if (currTexId != sprite->texture_.textureID)
                {
                    glActiveTexture(GL_TEXTURE0);
                    sprite->shader_.set("texture_diffuse", 0);
                    glBindTexture(GL_TEXTURE_2D, sprite->texture_.textureID);
                    currTexId = sprite->texture_.textureID;
                }
                glBindVertexArray(sprite->vao_);
                glDrawElements(GL_TRIANGLES, sprite->indices_.size(), GL_UNSIGNED_INT, 0);
            }
            break;
        }

        SDL_GL_SwapWindow(window_);
    }

    void RenderSys::cleanup()
    {
        for (auto &shader : shaders_)
            glDeleteProgram(shader.second.ID);
        for (auto &texture : textures_)
            glDeleteTextures(1, &texture.second);
        shaders_.clear();
        textures_.clear();
        spriteObjs_.clear();
    }
};