#include "Shader.hpp"

Shader::Shader()
{
    rendererID = 0;
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

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(getUniformLocation(name), 1, &value[0]);
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
        char tmp[256];
        getcwd(tmp, 256);
        std::cout << "Current working directory: " << tmp << std::endl;
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

void Shader::createShader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexShaderSource = parseShader(vertexPath);
    std::string fragmentShaderSource = parseShader(fragmentPath);

    unsigned int program = glCreateProgram();
    unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    compileShader(vertexShaderID, vertexShaderSource);
    compileShader(fragmentShaderID, fragmentShaderSource);

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
        rendererID =  0;
        return;
    }

    glDetachShader(program, vertexShaderID);
    glDetachShader(program, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    rendererID =  program;
}