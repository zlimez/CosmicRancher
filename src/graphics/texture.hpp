#pragma once
#include <glm/glm.hpp>
#include <string>
#include <glad/glad.h>

namespace graphics
{
    unsigned int texFromFile(const char *path, const char *directory, GLint SWrap = GL_REPEAT, GLint TWrap = GL_REPEAT, bool gamma = false);
}