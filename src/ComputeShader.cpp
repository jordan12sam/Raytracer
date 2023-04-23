#include "Shader.hpp"

void ComputeShader::createShader(const char* computePath)
{
    std::string computeShaderSource = parseShader(computePath);

    unsigned int program = glCreateProgram();
    unsigned int computeShaderID = glCreateShader(GL_COMPUTE_SHADER);

    compileShader(computeShaderID, computeShaderSource);

    glAttachShader(program, computeShaderID);

    glLinkProgram(program);

    int linkResult;
    glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
    if (linkResult == GL_FALSE)
    {
        int infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* infoLog = new char[infoLogLength];
        glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog);
        std::cerr << "Error linking shader program: " << infoLog << std::endl;
        delete[] infoLog;
        glDeleteProgram(program);
        glDeleteShader(computeShaderID);
        rendererID =  0;
        return;
    }

    glDetachShader(program, computeShaderID);
    glDeleteShader(computeShaderID);

    rendererID =  program;
    return;
}

void ComputeShader::compute(int x, int y, int z)
{
    glDispatchCompute(x, y, z);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}