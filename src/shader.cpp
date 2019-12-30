#include <shader.h>


Shader::Shader(const char *vertexShaderString, const char *fragmentShaderString)
{
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderString, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderString, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int Shader::setBool(const std::string & name, const bool value) const
{
    unsigned int tmp = glGetUniformLocation(ID, name.c_str());
    glUniform1i(tmp , (int)value);
    return tmp;
}

unsigned int Shader::setInt(const std::string & name, const int value) const
{
    unsigned int tmp = glGetUniformLocation(ID, name.c_str());
    glUniform1i(tmp, value);
    return tmp;
}

unsigned int Shader::setFloat(const std::string & name, const float value) const
{
    unsigned int tmp = glGetUniformLocation(ID, name.c_str());
    glUniform1f(tmp, value);
    return tmp;
}

unsigned int Shader::set3f(const std::string & name, 
    const float x, const float y, const float z) const
{
    unsigned int tmp = glGetUniformLocation(ID, name.c_str());
    glUniform3f(tmp, x, y, z);
    return tmp;
}

unsigned int Shader::set4f(const std::string & name, 
    const float x, const float y, const float z, const float w) const
{
    unsigned int tmp = glGetUniformLocation(ID, name.c_str());
    glUniform4f(tmp, x, y, z, w);
    return tmp;
}

unsigned int Shader::setVec3(const std::string & name, const glm::vec3 & vec) const
{
    unsigned int tmp = glGetUniformLocation(ID, name.c_str());
    glUniform3f(tmp, vec.x, vec.y, vec.z);
    return tmp;
}

unsigned int Shader::setMatrix4fv(const std::string & name, const glm::mat4 value) const
{
    unsigned int tmp = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(tmp, 1, GL_FALSE, glm::value_ptr(value));
    return tmp;
}