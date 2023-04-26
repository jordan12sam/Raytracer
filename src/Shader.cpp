#include "Shader.hpp"

Shader::Shader(const char* sourcePath, GLuint type)
{
    std::ifstream stream(sourcePath);
    if (!stream) {
        std::cerr << "Error: Invalid stream" << std::endl;
        perror(sourcePath);
    }
    std::stringstream buffer;
    buffer << stream.rdbuf();
    const char* source = buffer.str().c_str();

    ID = glCreateShader(type);
    glShaderSource(ID, 1, &source, nullptr);
    glCompileShader(ID);

    int compileResult;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE)
    {
        int infoLogLength;
        glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* infoLog = new char[infoLogLength];
        glGetShaderInfoLog(ID, infoLogLength, nullptr, infoLog);
        std::cerr << "Error compiling shader: " << infoLog << std::endl;
        delete[] infoLog;
        glDeleteShader(ID);
    }
}

Shader::~Shader()
{
    glDeleteShader(ID);
}

unsigned int Shader::getID() const
{
    return ID;
}