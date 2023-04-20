#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void bind() const;
    void unbind() const;
    int getUniformLocation(const std::string& name) const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
private:
    unsigned int rendererID;
    std::string parseShader(const char* filepath) const;
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) const;
    void compileShader(unsigned int shaderID, const std::string& shaderSource) const;
};