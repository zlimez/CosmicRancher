#pragma once
#include <glm/glm.hpp>
#include <string>
#include <glad/glad.h>

unsigned int texFromFile(const char *path, const std::string &directory, GLint SWrap = GL_REPEAT, GLint TWrap = GL_REPEAT, bool gamma = false);