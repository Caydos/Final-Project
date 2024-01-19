#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utility> // For std::pair
#include <string>
#include <vector>

namespace Shaders
{
    enum Type
    {
        UI,
        UI_OBJECT,
        WORLD_OBJECT
    };

    class Shader
    {
    public:
        unsigned int ID;
        std::vector<std::pair<bool, std::string>> boolean;
        std::vector<std::pair<int, std::string>> integer;
        std::vector<std::pair<float, std::string>> flotant;


        std::vector<std::pair<glm::vec2, std::string>> vector2;
        std::vector<std::pair<glm::vec3, std::string>> vector3;
        std::vector<std::pair<glm::vec4, std::string>> vector4;


        std::vector<std::pair<glm::mat2, std::string>> matrices2;
        std::vector<std::pair<glm::mat3, std::string>> matrices3;
        std::vector<std::pair<glm::mat4, std::string>> matrices4;

        // constructor generates the shader on the fly
        // ------------------------------------------------------------------------
        Shader(const char* vertexPath, const char* fragmentPath);
        // activate the shader
        // ------------------------------------------------------------------------
        void use() const;
        // utility uniform functions
        // ------------------------------------------------------------------------
        void setBool(const std::string& name, bool value);
        // ------------------------------------------------------------------------
        void setInt(const std::string& name, int value);
        // ------------------------------------------------------------------------
        void setFloat(const std::string& name, float value);
        // ------------------------------------------------------------------------
        void setVec2(const std::string& name, const glm::vec2& value);
        void setVec2(const std::string& name, float x, float y);
        // ------------------------------------------------------------------------
        void setVec3(const std::string& name, const glm::vec3& value);
        void setVec3(const std::string& name, float x, float y, float z);
        // ------------------------------------------------------------------------
        void setVec4(const std::string& name, const glm::vec4& value);
        void setVec4(const std::string& name, float x, float y, float z, float w);
        // ------------------------------------------------------------------------
        void setMat2(const std::string& name, const glm::mat2& mat);
        // ------------------------------------------------------------------------
        void setMat3(const std::string& name, const glm::mat3& mat);
        // ------------------------------------------------------------------------
        void setMat4(const std::string& name, const glm::mat4& mat);

    private:
        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        void checkCompileErrors(GLuint shader, std::string type);
    };
}

#endif