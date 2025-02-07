#include "texture.hpp"
#include <SDL2/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace graphics
{
    unsigned int texFromFile(std::string &filePath, GLint SWrap, GLint TWrap, bool gamma)
    {
        std::cout << "Loading texture: " << filePath << std::endl;
        unsigned int textureID;
        glGenTextures(1, &textureID);
        std::cout << "Texture ID: " << textureID << std::endl;

        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(false);
        unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, SWrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TWrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << filePath << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
}
