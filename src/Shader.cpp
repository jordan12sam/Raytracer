#include "Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexShaderSource = parseShader(vertexPath);
    std::string fragmentShaderSource = parseShader(fragmentPath);
    rendererID = createShader(vertexShaderSource, fragmentShaderSource);
}

Shader::~Shader()
{
    glDeleteProgram(rendererID);
}

void Shader::bind() const
{
    glUseProgram(rendererID);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

int Shader::getUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(rendererID, name.c_str());
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(getUniformLocation(name), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

std::string Shader::parseShader(const char* filepath) const
{
    std::ifstream stream(filepath);
    if (!stream) {
        std::cerr << "Error: Invalid stream" << std::endl;
        perror(filepath);
    }
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

void Shader::compileShader(unsigned int shaderID, const std::string& shaderSource) const
{
    const char* source = shaderSource.c_str();
    glShaderSource(shaderID, 1, &source, nullptr);
    glCompileShader(shaderID);

    int compileResult;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE)
    {
        int infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* infoLog = new char[infoLogLength];
        glGetShaderInfoLog(shaderID, infoLogLength, nullptr, infoLog);
        std::cerr << "Error compiling shader: " << infoLog << std::endl;
        delete[] infoLog;
        glDeleteShader(shaderID);
        return;
    }
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) const
{
    unsigned int program = glCreateProgram();
    unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    compileShader(vertexShaderID, vertexShader);
    compileShader(fragmentShaderID, fragmentShader);

    glAttachShader(program, vertexShaderID);
    glAttachShader(program, fragmentShaderID);

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
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        return 0;
    }

    glDetachShader(program, vertexShaderID);
    glDetachShader(program, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return program;
}