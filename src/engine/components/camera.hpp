#pragma once
#include "../entity.hpp"
#include "../defs.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace engine
{
    struct Camera : Component
    {
        // glm::mat4 getProjMat()
        // {
        //     glm::mat4 proj = glm::mat4(1.0f);
        //     proj = glm::ortho(position.x, position.x + width, position.y, position.y + height, -1.0f, 1.0f);
        //     return proj;
        // }

        // glm::mat4 getViewMat()
        // {
        //     glm::mat4 view = glm::mat4(1.0f);
        //     view = glm::translate(view, glm::vec3(-position.x, -position.y, 0.0f));
        //     return view;
        // }

        vec2 position;
        float width, height;
    };
}