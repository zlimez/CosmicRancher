#pragma once
#include <glm/glm.hpp>
#include <string>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

namespace graphics
{
    unsigned int texFromFile(std::string &filePath, GLint SWrap = GL_REPEAT, GLint TWrap = GL_REPEAT, bool gamma = false);
}