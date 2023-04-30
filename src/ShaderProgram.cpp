#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram()
{
    ID = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(ID);
}

void ShaderProgram::attach(Shader shader) const
{
    glAttachShader(ID, shader.getID());
}

void ShaderProgram::link() const
{
    glLinkProgram(ID);
}

void ShaderProgram::bind() const
{
    glUseProgram(ID);
}

void ShaderProgram::unbind() const
{
    glUseProgram(0);
}

int ShaderProgram::getUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(ID, name.c_str());
}

void ShaderProgram::setBool(const std::string& name, bool value) const
{
    glUniform1i(getUniformLocation(name), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
    glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::setIntArray(const std::string& name, int* value, int count) const
{
    glUniform1iv(getUniformLocation(name), count, value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setFloatArray(const std::string& name, float* value, int count) const
{
    glUniform1fv(getUniformLocation(name), count, value);
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}