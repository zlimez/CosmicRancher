#include "logger.hpp"

namespace utils
{
    void Logger::print(const glm::mat4 &matrix)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                std::cout << std::setw(8) << std::setprecision(4) << matrix[i][j] << " ";
        std::cout << '\n';
    }

    void Logger::print(const glm::vec3 &vector, const std::string &name)
    {
        std::cout << name << ": ";
        for (int i = 0; i < 3; ++i)
            std::cout << std::setw(8) << std::setprecision(4) << vector[i] << " ";
        std::cout << '\n';
    }

    void Logger::logOnce(const std::string &message)
    {
        static std::unordered_set<std::string> logged;
        if (logged.find(message) == logged.end())
        {
            std::cout << message << '\n';
            logged.insert(message);
        }
    }
};