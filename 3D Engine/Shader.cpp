#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Common.h"

using namespace Shaders;

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::use() const
{
    GameData* gameData = GetGameData();
    if (gameData->actualShader == this) { return; }
    gameData->actualShader = (Shader*)this;
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value)
{
    //for (int i = 0; i < this->vector2.size(); i++)
    //{
    //    if (this->vector2[i].second == name)
    //    {
    //        if (this->vector2[i].first == value) { return; }
    //        this->vector2[i].first = value;
    //        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    //        return;
    //    }
    //}
    //this->vector2.push_back(std::make_pair(value, name));
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y)
{
    //glm::vec2 vec2(x, y);
    //for (int i = 0; i < this->vector2.size(); i++)
    //{
    //    if (this->vector2[i].second == name)
    //    {
    //        if (this->vector2[i].first == vec2) { return; }
    //        this->vector2[i].first = vec2;
    //        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    //        return;
    //    }
    //}
    //this->vector2.push_back(std::make_pair(vec2, name));
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value)
{
    //for (int i = 0; i < this->vector3.size(); i++)
    //{
    //    if (this->vector3[i].second == name)
    //    {
    //        if (this->vector3[i].first == value) { return; }
    //        this->vector3[i].first = value;
    //        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    //        return;
    //    }
    //}
    //this->vector3.push_back(std::make_pair(value, name));
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z)
{
    //glm::vec3 vec3(x, y, z);
    //for (int i = 0; i < this->vector3.size(); i++)
    //{
    //    if (this->vector3[i].second == name)
    //    {
    //        if (this->vector3[i].first == vec3) { return; }
    //        this->vector3[i].first = vec3;
    //        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    //        return;
    //    }
    //}
    //this->vector3.push_back(std::make_pair(vec3, name));
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value)
{
    //for (int i = 0; i < this->vector4.size(); i++)
    //{
    //    if (this->vector4[i].second == name)
    //    {
    //        if (this->vector4[i].first == value) { return; }
    //        this->vector4[i].first = value;
    //        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    //        return;
    //    }
    //}
    //this->vector4.push_back(std::make_pair(value, name));
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
    //glm::vec4 vec4(x, y, z, w);
    //for (int i = 0; i < this->vector4.size(); i++)
    //{
    //    if (this->vector4[i].second == name)
    //    {
    //        if (this->vector4[i].first == vec4) { return; }
    //        this->vector4[i].first = vec4;
    //        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    //        return;
    //    }
    //}
    //this->vector4.push_back(std::make_pair(vec4, name));
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat)
{
    for (int i = 0; i < this->matrices2.size(); i++)
    {
        if (this->matrices2[i].second == name)
        {
            if (this->matrices2[i].first == mat) { return; }
            this->matrices2[i].first = mat;
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
            return;
        }
    }
    this->matrices2.push_back(std::make_pair(mat, name));
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) 
{
    for (int i = 0; i < this->matrices3.size(); i++)
    {
        if (this->matrices3[i].second == name)
        {
            if (this->matrices3[i].first == mat) { return; }
            this->matrices3[i].first = mat;
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
            return;
        }
    }
    this->matrices3.push_back(std::make_pair(mat, name));
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat)
{
    for (int i = 0; i < this->matrices4.size(); i++)
    {
        if (this->matrices4[i].second == name)
        {
            if (this->matrices4[i].first == mat) { return; }
            this->matrices4[i].first = mat;
            glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
            return;
        }
    }
    this->matrices4.push_back(std::make_pair(mat, name));
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}