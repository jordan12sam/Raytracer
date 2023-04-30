#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <windows.h>
#include <iostream>

class Shader
{
public:
    Shader(const char* sourcePath, GLuint type);
    ~Shader();
    unsigned int getID() const;
private:
    unsigned int ID;
};