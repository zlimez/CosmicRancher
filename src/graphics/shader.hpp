#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../utils/logger.hpp"

namespace graphics
{
    class Shader
    {
    public:
        // the program ID
        unsigned int ID;
        // constructor reads and builds the shader
        Shader() {};
        Shader(std::string &vertexPath, std::string &fragmentPath);
        // use/activate the shader
        void use();
        void set(const std::string &name, bool value) const;
        void set(const std::string &name, int value) const;
        void set(const std::string &name, float value) const;
        void set(const std::string &name, glm::mat4 value) const;
        void set(const std::string &name, glm::vec3 value) const;
    };
}
