#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();
    
    void attach(Shader shader) const;
    void link() const;
    void bind() const;
    void unbind() const;

    int getUniformLocation(const std::string& name) const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setIntArray(const std::string& name, int* value, int count) const;
    void setFloat(const std::string& name, float value) const;
    void setFloatArray(const std::string& name, float* value, int count) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
private:
    unsigned int ID;
};