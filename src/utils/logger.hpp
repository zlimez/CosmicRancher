#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <iomanip>
#include <unordered_set>
#include <glm/glm.hpp>

namespace utils
{
    class Logger
    {
    public:
        static void print(const glm::mat4 &matrix);
        static void print(const glm::vec3 &vector, const std::string &name);
        static void logOnce(const std::string &message);
    };
}