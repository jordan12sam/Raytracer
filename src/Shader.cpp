#include "Shader.hpp"

Shader::Shader(const char* sourcePath, GLuint type)
{
    std::ifstream infile(sourcePath);
    if (!infile) {
        std::cerr << "Error: Failed to open file " << sourcePath << std::endl;
        std::cerr << strerror(errno) << std::endl;

        TCHAR NPath[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, NPath);

        std::cerr << NPath << std::endl;

    }

    std::ostringstream oss;
    oss << infile.rdbuf();
    std::string contents = oss.str();

    char* buffer = new char[contents.length() + 1];
    std::strcpy(buffer, contents.c_str());
    const char* source = buffer;

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