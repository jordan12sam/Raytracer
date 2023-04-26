#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture
{
    private:
        unsigned int rendererID;
        std::string filePath;
        unsigned char *localBuffer;
        int width, height, bytesPerPixel;
    public:
        Texture(const std::string &path);
        ~Texture();

        void bind(unsigned int slot = 0) const;
        void unbind(unsigned int slot = 0) const;

        inline int getWidth() const { return width; }
        inline int getHeight() const { return height; }
};